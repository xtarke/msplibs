/*
 * 02_main_lcd.c
 *
 *  Created on: Mar 2, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina *
 */


#include <msp430.h>

#include <stdio.h>

#include "lib/gpio.h"
#include "displays/lcd.h"

#define BUTTON_PORT P2
#define BUTTON_0  BIT5

volatile uint8_t pulses = 0;

void config_ext_irq(){
    /* Pull up/down */
    PORT_REN(BUTTON_PORT) = BUTTON_0;

    /* Pull up */
    PORT_OUT(BUTTON_PORT) = BUTTON_0;

    /* Habilitação da IRQ apenas botão */
    PORT_IE(BUTTON_PORT) =  BUTTON_0;

    /* Transição de nível alto para baixo */
    PORT_IES(BUTTON_PORT) = BUTTON_0;

    /* Limpa alguma IRQ pendente */
    PORT_IFG(BUTTON_PORT) &= ~BUTTON_0;
}

void main(){
    char string[8];

    /* Configuração de hardware */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Inicializa hardare: veja lcd.h para
     * configurar pinos */
    lcd_init_4bits();
    /* Escreve string */

    x = PIN15;

    /* Configura interupções */
    config_ext_irq();

    /* Habilita IRQs e desliga CPU */
    __bis_SR_register(GIE);

    lcd_send_data(LCD_LINE_1, LCD_CMD);
    lcd_write_string("MSP430");

    _delay_cycles(100000);

    while (1){
        lcd_send_data(LCD_LINE_0, LCD_CMD);
        snprintf(string, 8, "%d", pulses);

        lcd_write_string(string);
        //i++;

        _delay_cycles(10000);
    }

}

/* Port 1 ISR (interrupt service routine) */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
    pulses++;

    /* Limpa sinal de IRQ do botão 0 */
    PORT_IFG(BUTTON_PORT) &= ~BUTTON_0;
}
