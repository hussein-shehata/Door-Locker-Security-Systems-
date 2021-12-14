/*
 * commands.h
 *
 *  Created on: Nov 1, 2021
 *      Author: Hussein
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

/* Definitions for sending commands between the Human interface MCU and the control MCU */

#define CHECK_PASSWORD 		0x01
#define CHANGE_PASSWORD 	0x02
#define WRONG_PASSWORD		0x13
#define CLOSE_THE_DOOR		0x03
#define OPEN_THE_DOOR		0x04
#define CREATE_PASSWORD		0x05
#define STORE_PASSWORD		0x09
#define MISMATCH_PASSWORD	0x10
#define MATCHED_PASSWORD	0x11
#define OPEN_DOOR 			0x15
#define CLOSE_DOOR			0x16
#define HOLD_DOOR			0x17
#define DOOR_DONE			0x18
#define THIEF				0x19
#define THIEF_CAUGHT		0x20
#endif /* COMMANDS_H_ */
