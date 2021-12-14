/*
 * control_mcu.c
 *
 *  Created on: Nov 1, 2021
 *      Author: Hussein
 */
#include "std_types.h"
#include "common_macros.h"
#include "commands.h"
#include "uart.h"
#include "timer0.h"
#include "util\delay.h"
#include "external_eeprom.h"
#include "i2c.h"
#include "dc_motor.h"
#include"buzzer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define EEPROM_BASE_ADDRESS 	0x0000
#define NO_OF_PASSWORD_LETTERS 	5


/*******************************************************************************
 *                      Functions Prototypes                                  *
 *******************************************************************************/
void drivers_init();
void store_password();
void read_password();
void create_password();
void check_password();
void open_door();
void ISR_no_of_occurance_door(void);
void thief();
void ISR_no_of_occurance_buzzer();

UART_ConfigType UartConfig={Disabled,_1Bit,_8Bit,9600};
Timer0_ConfigType myTIMER0config={Compare_Mode,Toggle,CLK_1024,0,250};
I2C_ConfigType myTWIconfig={100,0x01};

/* global variables to store the password and the password for checking*/
uint8 key[5];
uint8 key2[5];

/*flags to control the opening and closing door operations*/
uint8 open_door_flag=0;
uint8 hold_door_flag=0;
uint8 close_door_flag=0;

int main(){
	drivers_init();
uint8 command;
SREG |=(1<<7);  /* enable global interrupt */
	while(1){
		command=UART_recieveByte(); /* Receive the command from the interface MCU */
/* checks what command did the interface sent */
		if(command == CREATE_PASSWORD)
			create_password();

		else if(command == CHECK_PASSWORD)
			check_password();
		else if(command ==OPEN_DOOR)
			open_door();
		else if(command==THIEF)
			thief();
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*Description :
 * initialize all used drivers
 */
void drivers_init(){
	UART_init(&UartConfig);
	TWI_init(&myTWIconfig);
	DcMotor_init();
}
/*Description :
 * write the received password in the EEPROM
 */
void store_password(){
	uint8 i;
	for(i=0;i<NO_OF_PASSWORD_LETTERS;i++){
		EEPROM_writeByte((EEPROM_BASE_ADDRESS+i),key[i]);
		_delay_ms(10);
	}
}
/*Description:
 * function used to check whether the input password "key2" is equal to the one which is saved or not
 */
void read_password(){
	uint8 i;
	uint8 value[5];
/* this for loop is responsible for reading the value in the EEPROM and write it the value array */
	for(i=0;i<NO_OF_PASSWORD_LETTERS;i++){
		EEPROM_readByte((EEPROM_BASE_ADDRESS+i),&value[i]);
		_delay_ms(10);
		}
/* this for loop is responsible for checking if the input password is the same which is
 *  stored in the EEPROM before */
	for(i=0;i<NO_OF_PASSWORD_LETTERS;i++){
		if (key2[i] != value[i])
		{
/* if there is Mismatch send this command to the interface MCU to let it know and follow the procedure  */
			UART_sendByte(MISMATCH_PASSWORD);
			return;
		}
	}
	UART_sendByte(MATCHED_PASSWORD); /* send this command to let the interface MCU know that all is okay*/
}

/*Description :
 * after taking the password from the user this function is responsible for creating it
 */

void create_password(){

	UART_sendByte(CREATE_PASSWORD); /* send this command to make the interface MCU know that he is
	available now */

	/* Receive the password from the interface MCU*/
	for(uint8 i=0;i<5;i++){
		key[i]=UART_recieveByte();
	}
	store_password(); /* store the the received password now */
}

/*Description :
 * after taking the password from the user this function is responsible for checking for it
 */

void check_password(){
	uint8 i=0;
	/* send this command to make the interface MCU know that he is available now */
	UART_sendByte(CHECK_PASSWORD);

	/* Receive the password from the interface MCU*/
	for(i=0;i<5;i++){
		_delay_ms(1);
		key2[i]=UART_recieveByte();
		}
	read_password();	/*check whether the input password is equal to the one which is saved or not*/
}

/*Description :
 * check the no of interrupts to adjust the time of opening,holding and closing the door
 */
void ISR_no_of_occurance_door(void){
	static uint16 no_of_entering=0;
	static uint8 open_door_flag=0;
	static uint8 hold_door_flag=1;
	static uint8 close_door_flag=1;
	if((no_of_entering==469) && (open_door_flag==0)){ /* the if condition will be true only after 15sec
	 and then it will hold the door*/

		UART_sendByte(HOLD_DOOR); /* send this command to let the interface MCU know */
		open_door_flag=1;
		hold_door_flag=0;
		no_of_entering=0;
		DcMotor_Rotate(Off);
	}

	else if((no_of_entering==94) && (hold_door_flag==0)){/* the if condition will be true only after 3sec
	 and then it will close the door*/

		UART_sendByte(CLOSE_DOOR); /* send this command to let the interface MCU know */
		hold_door_flag=1;
		close_door_flag=0;
		no_of_entering=0;
		DcMotor_Rotate(CW);
	}
	else if((no_of_entering==469) && (close_door_flag==0)){
		/* the if condition will be true only after 15sec and then
		 *  the operation of opening and closing the door will be done*/
		close_door_flag=1;
		no_of_entering=0;
		DcMotor_Rotate(Off);
		UART_sendByte(DOOR_DONE); /* send this command to let the interface MCU know */
	}
	if(open_door_flag && hold_door_flag && close_door_flag){ /* the function of this if condition is to
	 make it available to open the door again if asked and deinit the timer
	 to not giving anymore interrupts until asked*/
		open_door_flag=0;
		hold_door_flag=1;
		close_door_flag=1;
		TIMER0_Deinit();

	}
	no_of_entering++;

}
/*Description :
 * after taking the command from the interface this function is responsible for opening the door
 */
void open_door(){
	DcMotor_Rotate(A_CW); /* rotate the motor anti-CW  "opening the door"*/
	TIMER0_init(&myTIMER0config);
	TIMER0_SetCallBack(ISR_no_of_occurance_door);
}

/*Description :
 * check the no of interrupts to adjust the time of opening and closing the buzzer
 */
void ISR_no_of_occurance_buzzer(){
	static uint16 no=0;
	if(no == 1876){
		buzzer_close();
		no=0;
		TIMER0_Deinit();
		UART_sendByte(THIEF_CAUGHT);
	}
	else
		no++;
}
/*Description :
 * after taking the command from the interface this function is responsible for opening the buzzer
 */
void thief(){
	TIMER0_init(&myTIMER0config);
	TIMER0_SetCallBack(ISR_no_of_occurance_buzzer);
	buzzer_open(); /* open the buzzer */
}



