/*
 * fsm.h
 *
 *  Created on: May 13, 2020
 *      Author: xtarke
 */

#ifndef APPS_FSM_FSM_H_
#define APPS_FSM_FSM_H_

/* Definição dos estados */
typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    NUM_STATES
} state_t;

void do_fsm();

/* Variável global compartilhada para
 * evitar chamada de função na ISR
 */
extern volatile state_t curr_state;

#endif /* APPS_FSM_FSM_H_ */
