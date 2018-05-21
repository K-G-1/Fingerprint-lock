#ifndef __include_H
#define __include_H	 
#include "sys.h"

#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "malloc.h"



#define FLASH_SAVE_ADDR  0X080E0000 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
#define TEXT_LENTH sizeof(password)	 		  	//数组长度	
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)

extern u16 ValidN;
extern u8 use_machine;
/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:密码状态，0：验证密码 1：:输入新密码 2：修改密码
*@Author		:K.G. 
****************************************************/
extern u8 password_status;
extern u8 fingerprint_status;
extern u8 ic_card_status;
extern u8 fingerprint_flag;
extern u8 key_input[5] ;
extern u8 password[4];
extern u8 ic_card_key[4];
extern u8 empty_key[4];
extern u8 display_manual;
extern u8 enter_press ;
extern u8 control_door_flag;
extern u8 beep_count;
#endif
