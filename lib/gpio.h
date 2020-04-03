/*
 * gpio.h : GPIO general purpose macros
 *
 *  Created on: Mar 2, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef LIB_GPIO_H_
#define LIB_GPIO_H_

/* Convert Px to PxOUT */
#define PORT_OUT(...) PORT_OUT_SUB(__VA_ARGS__)
#define PORT_OUT_SUB(port) (port##OUT)

/* Convert Px to PxIN */
#define PORT_IN(...) PORT_IN_SUB(__VA_ARGS__)
#define PORT_IN_SUB(port) (port##IN)

/* Convert Px to PxDIR */
#define PORT_DIR(...) PORT_DIR_SUB(__VA_ARGS__)
#define PORT_DIR_SUB(port) (port##DIR)

/* Convert Px to PxREN */
#define PORT_REN(...) PORT_REN_SUB(__VA_ARGS__)
#define PORT_REN_SUB(port) (port##REN)

/* Convert Px to PxIE */
#define PORT_IE(...) PORT_IE_SUB(__VA_ARGS__)
#define PORT_IE_SUB(port) (port##IE)

/* Convert Px to PxIES */
#define PORT_IES(...) PORT_IES_SUB(__VA_ARGS__)
#define PORT_IES_SUB(port) (port##IES)

/* Convert Px to PxIFG */
#define PORT_IFG(...) PORT_IFG_SUB(__VA_ARGS__)
#define PORT_IFG_SUB(port) (port##IFG)

#endif /* LIB_GPIO_H_ */
