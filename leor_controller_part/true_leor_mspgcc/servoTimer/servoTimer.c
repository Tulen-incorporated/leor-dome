/*
 * servoTimer.c
 *
 *  Created on: 11.02.2013
 *      Author: Administrator
 */

#include <stdint.h>
#include <stdbool.h>
#include <msp430g2553.h>

#include "servoTimer.h"

#define P1_SERVO_PINS (BIT4 + BIT5) // ���� ������������ �� ������ �����
#define P2_SERVO_PINS (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5) // ���� ������������ �� ������ �����
#define TIMER_PERIOD 22300 // mks

typedef struct listEntry listEntry;
struct listEntry{
  uint16_t servoTick;
  uint_fast8_t portNumber;
  uint8_t portMask;
  listEntry * nextEntryPtr;
};


void initServosOnTimerA0SMCLK4mHz()
{
  // ========================================
  // ������������� ������� A0.
  // ========================================
  // ������������� ������� ������� �� ������������.
  TA0CCR0 = TIMER_PERIOD;
  TA0CCR1 = TIMER_PERIOD/2;

  TA0CCTL0 = CM_0 /* ��� �������� */ |  CCIS_0 /* ������ ������ - �������� �� ����� */ | 0 /* ������/���� ������, �������� �� ����� */ |
      0 /* ����� ��������� */ | OUTMOD_0 /* ����� �� ����, �������� �� ����� */ | CCIE /* ���������� �� ��������� ��������� */;
  TA0CCTL1 = CM_0 | CCIS_0 | 0 | 0 | OUTMOD_0;// | CCIE;
  TA0CTL = TASSEL_2 /* �� SMCLK */ | ID_2 /* /4 */ | MC_1 /* ������� �� ��R0 */ | TACLR /*����� */ | TAIE /* ���������� �� ������������ ��������� */;

}

// ���������� �� TA0CCR0 CCIFG
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer0_A0()
{
  //���� �� ������ ���� - ������ ������������.
  switch (TA0IV) {
    case TA0IV_TAIFG:
      P1OUT ^= BIT0;
      break;

    case TA0IV_TACCR1:
      P1OUT ^= BIT6;
      break;

    default:
      case TA0IV_NONE:
      break;
  }
}

// ���������� �� TA0CCR2 TA0CCR1 CCIFG, TAIFG
#pragma vector = TIMER0_A1_VECTOR
__interrupt void timer0_A1()
{
  switch (TA0IV) {
    case TA0IV_TAIFG:
      P1OUT ^= BIT0;
      break;

    case TA0IV_TACCR1:
      P1OUT ^= BIT6;
      break;

    default:
      case TA0IV_NONE:
      break;
  }
}




