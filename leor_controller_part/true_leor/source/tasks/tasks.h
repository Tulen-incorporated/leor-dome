/*
 * tasks.h
 *
 *  Created on: 21.02.2013
 *      Author: Administrator
 */

#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>
#include <msp430.h>

#define TASK_NEW_BYTE_ON_UART BIT0
#define TASK_NEW_ANGLES_MESSAGE BIT1

extern uint16_t taskRegister;

#endif /* TASKS_H_ */
