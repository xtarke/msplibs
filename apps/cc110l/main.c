/*
 * main.c
 *
 *  Created on: Nov 06, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      - Exemplo de recepção e transmissão de dados pelo Radio CC110L.
 *      - CPU é desligado até o recebimento ou envio dos dados.
 *      - Utiliza o Lauchpad ESP-MSP430G2 com MSP430G2553
 *      - CC110L Air Module BoosterPack
 *
 *      - Clock da CPU é 1 MHZ.
 *
 *
 *               MSP430G2553
 *            -----------------
 *        /|\|            P2.7 |-> Slave Chip Select (GPIO)
 *         | |                 |
 *         --|RST         P2.6 |<- GDO0 (Dados recebidos pelo radio - IRQ)
 *           |                 |
 *           |            P1.6 |-> Data Out (UCB0SIMO)
 *           |                 |
 *           |            P1.7 |<- Data In (UCABSOMI)
 *           |                 |
 *           |            P1.5 |-> Serial Clock Out (UCB0CLK)
 *           |                 |
 *           |            P2.2 |-> LED
 *           |                 |
 *           |            P1.3 |<- Botão (envio de um frame)
 *
 */

#include <msp430.h>


#include <hardware_board_g2553.h>
#include "radio/TI_CC_CC1100-CC2500.h"
#include "radio/CC1100-CC2500.h"

#include "radio/TI_CC_spi.h"

extern char paTable[];
extern char paTableLen;

char txBuffer[4];
char rxBuffer[4];
unsigned int i = 0;

void main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  // 5ms delay to compensate for time to startup between MSP430 and CC1100/2500
  __delay_cycles(5000);
  
  TI_CC_SPISetup();                         // Initialize SPI port

  P2SEL = 0;                                // Sets P2.6 & P2.7 as GPIO
  TI_CC_PowerupResetCCxxxx();               // Reset CCxxxx
  writeRFSettings();                        // Write RF settings to config reg
  TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, paTable, paTableLen);//Write PATABLE

  // Configure ports -- switch inputs, LEDs, GDO0 to RX packet info from CCxxxx
  TI_CC_SW_PxREN = TI_CC_SW1;               // Enable Pull up resistor
  TI_CC_SW_PxOUT |= TI_CC_SW1;               // Enable pull up resistor
  TI_CC_SW_PxIES = TI_CC_SW1;               // Int on falling edge

  TI_CC_SW_PxIFG &= ~(TI_CC_SW1);           // Clr flags
  TI_CC_SW_PxIE = TI_CC_SW1;                // Activate interrupt enables
  TI_CC_LED_PxOUT &= ~(TI_CC_LED1 + TI_CC_LED2); // Outputs = 0
  TI_CC_LED_PxDIR |= TI_CC_LED1 + TI_CC_LED2;// LED Direction to Outputs

  TI_CC_GDO0_PxIES |= TI_CC_GDO0_PIN;       // Int on falling edge (end of pkt)
  TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;      // Clear flag
  TI_CC_GDO0_PxIE |= TI_CC_GDO0_PIN;        // Enable int on end of packet

  TI_CC_SPIStrobe(TI_CCxxx0_SRX);           // Initialize CCxxxx in RX mode.
                                            // When a pkt is received, it will
                                            // signal on GDO0 and wake CPU

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, enable interrupts
}


// The ISR assumes the interrupt came from a pressed button
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
  // If Switch was pressed
  if(TI_CC_SW_PxIFG & TI_CC_SW1)
  {
    // Build packet
    txBuffer[0] = 2;                        // Packet length
    txBuffer[1] = 0x01;                     // Packet address
    txBuffer[2] = 0x45; // Load switch inputs
    RFSendPacket(txBuffer, 3);              // Send value over RF
    __delay_cycles(5000);                   // Switch debounce
  }
  TI_CC_SW_PxIFG &= ~(TI_CC_SW1);           // Clr flag that caused int
}

// The ISR assumes the interrupt came from GDO0. GDO0 fires indicating that
// CCxxxx received a packet
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
  // if GDO fired
  if(TI_CC_GDO0_PxIFG & TI_CC_GDO0_PIN)
  {
    char len=2;                             // Len of pkt to be RXed (only addr
                                            // plus data; size byte not incl b/c
                                            // stripped away within RX function)
    if (RFReceivePacket(rxBuffer,&len))     // Fetch packet from CCxxxx
    TI_CC_LED_PxOUT ^= rxBuffer[1];         // Toggle LEDs according to pkt data
  }

  TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;      // After pkt RX, this flag is set.
}
