#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

void uart_init(void);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
	
#endif
