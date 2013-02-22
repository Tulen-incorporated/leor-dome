#ifndef LEORMESSAGE_H
#define LEORMESSAGE_H

#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#define MESSAGE_SERVO_COUNT 7

typedef struct
{
  float servoAngles[MESSAGE_SERVO_COUNT];
} LeorMessageData;

typedef struct
{
  uint8_t startMarker[3];
  LeorMessageData messageData;
  uint8_t checkSumm;
} LeorMessage;

inline uint8_t chkSummAlg(const LeorMessageData * message)
{
    const char * p = (const char*)(message);
    uint_fast8_t i;
    uint8_t summ = 0, oldSumm;
    for (i = 0; i < sizeof(*message); i++)
    {
        oldSumm = summ;
        summ += p[i];
        if (summ < oldSumm)
        {
            summ += 1;
        }
    }

    return summ;
}

inline bool isMessageCorrect(const LeorMessage * message)
{
    /*return (message->checkSumm == chkSummAlg(&message->messageData)) &&
           (message->startMarker == START_MARKER_BYTE);*/
  return false;
}

#endif // LEORMESSAGE_H
