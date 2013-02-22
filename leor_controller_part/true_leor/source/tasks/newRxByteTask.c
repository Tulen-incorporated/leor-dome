/*
 * newRxByteTask.c
 *
 *  Created on: 21.02.2013
 *      Author: Administrator
 */

#include "tasks.h"
#include "../containers/uartBuffer.h"
#include "../messages/leorMessage.h"
#include "../messages/preamble.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
  uartNothingSpecial,
  uartStartMarkerAquaired,
  uartComandIdAquaired
} UartState;

UartState uartState;

// TODO: ��������� �������� � ������������ ��������, ���� �������� :(
void newRxByteTask()
{
  switch (uartState)
  {
  case uartNothingSpecial: // ������ �� ��������� ��� ���������. ���� ���������� ������.
    break;
  case uartStartMarkerAquaired:
    break;
  case uartComandIdAquaired:
    break;
  }

end:
  IE2 |= UCA0RXIE;
  return;
}



