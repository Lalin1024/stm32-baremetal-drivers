/*
 * adc.c
 *
 * Description:
 *   Register-level ADC driver for STM32 microcontroller.
 *   This file contains functions to initialize the ADC,
 *   start a conversion, and read the converted value.
 *
 * Created on: Dec 31, 2025
 * Author: Lalin
 */

#include "adc.h"
#include "timer.h"

#define GPIOCEN     (1U<<2)

void adc_init()
{
    /* Enable clock access to GPIOC on AHB2 bus */
    RCC->AHB2ENR |= GPIOCEN;

    /* Configure PC0 as analog mode (ADC input pin) */
    GPIOC->MODER &= ~(3U<<0);
    GPIOC->MODER |=  (3U<<0);

    /* Configure AHB prescaler (no division) */
    RCC->CFGR &= ~(15U<<4);

    /* Enable clock access to ADC peripheral */
    RCC->AHB2ENR |= (1U<<13);

    /* Configure ADC clock source */
    ADC1_COMMON->CCR &= ~(1U<<17);
    ADC1_COMMON->CCR |=  (1U<<16);

    /* Configure regular conversion sequence:
       - 1 conversion in sequence
       - Channel selected as channel 1 */
    ADC1->SQR1 &= ~(15U<<0);
    ADC1->SQR1 &= ~(0x1F<<6);
    ADC1->SQR1 |=  (1U<<6);

    /* Exit deep power-down mode and enable ADC voltage regulator */
    ADC1->CR &= ~(1U<<29);
    ADC1->CR |=  (1U<<28);

    /* Wait for voltage regulator stabilization */
    delay(500);

    /* Start ADC calibration */
    ADC1->CR |= (1U<<31);
    while (ADC1->CR & (1U<<31)) {}

    /* Enable ADC peripheral */
    ADC1->CR |= (1U<<0);

    /* Wait until ADC is ready */
    while (!(ADC1->ISR & (1U<<0)));
}

/* Start ADC regular conversion */
void start_conversion()
{
    ADC1->CR |= (1U<<2);
}

/* Read ADC converted value */
uint16_t adc_read()
{
    /* Wait until end of conversion */
    while (!(ADC1->ISR & (1U<<2))) {}

    /* Return converted digital value */
    return (ADC1->DR);
}
