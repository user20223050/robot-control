#include "sys.h"
#include "usart.h"
#include "ThreeDof.h"

//PD8        joint1  direction pin
//PD9        joint2  direction pin
//PD10       joint3  direction pin
//PD11       joint4  direction pin
//PD12       joint5  direction pin
//PD7        joint6  direction pin

//TIM9   Oc1 joint1  pulse PE5 -TIM9 CH1
//TIM10  Oc1 joint2  pulse PF6 TIM10 CH1
//TIM11  Oc1 joint3  pulse PF7 TIM11 CH1
//TIM12  Oc1 joint4  PB14
//TIM13  OC1 joint5  PF8
//TIM14  OC1 JOINT6  PF9

char SEND_ARRAY[1] = {0x34};//50ms发送一次给上位机

void Joint_Direct_Init(void)//PD8 PD9 PD10 PD11 PD12 PD7Init
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//初始上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOD,GPIO_Pin_8,0);
	GPIO_WriteBit(GPIOD,GPIO_Pin_9,0);
	GPIO_WriteBit(GPIOD,GPIO_Pin_10,1);
	GPIO_WriteBit(GPIOD,GPIO_Pin_11,1);
	GPIO_WriteBit(GPIOD,GPIO_Pin_12,0);
	GPIO_WriteBit(GPIOD,GPIO_Pin_7,1);
}
void Joint_Pulse_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//PE5 -TIM9 CH1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//PF6 TIM10 CH1 PF7 TIM11 CH1
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9); //GPIOE5复用为定时器9
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10); //GPIOF6复用为定时器10
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_TIM11); //GPIOF7复用为定时器11
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); //GPIOB14复用为定时器9
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_TIM13); //GPIOF8复用为定时器10
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9复用为定时器11
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PE5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           //GPIOB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PE5
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0Xffff;// ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 4-1;//PSC APB2 Timer Clock/4 == 42M
 	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseInitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0Xffff;// ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 2-1;//PSC APB1 Timer Clock/2 == 42M
 	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0xffff;		//CCR
	
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);
	TIM_OC1Init(TIM10, &TIM_OCInitStructure);
	TIM_OC1Init(TIM11, &TIM_OCInitStructure);
	TIM_OC1Init(TIM12, &TIM_OCInitStructure);
	TIM_OC1Init(TIM13, &TIM_OCInitStructure);
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	
	TIM_ARRPreloadConfig(TIM9, DISABLE);
	TIM_ARRPreloadConfig(TIM10, DISABLE);
	TIM_ARRPreloadConfig(TIM11, DISABLE);
	TIM_ARRPreloadConfig(TIM12, DISABLE);
	TIM_ARRPreloadConfig(TIM13, DISABLE);
	TIM_ARRPreloadConfig(TIM14, DISABLE);
	
	TIM_OC1PreloadConfig(TIM9,TIM_OCPreload_Disable);
	TIM_OC1PreloadConfig(TIM10,TIM_OCPreload_Disable);
	TIM_OC1PreloadConfig(TIM11,TIM_OCPreload_Disable);
	TIM_OC1PreloadConfig(TIM12,TIM_OCPreload_Disable);
	TIM_OC1PreloadConfig(TIM13,TIM_OCPreload_Disable);
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Disable);
}

void Set_Joint_Pulse(uint16_t pulse1, uint16_t pulse2, uint16_t pulse3,uint16_t pulse4,uint16_t pulse5,uint16_t pulse6)
{
	TIM_SetCounter(TIM9,0);
	TIM_SetCompare1(TIM9,(pulse1/2));
	TIM_SetAutoreload(TIM9,pulse1);
	
	TIM_SetCounter(TIM10,0);
	TIM_SetCompare1(TIM10,(pulse2/2));
	TIM_SetAutoreload(TIM10,pulse2);
	
	TIM_SetCounter(TIM11,0);
	TIM_SetCompare1(TIM11,(pulse3/2));
	TIM_SetAutoreload(TIM11,pulse3);
	
	TIM_SetCounter(TIM12,0);
	TIM_SetCompare1(TIM12,(pulse4/2));
	TIM_SetAutoreload(TIM12,pulse4);
	
	TIM_SetCounter(TIM13,0);
	TIM_SetCompare1(TIM13,(pulse5/2));
	TIM_SetAutoreload(TIM13,pulse5);
	
	TIM_SetCounter(TIM14,0);
	TIM_SetCompare1(TIM14,(pulse6/2));
	TIM_SetAutoreload(TIM14,pulse6);
	
}
void Set_direction(uint8_t turn1,uint8_t turn2,uint8_t turn3,uint8_t turn4,uint8_t turn5,uint8_t turn6){
	GPIO_WriteBit(GPIOD,GPIO_Pin_8,turn1);
	GPIO_WriteBit(GPIOD,GPIO_Pin_9,turn2);
	GPIO_WriteBit(GPIOD,GPIO_Pin_10,turn3);
	GPIO_WriteBit(GPIOD,GPIO_Pin_11,turn4);
	GPIO_WriteBit(GPIOD,GPIO_Pin_12,turn5);
	GPIO_WriteBit(GPIOD,GPIO_Pin_7,turn6);
	
}
void Pulse_Inquire_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 50000-1;	//50ms
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1;	//1MHZ	APB1 Timer Clock 84M/84 == 1M
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器7更新中断

	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn ; //定时器7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ThreeDof_Init(void)
{
	Joint_Direct_Init();
	Joint_Pulse_Init();
	Pulse_Inquire_Init();
}

void TIM7_IRQHandler(void)
{
	static int Send_Times = 0;
	
	if(TIM_GetITStatus(TIM7,TIM_IT_Update) == SET)
	{
		Send_Times++;
		Serial_SendArray(SEND_ARRAY, 1);
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}
