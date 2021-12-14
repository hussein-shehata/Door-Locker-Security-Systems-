/*
 * timer0.c
 *
 *  Created on: Nov 1, 2021
 *      Author: Hussein
 */
#include "timer0.h"
#include "avr/io.h"
#include <avr/interrupt.h>
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void)=NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_OVF_vect){
	/* Call the Call Back function in the application after the edge is detected */
	if(g_callBackPtr !=NULL_PTR){
		g_callBackPtr();
	}
}
ISR(TIMER0_COMP_vect){
	/* Call the Call Back function in the application after the edge is detected */
	if(g_callBackPtr !=NULL_PTR){
		g_callBackPtr();
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer0 driver
 * 	1. Set the required mode.
 * 	2. Set the prescaler.
 * 	3. Enable the interrupt for compare or for overflow mode depending on what is the mode of the timer0.
 * 	4. set the initial value for the timer0
 * 	5. set the compare value for timer0 if operating on compare mode
 */
void TIMER0_init(const Timer0_ConfigType* Config_Ptr){
	TCCR0 |=(1<<FOC0); /* set FOC0 cause we are using non PWM mode*/
	TCCR0 =(TCCR0 &0XB7) | ((Config_Ptr->Mode)<<WGM01); /* enable WGM01 if we are using compare mode*/
	if((Config_Ptr->Mode)==Compare_Mode)
		TCCR0 = (TCCR0 &0XCF) |((Config_Ptr->Compare_Mode)<<4); /* configure the mode of compare such as toggle or clear when the compare matches*/
	TCCR0 = (TCCR0 &0XF8) |((Config_Ptr->Prescale)); /*configuring the prescale */
	TCNT0=Config_Ptr->intial_value; /* configure the initial value */
	if((Config_Ptr->Mode)==Compare_Mode){
		OCR0=Config_Ptr->compare_value;  /* configure the compare value if the timer is operating on compare mode only*/
		TIMSK =(TIMSK&0XFC) |(1<<OCIE0); /* enable interrupt for compare mode */
	}
	else
		TIMSK =(TIMSK&0XFC) |(1<<TOIE0); /*enable interrupt for overflow mode*/
}
/*
 * Description: Function to set the Call Back function address.
 */
void TIMER0_SetCallBack(void(*a_ptr)(void)){
	g_callBackPtr=a_ptr; /* Save the address of the Call back function in a global variable */
}

/*
 * Description: Function to De-initlize the timer to start from beginning again .
 */
void TIMER0_Deinit(){
	TCCR0=0;
	TCNT0=0;
	OCR0=0;
	TIMSK =(TIMSK&0XFC);
}
