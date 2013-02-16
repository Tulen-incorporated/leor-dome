#ifndef LEORMESSAGE_H
#define LEORMESSAGE_H

#include <stdint.h>

#define SERVO_COUNT 7
#define READY_BYTE ('r')
#define START_MARKER_BYTE (0xFF)

#pragma pack(push, 1)

struct LeorMessageData
{
    uint16_t servosTimes[SERVO_COUNT];
};

struct LeorMessage
{
    uint8_t startMarker;
    LeorMessageData messageData;
    uint8_t checkSumm;
};

#pragma pack(pop)

inline uint8_t chkSummAlg(const LeorMessageData * message);

inline bool isMessageCorrect(const LeorMessage * message);

#endif // LEORMESSAGE_H
