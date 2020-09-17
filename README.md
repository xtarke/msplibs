# MSP430 libs: exemplos para Microcontroladores MSP430

Notas, exemplos e bibliotecas para a disciplina de Microntroladores do Instituto Ferderal de Santa Catarina

## Getting Started

Clone o repositório e adicione os arquivos no seu projeto. Evite compilar mais de uma função main.

## Circuitos


- [Hardware LCD e IRQ externo](./figuras/display_irq.png)
- [Hardware diplays de 7 segmentos e WDT](./figuras/wdt_disp.png)


## Gravação com MSPDEBUG

```bash
mspdebug -n rf2500 "prog ./Debug/msplibs.out"
```

[Launchpad Linux](https://hackaday.com/2010/08/11/how-to-launchpad-programming-with-linux/)


https://dlbeer.co.nz/mspdebug/


## Simulação com Proteus (v8.9)

Validação dos periféricos e lista de problemas da simulação. Exemplos estão disponíveis no [diretório ./sim](./sim/)

###  MSP430G2553

  - Entrada e saída:
    - Saída: P1.4 e P3.7 não comutam. Veja [01_main_simple_gpio_all_bits.c](./01_main_simple_gpio_all_bits.c).

      - Entradas: pull-ups internos não funcionam. P1.4 não comuta. ToDo: validar P2.x e P3.x como entadas. Veja [01_main_simple_input.c](./01_main_simple_input.c)

     - Interrupções externas: Veja [03_main_ext_irq.c](./03_main_ext_irq.c)
        - Validado com ISR em P1.3.
        - Usar pull-up/down externos.
        - _ToDo_: verificar outros pinos e portas.

    - Temporizadores:
      - Validado timerA0. Veja [05_main_simple_timer0_a.c](./05_main_simple_timer0_a.c)
        - ISR com overflow e comparador 0 (CCR0).
      - _ToDo_: verificar outro temporizador.


    - Sistema de clock (DCO): Veja [04_main_simple_clock_g2553.c](./04_main_simple_clock_g2553.c)
      - Alterar CLOCK_1MHz, CLOCK_8MHz, CLOCK_12MHz ou CLOCK_16MHz verificando a frequência de P1.0.


    - I2C: Veja [09_main_i2c_single_byte.c](./09_main_i2c_single_byte.c)
      - Por algum motivo, não se pode desligar a CPU no Proteus. Use apenas GIE em [i2c_master_g2553.c](./i2c_master_g2553.c) na função i2c_master_write_reg.

    - ADC:
      - Exemplos não executam corretamente. 


###  MSP430F2132

  - Modo captura: F2132 P1.1 / TA1.0  (Timer 1/ CCR0)
