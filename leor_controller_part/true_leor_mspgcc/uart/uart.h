/*
 * uart.h
 *
 *  Created on: 11.02.2013
 *      Author: Administrator
 */

#ifndef UART_H_
#define UART_H_

void UARTinit9600on4mhzSMCLK();

//! ����������, ������ ����, �������!
void UARTinitListen();

//! � ���� � ���!
void UARTstopListen();

#endif /* UART_H_ */
