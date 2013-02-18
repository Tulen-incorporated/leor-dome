#ifndef LEORMESSAGE_H
#define LEORMESSAGE_H

#include <stdint.h>
#include <stdbool.h>

#define SERVO_COUNT 8
#define SERVO_MIN_TICK ((uint16_t)800) //ms
#define SERVO_MAX_TICK ((uint16_t)2300) //ms

#define READY_BYTE ('r')
#define START_MARKER_BYTE (0xFF)

#ifndef __MSP430__
#pragma pack(push, 1)
#endif

typedef struct
{
  uint16_t servosTimes[SERVO_COUNT];
} LeorMessageData;

typedef struct
{
    uint8_t startMarker;
    LeorMessageData messageData;
    uint8_t checkSumm;
} LeorMessage;

#ifndef __MSP430__
#pragma pack(pop)
#endif

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
    return (message->checkSumm == chkSummAlg(&message->messageData)) &&
           (message->startMarker == START_MARKER_BYTE);
}

#endif // LEORMESSAGE_H
