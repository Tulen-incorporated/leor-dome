/*
 * main.c
 */

#include <msp430g2553.h>
#include "servoTimer/servoTimer.h"

#pragma FUNC_NEVER_RETURNS (main);
void main(void) {

  // ========================================
  // Останавливаем вотчдог!
  // ========================================
  WDTCTL = WDTPW | WDTHOLD;
  _BIS_SR(GIE);

  // Все порты на OUT и на ноль,
  // чтобы не жрали електричество, потом, те, которые нужны - переопределим.
  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P1OUT = 0x00;
  P2OUT = 0x00;

  // ========================================
  // Инициализация клоков.
  // ========================================
  // ставим цифровой осцилятор (DCO) на 16мгц.
  DCOCTL = CALDCO_16MHZ;
  BCSCTL1 = CALBC1_16MHZ;

  BCSCTL2 = SELM_0 | DIVM_0 | /*~SELS |*/ DIVS_2 | 0;
  // MCLK от DCO | Делитель MCLK на 1 | SMCLK от DCO | Делитель SMCLK на 4 | DCO на внутреннем резисторе.
  //BSCTL3е интересует, т.к. мы не использует ACLK.

  /* Теперь у нас есть мастер клок (MCLK) на 16мгц и сабмастер клок (SMCLK) на 4мгц. */

  initServosOnTimerA0SMCLK4mHz();

}
