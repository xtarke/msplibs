/*
 * lcd.h
 *
 *  Created on: Feb 28, 2020
 *      Author: Renan Augusto Starke
 *      
 *      Adaptado de AVR e Arduino: Técnicas de Projeto, 2a ed. - 2012.
 *      Instituto Federal de Santa Catarina
 */

#ifndef _LCD_H
#define _LCD_H

#include <msp430.h>
#include <stdint.h>

#include "../lib/bits.h"

/* Configurações de hardware */

/* 0 para via de dados do LCD nos 4 LSBs do PORT empregado (Px0-D4, Px1-D5, Px2-D6, Px3-D7)
 * 1 para via de dados do LCD nos 4 MSBs do PORT empregado (Px4-D4, Px5-D5, Px6-D6, Px7-D7) */

#define DATA_NIBBLE 1
#define LCD_DATA_PORT P1

#define LCD_CTRL_PORT P2
#define E_PIN  BIT1
#define RS_PIN BIT0

/**
  * @brief  Configura hardware.
  * @param	Nenhum
  *
  * @retval Nenhum.
  */
void inic_LCD_4bits();


/**
  * @brief  Envia um dado estático para o display: caractere ou comando.
  * @param c: valor do comando.
  * @param cd: 0 para comando. 1 para caractere.
  *
  * @retval Nenhum
  */
void cmd_LCD(uint8_t c, uint8_t cd);

/**
  * @brief  Escreve um string estática (sem printf) no LCD. 
  * @param c: ponteiro para a string em RAM
  *
  * @retval Nenhum
  */
void escreve_LCD(char *c);

/**
  * @brief  Escreve um string estática (sem printf) no LCD. 
  * @param c: ponteiro para a string em FLASH
  *
  * @retval Nenhum
  */
void escreve_LCD_Flash(const char *c);


#endif
