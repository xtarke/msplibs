/*
 * 02_simple_display_mux.c
 *
 *  Created on: Feb 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      Exemplo de aplicação:
 *
 *      - Utiliza o WatchDog Timer
 *      para multiplexar 4 displays de 7 segmentos.
 *      - Utiliza IRQ externa para contar número
 *      de pulsos exibindo-os no display.
 *
 *
 */

#include <msp430.h>

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

#define BUTTON  BIT0
#define BUTTON_PORT P1

#include "lib/gpio.h"
#include "displays/simple_display_mux.h"

volatile uint16_t i = 0;

#define PULSES  BIT0

void config_ext_irq(){
    /* Pull up/down */
    P2REN = PULSES;

    /* Pull up */
    P2OUT = PULSES;

    /* Habilitação da IRQ apenas botão */
    P2IE =  PULSES;

    /* Transição de nível alto para baixo */
    P2IES = PULSES;

    /* Limpa alguma IRQ pendente */
    P2IFG &= ~PULSES;
}


/**
  * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
  *         Utililiza-se as calibrações internas gravadas na flash.
  *         Exemplo baseado na documentação da Texas: msp430g2xxx3_dco_calib.c
  *         Configura ACLK para utilizar VLO = ~10KHz
  * @param  none
  *
  * @retval none
  */
void init_clock_system(){

    /* Configuração do MCLK em 8MHz */

    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    /* Configura ACLK para usar VLO: ~10kHZ */
    BCSCTL3 |= LFXT1S_2;
}

void main(void)
{
    /* Para o watchdog timer
     * Necessário para código em depuração */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    init_clock_system();
    config_ext_irq();

    /* Inicializa displays */
    watchdog_display_mux_init();

    /* Entra em modo de economia de energia */
    __bis_SR_register(LPM0_bits + GIE);
}

/* Port 1 ISR (interrupt service routine) */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{
    /* Liga/desliga LED quando detectado borda no botão */
    watchdog_display_mux_write(i++);

    /* Limpa sinal de IRQ do botão 0 */
    P2IFG &= ~PULSES;
}

