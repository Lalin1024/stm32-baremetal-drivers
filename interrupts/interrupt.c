/*
 * interrupt.c
 *
 * External interrupt and timer interrupt configuration
 */

#include "interrupt.h"

#define GPIOBEN	(1U<<1)
#define SYSCFGEN	(1U<<0)
#define TIM6_EN	(1U<<4)

void p3_exti_init()
{
	/* Disable global interrupts during configuration */
	__disable_irq();

	/* Enable GPIOB and SYSCFG clocks */
	RCC->AHB2ENR|=GPIOBEN;
	RCC->APB2ENR|=SYSCFGEN;

	/* Configure PB3 as input */
	GPIOB->MODER&=~(3U<<6);

	/* Enable pull-up on PB3 */
	GPIOB->PUPDR&=~(3U<<6);
	GPIOB->PUPDR|=(1U<<7);

	/* Map EXTI line 3 to PB3 */
	SYSCFG->EXTICR[0]&=~(3U<<12);
	SYSCFG->EXTICR[0]|=(1U<<12);

	/* Unmask EXTI line 3 */
	EXTI->IMR1|=(1U<<3);

	/* Configure rising edge trigger */
	EXTI->RTSR1|=(1U<<3);

	/* Enable EXTI line 3 interrupt in NVIC */
	NVIC_EnableIRQ(EXTI3_IRQn);

	/* Re-enable global interrupts */
	__enable_irq();
}

void tim_interrupt(uint32_t value)
{
	/* Enable TIM6 clock */
	RCC->APB1ENR1|=TIM6_EN;

	/* Configure prescaler for 1ms time base */
	TIM6->PSC=15999;
	TIM6->ARR=((value)  -1);

	/* Reset counter */
	TIM6->CNT=0;

	/* Enable update interrupt */
	TIM6->DIER |= (1U<<0);

	/* Configure NVIC for TIM6 interrupt */
	NVIC_SetPriority(TIM6_DAC_IRQn,2);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);

	/* Start TIM6 */
	TIM6->CR1|=(1U<<0);
}
