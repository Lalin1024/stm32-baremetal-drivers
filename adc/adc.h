/*
 * adc.h
 *
 *  Created on: Feb 8, 2026
 *      Author: Personal
 */

#ifndef ADC_H_
#define ADC_H_
#include "stm32l433xx.h";
void adc_init();
void start_conversion();
uint16_t adc_read();
#endif /* ADC_H_ */
