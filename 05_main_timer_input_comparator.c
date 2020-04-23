/*
 * 05_main_timer_input_comparator.c
 *
 *  Created on: Apr 22, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo de parcial de aplicação:
 *
 *      - Utiliza o evente de captura do Timer A para borda
 *      de subida e descida de uma porta.
 *
 *
 */


#include <msp430.h>

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

/* Pinos de hardware */
#define SENSOR_PORT P2
#define SENSOR_PIN BIT0

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


/**
 * @brief  Configura temporizador A.
 *
 * @param  none
 *
 * @retval none
 */
void config_timerA_1(){
    /* Timer tipo A:
     *
     * CAP ->  Capture mode: 1
     * CM_3 ->  Capture mode: 1 - both edges
     * SCS -> Capture sychronize
     * CCIS_0 -> Capture input select: 0 - CCIxA
     * CCIE -> Capture/compare interrupt enable
     *
     * Configura comparador 0 do timer 1 para captura */
    TA1CCTL0 = CAP | CM_3 | SCS | CCIS_0 | CCIE;

    /* TASSEL_2 ->  Timer A clock source select: 2 - SMCLK
     * MC_2 ->  Timer A mode control: 2 - Continous up
     * TACLR ->  Timer A counter clear
     * TAIE ->  Timer A counter interrupt enable (overflow)
     *
     * Configuração da fonte do clock do timer 1 */
    TA1CTL = TASSEL_2 | MC_2 | TACLR |  TAIE;
}


/* Variáveis globais */
volatile uint16_t overflow = 0;
volatile uint16_t rise_timestamp = 0;
volatile uint16_t fall_timestamp = 0;

volatile uint16_t interval = 0;

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
    config_timerA_1();

    P1DIR = BIT0;

    /* Pull up */
    P2REN = BIT0;
    P2OUT = BIT0;

    P2DIR = 0;
    P2SEL = BIT0;

    while(1)
    {
        __bis_SR_register(LPM0_bits + GIE);

        /* Código da aplicação */
    }
}


/* ISR0 do Timer A: executado no evento de comparação de captura  */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    /* Borda de subida */
    if (TA1CCTL0 & CCI)
    {
        rise_timestamp = TA1CCR0;
        interval = fall_timestamp - rise_timestamp;
        overflow = 0;


        /* Pino de depuração: na detecção do borda de subida */
        P1OUT ^= BIT0;

        /*  Exit LPM0 on return to main */
        __bic_SR_register_on_exit(LPM0_bits + GIE);
    }
    /* Borda de descida */
    else
    {
        fall_timestamp = TA1CCR0;
    }
}



/* ISR1 do Timer A: executado toda a vez que o temporizador estoura, evento do comparador 1 ou evento do comparador 2 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TIMER1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TA1IV,0x0A))
    {
    /* Vector  0:  No interrupt */
    case  TA1IV_NONE:
        break;

        /* Vector  2:  TACCR1 CCIFG -> Comparação 1*/
    case  TA1IV_TACCR1:

        break;
        /* Vector  4:  TACCR2 CCIFG -> Comparação 2*/
    case TA1IV_TACCR2:
        break;

        /* Vector  6:  Reserved  */
    case TA1IV_6:
        break;

        /* Vector  8:  Reserved  */
    case TA1IV_8:
        break;

        /* Vector 10:  TAIFG -> Overflow do timer 0*/
    case TA1IV_TAIFG:
        overflow++;

        break;
    default:
        break;
    }
}
