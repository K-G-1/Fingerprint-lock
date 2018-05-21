#include "key.h"
#include "delay.h"
#include "include.h"
#include "lcd.h"
#include "RFID_API.h"
#include "stmflash.h"
/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:矩阵键盘的初始化，注意必须要底四位上拉推挽输出
                    高四位下拉推挽输入
*@Author		:K.G. 
****************************************************/
void key_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;
    GPIO_Init(GPIOD,&GPIO_InitStruct);
    
    
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
    GPIO_Init(GPIOD,&GPIO_InitStruct);
    
    GPIO_SetBits( GPIOD,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);  
    GPIO_ResetBits( GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); 
}
/***************************************************
*@version		:V1.0
*@CreatDate		:
*@Description	:测试矩阵键盘的hex值
*@Author		:K.G. 
****************************************************/
char key_reg(void)
{
    int reg_value = 0;
    int key_value = 0;
    reg_value = GPIO_ReadInputData(GPIOE)>>8;
    delay_ms(10); 
    
    GPIO_SetBits(GPIOE,GPIO_Pin_8);   
    GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11); 
    reg_value = GPIO_ReadInputData(GPIOE)>>8;
    key_value = reg_value&0xFF;
    
    GPIO_SetBits(GPIOE,GPIO_Pin_9);   
    GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11); 
    reg_value = GPIO_ReadInputData(GPIOE)>>8;
    key_value = reg_value&0xFF;
    
    GPIO_SetBits(GPIOE,GPIO_Pin_10);   
    GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_11); 
    reg_value = GPIO_ReadInputData(GPIOE)>>8;
    key_value = reg_value&0xFF;
    
    GPIO_SetBits(GPIOE,GPIO_Pin_11);   
    GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_8); 
    reg_value = GPIO_ReadInputData(GPIOE)>>8;
    key_value = reg_value&0xFF;
    
    GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 |GPIO_Pin_11);   
    GPIO_ResetBits(GPIOE, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15);

    return key_value;  
}
/***************************************************
*@version		:V1.0
*@CreatDate		:
*@Description	:矩阵键盘，默认支持连按，
*@Author		:K.G. 
****************************************************/
char keypress(void)
{
    static u8 KeyHold = 0;
    int KeyValue=17;  
    
    if((GPIO_ReadInputData(GPIOD)>>8&0xFF)!=0x0F)   
    {   
        delay_ms(10);  
        if((GPIO_ReadInputData(GPIOD)>>8&0xFF)!=0x0F&&KeyHold ==0)   
        {   
            
            GPIO_SetBits(GPIOD,GPIO_Pin_8);   
            GPIO_ResetBits(GPIOD,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);  
      
            switch(GPIO_ReadInputData(GPIOD)>>8&0xFF)  
            {   
                case 0x11:KeyValue=12;break;   

                case 0x21:KeyValue=11;break;   

                case 0x41:KeyValue=0;break;   

                case 0x81:KeyValue=10;break;   
            }   
            
            GPIO_SetBits(GPIOD,GPIO_Pin_9);  
            GPIO_ResetBits(GPIOD,GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11);  

            switch(GPIO_ReadInputData(GPIOD)>>8&0xff)  
            {   
             case 0x12:KeyValue=13;break;  

             case 0x22:KeyValue=3;break;   

             case 0x42:KeyValue=2;break;  

             case 0x82:KeyValue=1;break; 

            }  
            
            GPIO_SetBits(GPIOD,GPIO_Pin_10);   
            GPIO_ResetBits(GPIOD,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11);  

            switch(GPIO_ReadInputData(GPIOD)>>8&0xff)  
            {   
                case 0x14:KeyValue=14;break;  

                case 0x24:KeyValue=6;break;  

                case 0x44:KeyValue=5;break;  

                case 0x84:KeyValue=4;break;  
            }   
            
            GPIO_SetBits(GPIOD,GPIO_Pin_11);   
            GPIO_ResetBits(GPIOD,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);  
                            
            switch(GPIO_ReadInputData(GPIOD)>>8&0xff)   
            {  
                case 0x18:KeyValue=15;break;  

                case 0x28:KeyValue=9;break;   

                case 0x48:KeyValue=8;break;  

                case 0x88:KeyValue=7;break;   
            }   
            GPIO_SetBits(GPIOD,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 |GPIO_Pin_11);   
            GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15);  
            if((GPIO_ReadInputData(GPIOD)>>8&0xff)==0x0F&&KeyHold==1)    //按下就会中断程序
            {
                KeyValue = 17;
            }
               KeyHold = 1;
        }    
    }  
    else 
        KeyHold = 0;

    
    return KeyValue;
}

void ChangeMode(u8 key_value)
{
//    u8 key_value=0;
//    key_value = keypress();
    if(key_value == 15)                //切换模式，待修改
    {

        use_machine ++;
        beep_count =0;
        if(use_machine>2)
            use_machine = 0;
        LCD_Fill(0,100,120,160,WHITE); //初始清屏
    }
    
}

/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:按键开门，加上模式1下的按键切换功能
*@Author		:K.G. 
****************************************************/
void KeyOpen(u8 key_value)
{

    static u8 num = 0;
    u8 i = 0;
//    LCD_ShowxNum(0,130,key_value,2,12,0);
    if(key_value!= 17)
    {
        if(key_value<10&&num<4)                 //输入密码
        {
            key_input[num] = key_value+0x30;
            num++;
        }
        else if(key_value == 10)                //删除密码                
        {
            num--;
            key_input[num]=' ';           
        }
        else if(key_value == 11)
        {   
            if(password_status == 0&&fingerprint_status==0&&ic_card_status==0)
            {
                if(my_strcmp((const char *)password, (const char *)key_input) == 0)
                {
                    LCD_ShowString(0,145,120,12,12,(u8 *)"open door      ");
                    control_door_flag = 1;
                }
                else
                {
                    beep_count++;
                    LCD_ShowString(0,145,120,12,12,(u8 *)"password error ");
                }
                num = 0;
                memset(key_input,0,sizeof(key_input));
            }
            else if(password_status == 1)
            {
                if(my_strcmp((const char *)password, (const char *)key_input) == 0)
                {
                    LCD_ShowString(0,145,120,12,12,(u8 *)"modif password   ");
                    LCD_ShowString(0,115,120,12,12,(u8 *)"input new password   ");
                    num=0;
                    memset(key_input,' ',4*sizeof(u8));
                    password_status = 2;
                }
                else 
                {
                    LCD_ShowString(0,145,120,12,12,(u8 *)"password error   ");
                    num=0;
                    memset(key_input,' ',4*sizeof(u8));
                }
            }
            else if(password_status == 2)
            {
                strcpy((char *)password, (char *)key_input);
                LCD_ShowString(0,115,120,12,12,(u8 *)"modif   success!!!   ");
                
                password_status = 0;
                num=0;
                memset(key_input,' ',4*sizeof(u8));
            }
            else if(fingerprint_status ==1)
            {
                if(my_strcmp((const char *)password, (const char *)key_input) == 0)
                {
                    LCD_ShowString(0,115,120,12,12,(u8 *)"add finger           ");
                    LCD_ShowString(0,145,120,12,12,(u8 *)"please press         ");
                    num=0;
                    memset(key_input,' ',4*sizeof(u8));
                    fingerprint_status = 2;
                }
                else 
                {
                    LCD_ShowString(0,145,120,12,12,(u8 *)"password error   ");
                    num=0;
                    memset(key_input,' ',4*sizeof(u8));
                }
            }
            else if(ic_card_status ==1 )
            {
                if(my_strcmp((const char *)password, (const char *)key_input) == 0)
                {
                    LCD_ShowString(0,145,120,12,12,(u8 *)"add ic card     ");
                    RC522_Add_ic_card();
                    num=0;
                    memset(key_input,' ',4*sizeof(u8));
                    ic_card_status = 2;
                    LCD_ShowString(0,145,120,12,12,(u8 *)"please enter angain");
                }
                else 
                {
                    LCD_ShowString(0,145,120,12,12,(u8 *)"password error   ");
                    num=0;
                    memset(key_input,' ',4*sizeof(u8));
                }
            }
            else if(ic_card_status ==2 )
            {
                if(RC522_Read_Mess() == 0)
                {
                    LCD_ShowString(0,145,120,12,12,(u8 *)"add ic card success ");
                }
                else
                {
//                    ic_card_status = 1;
                    LCD_ShowString(0,145,120,12,12,(u8 *)"add ic card faild");
                }
            }
        }
        else if(key_value == 15)                //切换模式，待修改
        {
            use_machine ++;
            if(use_machine >2)
            {
                use_machine = 0;
            }
            LCD_Fill(0,100,120,160,WHITE); //初始清屏
        }
        else if(key_value == 14)                //切换模式，待修改
        {
            ic_card_status = 1;
            password_status = 0;
            fingerprint_status = 0;
            num=0;
            LCD_ShowString(0,115,120,12,12,(u8 *)"add ic card,input  ");
        }
        else if(key_value == 12)                //修改密码
        {
            display_manual = 1;
//            ic_card_status = 0;
//            password_status = 1;
//            fingerprint_status = 0;
//            num=0;
//            LCD_ShowString(0,115,120,12,12,(u8 *)"modif password,input");
        }
        else if(key_value == 13)                //修改密码
        {
            display_manual = 0;
//            ic_card_status = 0;
//            password_status = 0;
//            fingerprint_status = 1;
//            num=0;
//            LCD_ShowString(0,115,120,12,12,(u8 *)"add fingerprint,input");
        }
        else if(num >=4)
        {
            ;
        }
    }
    
    LCD_ShowString(60,120,128,12,12,"      ");
    for(i= 0; i<num; i++)
    {
        LCD_ShowString(60+i*8,120,8,12,12,"*");
    }
//    LCD_ShowString(60,130,24,12,12,key_input);
}

int my_strcmp(char const *p,char const *q)  
{  
//    assert(p);  
//    assert(q);  
    while(*p==*q)  
    {  
        if(*p=='\0')  
            return 0;  
        p++;  
        q++;  
    }  
    if(*p>*q)  
        return 1;  
    if(*p<*q)  
        return -1;  
    return -1;
}  

/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:
*@Author		:K.G. 
****************************************************/
void add_password(void)
{

    if(password_status == 1)
    {
        if(my_strcmp((const char *)password, (const char *)key_input) == 0)
        {
            LCD_ShowString(0,15,12*10,12,12,"input new password");
            LCD_ShowString(0,30,12*10,12,12,"                  ");
            LCD_Fill(0,130,130,160,WHITE);
            password_status = 2;
        }
        else 
        {
            LCD_ShowString(0,30,12*10,12,12,"password error     ");
        }
    }
    else if(password_status ==2 )
    {

        LCD_ShowString(0,15,12*10,12,12,"password modif ok ");
        LCD_ShowString(0,30,12*10,12,12,"                  ");
        strcpy((char *)password, (char *)key_input);
        STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)password,SIZE);  //写入flash 保存密码
        password_status = 0;

    }
    memset(key_input,0,sizeof(key_input));

}
