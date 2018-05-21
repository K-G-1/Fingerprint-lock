#include "include.h"
#include "led.h"
#include "lcd.h"
#include "IIC.h"
#include  <math.h> 
#include "24cxx.h" 
#include "oled.h"
#include "usart2.h" 
#include "as608.h"
#include "spi.h"
#include "lcd.h"
#include "key.h"
#include "timer.h"
#include "door.h"
#include "RC522.h"
#include "beep.h"
#include "stmflash.h"

u16 ValidN;//模块内有效指纹个数
/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:字符串匹配时易发生越界现象
*@Author		:K.G. 
****************************************************/
u8 use_machine = 0 ;
u8 key_input[5]={"    "};
u8 empty_key[4]={0,0,0,0};

//u8 password[4]={"1234"};
u8 password[4];
u8 num1 = 0;
u8 ic_card_key[4]={6,6,6,6};
u8 num2 = 0;
u8 beep_count = 0;

u8 password_status = 0;
u8 ic_card_status = 0;
u8 fingerprint_status = 0;
u8 fingerprint_flag = 0;
u8 enter_press = 0;
u8 control_door_flag = 0;

u8 display_manual = 0;
u8 manual_cmd = 0;

/***************************************************
*@version		:V1.0
*@CreatDate		:
*@Description	:main主函数，主要进行外设的初始化
*@Author		:K.G. 
****************************************************/
int main()
{
//    u8 key_value = 0;
    u8 error_count = 0;
    u8 triget = 0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);

    uart_init(115200);
    usart2_init(57600);//初始化串口2,用于与指纹模块通讯
    PS_StaGPIO_Init();	//初始化FR读状态引脚
    
    
    led_init();
    beep_init();
    key_init();
    door_init();
    IIC_Init();
    SPI1_Init();
    SPI2_Init();
    AT24CXX_Init();  
    
    MFRC522_Initializtion();
    
    
    TIM3_Int_Init(100-1,8400-1);	//10ms
    my_mem_init(SRAMIN);		//初始化内部内存池
    memset(key_input,0,sizeof(key_input));
//    STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)password,SIZE);  //写入flash 保存密码
    STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)password,SIZE);
//    STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)password,SIZE);  //写入flash 保存密码
#if USE_SPI_TFT

    LCD_Init();
    POINT_COLOR=BLACK;//设置字体为红色 
	LCD_ShowString(0,0,200,16,16,"1.8 TFT SPI");
    showimage(0,0);
    
    LCD_ShowString(0,100,120,12,12,"handle AS608....");
    while(PS_HandShake(&AS608Addr)&&error_count<3)//与AS608模块握手
	{
        error_count++;
		delay_ms(900);
		LCD_Fill(0,100,120,160,WHITE); //初始清屏
		LCD_ShowString(0,115,120,12,12,"not check as608");
		delay_ms(100);
		LCD_ShowString(0,130,120,12,12,"test connecte AS608 ");		  
	}
    if(error_count <3){
        PS_ReadSysPara(&AS608Para);  //读参数 
        LCD_Fill(0,100,120,160,WHITE); //清屏
        LCD_ShowString(0,115,120,12,12,"connection ok!!!");
        PS_ValidTempleteNum(&ValidN);//读库指纹个数
        LCD_ShowNum(100,115,ValidN,3,12);
        fingerprint_flag = 0;
    }
    else {
        LCD_Fill(0,100,120,160,WHITE); //清屏
        LCD_ShowString(0,115,120,12,12,"connection faild!!!");
        use_machine = 1;
        fingerprint_flag =1;
    }
#else

    OLED_Init();

    OLED_P6x8Str(0,0,"oled init");
    
    OLED_P6x8Str(1,0,"handle AS608....");	
	while(PS_HandShake(&AS608Addr)&& error_count<=3)//与AS608模块握手
	{
        error_count++;
		delay_ms(900);
		OLED_Fill(0x00); //初始清屏
		OLED_P6x8Str(1,0,"not check as608");
		delay_ms(100);
		OLED_P6x8Str(2,0,"test connecte AS608 ");		  
	}
    if(error_count<=3)
    {
        PS_ReadSysPara(&AS608Para);  //读参数 
        OLED_Fill(0x00); //清屏
        OLED_P6x8Str(3,0,"connection ok!!!");
        OLED_Fill(0x00); //清屏

        PS_ValidTempleteNum(&ValidN);//读库指纹个数
        OLED_P6x8data(5,0,ValidN);
    }
    else 
    {
        OLED_Fill(0x00); //清屏
        OLED_P6x8Str(3,0,"connection faild!!!");
    }
#endif    
  
//    control_door_flag  =1;
    RUN:
    TIM_Cmd(TIM3,DISABLE);
    LCD_Fill(0,0,128,160,WHITE);
    TIM_Cmd(TIM3,ENABLE);
    memset(key_input,0,sizeof(key_input));
    
    SPI2_Init();
    MFRC522_Initializtion();
    warn(1);
    
    delay_ms(100);
    warn(0);
    beep_count =0 ;
    while(1)
    {
        led1 = ~led1;
        
        if(display_manual ==1)
        {
            manual_cmd = 0;
            goto MANUL;
        }
//        key_value = keypress();
//        
//        if(key_value == 15 )
//        {
//            RC522_Add_ic_card();
//        }
//        else RC522_Read_Mess();
////        delay_ms(100);
        if(use_machine == 0)
        {
            LCD_ShowString(0,100,120,12,12,"use figure");
            
            press_FR();
        }
        else if(use_machine == 1)
        {
            LCD_ShowString(0,100,120,12,12,"use key    ");
            
        }
        else if(use_machine == 2)
        {
            LCD_ShowString(0,100,120,12,12,"use ic card");
            if(RC522_Read_Mess()==0 && control_door_flag ==0)
                control_door_flag= 1;
            
        }
        
        if(control_door_flag ==0){
            delay_ms(100);
        }else if(control_door_flag ==1)
        {
            LCD_ShowString(0,145,120,12,12,(u8 *)"open door      ");
            opendoor();
        }else if(control_door_flag ==2)
        {
            LCD_ShowString(0,145,120,12,12,(u8 *)"close door      ");
            closedoor();
        }
        
        if(fingerprint_status ==2)
        {
            if(fingerprint_flag == 0)
            {
                Add_FR();//增加指纹
                fingerprint_status = 0;
            }
            else if(fingerprint_flag == 1)
            {
                LCD_ShowString(0,115,120,12,12,"fingerprint is bad!!!");
                fingerprint_status = 0;
            }
        }
        
        
    }
    MANUL:
    triget = 0;
    beep_count = 0;
    SPI2_Init();
    MFRC522_Initializtion();
    LCD_Fill(0,0,130,160,WHITE);
    LCD_ShowString(0,15,12*10,12,12,"1.modif password");
    LCD_ShowString(0,30,12*10,12,12,"2.add fingerprint");
    LCD_ShowString(0,45,12*10,12,12,"3.add ic card    ");
    LCD_ShowString(0,60,12*10,12,12,"4.del fingerprint");
    LCD_ShowString(0,75,12*10,12,12,"5.del ic card    ");
    memset(key_input,0,sizeof(key_input));
    while(1)
    {
        led1 = ~led1;
        if(display_manual == 0)
            goto RUN;
        
        if(manual_cmd == 1)
        {
//            LCD_Fill(0,15,128,30,LGRAY);
            BACK_COLOR = LGRAY;
            LCD_ShowString(0,15,12*10,12,12,"1.modif password");
            BACK_COLOR = WHITE;
            LCD_ShowString(0,30,12*10,12,12,"2.add fingerprint");
            LCD_ShowString(0,45,12*10,12,12,"3.add ic card    ");
            LCD_ShowString(0,60,12*10,12,12,"4.del fingerprint");
            LCD_ShowString(0,75,12*10,12,12,"5.del ic card    ");
            
        }
        else if(manual_cmd == 2)
        {
//            LCD_Fill(0,15,128,30,LGRAY);
            BACK_COLOR = LGRAY;
            LCD_ShowString(0,30,12*10,12,12,"2.add fingerprint");
            BACK_COLOR = WHITE;
            LCD_ShowString(0,15,12*10,12,12,"1.modif password");
            LCD_ShowString(0,45,12*10,12,12,"3.add ic card    ");
            LCD_ShowString(0,60,12*10,12,12,"4.del fingerprint");
            LCD_ShowString(0,75,12*10,12,12,"5.del ic card    ");
        }
        else if(manual_cmd == 3)
        {
            BACK_COLOR = WHITE;
            LCD_ShowString(0,15,12*10,12,12,"1.modif password");
            LCD_ShowString(0,30,12*10,12,12,"2.add fingerprint");
            LCD_ShowString(0,60,12*10,12,12,"4.del fingerprint");
            LCD_ShowString(0,75,12*10,12,12,"5.del ic card    ");
            BACK_COLOR = LGRAY;
            LCD_ShowString(0,45,12*10,12,12,"3.add ic card    ");
            
        }
        else if(manual_cmd == 4)
        {
            BACK_COLOR = WHITE;
            LCD_ShowString(0,15,12*10,12,12,"1.modif password");
            LCD_ShowString(0,30,12*10,12,12,"2.add fingerprint");            
            LCD_ShowString(0,45,12*10,12,12,"3.add ic card    ");
            LCD_ShowString(0,75,12*10,12,12,"5.del ic card    ");
            BACK_COLOR = LGRAY;
            LCD_ShowString(0,60,12*10,12,12,"4.del fingerprint");
        }
        else if(manual_cmd == 5)
        {
            BACK_COLOR = WHITE;
            LCD_ShowString(0,15,12*10,12,12,"1.modif password");
            LCD_ShowString(0,30,12*10,12,12,"2.add fingerprint");            
            LCD_ShowString(0,45,12*10,12,12,"3.add ic card    ");
            LCD_ShowString(0,60,12*10,12,12,"4.del fingerprint");
            BACK_COLOR = LGRAY;
            LCD_ShowString(0,75,12*10,12,12,"5.del ic card    ");
            
        }
        else if(manual_cmd ==11&&triget ==0)
        {
            triget = 1;
            BACK_COLOR = WHITE;
            LCD_Fill(0,0,130,100,WHITE);
            LCD_ShowString(0,15,12*10,12,12,"input old password");
            password_status = 1;
            ic_card_status = 0;
            fingerprint_status = 0;
        }
        else if(manual_cmd ==11)
        {
            if(enter_press == 1)
            {
                enter_press = 0;
                add_password();             //修改密码
            }
            
        }
        else if(manual_cmd == 12&&triget == 0)
        {
            triget = 1;
            BACK_COLOR = WHITE;
            LCD_Fill(0,0,130,100,WHITE);
            LCD_ShowString(0,15,12*10,12,12,"input old password");
            LCD_ShowString(0,30,12*10,12,12,"add fingerprint   ");
            password_status = 0;
            ic_card_status = 0;
            fingerprint_status = 1;
        }
        else if(manual_cmd ==12)
        {
            if(enter_press == 1)
            {
                enter_press = 0;
                
                add_fingerprint();         //增加指纹
            }
        }
        else if(manual_cmd == 13&&triget == 0)
        {
            triget = 1;
            BACK_COLOR = WHITE;
            LCD_Fill(0,0,130,100,WHITE);
            LCD_ShowString(0,15,12*10,12,12,"input old password");
            LCD_ShowString(0,30,12*10,12,12,"add IC icard      ");
            password_status = 0;
            ic_card_status = 1;
            fingerprint_status = 0;
        }
        else if(manual_cmd ==13)
        {
            RC522_search();
            if(enter_press == 1)
            {
                enter_press = 0;
                add_ic();    
            }
        }
        else if(manual_cmd == 14&&triget == 0)
        {
            triget = 1;
            BACK_COLOR = WHITE;
            LCD_Fill(0,0,130,100,WHITE);
            LCD_ShowString(0,15,12*10,12,12,"input old password");
            LCD_ShowString(0,30,12*10,12,12,"del fingerprint     ");
            password_status = 0;
            ic_card_status = 0;
            fingerprint_status = 11;
        }
        else if(manual_cmd ==14)
        {

            if(enter_press == 1)
            {
                enter_press = 0;
                del_fingerprint();
            }
        }
        else if(manual_cmd == 15&&triget == 0)
        {
            triget = 1;
            BACK_COLOR = WHITE;
            LCD_Fill(0,0,130,100,WHITE);
            LCD_ShowString(0,15,12*10,12,12,"input old password");
            LCD_ShowString(0,30,12*10,12,12,"del IC icard      ");
            password_status = 0;
            ic_card_status = 11;
            fingerprint_status = 0;
        }
        else if(manual_cmd ==15)
        {
            RC522_search();
            if(enter_press == 1)
            {
                enter_press = 0;
                del_ic();   
            }
        }
        
        delay_ms(100);
    }
}



