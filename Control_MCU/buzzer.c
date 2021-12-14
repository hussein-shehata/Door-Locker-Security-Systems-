/*
 * buzzer.c
 *
 *  Created on: Nov 3, 2021
 *      Author: Hussein
 */

#include "buzzer.h"
#include "gpio.h"

/* Description :
 * this function is responsible about triggering the buzzer to give alarm
 */
void buzzer_open(){
	 GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}

/* Description :
 * this function is responsible about stopping the buzzer
 */
void buzzer_close(){
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
