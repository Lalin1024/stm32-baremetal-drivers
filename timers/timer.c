/*
 * timer.c
 *
 * System clock configuration and delay utility
 */

#include "timer.h"

#define HSION	(1U<<8)
#define HSIRDY	(1U<<10)
#define TIM6_EN	(1U<<4)

void SetClock_16MHz()
{
	/* Enable internal high-speed oscillator (HSI) */
	RCC->CR|=HSION;

	/* Wait until HSI is ready */
	while(!(RCC->CR & HSIRDY)){}

	/* Select HSI as system clock source */
	RCC->CFGR&=~(3U<<0);
	RCC->CFGR|=(1U<<0);

	/* Wait until HSI is used as system clock */
	while(!(RCC->CFGR & (1U<<2))){}
}

/* Blocking delay using TIM6 (16 MHz system clock) */
void delay(uint32_t value)
{
	/* Enable TIM6 clock */
	RCC->APB1ENR1|=TIM6_EN;

	/* Configure timer for millisecond delay */
	TIM6->PSC=15999;
	TIM6->ARR=((value)  -1);

	/* Reset counter */
	TIM6->CNT=0;

	/* Generate update event */
	TIM6->EGR|=(1U<<0);

	/* Start timer */
	TIM6->CR1|=(1U<<0);

	/* Clear update flag */
	TIM6->SR &= ~(1U << 0);

	/* Wait until update event occurs */
	while(!(TIM6->SR & (1U<<0)));
}
