/*
 * simple_display_mux.h
 *
 *  Created on: Feb 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef DISPLAY_LEDDISPLAY_H_
#define DISPLAY_LEDDISPLAY_H_

#include <stdint.h>

#define COM_ANODO
//#define COM_CATODO

#define DISPLAYS_DATA_PORT_DIR P1DIR
#define DISPLAYS_DATA_PORT_OUT P1OUT

#define NUMBER_DISPLAYS 2
#define DISPLAYS_MUX_PORT_DIR P2DIR
#define DISPLAYS_MUX_PORT_OUT P2OUT

/**
  * @brief  Configura hardware.
  * @param  Nenhum
  *
  * @retval Nenhum.
  */
void watchdog_display_mux_init();

/**
  * @brief  Escrevre nos displays de 7 segmentos.
  * @param  data: valor sem decimal sem conversão. Dados
  *             são convertidos internamente. data
  *             deve ser maior caso mais de dois displays.
  *
  * @retval Nenhum.
  */
void display_mux_write(uint8_t data);


/**
  * @brief  Escrevre nos displays de 7 segmentos usando um laço for.
  * @param  data: valor sem decimal sem conversão. Dados
  *             são convertidos internamente. data
  *             deve ser maior caso mais de dois displays.
  *
  * @retval Nenhum.
  */
void display_mux_write_for(uint8_t data);

#endif /* DISPLAY_LEDDISPLAY_H_ */
