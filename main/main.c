#include "uart.h"
#include "timer.h"
#include "i2c.h"
#include "interrupt.h"

#define GPIOBEN (1U<<1)

uint8_t id=0;
volatile uint8_t flag=0,tim_flag = 0;
volatile uint16_t val=0;

/* Interrupt handler prototypes */
void EXTI3_IRQHandler();
void TIM6_DAC_IRQHandler();

int main()
{
	/* System initialization */
	SetClock_16MHz();
	uart_init();
	i2c_init();
	p3_exti_init();
	tim_interrupt(500);

	uart_write("Initialization done\r\n");

	/* Enable GPIOB clock for LED control */
	RCC->AHB2ENR|=GPIOBEN;

	/* Configure PB13 as output (LED) */
	GPIOB->MODER&=~(3U<<26);
	GPIOB->MODER|=(1U<<26);

	/* Read device ID register */
	id=i2c_read(0x0C);
	uart_write_int(id);
	uart_write("\r\n");

	/* Configure proximity sensor registers */
	i2c_write(0x03,0x0B10);
	i2c_write(0x04,0x4210);
	i2c_write(0x06,0x0014);
	i2c_write(0x07,0x001E);

	/* Delay to allow sensor configuration to settle */
	delay(500);

	while(1)
	{
		if(flag)
		{
			flag = 0;

			/* Turn ON LED for 2 seconds on external interrupt */
			GPIOB->BSRR=(1U<<13);
			delay(2000);
		}

		/* Turn OFF LED */
		GPIOB->BSRR=(1U<<(13+16));

		/* Read proximity sensor value */
		val=i2c_read(0x08);
		uart_write("Prox= ");
		uart_write_int(val);
		uart_write("\r\n");
	}
}

/* External interrupt handler for EXTI line 3 */
void EXTI3_IRQHandler()
{
	 if (EXTI->PR1 & (1U << 3))
	    {
	        /* Clear EXTI pending flag */
	        EXTI->PR1 |= (1U << 3);
	        flag = 1;
	    }
}

/* Timer 6 interrupt handler */
void TIM6_DAC_IRQHandler()
{
	if (TIM6->SR & (1U<<0))
	    {
	        /* Clear update interrupt flag */
	        TIM6->SR &= ~(1U<<0);

	        /* Notify main loop */
	        tim_flag = 1;
	    }
}
