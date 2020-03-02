/*
 * 02_main_lcd.c
 *
 *  Created on: Mar 2, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina *
 */


#include <msp430.h>

#include <stdio.h>

#include "displays/lcd.h"

void main(){
    uint8_t i = 0;
    char string[8];

    /* Configuração de hardware */
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode  (para o FR2355)

    /* Inicializa hardare: veja lcd.h para
     * configurar pinos */
    lcd_init_4bits();
    /* Escreve string */

    lcd_send_data(LCD_LINE_1, LCD_CMD);
    lcd_write_string("MSP430");

    _delay_cycles(100000);

    while (1){
        lcd_send_data(LCD_LINE_0, LCD_CMD);
        snprintf(string, 8, "%d", i);

        lcd_write_string(string);
        i++;

        _delay_cycles(10000);
    }




}
