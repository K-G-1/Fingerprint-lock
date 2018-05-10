#ifndef __rc_H
#define __rc_H	 
#include "sys.h"

typedef struct int16_rcget{
				int16_t ROLL;
				int16_t PITCH;
				int16_t THROTTLE;
				int16_t YAW;
	      int16_t pitch_offset;
	      int16_t roll_offset;
	      int16_t yaw_offset;}T_RC_DATA;

				
extern T_RC_DATA Rc_Data;//1000~2000


void PWM_IN_Init(void);
void RC_Data_Refine(void);
void Deblocking(void);
				
#endif




