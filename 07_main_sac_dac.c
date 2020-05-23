/*
 * 07_main_sac_dac.c
 *
 *  Created on: May 21, 2020
 *      Author: Renan Augusto Starke
 *
 *      Exemplo simples de utilização do DAC.
 *      DAC faz parte do Combo analógico,
 *      portando a saída do conversor deve
 *      ser ligado ao PGA e o PGA ao pino
 *
 *              MSP430FR235x
 *            -----------------
 *        /|\|              XIN|-
 *         | |                 |
 *         --|RST          XOUT|-
 *           |                 |
 *           |       P1.1/OA0O | -->   DAC12 Ligado na saída do Ampop (PGA)
 *           |                 |
 *           |                 |
 */


#include <msp430.h>
#include <stdint.h>

#include "lib/bits.h"

#ifndef __MSP430FR2355__
#error "Clock system not supported for this device"
#endif

/**
 * @brief  Configura o DAC12.
 *
 * @param  none
 *
 * @retval none
 */
void init_dac(){
    /* MSP430FR2355 possui referências internas de tensão
     * que devem ser habilitadas no Power Management Module (PMM)
     *
     * Configurações abaixo ligam a referência de 2.5V internas
     */

    PMMCTL0_H = PMMPW_H;                      // Unlock the PMM registers
    PMMCTL2 = INTREFEN | REFVSEL_2;           // Enable internal 2.5V reference
    while(!(PMMCTL2 & REFGENRDY));            // Poll till internal reference settles

    /* SAC0DAC: Seleciona Vref interno e trigger do DAC por software (escrita em SAC0DAC)
     * DACSREF_1: Select int Vref as DAC reference
     *
     * SAC0DAT: Initial DAC data
     * SAC0DAC: Enable DAC
     *
     */

    SAC0DAC = DACSREF_1;
    SAC0DAT = 0;
    SAC0DAC |= DACEN;

    /* Para habilitar IRQ e trigger por um timer:
     *
     * SAC0DAC = DACSREF_1 + DACLSEL_2 + DACIE;  // Select int Vref as DAC reference
     *
     */
}

/**
  * @brief  Configura o Programmable Gain Amplifier (PGA).
  *
  * @param  none
  *
  * @retval none
  */
void init_pga(){
    /* SAC0OA: SAC OA Control Register *
     * NMUXEN: SAC Negative input MUX controL
     * PMUXEN: SAC Positive input MUX control
     * PSEL_1: 12-bit reference DAC source selected
     * NSEL_1: PGA source selected
     * OAPM  : SAC OA power mode selection: Select low speed and low power mode
     * SACEN: SAC eanble
     * OAEN: Output enable
     *
     * SAC0PGA: SAC PGA Control Register     *
     * MSEL_1: Set OA as buffer mode
     *
     * Ampop em modo seguidor (buffer)
                        |+
         DAC12  + --(+)-|   +
                        |      +-------  P1.1/OA0O    (Ligado ao pino P1.1)
                    (-)-|   +
                        |+
     */
    SAC0OA = NMUXEN + PMUXEN + PSEL_1 + NSEL_1;
    SAC0OA |= OAPM;
    SAC0PGA = MSEL_1;
    SAC0OA |= SACEN + OAEN;

    /* Configuração da função alternativa dos pinos
     *
     * Nesse caso apenas configura-se a saída (P1.1/OA0O)
     * Demais sinais não estão conectados aos pinos externos.
     *
     * Se necessário, liga-se todos assim:
     *
     * P1SEL0 |= BIT1 | BIT2 | BIT3;
     * P1SEL1 |= BIT1 | BIT2 | BIT3
     */

    P1SEL0 |= BIT1;
    P1SEL1 |= BIT1;
}


int main(void)
{
    uint16_t dac_data=0;

    /* Desabilita Watch dog */
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watch dog timer

    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;

    init_dac();
    init_pga();

    /* LED de depuração do main */
    P1DIR |= BIT0;

    while (1){
        /* Led de debug */
        CPL_BIT(P1OUT,BIT0);

        /* Incrementa e escreve novo dado no DAC */
        dac_data++;
        dac_data &= 0xFFF;
        SAC0DAT = dac_data;

        _delay_cycles(100000);
    }
}



/* ISR do DAC12. Executada quando o valor escrito em  SAC0DAT é lido pelo hardware
 *
 * Não é usado nesse exemplo, contudo é necessário quando para evitar que a CPU
 * escreva muito rápido no DAC
 *  */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = SAC0_SAC2_VECTOR
__interrupt void SAC0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SAC0_SAC2_VECTOR))) SAC0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(SAC0IV,SACIV_4))
    {
    case SACIV_0: break;
    case SACIV_2: break;
    case SACIV_4:

        break;
    default: break;
    }
}
