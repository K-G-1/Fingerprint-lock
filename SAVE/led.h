#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define led0 PEout(5)
#define led1 PEout(6)


void led_init(void);

#endif


