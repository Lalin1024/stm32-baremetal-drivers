#include "interrupt.h"

//Enabling USART3 for interrupt
void usart3_interrupt()
{
	NVIC_EnableIRQ(USART3_IRQn);

