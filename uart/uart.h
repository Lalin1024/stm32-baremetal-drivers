/*
 * uart.h
 *
 *  Created on: Feb 7, 2026
 *      Author: Personal
 */

#ifndef UART_H_
#define UART_H_

#include "stm32l433xx.h"
void uart_init();
void uart_write(const char* val);
void uart_write_int(uint32_t num);
#endif /* UART_H_ */
