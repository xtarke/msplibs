/*
 * fsm.c
 *
 *  Created on: May 13, 2020
 *      Author: Renan Augusto Starke
 *
 *
 *
 *
 *
 */

#include <msp430.h>

#include "../../lib/gpio.h"
#include "../../lib/bits.h"

#include "control.h"
#include "fsm.h"

/* Cabeçalho das funções dos estados */
void f_stateA(void);
void f_stateB(void);
void f_stateC(void);
void f_stateD(void);

/* Definição da estrutura mantenedora do vetor de estados */
typedef struct {
    state_t myState;
    void (*func)(void);
}fsm_t;

/* Mapeamento entre estado e funções */
fsm_t myFSM[] = {
    { STATE_A, f_stateA },
    { STATE_B, f_stateB },
    { STATE_C, f_stateC },
    { STATE_D, f_stateD },
};

/* Estado atual  */
volatile state_t curr_state = STATE_A;


void do_fsm(){
    (*myFSM[curr_state].func)();
}

void f_stateA(void){
    move_servo(0);
    unset_led();
  }

void f_stateB(void){
    set_led();
    move_servo(90);
}

void f_stateC(void){
    move_servo(45);
}

void f_stateD(void){
    move_servo(175);
}
