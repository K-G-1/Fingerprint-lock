#include "door.h"


void door_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;
	
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
    
}


void opendoor()
{
	u8 i;
	for(i=130;i>0;i--)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_8);
		GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
		delay_ms(10);
		GPIO_SetBits(GPIOE,GPIO_Pin_9);
		GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11);
		delay_ms(10);
		GPIO_SetBits(GPIOE,GPIO_Pin_10);
		GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_11);
		delay_ms(10);
		GPIO_SetBits(GPIOE,GPIO_Pin_11);
		GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_8);
		delay_ms(10);
	}
}

void closedoor(void)
{
	u8 i;
	for(i=130;i>0;i--)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_11);
		GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_10);
		delay_ms(10);
		GPIO_SetBits(GPIOE,GPIO_Pin_10);
		GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_9);
		delay_ms(10);
		GPIO_SetBits(GPIOE,GPIO_Pin_9);
		GPIO_ResetBits(GPIOE,GPIO_Pin_10|GPIO_Pin_8|GPIO_Pin_11);
		delay_ms(10);
		GPIO_SetBits(GPIOE,GPIO_Pin_8);
		GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
		delay_ms(10);
	}
}

