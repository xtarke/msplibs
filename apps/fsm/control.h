/*
 * control.h
 *
 *  Created on: May 13, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 */

#ifndef APPS_FSM_CONTROL_H_
#define APPS_FSM_CONTROL_H_

#include <stdint.h>

/* Some hardware definitions */
#define BUTTON_PORT P1
#define BUTTON BIT3

#define LED_PORT P1
#define LED BIT0

/* Couting debouncer samples */
#define BUTTON_SAMPLES (12)

void control_init();
void do_control();

void turn_servo_off();
void move_servo(uint16_t angle);

void set_led();
void unset_led();


#endif /* APPS_FSM_CONTROL_H_ */
