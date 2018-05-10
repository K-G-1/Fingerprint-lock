#include "include.h"


u16 Rc_Pwm_In[8];
T_RC_DATA Rc_Data;//1000~2000
u16  RC_Pwm_In_his[4];


void PWM_IN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	 
	
//////////////////////////////////////////////////////////////////////////////////////////////
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
  
	TIM3->PSC = (168/2)-1;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
/////////////////////////////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
  
	TIM4->PSC = (168/2)-1;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
}

void TIM3_IRQHandler(void)	
{
	static u16 temp_cnt1,temp_cnt1_2,temp_cnt2,temp_cnt2_2,temp_cnt3,temp_cnt3_2,temp_cnt4,temp_cnt4_2;
	

	
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1); //清除中断标志位

		if(GPIOC->IDR & GPIO_Pin_6)
		{
			temp_cnt1 = TIM_GetCapture1(TIM3);
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
		}
		else
		{
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			temp_cnt1_2 = TIM_GetCapture1(TIM3);
			if(temp_cnt1_2>=temp_cnt1)
				Rc_Pwm_In[0] = temp_cnt1_2-temp_cnt1;
			else
				Rc_Pwm_In[0] = 0xffff-temp_cnt1+temp_cnt1_2+1;
		}
	}
	if(TIM3->SR & TIM_IT_CC2) 
	{
		TIM3->SR = ~TIM_IT_CC2;
		TIM3->SR = ~TIM_FLAG_CC2OF;
		if(GPIOC->IDR & GPIO_Pin_7)
		{
			temp_cnt2 = TIM_GetCapture2(TIM3);
		}
		else
		{
			temp_cnt2_2 = TIM_GetCapture2(TIM3);
			if(temp_cnt2_2>=temp_cnt2)
				Rc_Pwm_In[1] = temp_cnt2_2-temp_cnt2;
			else
				Rc_Pwm_In[1] = 0xffff-temp_cnt2+temp_cnt2_2+1;
		}
	}
	if(TIM3->SR & TIM_IT_CC3) 
	{
		TIM3->SR = ~TIM_IT_CC3;
		TIM3->SR = ~TIM_FLAG_CC3OF;
		if(GPIOB->IDR & GPIO_Pin_0)
		{
			temp_cnt3 = TIM_GetCapture3(TIM3);
		}
		else
		{
			temp_cnt3_2 = TIM_GetCapture3(TIM3);
			if(temp_cnt3_2>=temp_cnt3)
				Rc_Pwm_In[2] = temp_cnt3_2-temp_cnt3;
			else
				Rc_Pwm_In[2] = 0xffff-temp_cnt3+temp_cnt3_2+1;
		}
	}
	if(TIM3->SR & TIM_IT_CC4) 
	{
		TIM3->SR = ~TIM_IT_CC4;
		TIM3->SR = ~TIM_FLAG_CC4OF;
		if(GPIOB->IDR & GPIO_Pin_1)
		{
			temp_cnt4 = TIM_GetCapture4(TIM3);
		}
		else
		{
			temp_cnt4_2 = TIM_GetCapture4(TIM3);
			if(temp_cnt4_2>=temp_cnt4)
				Rc_Pwm_In[3] = temp_cnt4_2-temp_cnt4;
			else
				Rc_Pwm_In[3] = 0xffff-temp_cnt4+temp_cnt4_2+1;
		}
	}
}

void TIM4_IRQHandler(void)		
{
	static u16 temp_cnt1,temp_cnt1_2,temp_cnt2,temp_cnt2_2,temp_cnt3,temp_cnt3_2,temp_cnt4,temp_cnt4_2;
	

	
	if(TIM4->SR & TIM_IT_CC1) 
	{
		TIM4->SR = ~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		TIM4->SR = ~TIM_FLAG_CC1OF;
		if(GPIOD->IDR & GPIO_Pin_12)
		{
			temp_cnt1 = TIM_GetCapture1(TIM4);
		}
		else
		{
			temp_cnt1_2 = TIM_GetCapture1(TIM4);
			if(temp_cnt1_2>=temp_cnt1)
				Rc_Pwm_In[4] = temp_cnt1_2-temp_cnt1;
			else
				Rc_Pwm_In[4] = 0xffff-temp_cnt1+temp_cnt1_2+1;
		}
	}
	if(TIM4->SR & TIM_IT_CC2) 
	{
		TIM4->SR = ~TIM_IT_CC2;
		TIM4->SR = ~TIM_FLAG_CC2OF;
		if(GPIOD->IDR & GPIO_Pin_13)
		{
			temp_cnt2 = TIM_GetCapture2(TIM4);
		}
		else
		{
			temp_cnt2_2 = TIM_GetCapture2(TIM4);
			if(temp_cnt2_2>=temp_cnt2)
				Rc_Pwm_In[5] = temp_cnt2_2-temp_cnt2;
			else
				Rc_Pwm_In[5] = 0xffff-temp_cnt2+temp_cnt2_2+1;
		}
	}
	if(TIM4->SR & TIM_IT_CC3) 
	{
		TIM4->SR = ~TIM_IT_CC3;
		TIM4->SR = ~TIM_FLAG_CC3OF;
		if(GPIOD->IDR & GPIO_Pin_14)
		{
			temp_cnt3 = TIM_GetCapture3(TIM4);
		}
		else
		{
			temp_cnt3_2 = TIM_GetCapture3(TIM4);
			if(temp_cnt3_2>=temp_cnt3)
				Rc_Pwm_In[6] = temp_cnt3_2-temp_cnt3;
			else
				Rc_Pwm_In[6] = 0xffff-temp_cnt3+temp_cnt3_2+1;
		}
	}
	if(TIM4->SR & TIM_IT_CC4) 
	{
		TIM4->SR = ~TIM_IT_CC4;
		TIM4->SR = ~TIM_FLAG_CC4OF;
		if(GPIOD->IDR & GPIO_Pin_15)
		{
			temp_cnt4 = TIM_GetCapture4(TIM4);
		}
		else
		{
			temp_cnt4_2 = TIM_GetCapture4(TIM4);
			if(temp_cnt4_2>=temp_cnt4)
				Rc_Pwm_In[7] = temp_cnt4_2-temp_cnt4;
			else
				Rc_Pwm_In[7] = 0xffff-temp_cnt4+temp_cnt4_2+1;
		}
	}
}



/*====================================================================================================*/
/*====================================================================================================*
**函数 : RcData_Refine
**功能 : 提炼遥控数据
**输入 : None
**输出 : None
**备注 : 无
**====================================================================================================*/
/*====================================================================================================*/

extern u16 Moto_duty[4];
void RC_Data_Refine(void)
{
  u8 chan,a;	

	u16 rcDataMax[4], rcDataMin[4];
	static int16_t rcDataCache[4][4], rcDataMean[4];
	static uint8_t rcValuesIndex = 0;

	rcValuesIndex++;
	for (chan = 0; chan < 4; chan++) {
		  //滑动平均值滤波，4次
		  if(Rc_Pwm_In[chan]>2800 || Rc_Pwm_In[chan]<800) 
				Rc_Pwm_In[chan] = RC_Pwm_In_his[chan];
			rcDataCache[chan][rcValuesIndex % 4] = Rc_Pwm_In[chan] ;		
		  RC_Pwm_In_his[chan] = Rc_Pwm_In[chan];
			
			rcDataMean[chan] = 0;
		  rcDataMax[chan]  = 0;
		  rcDataMin[chan]  = 25000;
		
			for (a = 0; a < 4; a++) {
				  // 记录缓存中最大值 && 最小值
				  if(rcDataCache[chan][a] > rcDataMax[chan])  rcDataMax[chan] = rcDataCache[chan][a];     
					if(rcDataCache[chan][a] < rcDataMin[chan])	rcDataMin[chan] = rcDataCache[chan][a]; 
				  // 求和
					rcDataMean[chan] += rcDataCache[chan][a];  
      }
			// 剔除缓存中 最大值 && 最小值 
			rcDataMean[chan] = (rcDataMean[chan] - (rcDataMax[chan] + rcDataMin[chan])) / 2;
	} 
	
	//for(chan=0;chan<6;chan++)
	 Rc_Data.YAW   = rcDataMean[0];
	 Rc_Data.THROTTLE  = rcDataMean[1];
	 Rc_Data.PITCH  =   rcDataMean[3];
	 Rc_Data.ROLL =  rcDataMean[2];

	

}


/*    上锁&解锁函数  */
void Deblocking(void)
{
	 static vs8 flag=1;
	 static vs16 time1=0,time2=0;
	 /*               遥控上锁                 */
	 /*     —————————            —————————     */
	 /*    |         |          |         |    */
	 /*    |         |          |         |    */
	 /*    |    \    |          |   /     |    */
	 /*    |     \   |          |  /      |    */
	 /*     —————————            —————————     */
	 /*   油门拉到最低         摇杆推到左上角  */
   if(ARMED && Rc_Data.ROLL >= 1800 && Rc_Data.PITCH <= 1200 && Rc_Data.THROTTLE <= 1200 &&  Rc_Data.YAW <= 1200)		
	 {  
			time1++; 
	 }	
	 else 
		 time1=0;
	 if(time1>30 && ARMED) 
	 { 
			ARMED = 0; 
			time1 = 0;
			led1=1;
	 }
   /*               遥控解锁                 */
	 /*     —————————            —————————     */
	 /*    |         |          |         |    */
	 /*    |         |          |         |    */
	 /*    |    /    |          |     \   |    */
	 /*    |   /     |          |      \  |    */
	 /*     —————————            —————————     */
	 /*   油门拉到最低         摇杆推到右上角  */
   if(!ARMED && Rc_Data.YAW >= 1800 && Rc_Data.PITCH <= 1200 && Rc_Data.THROTTLE <= 1200 &&  Rc_Data.ROLL <= 1200)		
		{
			time2++; 
			
		}	
	 else 
		 time2=0;
	 if(time2>=30 && !ARMED)
	 {
		  ARMED = 1; 
			time2 = 0;
		  led1=0;
	 }

}










