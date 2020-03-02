/*
 * gpio.h : GPIO general purpose macros
 *
 *  Created on: Mar 2, 2020
 *      Author: xtarke
 */

#ifndef LIB_GPIO_H_
#define LIB_GPIO_H_

/* Convert Px to PxOUT */
#define PORT_OUT(...) PORT_OUT_SUB(__VA_ARGS__)
#define PORT_OUT_SUB(port) (port##OUT)

/* Convert Px to PxDIR */
#define PORT_DIR(...) PORT_DIR_SUB(__VA_ARGS__)
#define PORT_DIR_SUB(port) (port##DIR)

#endif /* LIB_GPIO_H_ */
