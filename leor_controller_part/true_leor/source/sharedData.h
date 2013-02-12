/*
 * sharedData.h
 *
 *  Created on: 12.02.2013
 *      Author: Administrator
 */

#ifndef SHAREDDATA_H_
#define SHAREDDATA_H_

#include <stdint.h>
#include <stdbool.h>

#define SERVO_COUNT 8
#define BUFFER_LENGTH (SERVO_COUNT + 1) /* один байт на маркер начала сообщения */

// Это все для уарта.

//! Буффер для сообщения уарта
extern uint8_t buffer[BUFFER_LENGTH];

typedef enum{
  waitForMessage,
  MessageRecieved,
  MessageProcessed
} State;



#endif /* SHAREDDATA_H_ */
