#include "oled.h"
#include "codetable.h"

#include "IIC.h"



/*********************OLED写数据************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
    IIC_ADD_write(0x78,0x40,IIC_Data);

}
/*********************OLED写命令************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
    IIC_ADD_write(0x78,0x00,IIC_Command);
//    
//	I2c_Soft_Start();
//	I2c_Soft_SendByte(0x78);            //Slave address,SA0=0
//	I2c_Soft_WaitAsk();	
//	I2c_Soft_SendByte(0x00);			//write command
//	I2c_Soft_WaitAsk();	
//	I2c_Soft_SendByte(IIC_Command);
//	I2c_Soft_WaitAsk();	
//	I2c_Soft_Stop();
}
/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);			//取左四位，并把左四位的开始一位置一
	OLED_WrCmd((x&0x0f)|0x01);				//取右四位，并把右四位的最后一位置一
} 
/*********************OLED全屏************************************/
void OLED_Fill(unsigned char bmp_dat) 		 //全屏显示同一个数
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}
/*********************OLED复位************************************/
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}
/*********************OLED初始化************************************/
void OLED_Init(void)
{
	IIC_Init();
	delay_ms(100);//初始化之前的延时很重要！
	
	OLED_WrCmd(0xAE); //display off
    OLED_WrCmd(0x20); //Set Memory Addressing Mode    
    OLED_WrCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    OLED_WrCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
    OLED_WrCmd(0xc8); //Set COM Output Scan Direction
    OLED_WrCmd(0x00); //---set low column address
    OLED_WrCmd(0x10); //---set high column address
    OLED_WrCmd(0x40); //--set start line address
    OLED_WrCmd(0x81); //--set contrast control register
    OLED_WrCmd(0xff); //亮度调节 0x00~0xff
    OLED_WrCmd(0xa1); //--set segment re-map 0 to 127
    OLED_WrCmd(0xa6); //--set normal display
    OLED_WrCmd(0xa8); //--set multiplex ratio(1 to 64)
    OLED_WrCmd(0x3F); //
    OLED_WrCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    OLED_WrCmd(0xd3); //-set display offset
    OLED_WrCmd(0x00); //-not offset
    OLED_WrCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    OLED_WrCmd(0xf0); //--set divide ratio
    OLED_WrCmd(0xd9); //--set pre-charge period
    OLED_WrCmd(0x22); //
    OLED_WrCmd(0xda); //--set com pins hardware configuration
    OLED_WrCmd(0x12);
    OLED_WrCmd(0xdb); //--set vcomh
    OLED_WrCmd(0x20); //0x20,0.77xVcc
    OLED_WrCmd(0x8d); //--set DC-DC enable
    OLED_WrCmd(0x14); //
    OLED_WrCmd(0xaf); //--turn on oled panel
	OLED_Fill(0x00); //初始清屏
	OLED_Set_Pos(0,0);
} 

/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}
void OLED_P6x8data(unsigned char x,unsigned char y,int ch)
{
	unsigned char i=0;
	uint16_t bai,shi,ge;
	if(ch>=00)
	{
		
		OLED_P6x8Str(x,y," ");
	}
	else
	{
		ch = -ch;
		OLED_P6x8Str(x,y,"-");
	}
	bai = ch/100%10 +16;
	shi= ch/10%10   +16;
	ge = ch%10      +16;
	
	if(x>126){x=0;y++;}
	OLED_Set_Pos(x+6,y);

	for(i=0;i<6;i++)
	OLED_WrDat(F6x8[bai][i]);
	OLED_Set_Pos(x+12,y);
	for(i=0;i<6;i++)
		OLED_WrDat(F6x8[shi][i]);
	OLED_Set_Pos(x+18,y);
	for(i=0;i<6;i++)
		OLED_WrDat(F6x8[ge][i]);
	

}
/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])				//一共8页i，即是8行，每两行像是一个数字或一个汉字
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32; //将字符通过数字的加减转化成自己定义数组里的位置数,从而显示出来.0的asicii为48,48-32=16，数组中0正好是12位
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);		     	//分行打印所以分两次每次8个数
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}


void Draw_line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char *data)
{
//	unsigned int j=0;
	unsigned char x;
	
	
	OLED_Set_Pos(x0,y0);
	for(x=x0;x<x1;x++)
	{      
		OLED_WrDat(*data);
		data++;
	}
	
}


/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[100][100])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[x][j++]);
	    }
	}
}

/**********************************************************/
void oled_dis_str(void)
{
	OLED_P6x8Str(0,0,"Pitch:");
	OLED_P6x8Str(0,1,"Roll :");
	OLED_P6x8Str(0,2,"Yaw  :");
	OLED_P6x8Str(0,3,"Hight:");
}

void oled_dis_data(int P,int R,int Y,u16 H)
{
	OLED_P6x8data(36,2,P);
	OLED_P6x8data(36,3,R);
	OLED_P6x8data(36,4,Y);
	OLED_P6x8data(36,5,H);
	
}
