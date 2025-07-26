/*
* ssd1366.c
*
*  Created on: Mar 2, 2020
*      Author: Renan Augusto Starke
*      Instituto Federal de Santa Catarina *
*
*     - Oled ssd1306 low level functions
*
*   OLED control definitions from: https://github.com/yanbe/ssd1306-esp-idf-i2c
*   Draw functions base on Adafruit_GFX
*
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

static void ssd1306_single_command(uint8_t data){
    i2c_master_write_reg(OLED_I2C_ADDRESS, 0x00, &data, 1);
}

static void ssd1306_command_list(uint8_t *data, uint8_t size){
    i2c_master_write_reg(OLED_I2C_ADDRESS, data[0], data + 1, size - 1);
}

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

void ssd1306_clearDisplay_buffer(void) {
    memset(oled_buffer, 0, sizeof(oled_buffer));
}

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

void ssd1306_display_data(){
    uint8_t *data = oled_buffer;
    int i;
    static const uint8_t cmd[] = {
        0x00,
        OLED_CMD_SET_PAGE_RANGE,   // 0x22
        0, //
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

void ssd1306_draw_h_line(int16_t x, int16_t y, int16_t size, pixel_color_t color){
    int i;
    for (i=0;i < size;i++)
        ssd1306_draw_pixel(x+i,y,color);
}

void ssd1306_write_char(int16_t x, int16_t y, char data){

  uint8_t *font_ptr = font8x8_basic_tr[(uint8_t)data];

  if ((x >= 0) && (x < (OLED_WIDTH - FONT_PIXEL_WIDTH)) && (y >= 0)
      && (y < (OLED_HEIGHT - FONT_PIXEL_HEIGHT))) {
      
      /* Copy all 8x8 font char data to RAM display buffer */
      memcpy(&oled_buffer[x + (y >>  3) * OLED_WIDTH], font_ptr, 8);
    
  }
}

void ssd1306_writeFastVLine(int16_t x, int16_t y, int16_t h, pixel_color_t color){
    ssd1306_write_line(x, y, x, y + h - 1, color);
}

void ssd1306_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, pixel_color_t color){
    int16_t i;    
    for (i = x; i < x + w; i++) {
        ssd1306_writeFastVLine(i, y, h, color);
  }
}

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

void ssd306_write_string(int16_t x, int16_t y, char *data){
    uint16_t i;
    for (i=0; data[i] != '\0'; i++){
        ssd1306_write_char(x + i * 8, y, data[i]);
    }
}
