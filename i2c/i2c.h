/*
 * i2c.h
 *
 *  Created on: Feb 7, 2026
 *      Author: Personal
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32l433xx.h"
void i2c_init();
uint16_t i2c_read(uint8_t reg);
void i2c_write(uint8_t reg,uint16_t value);

#endif /* I2C_H_ */
