#ifndef __include_H
#define __include_H	 
#include "sys.h"

#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "malloc.h"


extern u16 ValidN;
extern u8 use_machine;
/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:ÃÜÂë×´Ì¬£¬0£ºÑéÖ¤ÃÜÂë 1£º:ÊäÈëĞÂÃÜÂë 2£ºĞŞ¸ÄÃÜÂë
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
