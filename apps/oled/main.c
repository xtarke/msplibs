/*
 * main.c
 *
 *  Created on: Jul 25, 2025
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - OLED SDD1366 aplicação exemplo.
 *      - Portado do repositório:
 *      - https://github.com/xtarke/msp430_thermo_hygrometer/tree/main
 *
 *           OLED SSD1306              MSP430G2553
 *             +-------+           +-------------------+
 *             |    SDA|<  -|---+->|P3.1/UCB0SDA       |        
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |    SCL|<----+-----|P3.2/UCB0SCL   P1.0| -> Led piscando
 *              -------            |                   |
 */

/* System includes */
#include <string.h>
#include <msp430.h>
#include <stdint.h>
#include "intrinsics.h"

/* Project includes */
#include "./lib/i2c_master_g2553.h"
#include "./lib/bits.h"
#include "./lib/gpio.h"
#include "displays/ssd1366.h"

#define LED BIT0
#define LED_PORT P1


/**
 * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
 *         Utililiza-se as calibrações internas gravadas na flash.
 *         Exemplo baseado na documentação da Texas: msp430g2xxx3_dco_calib.c  *
 * @param  none
 *
 * @retval none
 */
void init_clock_system(){

#ifdef CLOCK_1MHz
    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_1MHZ==0xFF)
        while(1);
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
#endif

#ifdef CLOCK_8MHz

    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    /* Outras fonte de clock devem ser configuradas também *
     * de acordo com a aplicação  */
#endif

#ifdef CLOCK_12MHz
    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_12MHZ==0xFF)
        while(1);
    DCOCTL = 0;
    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;
#endif

#ifdef CLOCK_16MHz
    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_16MHZ==0xFF)
        while(1);
    DCOCTL = 0;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
#endif

    /* Configure ACLK as VLO: ~12KHz
     * LFXT1 = VLO */
    BCSCTL3 |= LFXT1S_2;

}

/**
 * @brief  Configura temporizador watchdog como interrupção temporizada.
 *         Código no vetor __interrupt void watchdog_timer(void)
 *
 * @param  none
 *
 * @retval none
 */
void config_wd_as_timer(){
    /* Configura Watch dog como temporizador:
     *
     * WDT_ADLY_250 <= (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0)
     *
     * WDTPW -> "Senha" para alterar confgiuração.
     * WDTTMSEL -> Temporizador ao invés de reset.
     * WDTSSEL -> Fonte de clock de ACLK
     * WDTIS1+WDTIS0 -> Clock / 8192
     *
     */
    WDTCTL = WDT_ADLY_1000;
    /* Ativa IRQ do Watchdog */
    IE1 |= WDTIE;
}


int main(void) {
    /* Desliga Watchdog */
    WDTCTL = WDTPW | WDTHOLD;

    init_clock_system();
    init_i2c_master_mode();
    config_wd_as_timer();

    /* Debug LED */
    P1DIR |= BIT0;
    SET_BIT(P1OUT,BIT0);

    /* Aguarda a energização do display */
    __delay_cycles(160000);
    __delay_cycles(160000);
    __delay_cycles(160000);
    __delay_cycles(160000);

    /* Habilita todas as interrupções */
    __bis_SR_register(GIE);

    /* Inicializa o display */
    ssd1306_init();

    /* O display OLED necessita de 1024 bytes de frame buffer, *
     * O MSP430G253 possui 512 bytes de RAM, portanto a display
     * foi divido em 4 partes (linhas) de 256 bytes:
     * Veja: oled_partition_t em SDD1306.h
     */

    /* Escreve MSP na linha 1 */
    /* Os pixels são desenhados em ram interna do MCU *
     * e enviados para o display pela função
     * ssd1306_display_data(oled_partition_t line)
     */
    /* Escreve MSP na linha 1: *
     * a fonte é 16x16, logo cada caractere deve
     * ter um offset de 16 pixels em x e y */
    ssd1306_write_scaled_char(0,0, 'M',2);
    ssd1306_write_scaled_char(16,0, 'S',2);
    ssd1306_write_scaled_char(32,0, 'P',2);
    ssd1306_display_data(LINE_1);

    /* Escreve 430 na linha 2 */
    ssd1306_write_scaled_char(0,0, '4',2);
    ssd1306_write_scaled_char(16,0, '3',2);
    ssd1306_write_scaled_char(32,0, '0',2);
    ssd1306_display_data(LINE_2);

    /* Escreve IFSC na linha 3 */
    ssd1306_write_scaled_char(0,0, 'I',2);
    ssd1306_write_scaled_char(16,0, 'F',2);
    ssd1306_write_scaled_char(32,0, 'S',2);
    ssd1306_write_scaled_char(48,0, 'C',2);
    ssd1306_display_data(LINE_3);

    /* Escreve DAELN na linha 4 */
    ssd1306_write_scaled_char(0,0, 'D',2);
    ssd1306_write_scaled_char(16,0, 'A',2);
    ssd1306_write_scaled_char(32,0, 'E',2);
    ssd1306_write_scaled_char(48,0, 'L',2);
    ssd1306_write_scaled_char(64,0, 'N',2);
    ssd1306_display_data(LINE_4);

    uint16_t contador = 0;

    while (1){
        /* Coloque aqui a sua aplicação
         * exibindo dados dos sensores, botões, etc...
         * Atualiza somente as variávies que são alteradas
         * evitando excesso de transações no I2C.
         *
         * Exemplo: contador */
        uint16_t temp = 0;
        uint8_t digits[3];

        temp = contador;

        /* Separa os digitos */
        for (int i=2; i >= 0; i--){
            digits[i] = temp % 10;
            temp = temp / 10;
        }

        ssd1306_write_scaled_char(0,0, 'M',2);
        ssd1306_write_scaled_char(16,0, 'S',2);
        ssd1306_write_scaled_char(32,0, 'P',2);
        ssd1306_write_scaled_char(48,0, ' ',2);
        /* Envia cada dígito para o display */
        ssd1306_write_scaled_char(64,0, '0' + digits[0],2);
        ssd1306_write_scaled_char(80,0, '0' + digits[1],2);
        ssd1306_write_scaled_char(96,0, '0' + digits[2],2);
        ssd1306_display_data(LINE_1);

        contador++;

        /* Conta no máximo até 255 */
        contador &= 0xff;

        /* Desliga a CPU. Lembre-se de ligar com a instrução
         * __bic_SR_register_on_exit(CPUOFF); em alguma interrupção,
         * senão o main não executará */
        __bis_SR_register(LPM0_bits + GIE);        
    }

}

/* ISR do watchdog: executado toda a vez que o temporizador estoura
 * WDT é usado como temporizador normal */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    static uint16_t x = 0;

    /* Pisca o LED */
    PORT_OUT(LED_PORT) ^= LED;

    /* Duas interrupções para acordar o main */
    if (x >= 2) {
        x = 0;
        __bic_SR_register_on_exit(CPUOFF);
    }
    x++;
}
