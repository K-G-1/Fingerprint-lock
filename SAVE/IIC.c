#include "include.h"
#include "IIC.h"


/********************************IIC基础函数*****************************************************/
//初始化IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	IIC_SCL=1;
	IIC_SDA=1;
}
//产生IIC起始信号
u16 IIC_Start(void)
{
	SDA_OUT();//sda线输出
	SDA_H;     	  
	IIC_SCL=1;
	__NOP();
 	if(!SDA_READ) return 1;	//SDA线为低电平则总线忙,退出
	SDA_L;
	__NOP();
	return 0;
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	IIC_SCL=1; 
	delay_us(1);
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SCL=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
	delay_us(2);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	delay_us(2);
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}	

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if(txd&0x80)
				{
					SDA_H;
				}
				else SDA_L;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(void)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
				IIC_SCL=1;
        receive<<=1;
        if(SDA_READ)
					receive++;   
		delay_us(1); 
    }					 

    return receive;
}

/*********************************额外IIC函数****************************************************/


/************************************************************   
* 函数名:IIC_ADD_write   
* 描述 : 向特定设备id的特定地址，写入字节 
* 输入  :设备id，内部地址，数据    
* 输出  :无    
*/
void IIC_ADD_write(u8 DeviceAddr,u8 address,u8 Bytes)
{
	IIC_Start();
	IIC_Send_Byte(DeviceAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(address);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(Bytes);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_us(10);	
}

/************************************************************   
* 函数名:I2C_ReadByte   
* 描述 : 从特定设备id的特定地址读取内容
* 输入  :设备id，内部地址   
* 输出  :读取的内容   
*/
u8 IIC_ADD_read(u8 DeviceAddr,u8 address)
{
	  unsigned char temp;
   IIC_Start();
   IIC_Send_Byte(DeviceAddr);
   IIC_Wait_Ack();
 
    IIC_Send_Byte(address);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr+1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte();		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

/************************************************************   
* 函数名:IIC_Read_MultiBytes   
* 描述 : 从特定设备id的特定地址读取多个字节
* 输入  :设备id，内部地址，需要读的字节个数（0不读取）
* 输出  :读取的内容   
*************************************************************/
u8 IIC_Read_MultiBytes(u8 DeviceAddr,u8 address,u8 Len)
{
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=IIC_ADD_read(DeviceAddr,address+Len-t-1); 	 				   
	}
	return temp;
}


/************************************************************   
* 函数名:IIC_NoAddr_WriteByte   
* 描述 : 向特定设备id，写入字节 
* 输入  :设备id，内容   
* 输出  :无    
*/
void IIC_NoAddr_WriteByte(unsigned char address,unsigned char Bytes)
{

	IIC_Start();
	IIC_Send_Byte(address);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(Bytes);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);

}


/***************************************************************/











