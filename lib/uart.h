/*
 * uart.h
 *
 *  Created on: May 25, 2020
 *      Author: Renan Augusto Starke
 */

#ifndef LIB_UART_H_
#define LIB_UART_H_

#define CLOCK_1MHz

void init_uart();
void uart_send_data(char data);

#endif /* LIB_UART_H_ */
