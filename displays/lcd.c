/*
 * lcd.c
 *
 *  Created on: Aug 20, 2018
 *      Author: Renan Augusto Starke
 *      
 *      Adaptado de AVR e Arduino: Técnicas de Projeto, 2a ed. - 2012.
 *      Instituto Federal de Santa Catarina
 */

#include "lcd.h"


/* Macros privadas */
#define DISPLAY_PORT_OUT(...) DISPLAY_PORT_OUT_SUB(__VA_ARGS__)
#define DISPLAY_PORT_OUT_SUB(port) (port##OUT)
#define DISPLAY_PORT_DIR(...) DISPLAY_PORT_DIR_SUB(__VA_ARGS__)
#define DISPLAY_PORT_DIR_SUB(port) (port##DIR)

///* sinal de habilitação para o LCD */
//#define PULSO_ENABLE() _delay_us(1); SET_BIT(CONTR_LCD->PORT,E); _delay_us(1); CLR_BIT(CONTR_LCD->PORT,E); _delay_us(45)

static inline enable_pulse(){
    __delay_cycles(10000);// delay_us(1);
    SET_BIT(DISPLAY_PORT_DIR(LCD_CTRL_PORT),E_PIN);

    __delay_cycles(10000); //delay_us(1);
    CLR_BIT(DISPLAY_PORT_DIR(LCD_CTRL_PORT), E_PIN);

    __delay_cycles(10000); //_delay_us(45);
}



/**
 * @brief  Configura hardware: verificar lcd.h para mapa de pinos e nible de dados.
 * @param    Nenhum
 *
 * @retval Nenhum.
 */
void inic_LCD_4bits()
{
    /* Configura pinos de controle */
    SET_BIT(DISPLAY_PORT_DIR(LCD_CTRL_PORT), RS_PIN | E_PIN);

    /* Configure pinos de dados */
#if (NIBBLE_DADOS)
    DISPLAY_PORT_DIR(LCD_DATA_PORT) |=  0xF0;
#else
    DISPLAY_PORT_DIR(LCD_DATA_PORT) |=  0x0F;
#endif

    //RS em zero indicando que o dado para o LCD será uma instrução
    CLR_BIT(DISPLAY_PORT_OUT(LCD_DATA_PORT),RS_PIN | E_PIN);
    //pino de habilitação em zero
    //CLR_BIT(CONTR_LCD->PORT,E);

    //tempo para estabilizar a tensão do LCD, após VCC ultrapassar 4.5 V (na prática pode
    //ser maior).
    __delay_cycles(10000);

    //interface de 8 bits
#if (NIBBLE_DADOS)
    DISPLAY_PORT_OUT(LCD_DATA_PORT) = (DISPLAY_PORT_OUT(LCD_DATA_PORT) & 0x0F) | 0x30;
#else
    DISPLAY_PORT_OUT(LCD_DATA_PORT) = (DISPLAY_PORT_OUT(LCD_DATA_PORT) & 0xF0) | 0x03;
#endif

    //habilitação respeitando os tempos de resposta do LCD
    enable_pulse(); //PULSO_ENABLE();
    __delay_cycles(10000);  //    _delay_ms(5);
    enable_pulse();  //    PULSO_ENABLE();
    __delay_cycles(10000);   //    _delay_us(200);
    enable_pulse(); //    PULSO_ENABLE();

    /* Até aqui ainda é uma interface de 8 bits.
     * Muitos programadores desprezam os comandos acima, respeitando apenas o tempo de
     * estabilização da tensão (geralmente funciona). Se o LCD não for inicializado primeiro no
     * modo de 8 bits, haverá problemas se o microcontrolador for inicializado e o display já o tiver sido.*/

    //interface de 4 bits, deve ser enviado duas vezes (a outra está abaixo)
#if (NIBBLE_DADOS)
    DISPLAY_PORT_OUT(LCD_DATA_PORT) = (DISPLAY_PORT_OUT(LCD_DATA_PORT) & 0x0F) | 0x20;
#else
    DISPLAY_PORT_OUT(LCD_DATA_PORT) = (DISPLAY_PORT_OUT(LCD_DATA_PORT) & 0xF0) | 0x02;
#endif

    enable_pulse();
    //interface de 4 bits 2 linhas (aqui se habilita as 2 linhas)
    cmd_LCD(0x28,0);

    //são enviados os 2 nibbles (0x2 e 0x8)

    //desliga o display
    cmd_LCD(0x08,0);
    //limpa todo o display
    cmd_LCD(0x01,0);
    //mensagem aparente cursor inativo não piscando
    cmd_LCD(0x0C,0);
    //inicializa cursor na primeira posição a esquerda - 1a linha
    cmd_LCD(0x80,0);
}









//
///* Stream para utilização do fprintf */
//FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
//
///**
//  * @brief  Retorna o stream interno para utilizar a libc (printf no LCD)
//  * @param	Nenhum
//  *
//  * @retval FILE: ponteiro do stream para utiliza com fprintf.
//  */
//FILE * inic_stream(){
//	return &lcd_str;
//}
//
//
/**
 * @brief  Envia um dado estático para o display: caractere ou comando.
 * @param c: valor do comando.
 * @param cd: 0 para comando. 1 para caractere.
 *
 * @retval Nenhum
 */
void cmd_LCD(uint8_t c, uint8_t cd)				//c é o dado  e cd indica se é instrução ou caractere
{
    //    if(cd==0)
    //        CLR_BIT(CONTR_LCD->PORT,RS);
    //    else
    //        SET_BIT(CONTR_LCD->PORT,RS);



    //
    //    //primeiro nibble de dados - 4 MSB
    //    //compila código para os pinos de dados do LCD nos 4 MSB do PORT
    //    #if (NIBBLE_DADOS)
    //        DADOS_LCD->PORT = (DADOS_LCD->PORT & 0x0F)|(0xF0 & c);
    //    #else
    //        //compila código para os pinos de dados do LCD nos 4 LSB do POR
    //        DADOS_LCD->PORT = (DADOS_LCD->PORT & 0xF0)|(c>>4);
    //    #endif
    //
    //    PULSO_ENABLE();
    //
    //    //segundo nibble de dados - 4 LSB
    //    #if (NIBBLE_DADOS)
    //        //compila código para os pinos de dados do LCD nos 4 MSB do PORT
    //        DADOS_LCD->PORT = (DADOS_LCD->PORT & 0x0F) | (0xF0 & (c<<4));
    //    #else
    //        //compila código para os pinos de dados do LCD nos 4 LSB do PORT
    //        DADOS_LCD->PORT = (DADOS_LCD->PORT & 0xF0) | (0x0F & c);
    //    #endif
    //
    //    PULSO_ENABLE();
    //
    //    //se for instrução de retorno ou limpeza espera LCD estar pronto
    //    if((cd==0) && (c<4))
    //        _delay_ms(2);
}


//
///**
//  * @brief  Escreve um string estática (sem printf) no LCD.
//  * @param c: ponteiro para a string em RAM
//  *
//  * @retval Nenhum
//  */
//void escreve_LCD(char *c)
//{
//   for (; *c!=0;c++) cmd_LCD(*c,1);
//}
//
///**
//  * @brief  Escreve um string estática (sem printf) no LCD.
//  * @param c: ponteiro para a string em FLASH
//  *
//  * @retval Nenhum
//  */
//void escreve_LCD_Flash(const char *c)
//{
//   for (;pgm_read_byte(&(*c))!=0;c++) cmd_LCD(pgm_read_byte(&(*c)),1);
//}
//
///**
//  * @brief Escreve um caractere no stream. Função utilizada pela libc.
//  * @param c: caractere a ser escrito.
//  * @param fp: ponteiro do stream inicilizado que receberá o dado.
//  *
//  * @retval int maior que zero se sucesso.
//  */
//int lcd_putchar(char c, FILE *fp){
//	cmd_LCD(c,1);
//
//	return 1;
//}
