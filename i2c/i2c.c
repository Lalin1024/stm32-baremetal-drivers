/*
 * i2c.c
 *
 * Bare-metal I2C driver implementation
 */

#include "i2c.h"

#define GPIOAEN	(1U<<0)
#define I2C1EN	(1U<<21)

void i2c_init()
{
	/* PA9 -> I2C1_SCL, PA10 -> I2C1_SDA */

	/* Enable GPIOA clock */
	RCC->AHB2ENR|=GPIOAEN;

	/* Configure PA9 and PA10 as alternate function */
	GPIOA->MODER&=~((3U<<18)|(3U<<20));
	GPIOA->MODER|=((1U<<19)|(1U<<21));

	/* Enable pull-up resistors */
	GPIOA->PUPDR&=~(15U<<18);
	GPIOA->PUPDR|=((1U<<18)|(1U<<20));

	/* Configure open-drain output */
	GPIOA->OTYPER|=((1U<<9)|(1U<<10));

	/* Set medium speed */
	GPIOA->OSPEEDR&=~((3U<<18)|(3U<<20));
	GPIOA->OSPEEDR|=((1U<<18)|(1U<<20));

	/* Select alternate function AF4 for I2C */
	GPIOA->AFR[1]&=~((15U<<4)|(15U<<8));
	GPIOA->AFR[1]|=((4U<<4)|(4U<<8));

	/* Enable I2C1 clock */
	RCC->APB1ENR1|=I2C1EN;

	/* Disable I2C peripheral before configuration */
	I2C1->CR1&=~(1U<<0);

	/* Enable analog noise filter */
	I2C1->CR1|=(1U<<12);

	/* Disable digital noise filter */
	I2C1->CR1 &=~(0xF<<8);

	/* Configure I2C timing */
	I2C1->TIMINGR=0;
	I2C1->TIMINGR|=((0x3<<28)|(0x13<<0)|(0xF<<8)|(0x2<<16)|(0x4<<20));

	/* Enable clock stretching */
	I2C1->CR1&=~(1U<<17);

	/* Enable I2C peripheral */
	I2C1->CR1|=(1U<<0);
}

void i2c_write(uint8_t reg,uint16_t value)
{
	/* Configure I2C transfer for write operation */
	I2C1->CR2=((3U<<16)|(1U<<25)|(0x60<<1)|(0U<<10)|(1U<<13));

	/* Send register address */
	while(!(I2C1->ISR & (1U<<1))){}
	I2C1->TXDR=reg;

	/* Send low byte */
	while(!(I2C1->ISR & (1U<<1))){}
	I2C1->TXDR=(value & 0xFF);

	/* Send high byte */
	while(!(I2C1->ISR & (1U<<1))){}
	I2C1->TXDR=(value>>8);
}

uint16_t i2c_read(uint8_t reg)
{
	uint8_t low,high;

	/* Configure I2C transfer for register address write */
	I2C1->CR2=((1U<<16)|(0U<<25)|(0x60<<1)|(0U<<10)|(1U<<13));

	/* Send register address */
	while(!(I2C1->ISR & (1U<<1))){}
	I2C1->TXDR=reg;

	/* Wait for transfer complete */
	while(!(I2C1->ISR & (1U<<6))){}

	/* Configure repeated start for read operation */
	I2C1->CR2=((2U<<16)|(1U<<25)|(0x60<<1)|(1U<<10)|(1U<<13));

	/* Read low byte */
	while(!(I2C1->ISR & (1U<<2))){}
	low=I2C1->RXDR;

	/* Read high byte */
	while(!(I2C1->ISR & (1U<<2))){}
	high=I2C1->RXDR;

	/* Combine and return 16-bit data */
	return((high<<8)|low);
}
