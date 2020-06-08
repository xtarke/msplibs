/*
 * i2c_master_g2553.h
 *
 *  Created on: May 28, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef LIB_I2C_MASTER_G2553_H_
#define LIB_I2C_MASTER_G2553_H_

#define CLOCK_24MHz

typedef enum i2c_modeE_enum{
    IDLE_MODE,
    NACK_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    SWITCH_TO_RX_MODE,
    SWITHC_TO_TX_MODE,
    TIMEOUT_MODE
} i2c_mode;

void init_i2c_master_mode();
i2c_mode i2c_write_single_byte(uint8_t dev_addr, uint8_t byte);

i2c_mode i2c_master_write_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count);
i2c_mode i2c_master_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count, uint8_t *data);

#endif /* LIB_I2C_MASTER_G2553_H_ */
