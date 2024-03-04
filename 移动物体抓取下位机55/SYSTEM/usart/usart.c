#include "sys.h"
#include "usart.h"	
#include <stdlib.h>
#include <string.h>
#include "ThreeDof.h"

uint8_t Start_T[6] = {0x11,0x11,0x11,0x11,0x11,0x11};
uint8_t Stop[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void uart_init(void){
	
   //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //GPIOA9 A10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9 A10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 460800;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
    USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

}

void pulse_message(uint8_t *receive){
	
	uint16_t pulse[6] = {0};
	static uint16_t old_pulse[6] = {0};
	uint8_t direction[6] = {0};

			for(uint8_t i = 0;i < 6;i++){
				pulse[i] |= receive[1+i*6];
				pulse[i] |= receive[2+i*6]<<8;
				direction[i] |= receive[3+i*6];
			}
		if((old_pulse[0]!=0xffff) && pulse[0] == 0xffff)//为防止一出现0xffff就关闭定时器，只有出现前一个脉冲不是0xffff，且当次为0xffff时关掉定时器
			TIM_Cmd(TIM9,DISABLE);
		if((old_pulse[0] == 0xffff) && pulse[0] != 0xffff)//当该轴需要运动时，先判断先前是否处于停止状态，才开启定时器，防止多次开定时器
			TIM_Cmd(TIM9,ENABLE);
		if((old_pulse[1]!=0xffff) && pulse[1] == 0xffff)
			TIM_Cmd(TIM10,DISABLE);
		if((old_pulse[1] == 0xffff) && pulse[1] != 0xffff)
			TIM_Cmd(TIM10,ENABLE);
		if((old_pulse[2]!=0xffff) && pulse[2] == 0xffff)
			TIM_Cmd(TIM11,DISABLE);
		if((old_pulse[2] == 0xffff) && pulse[2] != 0xffff)
			TIM_Cmd(TIM11,ENABLE);
	    if((old_pulse[3]!=0xffff) && pulse[3] == 0xffff)//为防止一出现0xffff就关闭定时器，只有出现前一个脉冲不是0xffff，且当次为0xffff时关掉定时器
			TIM_Cmd(TIM12,DISABLE);
		if((old_pulse[3] == 0xffff) && pulse[3] != 0xffff)//当该轴需要运动时，先判断先前是否处于停止状态，才开启定时器，防止多次开定时器
			TIM_Cmd(TIM12,ENABLE);
		if((old_pulse[4]!=0xffff) && pulse[4] == 0xffff)
			TIM_Cmd(TIM13,DISABLE);
		if((old_pulse[4] == 0xffff) && pulse[4] != 0xffff)
			TIM_Cmd(TIM13,ENABLE);
		if((old_pulse[5]!=0xffff) && pulse[5] == 0xffff)
			TIM_Cmd(TIM14,DISABLE);
		if((old_pulse[5] == 0xffff) && pulse[5] != 0xffff)
			TIM_Cmd(TIM14,ENABLE);
		
		Set_Joint_Pulse(pulse[0],pulse[1],pulse[2],pulse[3],pulse[4],pulse[5]);	
		Set_direction(direction[0],direction[1],direction[2],direction[3],direction[4],direction[5]);
		
		for(uint8_t i=0;i<6;i++){
			old_pulse[i] = pulse[i];
		}
}
		
void state_message(uint8_t *receive){
	
	if(receive[0] == 0xbb){
		TIM_Cmd(TIM7,ENABLE);
		TIM_Cmd(TIM9,ENABLE);
		TIM_Cmd(TIM10,ENABLE);
		TIM_Cmd(TIM11,ENABLE);
		TIM_Cmd(TIM12,ENABLE);
		TIM_Cmd(TIM13,ENABLE);
		TIM_Cmd(TIM14,ENABLE);
	}
	if(receive[0] == 0x1f){
		TIM_Cmd(TIM7,DISABLE);
		TIM_Cmd(TIM9,DISABLE);
		TIM_Cmd(TIM10,DISABLE);
		TIM_Cmd(TIM11,DISABLE);
		TIM_Cmd(TIM12,DISABLE);
		TIM_Cmd(TIM13,DISABLE);
		TIM_Cmd(TIM14,DISABLE);
	}

}
void USART1_IRQHandler(void){  //串口1中断服务程序
	
	static uint8_t old_receive = 0;
	static uint8_t same_sign = 0;
	
	static uint8_t Get_Data[36] = {0};
	static uint8_t num = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		uint8_t ReceiveData = USART_ReceiveData(USART1);
		Get_Data[num] = ReceiveData;
		num++;
		
		if(ReceiveData == old_receive){
		same_sign++;
		}
		if(ReceiveData != old_receive){
		same_sign = 0;
		}
		old_receive = ReceiveData;
		
		if(same_sign == 3){
		state_message(Get_Data);	
		num = 0;
		old_receive = 0;	
		}
		
		
		if(num == 36){
		pulse_message(Get_Data);
		num = 0;
		}
	} 
USART_ClearITPendingBit(USART1, USART_IT_RXNE);
} 

 



