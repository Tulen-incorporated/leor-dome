/*
 * servoTimer.h
 *
 *  Created on: 11.02.2013
 *      Author: Administrator
 */

#ifndef SERVOTIMER_H_
#define SERVOTIMER_H_

#define SERVO_MIN_TICK 800
#define SERVO_MAX_TICK 2300

#include <stdint.h>
#include <float.h>

void initServosOnTimerA0SMCLK4mHz();

inline uint16_t angleToTick(const float * angle)
{
  return (*angle - 0) / (180 - 0) * (SERVO_MAX_TICK - SERVO_MIN_TICK) + SERVO_MIN_TICK;
}

#endif /* SERVOTIMER_H_ */
