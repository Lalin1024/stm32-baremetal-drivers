/*
 * uart.c
 *
 * UART driver implementation for USART2
 *
 * Created on: Feb 6, 2026
 * Author: lalin
 */

#include "uart.h"

/* Clock enable macros */
#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)

/* System clock frequency */
#define SYSCLK		(16000000UL)

/* USART enable bit */
#define USARTEN		(1U<<0)

/* Private function prototype for baud rate configuration */
static void set_baud(USART_TypeDef* USART,uint32_t bard);

void uart_init()
{
	/* PA2 -> USART2_TX, PA3 -> USART2_RX */

	/* Enable GPIOA clock */
	RCC->AHB2ENR|=GPIOAEN;

	/* Select system clock as USART2 clock source */
	RCC->CCIPR&=~(3U<<2);
	RCC->CCIPR|=(1U<<2);

	/* Configure PA2 and PA3 to alternate function mode */
	GPIOA->MODER&=~((3U<<4)|(3U<<6));
	GPIOA->MODER|=((1U<<5)|(1U<<7));

	/* Select AF7 for USART2 TX and RX */
	GPIOA->AFR[0]&=~((15U<<8)|(15U<<12));
	GPIOA->AFR[0]|=((7U<<8)|(7U<<12)); // AF7 mapped to USART2

	/* Enable USART2 clock on APB1 bus */
	RCC->APB1ENR1|=UART2EN;

	/* Configure GPIO output speed for high-speed operation */
	GPIOA->OSPEEDR |= (3U<<4)|(3U<<6);

	/* Reset USART control register */
	USART2->CR1=0;

	/* Configure baud rate */
	set_baud(USART2,115200);

	/* Enable USART peripheral */
	USART2->CR1|=USARTEN;

	/* Enable transmitter */
	USART2->CR1|=(1U<<3);
}

/* Configure USART baud rate */
static void set_baud(USART_TypeDef* USART,uint32_t baud)
{
	/* Baud rate calculation based on system clock */
	USART->BRR=((SYSCLK)/baud);
}

/* Transmit a null-terminated string over UART */
void uart_write(const char* val)
{
	while(*val)
	{
		/* Wait until transmit data register is empty */
		while(!(USART2->ISR & (1U<<7)));

		/* Load character into transmit data register */
		USART2->TDR=(*val++);
	}
}

/* Transmit an unsigned integer value over UART */
void uart_write_int(uint32_t num)
{
	char buff[12];
	int i=0;

	/* Handle zero explicitly */
	if(num==0)
	{
		uart_write("0");
		return;
	}
	else
	{
		/* Convert integer to ASCII (reverse order) */
		while(num!=0)
		{
			buff[i++]=(num%10)+'0';
			num/=10;
		}

		/* Null-terminate the string */
		buff[i]='\0';

		/* Reverse the string to correct digit order */
		for(int j=0;j<i/2;j++)
		{
			char tmp=buff[j];
			buff[j]=buff[i-1-j];
			buff[i-1-j]=tmp;
		}

		/* Transmit the formatted string */
		uart_write(buff);
	}
}
