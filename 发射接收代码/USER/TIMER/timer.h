#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "delay.h"
	
extern int timer2_flag;
extern int timer5_flag;
extern int count;
void TIM2_Init(u16 auto_data,u16 fractional);
void TIM5_Init(u16 auto_data,u16 fractional);
	
#endif
