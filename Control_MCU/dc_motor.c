/*
 * DC_Motor.c
 *
 *  Created on: Oct 9, 2021
 *      Author: Hussein
 */

#include "dc_motor.h"
#include "gpio.h"
//#include "timer0.h"

/*
 * Description :
 * Function responsible for initialize the DC-motor.
 */

void DcMotor_init(void){
	GPIO_setupPinDirection(DC_MOTOR_1ST_PORT_ID,DC_MOTOR_1ST_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_2ST_PORT_ID,DC_MOTOR_2ST_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_E_PORT_ID,DC_MOTOR_E_PIN_ID,PIN_OUTPUT);
}
/*
 * Description :
 * Function responsible for configuring the Speed and the direction of rotation of the motor with respect to
 * the parameters passed to the function
 */
void DcMotor_Rotate(Dc_Motor_State state){
	/*configuring the motor according to the passed state parameter.*/
	switch (state){
	case Off:
		//PWM_Timer0_Start(0); /*make PWM with 0% duty cycle*/
		/* Reseting both PINS which are responsible for the direction of the motor
		 * therefore, it stops the motor
		 */
		GPIO_writePin(DC_MOTOR_2ST_PORT_ID,DC_MOTOR_2ST_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_1ST_PORT_ID,DC_MOTOR_1ST_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_E_PORT_ID,DC_MOTOR_E_PIN_ID,LOGIC_LOW);
		break;
	case A_CW:
		/* Writing to both PINS which are responsible for the direction of the motor
		 * therefore, it rotates Anti_ Clock wise
		 */
		GPIO_writePin(DC_MOTOR_2ST_PORT_ID,DC_MOTOR_2ST_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_1ST_PORT_ID,DC_MOTOR_1ST_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_E_PORT_ID,DC_MOTOR_E_PIN_ID,LOGIC_HIGH);
		/* Make PWM with (speed%) duty cycle*/
		//PWM_Timer0_Start(speed);
		break;
	case CW:
		/* Writing to both PINS which are responsible for the direction of the motor
		 * therefore, it rotates Clock wise
		 */
		GPIO_writePin(DC_MOTOR_1ST_PORT_ID,DC_MOTOR_1ST_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_2ST_PORT_ID,DC_MOTOR_2ST_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_E_PORT_ID,DC_MOTOR_E_PIN_ID,LOGIC_HIGH);
		/* Make PWM with (speed%) duty cycle*/
		//PWM_Timer0_Start(speed);
		break;
	}
}
