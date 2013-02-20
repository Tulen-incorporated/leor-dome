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

#define P1_SERVO_PINS (BIT4 | BIT5) // Пины сервомашинок на первом порту
#define P2_SERVO_PINS (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5) // Пины сервомашинок на втором порту

#define TIMER_PERIOD 22300 // mks (Период сигнала на сервы)

//! Значения, которые установятся на сервах, пока ничего не пришло с компа.
#define DEFAULT_SERVO_TICK (uint16_t)(SERVO_MIN_TICK + SERVO_MAX_TICK)/2

//! Указатель на следующую серву.
const ServoListEntry ** carretPtr;

//! Инициализация структуры сервомашинки s, на время tick в мс, выдача будет идти в регистр portReg по маске &= portMask
static inline void initServo(ServoListEntry * s, const uint16_t tick, volatile uint8_t * portReg, const uint8_t portMask)
{
  s->servoTick = tick;
  s->portReg = portReg;
  s->portMask = portMask;
}

void initServosOnTimerA0SMCLK4mHz()
{
  // Первым делом сервомашинки!
  // Инициализируем массив указателей.
  initServoSortedList();

  // Заполняем значения для серв.
  initServo(&arrangedServoList[0], DEFAULT_SERVO_TICK, &P2OUT, ~BIT0); //p2.0
  initServo(&arrangedServoList[1], DEFAULT_SERVO_TICK, &P2OUT, ~BIT1); //p2.1
  initServo(&arrangedServoList[2], DEFAULT_SERVO_TICK, &P2OUT, ~BIT2); //p2.2
  initServo(&arrangedServoList[3], DEFAULT_SERVO_TICK, &P2OUT, ~BIT3); //p2.3
  initServo(&arrangedServoList[4], DEFAULT_SERVO_TICK, &P2OUT, ~BIT4); //p2.4
  initServo(&arrangedServoList[5], DEFAULT_SERVO_TICK, &P2OUT, ~BIT5); //p2.5

  initServo(&arrangedServoList[6], DEFAULT_SERVO_TICK, &P1OUT, ~BIT4); //p1.4
  initServo(&arrangedServoList[7], DEFAULT_SERVO_TICK, &P1OUT, ~BIT5); //p1.5

  // Сортируем на всякий случей.
  sortServoSortedList();

  // Каретку на ноль
  carretPtr = sortedServoList;

  // ========================================
  // инициализация таймера A0.
  // ========================================

  TA0CCR0 = TIMER_PERIOD + 1; // Один тик даем на то, что будем работать по прерыванию переполнения, а не сброса на ноль
  TA0CCR1 = sortedServoList[0]->servoTick; // Ставим время первого выключения.

  TA0CCTL0 = CM_0 /* без захватов */ |  0 /* откуда захват - значения не имеет */ | 0 /* асинхр/синх захват, значения не имеет */ |
      0 /* режим сравнения */ | 0 /* режим на пине, значения не имеет */ | CCIE /* прерывание по сравнению разрешено */;
  TA0CCTL1 = CM_0 | 0 | 0 | 0 | 0 | CCIE;

  TA0CTL = TASSEL_2 /* от SMCLK */ | ID_2 /* /4 */ | MC_1 /* считаем до ССR0 */ | TACLR /*Сброс */ |
      0 /* Без прерываний на переполнение */;

  // Все, сервы работают, остальное в прерываниях.
}

// Прерывание по TA0CCR0 CCIFG
#pragma INTERRUPT (timer0_A0);
#pragma vector = TIMER0_A0_VECTOR
void timer0_A0()
{
  // register uint16_t taiv = TA0IV;
  // Собсно сюда мы можем попасть только в одном случае - таймер переполнился.
  // Случилось это на одну миллисекунду позже чем надо, как раз тогда, когда таймер должен был сброситься (смотри инициализацию)
  // Ну раз мы сюда пришли, нужно подать на все сервы единичку.
  P1OUT |= P1_SERVO_PINS;
  P2OUT |= P2_SERVO_PINS;

  // А еще выставить время первого выключения следующего прохода.
  TACCR1 = sortedServoList[0]->servoTick;

  // И сбросить каретку.
  carretPtr = sortedServoList;
}

// Прерывание по TA0CCR2 TA0CCR1 CCIFG, TAIFG
#pragma INTERRUPT (timer0_A1);
#pragma vector = TIMER0_A1_VECTOR
void timer0_A1()
{
  // Сюда мы приходим только, когда нам нужно выключить очередную сервомашинку. ЛЕТС ДУ ЗЫС.

  do {
    // Идем по каретке.
    *((*carretPtr)->portReg) &= (*carretPtr)->portMask;

    // Каретку дальше
    carretPtr++;

    // Если каретка уехала за границу
    if ((carretPtr - sortedServoList) >= SERVO_COUNT)
    {
      return; // Уходим из прерывания.
    }
  } while ((*carretPtr)->servoTick <= TA0R+1);

  // Если Каретка не уехала за границу
  // Ставим время следующего срабатывания для следуюущей сервы.
  TACCR0 = (*carretPtr)->servoTick;
}




