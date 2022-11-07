/*
 * fsm.h
 *
 *  Created on: May 13, 2020
 *      Author: xtarke
 */

#ifndef APPS_FSM_FSM_H_
#define APPS_FSM_FSM_H_

#define BOTAO_PORT P1
#define BOTAO_PIN BIT1

#define VERDE_PORT P4
#define VERDE_PIN BIT7

/* Nessa aplicação, liga-se vermelho e verde como exemplo do amarelo */
#define AMARELO_PORT P2
#define AMARELO_PIN BIT0

#define VERMELHO_PORT P1
#define VERMELHO_PIN BIT0

/* Definição dos estados */
typedef enum {
    ESTADO_REPOUSO,
    ESTADO_VERMELHO,
    ESTADO_AMARELO,
    ESTADO_VERDE,
    NUM_ESTADOS
} state_t;

void do_fsm();
void config_semaforo();


#endif /* APPS_FSM_FSM_H_ */
