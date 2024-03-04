#include "sys.h"
#include "usart.h"	
#include <stdlib.h>
#include <string.h>
#include "ThreeDof.h"

uint8_t Start_T[6] = {0x11,0x11,0x11,0x11,0x11,0x11};
uint8_t Stop[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
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
	
   //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //GPIOA9 A10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9 A10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 460800;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

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
		if((old_pulse[0]!=0xffff) && pulse[0] == 0xffff)//Ϊ��ֹһ����0xffff�͹رն�ʱ����ֻ�г���ǰһ�����岻��0xffff���ҵ���Ϊ0xffffʱ�ص���ʱ��
			TIM_Cmd(TIM9,DISABLE);
		if((old_pulse[0] == 0xffff) && pulse[0] != 0xffff)//��������Ҫ�˶�ʱ�����ж���ǰ�Ƿ���ֹͣ״̬���ſ�����ʱ������ֹ��ο���ʱ��
			TIM_Cmd(TIM9,ENABLE);
		if((old_pulse[1]!=0xffff) && pulse[1] == 0xffff)
			TIM_Cmd(TIM10,DISABLE);
		if((old_pulse[1] == 0xffff) && pulse[1] != 0xffff)
			TIM_Cmd(TIM10,ENABLE);
		if((old_pulse[2]!=0xffff) && pulse[2] == 0xffff)
			TIM_Cmd(TIM11,DISABLE);
		if((old_pulse[2] == 0xffff) && pulse[2] != 0xffff)
			TIM_Cmd(TIM11,ENABLE);
	    if((old_pulse[3]!=0xffff) && pulse[3] == 0xffff)//Ϊ��ֹһ����0xffff�͹رն�ʱ����ֻ�г���ǰһ�����岻��0xffff���ҵ���Ϊ0xffffʱ�ص���ʱ��
			TIM_Cmd(TIM12,DISABLE);
		if((old_pulse[3] == 0xffff) && pulse[3] != 0xffff)//��������Ҫ�˶�ʱ�����ж���ǰ�Ƿ���ֹͣ״̬���ſ�����ʱ������ֹ��ο���ʱ��
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
void USART1_IRQHandler(void){  //����1�жϷ������
	
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

 



