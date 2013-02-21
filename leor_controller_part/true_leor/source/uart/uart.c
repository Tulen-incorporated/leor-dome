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
#include "../containers/uartBuffer.h"
#include "../tasks/tasks.h"

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

  // Снимаем флаги прерываний, т.к. снятие резета устанавливает TXIFG
  // Ну и на всякий случай.ы
  IFG2 &= ~(UCA0TXIFG | UCA0RXIFG);

  // Разрешаем прерывания по RX
  IE2 &= UCA0RXIE;
}

#pragma INTERRUPT (uartRxInterrupt);
#pragma vector = USCIAB0RX_VECTOR
void uartRxInterrupt()
{
  // IE2 &= ~UCA0RXIFG; Происходит автоматически, когда читаешь из RXBUF
  register bufferValueType tmp = UCA0RXBUF;
  cycledBufferPushBack(&rxBuffer, tmp);

  // Включаем вложенные прерыания, для таймера, он приоритетнее
  __bis_SR_register(GIE);

  // Ставим флаг того, что нужно сделать.
  taskRegister |= TASK_NEW_BYTE_ON_UART;
  // Будим контроллер по выходу из прерывания, для него появилась работа!
  __bis_SR_register_on_exit(GIE);
}

