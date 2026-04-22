
#include "uart.h"

#define SYSCLK		16000000UL
#define	GPIOAEN		(1U<<0)
#define	GPIOBEN		(1U<<1)
#define USART2EN	(1U<<17)
#define USART3EN	(1U<<18)

static void baud_rate(USART_TypeDef* USART,uint32_t baud);

void uart_init()
{
	/*PA2=USART2_TX, PA3,USART2_RX, PB11=USART3_RX*/
	RCC->AHB2ENR|=(GPIOAEN|GPIOBEN);

	GPIOA->MODER&=~((3U<<4)|(3U<<6));
	GPIOA->MODER|=((1U<<5)|(1U<<7));

	GPIOB->MODER&=~(3U<<22);
	GPIOB->MODER|=(1U<<23);

	GPIOA->OSPEEDR&=~((3U<<4)|(3U<<6));
	GPIOA->OSPEEDR|=((1U<<5)|(1U<<7));	//High speed for Transmission and Reception USART2

	GPIOB->OSPEEDR&=~(3U<<22);
	GPIOB->OSPEEDR|=(3U<<22);//Medium speed for USART3_RX


	GPIOA->AFR[0]&=~((15U<<8)|(15U<<12));
	GPIOA->AFR[0]|=((7U<<8)|(7U<<12));

	GPIOB->AFR[1]&=~(15U<<12);
	GPIOB->AFR[1]|=(7U<<12);

	RCC->APB1ENR1&=~(3U<<17);
	RCC->APB1ENR1|=(USART2EN|USART3EN);

	USART2->CR1&=~((1U<<28)|(1U<<12));	//8 data bits, 1 Start bit
	USART3->CR1&=~((1U<<28)|(1U<<12));	//8 data bits, 1 Start bit

	baud_rate(USART2,115200);
	baud_rate(USART3,9600);

	USART2->CR2&=~(3U<<12);
	USART3->CR2&=~(3U<<12);

	USART2->CR1|=(1U<<0);
	USART3->CR1|=(1U<<0);

	USART2->CR1|=((1U<<3)|(1U<<2)); //Both TX and RX enabled

	USART3->CR1|=(1U<<2);

	USART3->CR1|=(1U<<5);//Enabling interrupt for reception

}


void uart_write(const char* value)
{
	while(*value)
		{
		while(!(USART2->ISR & (1U<<7))){}

		USART2->TDR=(*value++);
	}
}



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


static void baud_rate(USART_TypeDef* USART,uint32_t baud)
{
	USART->BRR =( (SYSCLK + (baud/2)) / baud);
}



