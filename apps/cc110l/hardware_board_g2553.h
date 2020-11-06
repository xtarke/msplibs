/*
 * hardware_board_g2553.c
 *
 *  Created on: Nov 06, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - Configurações dos pinos.
 *
 *      - Baseado no MSP430/CC1100-2500 Interface Code Library v1.1
 *
 */


#ifndef HARDWARE_BOARD_H_
#define HARDWARE_BOARD_H_

#define TI_CC_LED_PxOUT         P2OUT
#define TI_CC_LED_PxDIR         P2DIR
#define TI_CC_LED1              BIT2
#define TI_CC_LED2              BIT3

#define TI_CC_SW_PxIN           P1IN
#define TI_CC_SW_PxIE           P1IE
#define TI_CC_SW_PxIES          P1IES
#define TI_CC_SW_PxIFG          P1IFG
#define TI_CC_SW_PxREN          P1REN
#define TI_CC_SW_PxOUT          P1OUT
#define TI_CC_SW1               BIT3

#define TI_CC_GDO0_PxOUT        P2OUT
#define TI_CC_GDO0_PxIN         P2IN
#define TI_CC_GDO0_PxDIR        P2DIR
#define TI_CC_GDO0_PxIE         P2IE
#define TI_CC_GDO0_PxIES        P2IES
#define TI_CC_GDO0_PxIFG        P2IFG
#define TI_CC_GDO0_PIN          BIT6

#define TI_CC_GDO1_PxOUT        P3OUT
#define TI_CC_GDO1_PxIN         P3IN
#define TI_CC_GDO1_PxDIR        P3DIR
#define TI_CC_GDO1_PIN          0x04

#define TI_CC_GDO2_PxOUT        P1OUT
#define TI_CC_GDO2_PxIN         P1IN
#define TI_CC_GDO2_PxDIR        P1DIR
#define TI_CC_GDO2_PIN          BIT0

#define TI_CC_CSn_PxOUT         P2OUT
#define TI_CC_CSn_PxDIR         P2DIR
#define TI_CC_CSn_PIN           BIT7

// USCIB0 for Launchpad w/USCI
#define TI_CC_SPI_USCIB0_PxSEL  P1SEL
#define TI_CC_SPI_USCIB0_PxSEL2  P1SEL2
#define TI_CC_SPI_USCIB0_PxDIR  P1DIR
#define TI_CC_SPI_USCIB0_PxIN   P1IN
#define TI_CC_SPI_USCIB0_SIMO   BIT7
#define TI_CC_SPI_USCIB0_SOMI   BIT6
#define TI_CC_SPI_USCIB0_UCLK   BIT5

//******************************************************************************
//  These constants are used to identify the chosen SPI and UART interfaces.
//******************************************************************************
#define TI_CC_SER_INTF_NULL    0
#define TI_CC_SER_INTF_USART0  1
#define TI_CC_SER_INTF_USART1  2
#define TI_CC_SER_INTF_USCIA0  3
#define TI_CC_SER_INTF_USCIA1  4
#define TI_CC_SER_INTF_USCIA2  5
#define TI_CC_SER_INTF_USCIA3  6
#define TI_CC_SER_INTF_USCIB0  7
#define TI_CC_SER_INTF_USCIB1  8
#define TI_CC_SER_INTF_USCIB2  9
#define TI_CC_SER_INTF_USCIB3  10
#define TI_CC_SER_INTF_USI     11
#define TI_CC_SER_INTF_BITBANG 12


//******************************************************************************
// Select which port will be used for interface to CCxxxx
//******************************************************************************
#define TI_CC_RF_SER_INTF       TI_CC_SER_INTF_USCIB0  // Interface to CCxxxx


#endif /* HARDWARE_BOARD_H_ */

