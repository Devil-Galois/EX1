#ifndef __EXTI_H
#define __EXTI_H
#include "sys.h"
#include "delay.h"
/*  �ⲿ�жϳ�ʼ��	 */
void EXTIX_Init(void);
/*  �жϽӿڳ�ʼ��  ʹ�ýӿ�ΪPF0*/
void Input_exit_Init(void);
/*  ����Դ����ӿ�  ʹ��PG0  */
void Output_exti_Init(void);

#endif