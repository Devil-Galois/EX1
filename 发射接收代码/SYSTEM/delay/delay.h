#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f4xx.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////  
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��ucosii)
//����delay_us,delay_ms
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	
#define SYSCLK 168    //ϵͳʱ��
void delay_init(void);
//void delay_init(u8 a);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);


#endif





























