#include "stm32f4xx.h"
#include "usart.h"
#include "ThreeDof.h"
#include "key.h"
#include "myflash.h"

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    ThreeDof_Init();
	uart_init();
	while(1)
	{
		
	}
}
