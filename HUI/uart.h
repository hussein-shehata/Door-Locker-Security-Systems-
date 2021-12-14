/*
 * uart.h
 *
 *  Created on: Oct 29, 2021
 *      Author: Hussein
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*Configuration parameters*/
 typedef enum{
	 Disabled,Reserved,Even_Parity,Odd_Parity
 }UART_Parity_Mode;

typedef enum{
	_1Bit,_2Bit
}UART_Stop_Bit;

typedef enum{
	_5Bit,_6Bit,_7Bit,_8Bit,_9Bit=7
}UART_Bit_Size;


typedef struct{
	UART_Parity_Mode ParityMode;
	UART_Stop_Bit StopBit;
	UART_Bit_Size BitSize;
	uint32 Baud_Rate;
}UART_ConfigType;

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType* Config_Ptr);
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
