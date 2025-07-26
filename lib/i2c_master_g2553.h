/*
 * i2c_master_f247.h
 *
 *  Created on: Jun 01, 2024
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef LIB_I2C_MASTER_F2247_G2xxx_H_
#define LIB_I2C_MASTER_F2247_G2xxx_H_

#include <stdint.h>

#define CLOCK_16MHz

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

#ifdef __cplusplus
    #define EXPORT_C extern "C"
#else
    #define EXPORT_C
#endif

EXPORT_C void init_i2c_master_mode();
EXPORT_C i2c_mode i2c_write_single_byte(uint8_t dev_addr, uint8_t byte);
EXPORT_C i2c_mode i2c_master_write_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count);
EXPORT_C i2c_mode i2c_master_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count, uint8_t *data);
EXPORT_C void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);

#endif /* LIB_I2C_MASTER_F2247_G2xxx_H_ */
