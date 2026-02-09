/*
 * interrupt.h
 *
 *  Created on: Feb 8, 2026
 *      Author: Personal
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "stm32l433xx.h"
void p3_exti_init();
void tim_interrupt(uint32_t);
#endif /* INTERRUPT_H_ */
