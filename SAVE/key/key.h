#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

void key_init(void);
char keypress(void);
char key_reg(void);
void ChangeMode(u8 key_value );
void KeyOpen(u8 key_value);
int my_strcmp(char const *p,char const *q)  ;
void add_password(void);
#endif

