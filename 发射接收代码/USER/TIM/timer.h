#ifndef __TIMER_H
#define __TIMER_H
#include "main.h"
#include "sys.h"

#ifdef		SEND
extern u16 * p;
extern int STOP;
extern int __index;
extern u8 * p_element;
#endif



#ifndef		SEND
extern int cnt;
extern int IC2Value;
extern int index_re;
extern u8 RECEIVE[];
extern int RECEIVE_Finish;
extern int RECEIVE_Start;
#endif


#ifdef		SEND
void Init_tim6();
void Init_tim8();
void TIM5_Init(u16 auto_data,u16 fractional);
#endif


#ifndef		SEND
void Init_tim3();
#endif

#ifdef		SEND
void Tim4_CLK_Init_B7(u16 __PSC,u16 __ARR);
void Tim2_CLK_Init_A1(u16 __PSC,u16 __ARR);
#endif

#ifndef		SEND
void Tim4_CLK_Init_B6(u16 __PSC,u16 __ARR);			
void Tim2_CLK_Init_A3(u16 __PSC,u16 __ARR);
#endif

#endif