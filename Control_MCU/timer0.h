/*
 * timer0.h
 *
 *  Created on: Nov 1, 2021
 *      Author: Hussein
 */

#ifndef TIMER0_H_
#define TIMER0_H_
#include "std_types.h"

typedef enum{
	Normal_Mode,Compare_Mode
}Timer0_Mode;

typedef enum{
	CLK=1,CLK_8,CLK_64,CLK_256,CLK_1024
}Timer0_Prescale;

typedef enum{
	Normal,Toggle,Clear,Set
}Comapre_Operating_Mode;

/* structure to hold the configuration of the timer */
typedef struct{
	Timer0_Mode Mode;
	Comapre_Operating_Mode Compare_Mode;
	Timer0_Prescale Prescale;
	uint8 intial_value;
	uint8 compare_value;
}Timer0_ConfigType;

/*
 * Description : Function to initialize the Timer0 driver
 * 	1. Set the required mode.
 * 	2. Set the prescaler.
 * 	3. Enable the interrupt for compare or for overflow mode depending on what is the mode of the timer0.
 * 	4. set the initial value for the timer0
 * 	5. set the compare value for timer0 if operating on compare mode
 */
void TIMER0_init(const Timer0_ConfigType* Config_Ptr);
/*
 * Description: Function to set the Call Back function address.
 */
void TIMER0_SetCallBack(void(*a_ptr)(void));
/*
 * Description: Function to De-initlize the timer to start from beginning again .
 */
void TIMER0_Deinit();
#endif /* TIMER0_H_ */
