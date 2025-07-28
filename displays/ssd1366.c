/**
 * @file ssd1366.c
 * @brief Funções de baixo nível para controle do display OLED SSD1306.
 * @author Renan Augusto Starke
 * @date Março 2, 2020
 *
 * Este arquivo contém as implementações das funções para o driver do display OLED SSD1306.
 * As definições de controle do OLED são baseadas em https://github.com/yanbe/ssd1306-esp-idf-i2c.
 * As funções de desenho são baseadas na biblioteca Adafruit_GFX.
 * Algumas definições adicionais são de http://robotcantalk.blogspot.com/2015/03/interfacing-arduino-with-ssd1306-driven.html [2].
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __MSP430G2553__
#include "../lib/i2c_master_g2553.h"
#endif


#include "ssd1366.h"
#include "font8x8_basic.h"

/* Following definitions are from:
   http://robotcantalk.blogspot.com/2015/03/interfacing-arduino-with-ssd1306-driven.html
*/

/** @brief Endereço I2C do display OLED. */
#define OLED_I2C_ADDRESS   0x3C

// Configuration data
#define OLED_DISPLAY_WIDTH_PX           0x80
#define OLED_PAGE_HEIGHT_PX             0x08

#define OLED_HEIGHT 64
#define OLED_WIDTH 128

// Control byte
#define OLED_CONTROL_BYTE_CMD_SINGLE    0x80
#define OLED_CONTROL_BYTE_CMD_STREAM    0x00
#define OLED_CONTROL_BYTE_DATA_STREAM   0x40

// Fundamental commands (pg.28)
#define OLED_CMD_SET_CONTRAST           0x81    // follow with 0x7F
#define OLED_CMD_DISPLAY_RAM            0xA4
#define OLED_CMD_DISPLAY_ALLON          0xA5
#define OLED_CMD_DISPLAY_NORMAL         0xA6
#define OLED_CMD_DISPLAY_INVERTED       0xA7
#define OLED_CMD_DISPLAY_OFF            0xAE
#define OLED_CMD_DISPLAY_ON             0xAF
#define OLED_DEACTIVATE_SCROLL          0x2E

// Addressing Command Table (pg.30)
#define OLED_CMD_SET_MEMORY_ADDR_MODE   0x20    // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define OLED_CMD_SET_COLUMN_RANGE       0x21    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define OLED_CMD_SET_PAGE_RANGE         0x22    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7

// Hardware Config (pg.31)
#define OLED_CMD_SET_DISPLAY_START_LINE 0x40
//#define OLED_CMD_SET_SEGMENT_REMAP      0xA0
#define OLED_CMD_SET_SEGMENT_REMAP      0xA1
#define OLED_CMD_SET_MUX_RATIO          0xA8    // follow with 0x3F = 64 MUX
#define OLED_CMD_SET_COM_SCAN_MODE      0xC8
#define OLED_CMD_SET_DISPLAY_OFFSET     0xD3    // follow with 0x00
#define OLED_CMD_SET_COM_PIN_MAP        0xDA    // follow with 0x12
#define OLED_CMD_NOP                    0xE3    // NOP
#define OLED_SETSTARTLINE               0x40

// Timing and Driving Scheme (pg.32)
#define OLED_CMD_SET_DISPLAY_CLK_DIV    0xD5    // follow with 0x80
#define OLED_CMD_SET_PRECHARGE          0xD9    // follow with 0xF1
#define OLED_CMD_SET_VCOMH_DESELCT      0xDB    // follow with 0x30

// Charge Pump (pg.62)
#define OLED_CMD_SET_CHARGE_PUMP        0x8D    // follow with 0x14


/**
 * @brief Buffer de memória para o display OLED.
 *
 * Este buffer armazena o estado dos pixels antes de serem enviados para o display físico.
 * - Se o microcontrolador for um MSP430G2553, o buffer é particionado em 4
 *   (`(OLED_WIDTH * ((OLED_HEIGHT + 7) / 8))/4`) devido à limitação de RAM [5].
 * - Caso contrário, é alocado um buffer completo de 1KB (`OLED_WIDTH * ((OLED_HEIGHT + 7) / 8)`) [5].
 */
#if defined(__MSP430G2553__)
    /* Not enough RAM for 1k OLED frame Buffer *
     * Using 4 partitions                      */
    uint8_t oled_buffer[(OLED_WIDTH * ((OLED_HEIGHT + 7) / 8))/4];
#else
    /*1k OLED frame Buffer */
    uint8_t oled_buffer[(OLED_WIDTH * ((OLED_HEIGHT + 7) / 8))];
#endif

static void ssd1306_single_command(uint8_t data);
static void ssd1306_command_list(uint8_t *data, uint8_t size);

/**
 * @brief Macro para trocar os valores de duas variáveis int16_t.
 * @param a Primeira variável.
 * @param b Segunda variável.
 */
#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

static const uint8_t init[3][5] = {
    {
        0x00,
        OLED_CMD_DISPLAY_OFF,           // 0xAE
        OLED_CMD_SET_DISPLAY_CLK_DIV,   // 0xD5
        0x80,                           // the suggested ratio 0x80
        OLED_CMD_SET_MUX_RATIO          // 0xA8
    },
    {
        0x00,
        OLED_CMD_SET_DISPLAY_OFFSET,    // 0xD3
        0x0,                            // no offset
        OLED_SETSTARTLINE | 0x0,        // 0x40 line #0
        OLED_CMD_SET_CHARGE_PUMP        // 0x8D
    },
    {
        0x00,
        OLED_CMD_SET_MEMORY_ADDR_MODE,      // 0x20
        0x00,                               // 0x0 act like ks0108
        OLED_CMD_SET_SEGMENT_REMAP | 0x1,   //0xa1
        OLED_CMD_SET_COM_SCAN_MODE,         //0xc8
    },
};

static const uint8_t init_disp_on[] = {
    0x00,
    OLED_CMD_SET_VCOMH_DESELCT, // 0xDB
    0x40,
    OLED_CMD_DISPLAY_RAM, // 0xA4
    OLED_CMD_DISPLAY_NORMAL,       // 0xA6
    OLED_DEACTIVATE_SCROLL,
    OLED_CMD_DISPLAY_ON         // Main screen turn on
};

/**
 * @brief Envia um comando único para o display OLED via I2C.
 * @param data O comando de 8 bits a ser enviado.
 */
static void ssd1306_single_command(uint8_t data){
    i2c_master_write_reg(OLED_I2C_ADDRESS, 0x00, &data, 1);
}

/**
 * @brief Envia uma lista de comandos para o display OLED via I2C.
 * @param data Ponteiro para o array de comandos. O primeiro byte em `data` é o byte de controle (0x00 para stream de comandos),
 *             e os bytes subsequentes são os comandos a serem enviados.
 * @param size O número total de bytes no array `data`.
 */
static void ssd1306_command_list(uint8_t *data, uint8_t size){
    i2c_master_write_reg(OLED_I2C_ADDRESS, data[0], data + 1, size - 1);
}

/**
 * @brief Inicializa o display OLED SSD1306.
 *
 * Esta função envia uma sequência de comandos de inicialização para configurar o display,
 * incluindo:
 * - Desligar o display (OLED_CMD_DISPLAY_OFF).
 * - Configurar a divisão do clock (OLED_CMD_SET_DISPLAY_CLK_DIV, 0x80).
 * - Definir a taxa do multiplexador (OLED_CMD_SET_MUX_RATIO) com a altura do OLED menos 1.
 * - Definir o offset do display (OLED_CMD_SET_DISPLAY_OFFSET, 0x0).
 * - Definir a linha de início (OLED_SETSTARTLINE).
 * - Configurar a bomba de carga (OLED_CMD_SET_CHARGE_PUMP, 0x14).
 * - Definir o modo de endereçamento de memória para horizontal (OLED_CMD_SET_MEMORY_ADDR_MODE, 0x00).
 * - Configurar o remapeamento de segmento (OLED_CMD_SET_SEGMENT_REMAP | 0x1).
 * - Definir o modo de varredura COM (OLED_CMD_SET_COM_SCAN_MODE).
 * - Definir o mapeamento de pinos COM (OLED_CMD_SET_COM_PIN_MAP, 0x12).
 * - Definir o contraste (OLED_CMD_SET_CONTRAST, 0xCF).
 * - Definir o período de pré-carga (OLED_CMD_SET_PRECHARGE, 0xF1).
 * - Definir o nível de deseleção VCOMH (OLED_CMD_SET_VCOMH_DESELCT, 0x40).
 * - Ativar o display RAM e modo normal, desativar scroll e ligar o display (OLED_CMD_DISPLAY_RAM,
 *   OLED_CMD_DISPLAY_NORMAL, OLED_DEACTIVATE_SCROLL, OLED_CMD_DISPLAY_ON).
 */
void ssd1306_init(){
    /* Send all initialization commands */
    ssd1306_command_list((uint8_t *)init[0], sizeof(init[0]));
    ssd1306_single_command(OLED_HEIGHT - 1);
    ssd1306_command_list((uint8_t *)init[1], sizeof(init[1]));
    ssd1306_single_command(0x14);
    ssd1306_command_list((uint8_t *)init[2], sizeof(init[2]));

    ssd1306_single_command(OLED_CMD_SET_COM_PIN_MAP);
    /* Pin MAP must be followed by 0x12*/
    ssd1306_single_command(0x12); 
    ssd1306_single_command(OLED_CMD_SET_CONTRAST);
    /* Contrast  must be followed by 0xCF*/
    ssd1306_single_command(0xCF);  
    ssd1306_single_command(OLED_CMD_SET_PRECHARGE);
    /* Precharge must be followed by 0xF1 */
    ssd1306_single_command(0xF1); 
    ssd1306_command_list((uint8_t *)init_disp_on, sizeof(init_disp_on));

}


/**
 * @brief Limpa o buffer de exibição na memória (RAM) para zeros.
 *
 * Todos os pixels no `oled_buffer` são definidos como 'desligados' (0x00),
 * preparando o buffer para novos desenhos [9].
 */
void ssd1306_clearDisplay_buffer(void) {
    memset(oled_buffer, 0, sizeof(oled_buffer));
}

/**
 * @brief Desenha um pixel no buffer de exibição na posição (x, y) com a cor especificada.
 *
 * A cor do pixel pode ser `WHITE_PIXEL` (ligado) ou `BLACK_PIXEL` (desligado).
 * As operações de manipulação de bits são realizadas diretamente no `oled_buffer`,
 * considerando que cada byte representa 8 pixels verticais [9, 10].
 *
 * @param x Coordenada X (coluna) do pixel.
 * @param y Coordenada Y (linha) do pixel.
 * @param color Cor do pixel (`WHITE_PIXEL` ou `BLACK_PIXEL`).
 */
void ssd1306_draw_pixel(int16_t x, int16_t y, pixel_color_t color){
    if ((x >= 0) && (x < OLED_WIDTH && (y >= 0) && (y < OLED_HEIGHT))) {
        uint16_t i = x + (y >> 3) * OLED_WIDTH;

        if (i > sizeof(oled_buffer) * 4)
            return;

        if (color)
            // oled_buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y & 7));
            //frame_buffer[x + (y >> 3) * OLED_WIDTH] &= ~(1 << (y & 7));
            oled_buffer[i] &= ~(1 << (y & 7));
        else
            //oled_buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y & 7));
            //frame_buffer[x + (y >> 3) * OLED_WIDTH] |= (1 << (y & 7));
            oled_buffer[i] |= (1 << (y & 7));
    }
}

/**
 * @brief Limpa a tela física do display OLED, preenchendo-a com pixels desligados.
 *
 * Esta função envia comandos para definir o range de páginas e colunas,
 * e então envia bytes de `0xff` (que interpretados pelo `oled_buffer[i] |= (1 << (y & 7))` se o bit for 1 ele vira 0, entao 0xff desliga o display)
 * para todas as 8 páginas do display, efetivamente desligando todos os pixels.
 */
void ssd1306_display_clear() {
    static const uint8_t cmd[] = {0x00,
                    OLED_CMD_SET_PAGE_RANGE,   // 0x22
                    0, //
                    0xFF,
                    OLED_CMD_SET_COLUMN_RANGE, // 0x21
                    0 };

    ssd1306_command_list((uint8_t *)cmd, sizeof(cmd));
    ssd1306_single_command(OLED_WIDTH - 1);

    uint8_t zero[128];
    memset(zero, 0xff, 128);
    
    uint8_t i;
    for (i = 0; i < 8; i++) {
         i2c_master_write_reg(OLED_I2C_ADDRESS, 0x40, zero, 128);
    }   
}

/**
 * @brief Envia os dados do buffer de exibição (`oled_buffer`) para uma partição/página específica do display.
 *
 * Esta função é responsável por transferir o conteúdo do buffer de RAM para o display OLED físico.
 * - Para MSP430G2553, envia 256 bytes em duas iterações de 128 bytes.
 * - Para outras plataformas, envia 1024 bytes (o buffer completo) em iterações de 128 bytes.
 * Ela define o range de páginas e colunas antes de enviar os dados.
 *
 * @param line A partição/página a ser atualizada. O valor pode ser usado para definir o início do range de páginas.
 */
void ssd1306_display_data(oled_partition_t line){
    uint8_t *data = oled_buffer;
    int i;
    const uint8_t cmd[] = {
        0x00,
        OLED_CMD_SET_PAGE_RANGE,   // 0x22
        (uint8_t) line,  //
        0xFF,
        OLED_CMD_SET_COLUMN_RANGE, // 0x21
        0};

    ssd1306_command_list((uint8_t *)cmd, sizeof(cmd));
    ssd1306_single_command(OLED_WIDTH - 1);

//    for (i=0; i < 1024; i+=128){
//        i2c_master_write_reg(OLED_I2C_ADDRESS, 0x40, data + i, 128);
//    }
#if defined(__MSP430G2553__)
    for (i=0; i < 256; i+=128){
#else
    for (i=0; i < 1024; i+=128) {
#endif
        i2c_master_write_reg(OLED_I2C_ADDRESS, 0x40, data + i, 128);
    }
}

/**
 * @brief Preenche uma região horizontal do display com os dados fornecidos.
 *
 * Esta função é usada para escrever dados diretamente em uma seção horizontal
 * do display, definindo primeiro a página e a coluna de início .
 * É útil para atualizações rápidas de seções específicas.
 *
 * @param x A página de início para a região (0-7).
 * @param hor_size O tamanho horizontal (largura) da região em bytes (até 128).
 * @param data Ponteiro para os dados a serem escritos.
 */
void ssd1306_fill_region(uint8_t x, uint8_t hor_size, uint8_t *data) {
    
    if (x > 8 || hor_size > 128)
    return;

    uint8_t cur_page = x;
    uint8_t init[] = {OLED_CONTROL_BYTE_CMD_STREAM,
                        0x00,
                        0x10,
                        0xB0 | cur_page};

    ssd1306_command_list((uint8_t *)init, sizeof(init));

    i2c_master_write_reg(OLED_I2C_ADDRESS, OLED_CONTROL_BYTE_CMD_STREAM, data, hor_size);
}

/**
 * @brief Desenha uma linha horizontal no buffer de exibição.
 *
 * Esta função itera sobre os pixels da linha e chama `ssd1306_draw_pixel`
 * para desenhar cada pixel individualmente.
 *
 * @param x Coordenada X de início da linha.
 * @param y Coordenada Y da linha.
 * @param size Comprimento da linha em pixels.
 * @param color Cor da linha (`WHITE_PIXEL` ou `BLACK_PIXEL`).
 */
void ssd1306_draw_h_line(int16_t x, int16_t y, int16_t size, pixel_color_t color){
    int i;
    for (i=0;i < size;i++)
        ssd1306_draw_pixel(x+i,y,color);
}

/**
 * @brief Escreve um único caractere 8x8 no buffer de exibição.
 *
 * O caractere é copiado do array `font8x8_basic_tr` (tabela de fonte 8x8)
 * para a posição especificada no `oled_buffer`. O caractere é escrito como um bloco de 8x8 pixels [13, 14].
 *
 * @param x Coordenada X de início do caractere.
 * @param y Coordenada Y de início do caractere.
 * @param data O caractere ASCII a ser escrito.
 */
void ssd1306_write_char(int16_t x, int16_t y, char data){

  uint8_t *font_ptr = font8x8_basic_tr[(uint8_t)data];

  if ((x >= 0) && (x < (OLED_WIDTH - FONT_PIXEL_WIDTH)) && (y >= 0)
      && (y < (OLED_HEIGHT - FONT_PIXEL_HEIGHT))) {
      
      /* Copy all 8x8 font char data to RAM display buffer */
      memcpy(&oled_buffer[x + (y >>  3) * OLED_WIDTH], font_ptr, 8);
    
  }
}

/**
 * @brief Desenha uma linha vertical rápida no buffer de exibição.
 *
 * Esta é uma função auxiliar que delega o desenho da linha para `ssd1306_write_line`,
 * passando as coordenadas de início e fim da linha vertical.
 *
 * @param x Coordenada X da linha.
 * @param y Coordenada Y de início da linha.
 * @param h Altura da linha em pixels.
 * @param color Cor da linha (`WHITE_PIXEL` ou `BLACK_PIXEL`).
 */
void ssd1306_writeFastVLine(int16_t x, int16_t y, int16_t h, pixel_color_t color){
    ssd1306_write_line(x, y, x, y + h - 1, color);
}

/**
 * @brief Preenche um retângulo no buffer de exibição.
 *
 * O retângulo é preenchido iterando sobre sua largura e desenhando uma linha
 * vertical rápida (`ssd1306_writeFastVLine`) para cada coluna [14].
 *
 * @param x Coordenada X de início do retângulo.
 * @param y Coordenada Y de início do retângulo.
 * @param w Largura do retângulo.
 * @param h Altura do retângulo.
 * @param color Cor de preenchimento (`WHITE_PIXEL` ou `BLACK_PIXEL`).
 */
void ssd1306_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, pixel_color_t color){
    int16_t i;    
    for (i = x; i < x + w; i++) {
        ssd1306_writeFastVLine(i, y, h, color);
  }
}

/**
 * @brief Desenha uma linha entre dois pontos (x0, y0) e (x1, y1) no buffer de exibição.
 *
 * Implementa um algoritmo de linha para
 * desenhar os pixels entre os dois pontos de forma eficiente. Ele lida com linhas
 * em qualquer orientação e inclinação, otimizando o desenho de pixels individuais.
 *
 * @param x0 Coordenada X do ponto de início.
 * @param y0 Coordenada Y do ponto de início.
 * @param x1 Coordenada X do ponto de fim.
 * @param y1 Coordenada Y do ponto de fim.
 * @param color Cor da linha (`WHITE_PIXEL` ou `BLACK_PIXEL`).
 */
void ssd1306_write_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, pixel_color_t color){
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx >> 1;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            ssd1306_draw_pixel(y0, x0, color);
        } else {
            ssd1306_draw_pixel(x0, y0, color);
    }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/**
 * @brief Escreve um caractere 8x8 com escala no buffer de exibição.
 *
 * Cada pixel do caractere original 8x8 (obtido de `font8x8_basic_tr`) é escalado
 * por um fator `scale`. Isso significa que um único pixel da fonte será desenhado
 * como um bloco de `scale`x`scale` pixels usando `ssd1306_fillRect`.
 *
 * @param x Coordenada X de início do caractere escalado.
 * @param y Coordenada Y de início do caractere escalado.
 * @param data O caractere ASCII a ser escrito.
 * @param scale Fator de escala para o caractere. Ex: scale=2 fará um caractere de 16x16 pixels.
 */
void ssd1306_write_scaled_char(int16_t x, int16_t y, char data, uint8_t scale){

    int8_t i;
    int8_t j;
    uint8_t *font_ptr =  font8x8_basic_tr[(uint8_t)data];

    for (i = 0; i < 8; i++) {
        uint8_t line = *(font_ptr + i);
        
        for (j = 0; j < 8; j++, line >>= 1) {
            if (line & 1)
                ssd1306_fillRect(x + i * scale, y + j * scale, scale, scale, WHITE_PIXEL);
            else 
                ssd1306_fillRect(x + i * scale, y + j * scale, scale, scale, BLACK_PIXEL);
        }
    }
}
/**
 * @brief Escreve uma string de caracteres no buffer de exibição.
 *
 * Esta função itera sobre a string `data` e chama `ssd1306_write_char`
 * para cada caractere, posicionando-os sequencialmente a cada 8 pixels na coordenada X .
 *
 * @param x Coordenada X de início da string.
 * @param y Coordenada Y de início da string.
 * @param data Ponteiro para a string de caracteres a ser escrita.
 */
void ssd306_write_string(int16_t x, int16_t y, char *data){
    uint16_t i;
    for (i=0; data[i] != '\0'; i++){
        ssd1306_write_char(x + i * 8, y, data[i]);
    }
}
