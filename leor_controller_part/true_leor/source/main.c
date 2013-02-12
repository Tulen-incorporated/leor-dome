/*
 * main.c
 */

#include <msp430.h>
#include "servoTimer/servoTimer.h"

#pragma FUNC_NEVER_RETURNS (main);
void main(void) {

  // ========================================
  // ������������� �������!
  // ========================================
  WDTCTL = WDTPW | WDTHOLD;
  _BIS_SR(GIE);

  // ��� ����� �� OUT � �� ����,
  // ����� �� ����� �������������, �����, ��, ������� ����� - �������������.
  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P1OUT = 0x00;
  P2OUT = 0x00;

  // ========================================
  // ������������� ������.
  // ========================================
  // ������ �������� ��������� (DCO) �� 16���.
  DCOCTL = CALDCO_16MHZ;
  BCSCTL1 = CALBC1_16MHZ;

  BCSCTL2 = SELM_0 | DIVM_0 | /*~SELS |*/ DIVS_2 | 0;
  // MCLK �� DCO | �������� MCLK �� 1 | SMCLK �� DCO | �������� SMCLK �� 4 | DCO �� ���������� ���������.
  //BSCTL3� ����������, �.�. �� �� ���������� ACLK.

  /* ������ � ��� ���� ������ ���� (MCLK) �� 16��� � ��������� ���� (SMCLK) �� 4���. */

  initServosOnTimerA0SMCLK4mHz();

}
