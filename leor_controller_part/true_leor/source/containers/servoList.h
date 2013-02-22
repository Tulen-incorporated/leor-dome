/*
 * servoList.h
 *
 *  Created on: 18.02.2013
 *      Author: Administrator
 */

#ifndef SERVOLIST_H_
#define SERVOLIST_H_

#include <stdint.h>

#define SERVO_COUNT 8

typedef struct ServoListEntry ServoListEntry;
struct ServoListEntry{
  uint16_t servoTick;
  volatile uint8_t * portReg;
  uint8_t portMask;
};

extern ServoListEntry arrangedServoList[SERVO_COUNT];
extern const ServoListEntry * sortedServoList[SERVO_COUNT];

inline void sortServoSortedList()
{
  register uint_fast16_t i, j;

  for (i = 0; i < SERVO_COUNT - 1; i++)
  {
    for (j = i; j < SERVO_COUNT - 1; j++)
    {
      if (sortedServoList[j]->servoTick > sortedServoList[j+1]->servoTick)
      {
        register const ServoListEntry * tmpPtr = sortedServoList[j];
        sortedServoList[j] = sortedServoList[j + 1];
        sortedServoList[j + 1] = tmpPtr;
      }
    } // for j
  } // for i
}

inline void initServoSortedList()
{
  register uint_fast16_t i;
  for (i = 0; i < SERVO_COUNT; i++)
  {
    sortedServoList[i] = &arrangedServoList[i];
  }
}


#endif /* SERVOLIST_H_ */
