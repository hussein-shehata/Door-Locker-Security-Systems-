/*
 * uart.c
 *
 *  Created on: Oct 29, 2021
 *      Author: Hussein
 */

#include "uart.h"
#include "common_macros.h"
#include <avr/io.h>

void UART_init(const UART_ConfigType* Config_Ptr){
	uint16 myUbrr=0;
	UCSRA |= (1<<U2X); /* U2X = 1 for double transmission speed */
	/************************** UCSRB Description **************************
		 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
		 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
		 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
		 * RXEN  = 1 Receiver Enable
		 * RXEN  = 1 Transmitter Enable
		 * UCSZ2 = 0 For 8-bit data mode
		 * RXB8 & TXB8 will be configured later
		 ***********************************************************************/
	UCSRB =(1<<RXEN) |(1<<TXEN);
	if(Config_Ptr->BitSize ==9)
		UCSRB |=(1<<UCSZ2); /* UCSZ2 must be set in 9 Bit Mode */
	/************************** UCSRC Description **************************
		 * URSEL   = 1 The URSEL must be one when writing the UCSRC
		 * UMSEL   = 0 Asynchronous Operation
		 * UCPOL   = 0 Used with the Synchronous operation only
		 ***********************************************************************/
	UCSRC =(1<<URSEL);
	if(Config_Ptr->BitSize ==9){
			UCSRC =(UCSRC &0XF9) |(((Config_Ptr->BitSize)-4)<<1); /* configuring the Bit Size*/
			//UCSRB |=(1<<UCSZ2); /* UCSZ2 must be set in 9 Bit Mode */
		}
		else
			UCSRC =(UCSRC &0XF9) |((Config_Ptr->BitSize)<<1); /* configuring the Bit Size*/

	UCSRC = (UCSRC & 0xCF) | ((Config_Ptr->ParityMode)<<4);/* configuring the Parity Mode*/
	UCSRC = (UCSRC &0XF7) |(Config_Ptr->StopBit<<3); /* configuring the number of Stop Bit */

	/* Calculate the UBRR register value */
	myUbrr = (uint16)(((F_CPU / ((Config_Ptr->Baud_Rate) * 8UL))) - 1);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRL = myUbrr;
	UBRRH = myUbrr>>8;

}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data){
	//UCSRB |= ((data<<8)<<TXB8); /*when using 9 bit mode , we have to put the 9th bit in TXB8 first*/
	/*
	* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	* transmitting a new byte so wait until this flag is set to one
	*/
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/*
	* Put the required data in the UDR register and it also clear the UDRE flag as
	* the UDR register is not empty now
	*/
	UDR=data;
}
/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void){
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/*
	* Read the received data from the Rx buffer (UDR)
	* The RXC flag will be cleared after read the data
	*/
	return UDR;
}
/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
