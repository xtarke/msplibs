/*
 *  i2c_master_g2553.c
 *
 *  Created on: May 28, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      - Biblioteca de comunicação I2C em modo Master
 *
 *                          .   .
 *                         /|\ /|\
 *               CI_xyz    10k 10k     MSP430FR2355
 *              -------     |   |   -------------------
 *             |    SDA|<  -|---+->| P1.2/UCB0SDA      |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |    SCL|<----+-----| P1.3/UCB0SCL      |
 *              -------            |                   |
 *
 */
/* System includes */
#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>

/* Project includes */
#include "i2c_master_fr2355.h"


#ifndef __MSP430FR2355__
#error "Library no supported/validated in this device."
#endif

struct i2c_status_t {
    /* Used to track the state of the software state machine*/
    i2c_mode state;
    /* Device Addr */
    uint8_t device_addr;
    /* RX: Pointers and index */
    uint8_t *data_to_receive;
    uint8_t rx_byte_count;
    uint8_t rx_index;
    /* TX: Pointers and index */
    uint8_t *data_to_send;
    uint8_t tx_byte_count;
    uint8_t tx_index;
};

/* Estado do módulo I2C */
volatile struct i2c_status_t i2c_status = {0};

void init_i2c_master_mode()
{
    /* Muda P1.6 e P1.7 para modo UCB0 */
    P1SEL0 |= BIT2 | BIT3;

    /* Mantém controlador em reset */
    UCB0CTLW0 |= UCSWRST;
    /* I2C Master, synchronous mode */
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSYNC; // I2C mode, Master mode, sync
    /* Automatic stop generated  after UCB0TBCNT is reached */
    UCB0CTLW1 |= UCASTP_2;


#ifdef CLOCK_24MHz
    UCB0BRW = 240;                       // baudrate = SMCLK / 240
#else
#error "Clock system not defined for I2C Master Mode support"
#endif

    /* Number of bytes to be received */
    //UCB0TBCNT = 0x0005;
    /* Dummy Slave Address */
    UCB0I2CSA = 0x01;
    UCB0CTL1 &= ~UCSWRST;
    UCB0IE |= UCSTPIE | UCRXIE | UCTXIE | UCNACKIE | UCBCNTIE;

}


/**
 * @brief  Lê registradores de um dispositivo I2C.
 *         Utiliza IRQ de transmissão para o envio dos bytes.
 *
 *         Use com IRS habilitadas.
 *
 * @param  dev_addr: endereço I2C dos dispositivo.
 *         reg_addr: registrador inicial.
 *         count: número de bytes.
 *         data: vetor onde será armazenado os dados recebidos.
 *
 * @retval i2c_mode: possíveis erros de transmissão.
 */
i2c_mode i2c_master_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count, uint8_t *data)
{
    /* Initialize state machine */
    i2c_status.state = TX_REG_ADDRESS_MODE;
    i2c_status.data_to_receive = data;

    i2c_status.device_addr = reg_addr;
    i2c_status.rx_byte_count = count;
    i2c_status.tx_byte_count = 0;
    i2c_status.rx_index = 0;
    i2c_status.tx_index = 0;


    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCSTPIE | UCRXIE | UCTXIE | UCNACKIE | UCBCNTIE);

    UCB0CTL1 |= UCTR + UCTXSTT;
    __bis_SR_register(CPUOFF + GIE);

    return  i2c_status.state;
}

/**
 * @brief  Escreve um byte no barramento I2C.
 *         Utiliza IRQ de transmissão para o envio do byte.
 *
 *         Use com IRS habilitadas.
 *
 * @param  dev_addr: endereço I2C dos dispositivo.
 *         byte: byte a ser envidado.
 *
 * @retval i2c_mode: possíveis erros de transmissão.
 */
i2c_mode i2c_write_single_byte(uint8_t dev_addr, uint8_t byte){
    return i2c_master_write_reg(dev_addr, byte, 0, NULL);
}


/**
 * @brief  Escreve nos registradores de um dispositivo I2C.
 *         Utiliza IRQ de transmissão para o envio dos bytes.
 *
 *         Use com ISR habilitadas.
 *
 * @param  dev_addr: endereço I2C dos dispositivo.
 *         reg_addr: registrador inicial.
 *         reg_data: dados enviados. Devem permanacer estáticos durante a transmissão.
 *         count: número de bytes.
 *
 * @retval i2c_mode: possíveis erros de transmissão.
 */
i2c_mode i2c_master_write_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    /* Initialize state machine */
    i2c_status.state = TX_REG_ADDRESS_MODE;
    i2c_status.device_addr = reg_addr;
    i2c_status.data_to_send = reg_data;

    /* Use pointers from main:
     *
     * Copy register data to TransmitBuffer
     *
    CopyArray(reg_data, TransmitBuffer, count); */

    i2c_status.tx_byte_count = count;
    i2c_status.rx_byte_count = 0;
    i2c_status.rx_index = 0;
    i2c_status.tx_index = 0;


    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCSTPIE | UCRXIE | UCTXIE | UCNACKIE | UCBCNTIE);

    /* I2C TX, start condition */
    UCB0CTL1 |= UCTR + UCTXSTT;

    __bis_SR_register(GIE);

    return i2c_status.state;
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCIB0_ISR(void)
{
    uint8_t data;

    switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_NONE:             // No interrupts break;
            break;
        case USCI_I2C_UCALIFG:      // Arbitration lost
            /* Unblock if NACK is receive */
            i2c_status.state = NACK_MODE;
            break;
        case USCI_I2C_UCSTTIFG:     // START condition detected with own address (slave mode only)
            break;
        case USCI_I2C_UCSTPIFG:     // STOP condition detected (master & slave mode)
            /* Unblock when stop condition is receiveid */
            i2c_status.state = IDLE_MODE;
           break;
        case USCI_I2C_UCRXIFG3:     // RXIFG3
            break;
        case USCI_I2C_UCTXIFG3:     // TXIFG3
            break;
        case USCI_I2C_UCRXIFG2:     // RXIFG2
            break;
        case USCI_I2C_UCTXIFG2:     // TXIFG2
            break;
        case USCI_I2C_UCRXIFG1:     // RXIFG1
            break;
        case USCI_I2C_UCTXIFG1:     // TXIFG1
            break;
        case USCI_I2C_UCRXIFG0:     // RXIFG0
            data =  UCB0RXBUF;

            if (i2c_status.rx_byte_count) {
                if (i2c_status.data_to_receive)
                    i2c_status.data_to_receive[i2c_status.rx_index++] = data;
                i2c_status.rx_byte_count--;
            }
            if (i2c_status.rx_byte_count == 1) {
                UCB0CTL1 |= UCTXSTP;
            }
            else if (i2c_status.rx_byte_count == 0) {
                //IE2 &= ~UCB0RXIE;
                i2c_status.state = IDLE_MODE;
                __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
            }


            break; // Vector 24: RXIFG0 break;
        case USCI_I2C_UCTXIFG0:     // TXIFG0
            switch (i2c_status.state)
            {
                case TX_REG_ADDRESS_MODE:
                    UCB0TXBUF = i2c_status.device_addr;
                    if (i2c_status.rx_byte_count)
                        i2c_status.state = SWITCH_TO_RX_MODE;   // Need to start receiving now
                        else
                            i2c_status.state = TX_DATA_MODE;        // Continue to transmision with the data in Transmit Buffer
                    break;

                case SWITCH_TO_RX_MODE:
                    UCB0CTL1 &= ~UCTR;            // Switch to receiver
                    i2c_status.state = RX_DATA_MODE;    // State state is to receive data
                    UCB0CTL1 |= UCTXSTT;          // Send repeated start
                    if (i2c_status.rx_byte_count == 1) {
                        //Must send stop since this is the N-1 byte
                        while((UCB0CTL1 & UCTXSTT));
                        UCB0CTL1 |= UCTXSTP;      // Send stop condition
                    }
                    break;

                case TX_DATA_MODE:
                    if (i2c_status.tx_byte_count) {
                        UCB0TXBUF = i2c_status.data_to_send[i2c_status.tx_index++];
                        i2c_status.tx_byte_count--;
                    }
                    else {
                        //Done with transmission
                        UCB0CTL1 |= UCTXSTP;     // Send stop condition
                        i2c_status.state = IDLE_MODE;
                        __bic_SR_register_on_exit(CPUOFF);
                    }
                    break;

                default:
                    __no_operation();
                    break;
            }

                break;
            case USCI_I2C_UCBCNTIFG:    // Byte count limit reached (UCBxTBCNT)
                break;
            case USCI_I2C_UCCLTOIFG:    // Clock low timeout - clock held low too long
                break;
            case USCI_I2C_UCBIT9IFG:    // Generated on 9th bit of a transmit (for debugging)
                break;
            default:
                break;
            }
    }
