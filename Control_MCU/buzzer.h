/*
 * buzzer.h
 *
 *  Created on: Nov 3, 2021
 *      Author: Hussein
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_PORT_ID 			PORTA_ID
#define BUZZER_PIN_ID 			PIN0_ID


/* Description :
 * this function is responsible about triggering the buzzer to give alarm
 */
void buzzer_open();
/* Description :
 * this function is responsible about stopping the buzzer
 */
void buzzer_close();


#endif /* BUZZER_H_ */
