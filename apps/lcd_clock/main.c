/*
 * 02_main_lcd.c
 *
 *  Created on: Mar 2, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina *
 */

/* Includes de sistema */
#include <msp430.h>
#include <stdio.h>

/* Includes do projeto */
#include "lcd.h"
#include "relogio.h"

void init_clock_system(){

    /* Configuração do MCLK em 8MHz */

    /* Se calibração foi apagada, para aplicação
    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ; */

    /* Configura ACLK para usar VLO: ~10kHZ */
    BCSCTL3 |= LFXT1S_2;
}



void main(){
    char string[12];

    /* Configuração de hardware */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Systema de clock e applicação */
    init_clock_system();
    init_relogio();

    /* Inicializa hardare: veja lcd.h para
     * configurar pinos */
    lcd_init_4bits();

    /* Escreve string */
    lcd_send_data(LCD_LINE_1, LCD_CMD);
    lcd_write_string("MSP430");

    __bis_SR_register(LPM0_bits + GIE);

    while (1){
        lcd_send_data(LCD_LINE_0, LCD_CMD);

        /* Para o snprintf funcionar com modificadores:
         * Configuração do projeto -> MSP430 Compiler -> Edit Flags -> Mudar para: printf_support=nofloat   */
        snprintf(string, 12, "%02d:%02d:%02d", obter_horas(), obter_minutos(), obter_segundos());
        lcd_write_string(string);

        if (em_ajuste())
           lcd_send_data('*', LCD_DATA);
       else
           lcd_send_data(' ', LCD_DATA);

        __bis_SR_register(LPM0_bits);
    }
}
