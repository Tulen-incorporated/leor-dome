/*
 * servoTimer.c
 *
 *  Created on: 11.02.2013
 *      Author: Administrator
 */

#include "servoTimer.h"

#include <stdint.h>
#include <stdbool.h>

#include <msp430g2553.h>

#include "../leorMessage.h"
#include "servoList.h"

#define P1_SERVO_PINS (BIT4 | BIT5) // ���� ������������ �� ������ �����
#define P2_SERVO_PINS (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5) // ���� ������������ �� ������ �����

#define TIMER_PERIOD 22300 // mks (������ ������� �� �����)

//! ��������, ������� ����������� �� ������, ���� ������ �� ������ � �����.
#define DEFAULT_SERVO_TICK (uint16_t)(SERVO_MIN_TICK + SERVO_MAX_TICK)/2

//! ��������� �� ��������� �����.
const ServoListEntry ** carretPtr;

//! ������������� ��������� ������������ s, �� ����� tick � ��, ������ ����� ���� � ������� portReg �� ����� &= portMask
static inline void initServo(ServoListEntry * s, const uint16_t tick, volatile uint8_t * portReg, const uint8_t portMask)
{
  s->servoTick = tick;
  s->portReg = portReg;
  s->portMask = portMask;
}

void initServosOnTimerA0SMCLK4mHz()
{
  // ������ ����� ������������!
  // �������������� ������ ����������.
  initServoSortedList();

  // ��������� �������� ��� ����.
  initServo(&arrangedServoList[0], DEFAULT_SERVO_TICK, &P2OUT, ~BIT0); //p2.0
  initServo(&arrangedServoList[1], DEFAULT_SERVO_TICK, &P2OUT, ~BIT1); //p2.1
  initServo(&arrangedServoList[2], DEFAULT_SERVO_TICK, &P2OUT, ~BIT2); //p2.2
  initServo(&arrangedServoList[3], DEFAULT_SERVO_TICK, &P2OUT, ~BIT3); //p2.3
  initServo(&arrangedServoList[4], DEFAULT_SERVO_TICK, &P2OUT, ~BIT4); //p2.4
  initServo(&arrangedServoList[5], DEFAULT_SERVO_TICK, &P2OUT, ~BIT5); //p2.5

  initServo(&arrangedServoList[6], DEFAULT_SERVO_TICK, &P1OUT, ~BIT4); //p1.4
  initServo(&arrangedServoList[7], DEFAULT_SERVO_TICK, &P1OUT, ~BIT5); //p1.5

  // ��������� �� ������ ������.
  sortServoSortedList();

  // ������� �� ����
  carretPtr = sortedServoList;

  // ========================================
  // ������������� ������� A0.
  // ========================================

  TA0CCR0 = TIMER_PERIOD + 1; // ���� ��� ���� �� ��, ��� ����� �������� �� ���������� ������������, � �� ������ �� ����
  TA0CCR1 = sortedServoList[0]->servoTick; // ������ ����� ������� ����������.

  TA0CCTL0 = CM_0 /* ��� �������� */ |  0 /* ������ ������ - �������� �� ����� */ | 0 /* ������/���� ������, �������� �� ����� */ |
      0 /* ����� ��������� */ | 0 /* ����� �� ����, �������� �� ����� */ | CCIE /* ���������� �� ��������� ��������� */;
  TA0CCTL1 = CM_0 | 0 | 0 | 0 | 0 | CCIE;

  TA0CTL = TASSEL_2 /* �� SMCLK */ | ID_2 /* /4 */ | MC_1 /* ������� �� ��R0 */ | TACLR /*����� */ |
      0 /* ��� ���������� �� ������������ */;

  // ���, ����� ��������, ��������� � �����������.
}

// ���������� �� TA0CCR0 CCIFG
#pragma INTERRUPT (timer0_A0);
#pragma vector = TIMER0_A0_VECTOR
void timer0_A0()
{
  // register uint16_t taiv = TA0IV;
  // ������ ���� �� ����� ������� ������ � ����� ������ - ������ ������������.
  // ��������� ��� �� ���� ������������ ����� ��� ����, ��� ��� �����, ����� ������ ������ ��� ���������� (������ �������������)
  // �� ��� �� ���� ������, ����� ������ �� ��� ����� ��������.
  P1OUT |= P1_SERVO_PINS;
  P2OUT |= P2_SERVO_PINS;

  // � ��� ��������� ����� ������� ���������� ���������� �������.
  TACCR1 = sortedServoList[0]->servoTick;

  // � �������� �������.
  carretPtr = sortedServoList;
}

// ���������� �� TA0CCR2 TA0CCR1 CCIFG, TAIFG
#pragma INTERRUPT (timer0_A1);
#pragma vector = TIMER0_A1_VECTOR
void timer0_A1()
{
  // ���� �� �������� ������, ����� ��� ����� ��������� ��������� ������������. ���� �� ���.

  do {
    // ���� �� �������.
    *((*carretPtr)->portReg) &= (*carretPtr)->portMask;

    // ������� ������
    carretPtr++;

    // ���� ������� ������ �� �������
    if ((carretPtr - sortedServoList) >= SERVO_COUNT)
    {
      return; // ������ �� ����������.
    }
  } while ((*carretPtr)->servoTick <= TA0R+1);

  // ���� ������� �� ������ �� �������
  // ������ ����� ���������� ������������ ��� ���������� �����.
  TACCR0 = (*carretPtr)->servoTick;
}




