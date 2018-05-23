#include "RFID_API.h"
#include "led.h"

u8 CardID[4];   //存放M卡类别
u8 study_num[4]={15,0x08,21,14};   //学号
u8 Card_Type[2];   //卡的类型
u8 key_func =3;			//3,无操作  ；0，运行控制功能；   1  运行读卡信息    ；2添加用户卡
u8  DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 					//RFID的读取数据的密码

u8 door_key[6] = {6,6,4,7,4,8};																			//自定义的密码  用于开门
u8 door_key1[6] = "123456";

unsigned char RC522_Wbuff[16];
unsigned char RC522_Rbuff[16];

u8 RC522_search(void)
{
    static u8 status;
    
    status = PcdRequest(0x52,Card_Type);
    
    if(status == MI_OK)
    {
        LCD_ShowString(0,115,120,12,12,"have card");
    }
    else 
    {
        LCD_ShowString(0,115,120,12,12,"no   card");
    }
    return status;
}
u8 RC522_Read_Mess(void)
{
	static u8 status;
	static char *Str_temp;
    char *key_temp;
    status = PcdRequest(0x52,Card_Type);			
    


    if(status == MI_OK)
    {
        
        status = PcdAnticoll(CardID);/*防冲撞*/
        status = PcdSelect(CardID);//选卡
        if(status == MI_OK)
        {
            if(status == MI_OK)
            {
                GPIO_SetBits(GPIOE,GPIO_Pin_5);
            }
            else 
            {
                GPIO_ResetBits(GPIOE,GPIO_Pin_5);
            }
            status = PcdAuthState(PICC_AUTHENT1A,2,DefaultKey,CardID);
            memset(RC522_Rbuff,0,16);   //清空数组
            status = PcdRead(2,RC522_Rbuff);
            Str_temp = mymalloc(SRAMIN,16);
            sprintf(Str_temp,"%d%d%d%d",RC522_Rbuff[0],RC522_Rbuff[1],RC522_Rbuff[2],RC522_Rbuff[3]);  //显示卡号
//            LCD_ShowString(0,115,120,12,12,Str_temp);
            key_temp = mymalloc(SRAMIN,16);
            sprintf(key_temp,"%d%d%d%d",ic_card_key[0],ic_card_key[1],ic_card_key[2],ic_card_key[3]);
            if(strcmp(key_temp,Str_temp)==0)
            {
                LCD_ShowString(60,115,120,12,12,"ok   ");
                status = MI_OK;
            }
            else
            {
                LCD_ShowString(60,115,120,12,12,"faild");
                status = KEY_ERROR;
            }
            myfree(SRAMIN,Str_temp);//释放内存
            Str_temp=0;			//指向空地址
            myfree(SRAMIN,key_temp);//释放内存
            key_temp=0;			//指向空地址
        }
    }
    else{
        GPIO_ResetBits(GPIOE,GPIO_Pin_5);
        LCD_Fill(0,115,120,135,WHITE);
    }
	
    return status;
}

u8 RC522_Add_User(u8 num)
{
	char status;
	u8 name[18] = "顾毅  ";
//	u8 Card_Type[4];
//	u8 Last_Key=0;

//	u8* tips;

	status = write_DoorKey(door_key,&name[0]);	
		
	if(status == MI_OK)
	{


//		Show_Str(100,400,300,24,"用户添加成功",24,1);	
//		Show_Str(130,370,90,24,&name[0],24,1);   //只显示前3个字

	}
	else 
	{
//		Show_Str(100,400,300,24,"用户添加失败",24,1);	
	}
	delay_ms(1000);
//	Show_Str(100,400,300,24,"               ",24,0);
//	Show_Str(130,370,90,24,"           ",24,0);   //只显示前3个字

    return status;
}

u8  write_DoorKey(u8 *key,u8* name)  //写入”看门"密码
{
	char status;
	u8 i;
	u8 Card_Type[4];
	status = PcdRequest(0x52,Card_Type);			
	status = PcdAnticoll(CardID);/*防冲撞*/
	status = PcdSelect(CardID);//选卡
	status = PcdAuthState(PICC_AUTHENT1A,2,DefaultKey,CardID);
	memset(RC522_Wbuff,0,16);
	if(status == MI_OK)
	{
		status = PcdWrite(2,key);

	}
	status = PcdAuthState(PICC_AUTHENT1A,1,DefaultKey,CardID);
	for(i=0;i<6;i++)          //6字节 姓名，0-5
	{
		RC522_Wbuff[i] = name[i];
	}							
	for(i = 0 ;i < 4;i++)   //4字节卡号  6 - 9
	{
		RC522_Wbuff[6+i] = study_num[i];
	}
	status = PcdWrite(1,RC522_Wbuff);
	return status;
}


u8 RC522_Add_ic_card(void)
{
    char status;

	u8 Card_Type[4];
	status = PcdRequest(0x52,Card_Type);			
	status = PcdAnticoll(CardID);/*防冲撞*/
	status = PcdSelect(CardID);//选卡
	status = PcdAuthState(PICC_AUTHENT1A,2,DefaultKey,CardID);
	memset(RC522_Wbuff,0,16);
	if(status == MI_OK)
	{
        delay_ms(10);
		status = PcdWrite(2,ic_card_key);
        if(status == 0)
        {
            LCD_ShowString(0,145,120,12,12,"write success");
        }else
            LCD_ShowString(0,145,120,12,12,"write faild  ");
	}
    else 
        LCD_ShowString(0,145,120,12,12,"no card  ");
    delay_ms(500);
//	status = PcdAuthState(PICC_AUTHENT1A,1,DefaultKey,CardID);
//	for(i=0;i<6;i++)          //6字节 姓名，0-5
//	{
//		RC522_Wbuff[i] = name[i];
//	}							
//	for(i = 0 ;i < 4;i++)   //4字节卡号  6 - 9
//	{
//		RC522_Wbuff[6+i] = study_num[i];
//	}
//	status = PcdWrite(1,RC522_Wbuff);
	return status;
}
/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:选卡之后向快2写入0000
*@Author		:K.G. 
****************************************************/
u8 RC522_del_ic_card(void)
{
    char status;
	u8 i=0;
	u8 Card_Type[4];
	status = PcdRequest(0x52,Card_Type);			
	status = PcdAnticoll(CardID);/*防冲撞*/
	status = PcdSelect(CardID);//选卡
	status = PcdAuthState(PICC_AUTHENT1A,2,DefaultKey,CardID);
	memset(RC522_Wbuff,0,16);
	if(status == MI_OK)
	{
        delay_ms(10);
        while(PcdWrite(2,RC522_Wbuff) !=0 && i <10)
        {
            i++;
        }
        if(i>+10)
        {
            LCD_ShowString(0,145,120,12,12,"write faild  ");
           return  MI_ERR;
        }
        if(status == 0)
        {
            LCD_ShowString(0,145,120,12,12,"write success");
        }else
            LCD_ShowString(0,145,120,12,12,"write faild  ");
	}
    else 
        LCD_ShowString(0,145,120,12,12,"no card  ");
    delay_ms(500);

	return status;
}
/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:增加ic 卡
*@Author		:K.G. 
****************************************************/
void add_ic(void)
{
    u8 status =1;
    if(ic_card_status ==1 )
    {
        
        if(strcmp((const char *)password, (const char *)key_input) == 0)
        {
            status = RC522_Add_ic_card();   
            if(status == 0)
            {
                ic_card_status = 2;
                LCD_ShowString(0,75,120,12,12,(u8 *)"             ");
            }                
            else 
                LCD_ShowString(0,75,120,12,12,(u8 *)"please replay");  
            LCD_ShowString(0,60,120,12,12,(u8 *)"please enter angain");
        }
        else 
        {
            LCD_ShowString(0,60,120,12,12,(u8 *)"password error   ");                       
        }
    }
    else if(ic_card_status ==2 )
    {
        if(RC522_Read_Mess() == 0)
        {
            LCD_ShowString(0,60,120,12,12,(u8 *)"add ic card success ");
            ic_card_status = 0;
        }
        else
        {
            LCD_ShowString(0,60,120,12,12,(u8 *)"add ic card faild");
        }
    }
    memset(key_input,0,sizeof(key_input));
}
/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:删除ic卡
*@Author		:K.G. 
****************************************************/

void del_ic(void)
{
    u8 status = 0;
    if(ic_card_status ==11 )
    {
        if(strcmp((const char *)password, (const char *)key_input) == 0)
        {
            status = RC522_del_ic_card();  
            
            if(status == 0)
            {
                ic_card_status = 12;
                LCD_ShowString(0,75,120,12,12,(u8 *)"             ");
            }                
            else 
                LCD_ShowString(0,75,120,12,12,(u8 *)"please replay"); 
            
            LCD_ShowString(0,60,120,12,12,(u8 *)"please enter angain");
        }
        else 
        {
            LCD_ShowString(0,60,120,12,12,(u8 *)"password error   ");                       
        }
    }
    else if(ic_card_status ==12 )
    {
        if(RC522_Read_Mess() != 0)
        {
            LCD_ShowString(0,60,120,12,12,(u8 *)"del ic card success ");
            ic_card_status = 0;
        }
        else
        {
            LCD_ShowString(0,60,120,12,12,(u8 *)"del ic card faild");
        }
    }
    memset(key_input,0,sizeof(key_input));
}

