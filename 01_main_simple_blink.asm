;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Inicializar stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Desativar WDT
			bis.b #001h, &P1DIR

; Constantes
LED 		.equ 01
DELAY 		.equ 25000
COUNTER		.equ R15

;-------------------------------------------------------------------------------
; Loop principal
;-------------------------------------------------------------------------------
MAIN		xor.b #LED, &P1OUT     ; comuta o LED
    		mov.w #DELAY, COUNTER  ; init. delay

L1			dec.w COUNTER	; Dec. contador
			jnz L1			; Delay terminado?
			jmp MAIN		; Pisca novamente

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack

;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
