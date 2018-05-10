#ifndef __RFID_API_H
#define __RFID_API_H	

#include "include.h"
#include "RC522.h"
#include "spi.h"
#include "lcd.h"

extern u8 CardID[4],Card_Type[2], DefaultKey[6];
extern unsigned char RC522_Wbuff[16],RC522_Rbuff[16];






u8 RC522_Read_Mess(void);
u8 RC522_Add_User(u8 num);
u8  write_DoorKey(u8 *key,u8* name) ;
u8 RC522_Add_ic_card(void);
void add_ic(void);
void del_ic(void);

u8 RC522_search(void);











#endif



