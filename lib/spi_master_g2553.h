/*
 * uart.h
 *
 *  Created on: May 25, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef LIB_UART_G2553_H_
#define LIB_UART_G2553_H_

#include <stdint.h>

#define CLOCK_16MHz

#define SLAVE_CS_PORT   P1
#define SLAVE_CS_PIN    BIT0

void init_spi_master_mode();
void spi_master_send_package(uint8_t *data, uint8_t size);
void spi_master_receive_package(uint8_t *data, uint8_t size);
void spi_master_send_receive_package(uint8_t *tx_data, uint8_t *rx_data, uint8_t size);

#endif /* LIB_UART_G2553_H_ */
