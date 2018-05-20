#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"


#define BEEP PBout(2)

void beep_init(void);

void warn(u8 status);
#endif
