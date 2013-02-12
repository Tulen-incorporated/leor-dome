/*
 * dummyInterrupts.c
 *
 *  Created on: 12.02.2013
 *      Author: Administrator
 */

#include <msp430.h>

// Если мы попали в какой-то из этих интерруптов, что-то не так, зажгем лампочку! KERNEL PANIC!
#pragma INTERRUPT (dummyPlug);
#pragma vector = ADC10_VECTOR, TIMER1_A0_VECTOR, TIMER1_A1_VECTOR, COMPARATORA_VECTOR, \
    WDT_VECTOR, NMI_VECTOR, PORT1_VECTOR, PORT2_VECTOR, USCIAB0TX_VECTOR
#pragma FUNC_NEVER_RETURNS (dummyPlug);
void dummyPlug()
{
  P1DIR |= BIT0;
  P1OUT |= BIT0;

  while(1){}
}
