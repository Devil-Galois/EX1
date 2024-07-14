#ifndef __EXTI_H
#define __EXTI_H
#include "sys.h"
#include "delay.h"
/*  外部中断初始化	 */
void EXTIX_Init(void);
/*  中断接口初始化  使用接口为PF0*/
void Input_exit_Init(void);
/*  触发源输入接口  使用PG0  */
void Output_exti_Init(void);

#endif