#ifndef _oled_H_
#define	_oled_H_



#include "stm32f4xx.h"
//#include "include.h"


#define X_WIDTH 128


void OLED_Init(void);
void OLED_Fill(unsigned char bmp_dat);

void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])	;
void OLED_P6x8data(unsigned char x,unsigned char y,int ch);
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);


void oled_dis_data(int P,int R,int Y,u16 H);
void oled_dis_str(void);
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[100][100]);
 void Draw_line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char *data);





#endif


