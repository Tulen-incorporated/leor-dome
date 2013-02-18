/*
 * uart.c
 *
 *  Created on: 11.02.2013
 *      Author: Administrator
 */

#include <msp430g2553.h>
#include <stdint.h>
#include <stdbool.h>

#include "uart.h"

inline void UARTinit9600on4mhzSMCLK()
{
  // ========================================
  // ������������� UART �� USCI0.
  // ========================================
  // ������ ���� �� �����.
  UCA0CTL1 |= UCSWRST;

  // ���� �� ����. // ������ � ������ ����� ������� ����� ���� ��� ����.
  P1SEL |= (BIT1 | BIT2);
  P1SEL2 |= (BIT1 | BIT2);

  UCA0CTL0 = /*~UCPEN | ~UCPAR | ~UCMSB | ~UC7BIT | ~UCSPB |*/ UCMODE_0 /*| ~UCSYNC*/;
  // ��� ���� �������� | �������� ��� �������� | ������� ��� ������ |
  // ������������ ����� | ���� ������� | ����� UART | ����������� ������.

  UCA0CTL1 = UCSSEL_2 /*| ~UCRXEIE | ~UCBRKIE | ~UCDORM | ~UCTXADDR | ~UCTXBRK | UCSWRST */; // ����� ���� �� �������.
  // �� ��������� ����� | �� ��������� �� ������ �� RX | � �� TX | �� ���� | ��������� ����� �� ������ | � �� �������

  // ���� ���� �� 9600 ��� UCOS16 == 1 � 4��� �����. �� ������� � �������.
  UCA0BR0 = 26 % 0x100;
  UCA0BR1 = 26 / 0x100;
  // ���������
  UCA0MCTL = UCBRF_1 | UCBRS_0 | UCOS16; // UCOS16 == ������������ �������, ��������� �� ��������.

  // ������� �����, �� ������ � ������, �� �� ��������, ���� �� ������� initListen;
  UCA0CTL1 &= ~UCSWRST;
}

#pragma INTERRUPT (uartRxInterrupt);
#pragma vector = USCIAB0RX_VECTOR
void uartRxInterrupt()
{
  /*
  // �������� ��������� ���������, ��� �������, �� ������������
  __enable_interrupt();

  //IE2 &= ~UCA0RXIFG; ���������� �������������, ����� ������� �� RXBUF
  register uint8_t buff = UCA0RXBUF;

  // ���� ��� �� ������ ����, �������� �� ����
  if (!bufferIndex && buff != BEGIN_MARKER)
  {
    return; // �������� �� ���� ����, ���� ������� ������.
  }

  // ���� � ��� ��� ���� ����� ����� �� ���� �����, ��� ������ ������ �� �����.
  if (bufferIndex >= MESSAGE_LENGTH)
  {
    return; // �������� �� ���� ����, �� ��� �� �����.
  }

  // ����� � ������.
  buffer[bufferIndex] = buff;
  bufferIndex++;

  // ���� �� ������� ��������� ������ ������ ������ ���� ������ (� �� ����������, ��� ����������� �������� ����)
  if (bufferIndex == MESSAGE_LENGTH)
  {
    newAnglesAvailible = true; // ���������� ���� ����������� ����� ������.
  }
  */
}

