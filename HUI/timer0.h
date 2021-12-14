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

typedef struct{
	Timer0_Mode Mode;
	Comapre_Operating_Mode Compare_Mode;
	Timer0_Prescale Prescale;
	uint8 intial_value;
	uint8 compare_value;
}Timer0_ConfigType;
void TIMER0_init(const Timer0_ConfigType* Config_Ptr);
void TIMER0_SetCallBack(void(*a_ptr)(void));
void TIMER0_Deinit();
#endif /* TIMER0_H_ */
