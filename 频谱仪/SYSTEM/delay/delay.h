#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f4xx.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////  
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//使用SysTick的普通计数模式对延迟进行管理(支持ucosii)
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	
#define SYSCLK 168    //系统时钟
void delay_init(void);
//void delay_init(u8 a);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);


#endif





























