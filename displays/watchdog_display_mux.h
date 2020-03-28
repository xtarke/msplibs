/*
 * watchdog_display_mux.h
 *
 *  Created on: Mar 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef DISPLAY_LEDDISPLAY_H_
#define DISPLAY_LEDDISPLAY_H_

#include <stdint.h>

#define COM_ANODO
//#define COM_CATODO

#define DISPLAYS_DATA_PORT P3
#define DISPLAYS_MUX_PORT P1


/**
  * @brief  Configura hardware.
  * @param  Nenhum
  *
  * @retval Nenhum.
  */
void watchdog_display_mux_init();

/**
  * @brief  Escreve nos displays de 7 segmentos.
  * @param  data: valor sem decimal sem conversão. Dados
  *             são convertidos internamente. data
  *             deve ser maior caso mais de dois displays.
  *
  * @retval Nenhum.
  */
void watchdog_display_mux_write(uint16_t data);

#endif /* DISPLAY_LEDDISPLAY_H_ */
