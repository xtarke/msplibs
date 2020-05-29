/*
 *  i2c_master_g2553.c
 *
 *  Created on: May 28, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      - Biblioteca de comunicação I2C em modo Master
 *      - Baseado em msp430g2xx3_usci_i2c_standard_master.c de
 *      Nima Eskandari -- Texas Instruments Inc.
 *
 *                          .   .
 *                         /|\ /|\
 *               CI_xyz    10k 10k     MSP430G2xx3
 *              -------     |   |   -------------------
 *             |    SDA|<  -|---+->|P1.7/UCB0SDA       |-
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |    SCL|<----+-----|P1.6/UCB0SCL       |
 *              -------            |                   |
 *
 */
/* System includes */
#include <lib/uart_g2553.h>
#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>

/* Project includes */
#include "i2c_master_g2553.h"


#ifndef __MSP430G2553__
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
    /* Muda P1.6 e P1.7 para modo USCI_B0 */
    P1SEL |= BIT6 + BIT7;
    P1SEL2|= BIT6 + BIT7;

    /* Mantém controlador em reset */
    UCB0CTL1 |= UCSWRST;
    /* I2C Master, synchronous mode */
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
    /* Use SMCLK, keep SW reset */
    UCB0CTL1 = UCSSEL_2 + UCSWRST;

#ifdef CLOCK_16MHz
    /* fSCL = SMCLK/160 = ~100kHz */
    UCB0BR0 = 160;
    UCB0BR1 = 0;
#else
    #error "Clock system not defined for I2C Master Mode support"
#endif
    /* Dummy Slave Address */
    UCB0I2CSA = 0x01;
    /* Clear SW reset, resume operation */
    UCB0CTL1 &= ~UCSWRST;
    UCB0I2CIE |= UCNACKIE;
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
    IFG2 &= ~(UCB0TXIFG + UCB0RXIFG);       // Clear any pending interrupts
    IE2 &= ~UCB0RXIE;                       // Disable RX interrupt
    IE2 |= UCB0TXIE;                        // Enable TX interrupt

    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);              // Enter LPM0 w/ interrupts

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
    IFG2 &= ~(UCB0TXIFG + UCB0RXIFG);       // Clear any pending interrupts
    IE2 &= ~UCB0RXIE;                       // Disable RX interrupt
    IE2 |= UCB0TXIE;                        // Enable TX interrupt

    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);              // Enter LPM0 w/ interrupts

    return i2c_status.state;
}


void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    uint8_t copyIndex = 0;
    for (copyIndex = 0; copyIndex < count; copyIndex++)
    {
        dest[copyIndex] = source[copyIndex];
    }
}


//******************************************************************************
// I2C Interrupt For Received and Transmitted Data******************************
//******************************************************************************

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  if (IFG2 & UCB0RXIFG)                 // Receive Data Interrupt
  {
      //Must read from UCB0RXBUF
      uint8_t rx_val = UCB0RXBUF;

      if (i2c_status.rx_byte_count) {
          if (i2c_status.data_to_receive)
              i2c_status.data_to_receive[i2c_status.rx_index++] = rx_val;
          i2c_status.rx_byte_count--;
      }

      if (i2c_status.rx_byte_count == 1) {
          UCB0CTL1 |= UCTXSTP;
      }
      else if (i2c_status.rx_byte_count == 0) {
          IE2 &= ~UCB0RXIE;
          i2c_status.state = IDLE_MODE;
          __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
      }
  }
  else if (IFG2 & UCB0TXIFG)            // Transmit Data Interrupt
  {
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
              IE2 |= UCB0RXIE;              // Enable RX interrupt
              IE2 &= ~UCB0TXIE;             // Disable TX interrupt
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
                  IE2 &= ~UCB0TXIE;                       // disable TX interrupt
                  __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
              }
              break;

          default:
              __no_operation();
              break;
      }
  }
}


//******************************************************************************
// I2C Interrupt For Start, Restart, Nack, Stop ********************************
//******************************************************************************

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIAB0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (UCB0STAT & UCNACKIFG)
    {
        /* Limpa NACK flags, sinaliza NACK e acorda CPU */
        i2c_status.state = NACK_MODE;
        UCB0STAT &= ~UCNACKIFG;
        __bic_SR_register_on_exit(CPUOFF);
    }
    /* Stop or NACK Interrupt */
    if (UCB0STAT & UCSTPIFG)
    {
        /* Limpa START/STOP/NACK Flags */
        UCB0STAT &=  ~(UCSTTIFG + UCSTPIFG + UCNACKIFG);
    }
    if (UCB0STAT & UCSTTIFG)
    {
        /* Limpa START Flags */
        UCB0STAT &= ~(UCSTTIFG);
    }
}
