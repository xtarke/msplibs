/*
 * relogio.h
 *
 *  Created on: Apr 17, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef RELOGIO_H_
#define RELOGIO_H_

#include <msp430.h>

/* Mapa de hardware */
#define RELOGIO_BUTTONS_PORT P3
#define BUTTON_ADJ  BIT3
#define BUTTON_MIN  BIT4
#define BUTTON_HORA BIT5

void init_relogio();

uint8_t obter_segundos();
uint8_t obter_minutos();
uint8_t obter_horas();
uint8_t em_ajuste();

#endif /* RELOGIO_H_ */
