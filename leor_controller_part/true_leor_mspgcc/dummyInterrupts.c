/*
 * dummyInterrupts.c
 *
 *  Created on: 12.02.2013
 *      Author: Administrator
 */

#include <msp430g2553.h>

#pragma vector = ADC10_VECTOR, TIMER1_A0_VECTOR, TIMER1_A1_VECTOR, COMPARATORA_VECTOR, \
    WDT_VECTOR, NMI_VECTOR, PORT1_VECTOR, PORT2_VECTOR, USCIAB0TX_VECTOR
__interrupt void dummyPlug()
{
  P1DIR |= BIT0;
  P1OUT |= BIT0;

  while(1){}
}
