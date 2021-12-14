/*
 * hui.c
 *
 *  Created on: Nov 1, 2021
 *      Author: Hussein
 */
#include "std_types.h"
#include "common_macros.h"
#include "commands.h"
#include "uart.h"
#include "timer0.h"
#include "Lcd.h"
#include "keypad.h"
#include "util\delay.h"


#define UNRESERVED 255    /* to make hold this value if there is nothing he should do */

/*******************************************************************************
 *                      Functions Prototypes                                  *
 *******************************************************************************/
void drivers_init();
void intro();
void create_password();
void check_password();
void main_menu();
void open_door();



UART_ConfigType UartConfig={Disabled,_1Bit,_8Bit,9600};


uint8 pass[6];
uint8 flag=0;


int main(){
	drivers_init();
	intro();

	while(1){
		if(flag==0)
			{
			create_password();
			check_password();

			if(UART_recieveByte() == MISMATCH_PASSWORD){
				LCD_clearScreen();
				LCD_displayString("Wrong Input");
				_delay_ms(10);
				intro();
			}
		}
		else
			main_menu();
}
}




/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*Description :
 * initialize all used drivers
 */
void drivers_init(){
	LCD_init();
	UART_init(&UartConfig);
}

/*Description :
 * display the intro message and request the user to create his password
 */
void intro(){
	uint8 i=0;
	LCD_clearScreen();
	LCD_displayString("Enter new pass:");
	/* this for loop is responsible for taking the password from the user and store the input in Pass array*/
	for (i=0;i<5;i++)
	{
		_delay_ms(50);
		pass[i]=KEYPAD_getPressedKey();  /* array to store the password*/
		LCD_moveCursor(1,i);
		LCD_displayCharacter('*');
	}
	while(KEYPAD_getPressedKey() !=13){} /* wait till the user press enter */
	flag=0; /* making this flag =0 so the main function switches to create password and check password functions*/
}
/*Description :
 * after taking the password from the user this function is responsible for creating it by sending it to the
 * control MCU and the control MCU will do the rest of this job
 */
void create_password(){
	UART_sendByte(CREATE_PASSWORD); /* make the control MCU know that he should do the create password function now*/
	while(UART_recieveByte() != CREATE_PASSWORD){} /* to wait for control MCU to be available */
	for(uint8 i=0;i<5;i++){
		UART_sendByte(pass[i]);} /* sending the password by UART to the control MCU */
}
/*Description :
 * after taking the password from the user for the second time this function is responsible for
 *  sending it to the control MCU and the control MCU will do the rest of this job to check if
 *  this input is correct or not
 */
void check_password(){
	uint8 i;
	LCD_clearScreen();
	LCD_displayString("Reenter pass:");
	/* this for loop is responsible for taking the password from the user and store the input in Pass array*/
	for (i=0;i<5;i++){
		_delay_ms(50);
		pass[i]=KEYPAD_getPressedKey(); /* array to store the password*/
		LCD_moveCursor(1,i);
		LCD_displayCharacter('*');

	}
	while(KEYPAD_getPressedKey() !=13){} /* wait till the user press enter */
	UART_sendByte(CHECK_PASSWORD); /* make the control MCU know that he should do the check password function now*/
		while(UART_recieveByte() != CHECK_PASSWORD){} /* to wait for control MCU to be available */
	for(uint8 i=0;i<5;i++)
	{
		_delay_ms(1);
		UART_sendByte(pass[i]); /* sending the password by UART to the control MCU */

	}
		flag=UNRESERVED; /* means now the flag has no use */
}

/*Description :
 * this function is reponsible for displaying the main menu for the user and take the input of what function
 * does he want to do
 */
void main_menu(){
	uint8 key;
	LCD_clearScreen();
	LCD_displayString("+: open door");
	LCD_moveCursor(1,0);
	LCD_displayString("-: change pass");
	key=KEYPAD_getPressedKey(); /* get the user request to do what he wants */
	uint8 wrong_counter=0;  /* counter to see how much time did the user enter a wrong password*/
	do {
		check_password(); /* call function to check the password*/

		/* if the user enter wrong password this if condition will become true*/
		if(UART_recieveByte() == MISMATCH_PASSWORD){
			wrong_counter++;
			LCD_clearScreen();
			LCD_displayString("wrong password");
			_delay_ms(10);
			if(wrong_counter ==3)
				break; /* break the do while loop when the password is incorrect for 3 times a row*/
		}

		else
			break; /* break the do while loop when the password is correct*/
	}while(1);
	if(wrong_counter ==3){
		LCD_clearScreen();
		LCD_displayString("thief");
		UART_sendByte(THIEF);  /* send THIEF command to the control MCU so it can do the procedure */

		/* wait for the control MCU to tell the interface MCU that the procedure is complete now */
		while(UART_recieveByte() !=THIEF_CAUGHT){}
		wrong_counter=0; /* reseting the counter */

	}
	else {
		LCD_clearScreen();

	switch (key){
	case '+':
			open_door();	/* the user has requested to open the door*/
			break;

	case '-':
		intro();  /* the user has requested to change the password and it is same function as the intro fucntion*/
	}
}
}

/*Description :
 * if the user chose to open door this function will be called to open the door by sending this command to
 * the control MCU and display the state of the door on the LCD
 */
void open_door(){
	/*send open door command to the control MCU to make do the procedure for this command*/
	UART_sendByte(OPEN_DOOR);
	LCD_clearScreen();
	LCD_displayString("Opening...");
	/* wait for the control MCU to finish opening the door and start holding it */
	while(UART_recieveByte() != HOLD_DOOR){}
	LCD_clearScreen();
	LCD_displayString("	HOLDING...");
	/* wait for the control MCU to finish holding and start closing the door */
	while(UART_recieveByte() != CLOSE_DOOR){}
	LCD_clearScreen();
	LCD_displayString("	Closing...");
	while(UART_recieveByte() != DOOR_DONE){} /* the the control MCU send this then the procedure is complete*/

}
