/*
 * DC_Motor.h
 *
 *  Created on: Oct 9, 2021
 *      Author: Hussein
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include"std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_MOTOR_1ST_PORT_ID PORTB_ID
#define DC_MOTOR_1ST_PIN_ID PIN0_ID
#define DC_MOTOR_2ST_PORT_ID PORTB_ID
#define DC_MOTOR_2ST_PIN_ID PIN1_ID
#define DC_MOTOR_E_PORT_ID PORTB_ID
#define DC_MOTOR_E_PIN_ID PIN2_ID

typedef enum{
	CW,A_CW,Off
}Dc_Motor_State;
/*
 * Description :
 * Function responsible for initialize the DC-motor.
 */
void DcMotor_init(void);
/*
 * Description :
 * Function responsible for configuring the Speed and the direction of rotation of the motor with respect to
 * the parameters passed to the function
 */
void DcMotor_Rotate(Dc_Motor_State state);


#endif /* DC_MOTOR_H_ */
