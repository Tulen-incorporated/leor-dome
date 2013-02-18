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
  // Инициализация UART на USCI0.
  // ========================================
  // Ставим уарт на резет.
  UCA0CTL1 |= UCSWRST;

  // Пины на уарт. // Первая и вторая ножки первого порта идут под уарт.
  P1SEL |= (BIT1 | BIT2);
  P1SEL2 |= (BIT1 | BIT2);

  UCA0CTL0 = /*~UCPEN | ~UCPAR | ~UCMSB | ~UC7BIT | ~UCSPB |*/ UCMODE_0 /*| ~UCSYNC*/;
  // Без бита четности | Нечетный бит четности | Меньший бит первым |
  // восьмибитное слово | Один стопбит | Режим UART | Асинхронная работа.

  UCA0CTL1 = UCSSEL_2 /*| ~UCRXEIE | ~UCBRKIE | ~UCDORM | ~UCTXADDR | ~UCTXBRK | UCSWRST */; // Резет пока не трогаем.
  // от сабмастер клока | не реагируем на ошибки по RX | и по TX | не спим | следующее слово не адресс | и не перерыв

  // Бауд рейт на 9600 при UCOS16 == 1 и 4мгц клоке. Из таблицы в мануале.
  UCA0BR0 = 26 % 0x100;
  UCA0BR1 = 26 / 0x100;
  // Модуляция
  UCA0MCTL = UCBRF_1 | UCBRS_0 | UCOS16; // UCOS16 == Оверсамплинг включен, остальное по таблицам.

  // снимаем резет, мы готовы к работе, но не работаем, пока не вызовут initListen;
  UCA0CTL1 &= ~UCSWRST;
}

#pragma INTERRUPT (uartRxInterrupt);
#pragma vector = USCIAB0RX_VECTOR
void uartRxInterrupt()
{
  /*
  // Включаем вложенные прерыания, для таймера, он приоритетнее
  __enable_interrupt();

  //IE2 &= ~UCA0RXIFG; Происходит автоматически, когда читаешь из RXBUF
  register uint8_t buff = UCA0RXBUF;

  // Если это не первый байт, которого мы ждем
  if (!bufferIndex && buff != BEGIN_MARKER)
  {
    return; // Забиваем на этот байт, ждем маркера начала.
  }

  // Если у нас уже есть набор углов на этом такте, нам больше ничего не нужно.
  if (bufferIndex >= MESSAGE_LENGTH)
  {
    return; // Забиваем на этот байт, он нам не нужен.
  }

  // Пишем в буффер.
  buffer[bufferIndex] = buff;
  bufferIndex++;

  // Если мы добрали сообщение нужной длинны именно этим байтом (а не предидущим, что проверяется условием выше)
  if (bufferIndex == MESSAGE_LENGTH)
  {
    newAnglesAvailible = true; // Выставляем флаг доступности новых данных.
  }
  */
}

