#include "sys.h"
#include "usart.h"	
#include <stdlib.h>
#include <string.h>
#include "ThreeDof.h"

const uint8_t Start_T[6] = {0x11,0x11,0x11,0x11,0x11,0x11};
const uint8_t Stop[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
const uint16_t t[256] ={0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0x0E40,0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,0x5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040};
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
uint16_t CRC16_ModBus(uint8_t *Dat,uint8_t Len)
{
	uint16_t crc = 0xffff;
	uint8_t zj;
	for (uint8_t i= 0; i < Len ; i++)
	{
		zj = (crc & 0xFF) ^ Dat[i];
		crc >>= 8;
		crc^=t[zj];
	}
	return crc;
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
	
	uint8_t data1[6] = {0};
	uint8_t data2[6] = {0};
	uint8_t data3[6] = {0};
	uint8_t data4[6] = {0};
	uint8_t data5[6] = {0};
	uint8_t data6[6] = {0};
	uint16_t crc_check[6] = {0};
	uint16_t pulse[6] = {0};
	static uint16_t old_pulse[6] = {0};
	uint8_t direction[6] = {0};

	for(uint8_t i = 0; i < 6; i++){
	    data1[i] = receive[0+i];
		data2[i] = receive[6+i];
		data3[i] = receive[12+i];
		data4[i] = receive[18+i];
		data5[i] = receive[24+i];
		data6[i] = receive[30+i];
		pulse[i] |= receive[1+i*6];
		pulse[i] |= receive[2+i*6]<<8;
		direction[i] |= receive[3+i*6];
	}
	crc_check[0] = CRC16_ModBus(data1,4);
	crc_check[1] = CRC16_ModBus(data2,4);
	crc_check[2] = CRC16_ModBus(data3,4);
	crc_check[3] = CRC16_ModBus(data4,4);
	crc_check[4] = CRC16_ModBus(data5,4);
	crc_check[5] = CRC16_ModBus(data6,4);
	if(crc_check[0] != ((uint16_t)data1[4]|((uint16_t)data1[5]<<8)) || crc_check[1] != ((uint16_t)data2[4]|((uint16_t)data2[5]<<8)) || crc_check[2] != ((uint16_t)data3[4]|((uint16_t)data3[5]<<8)) || crc_check[3] != ((uint16_t)data4[4]|((uint16_t)data4[5]<<8)) || crc_check[4] != ((uint16_t)data5[4]|((uint16_t)data5[5]<<8)) || crc_check[5] != ((uint16_t)data6[4]|((uint16_t)data6[5]<<8))) {
		TIM_Cmd(TIM7,DISABLE);
		TIM_Cmd(TIM9,DISABLE);
		TIM_Cmd(TIM10,DISABLE);
		TIM_Cmd(TIM11,DISABLE);
		TIM_Cmd(TIM12,DISABLE);
		TIM_Cmd(TIM13,DISABLE);
		TIM_Cmd(TIM14,DISABLE);
		return;
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

 



