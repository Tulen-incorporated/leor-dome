/*
 * uartBuffer.h
 *
 *  Created on: 21.02.2013
 *      Author: Administrator
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_

#include "../messages/leorMessage.h"
#include <stddef.h>

#define RX_BUFFER_SIZE (150) // Чтоб влезло!

//! Тип данных для буферов, В нашем случае - байты.
typedef uint8_t bufferValueType;

//! Статус кольцевого буфера.
typedef enum
{
  bufferEmpty,
  bufferFull,
  bufferNormal
} CycledBufferState;

//! Структура описывающая кольцевой буффер.
typedef struct{
  bufferValueType * data;
  bufferValueType * begin;
  bufferValueType * end;
  size_t size;
  CycledBufferState state;
} CycledBuffer;


inline void cycledBufferPushBack(CycledBuffer * buffer, uint8_t value)
{
  if (buffer->state == bufferFull)
    return; // TODO: ПАНИКА!

  *(buffer->end) = value;
  buffer->end++;

  if (buffer->end - buffer->data >= buffer->size)
    buffer->end = buffer->data;

  if (buffer->end == buffer->begin)
    buffer->state = bufferFull;
}

inline void cycledBufferPopFront(CycledBuffer * buffer, uint8_t * value)
{
  if (buffer->state == bufferEmpty)
    return; // TODO: ПАНИКА!

  *value = *(buffer->begin);
  buffer->begin++;

  if (buffer->begin - buffer->data >= buffer->size)
    buffer->end = buffer->data;

  if (buffer->begin == buffer->end)
    buffer->state = bufferEmpty;
  else
    buffer->state = bufferNormal;
}

inline void bufferDropFront(CycledBuffer * buffer, size_t count)
{
  buffer->begin += count;
  register size_t diff = buffer->begin - buffer->data;

  if (diff > buffer->size)
    buffer->begin = buffer->data + (diff - buffer->size);

  if (buffer->begin >= buffer->end)
  {
    buffer->begin = buffer->end;
    buffer->state = bufferEmpty;
  }
}

inline size_t CycledBufferDataSize(const CycledBuffer * buffer)
{
  switch (buffer->state){
    case bufferEmpty:
      return 0;
    case bufferFull:
      return buffer->size;
    case bufferNormal:
      return (buffer->end > buffer->begin) ?
          (buffer->end - buffer->begin) :
          (buffer->end - buffer->data + (buffer->data + buffer->size) - buffer->begin);
    default:
      return 0;
  }
}

extern CycledBuffer rxBuffer;
extern bufferValueType rxBufferData[RX_BUFFER_SIZE];


inline void initRxBuffer()
{
  rxBuffer.begin = rxBuffer.end = rxBuffer.data = rxBufferData;
  rxBuffer.size = RX_BUFFER_SIZE;
  rxBuffer.state = bufferEmpty;
}


#endif /* UARTBUFFER_H_ */
