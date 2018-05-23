#include "timer.h"
#include "key.h"
#include "include.h"
#include "lcd.h"
#include "beep.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

extern vu16 USART2_RX_STA;

//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART2_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIM7更新中断标志    
		TIM_Cmd(TIM7, DISABLE);  //关闭TIM7 
	}	    
}
 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7时钟使能    
	
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM7中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}


//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM3_Int_Init(u16 arr,u16 psc)
{	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟

    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
    TIM_Cmd(TIM3,ENABLE); //使能定时器3

    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}


extern u8 use_machine;
extern u8 display_manual ;
extern u8 beep_count ;
extern u8 control_door_flag;
static u8 times_count_1 = 0;
static u16 times_count_2 = 0;
static u16 times_count_3 = 0;
extern u8 manual_cmd;
//定时器3中断服务程序		    
void TIM3_IRQHandler(void)
{ 	
    u8 key_value ;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//是更新中断
	{	 	
		   
        times_count_1++;
        
        if(times_count_1 %10 ==0)
        {
            key_value = 17;
            key_value = keypress();
        }
        if(display_manual == 0)
        {
            not_use_manual(key_value);
        }
        else if (display_manual == 1)
        {
            use_manual(key_value);
        }
        
        if(times_count_1>=100)//100*10 ==1000ms
        {
            if(beep_count >= 3)
            {
                times_count_3 ++;
            }
            times_count_1 = 0;
        }
//步进电机延时        
        if(control_door_flag != 0)
        {
            times_count_2++;
        }
        if(times_count_2 >= 500&&times_count_2< 1000)
        {
            control_door_flag = 2;
        }
        else if(times_count_2>= 1000)
        {
            times_count_2=0;
            control_door_flag = 0;
        }
//报警延时
        if(times_count_3 >=3)
        {
            warn(0);
            beep_count =0;
            times_count_3 = 0;
        }
        else if(times_count_3 > 0)
        {
            warn(1);
        }
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIM7更新中断标志    
	}	    
}


/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:当显示菜单时，不进行开门操作
manual_cmd:         1 :欲添加密码
                    2：欲增加指纹
                    11：正式修改密码
                    12：正式增加指纹
*@Author		:K.G. 
****************************************************/
void use_manual(u8 key_value)
{
    static u8 num = 0;
    if(times_count_1 %10 == 0)
    {
        if(key_value != 17)
        {
            
            if(key_value<10&&num<4&&manual_cmd >= 10)                 //输入密码
            {
                key_input[num] = key_value+0x30;
                num++;
            }
            else if(key_value == 10&&manual_cmd >= 10)                //删除密码                
            {
                num--;
                key_input[num]=' ';           
            }
            else if(key_value == 1&&manual_cmd <=10)             
            {
                    manual_cmd = 1;                   
            }
            else if(key_value == 2&&manual_cmd <=10)
            {
                    manual_cmd = 2;
            }
            else if(key_value == 3&&manual_cmd <=10)
            {
                    manual_cmd = 3;
            }
            else if(key_value == 4&&manual_cmd <=10)
            {
                    manual_cmd = 4;
            }
            else if(key_value == 5&&manual_cmd <=10)
            {
                    manual_cmd = 5;
            }
            else if (key_value == 11)
            {
                num = 0;
                
                
                if(manual_cmd == 1)
                {
                    manual_cmd = 11;
   
                }
                else if(manual_cmd ==2 )
                {
                    manual_cmd = 12;
                }
                else if(manual_cmd ==3 )
                {
                    manual_cmd = 13;
                }
                else if(manual_cmd ==4 )
                {
                    manual_cmd = 14;
                }
                else if(manual_cmd ==5 )
                {
                    manual_cmd = 15;
                }
                else if (manual_cmd >10)
                {
                    enter_press = 1;
                }
                
                else {
                    
                }
            }
            else if (key_value == 13)
            {
                display_manual = 0;
            }
            
            if(num!= 0 )
            {
                
                LCD_ShowString(60,130,24,12,12,key_input);
            }
            else 
                LCD_Fill(0,130,128,160,WHITE);
            LCD_ShowxNum(0,130,key_value,2,12,0);
        }
        
        
    }
        
}

/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:当不显示菜单时，需要进行开门操作，模式转换操作
*@Author		:K.G. 
****************************************************/
void not_use_manual(u8 key_value)
{
    if(times_count_1 %10 ==0)
        {
            if(use_machine == 1)
            {
                KeyOpen(key_value);
            }else {
                ChangeMode(key_value);
            }
        }
        
        
            
        
    
}


