#ifndef _PE4302_H_
#define _PE4302_H_

#include "sys.h"
#include "delay.h"



#define RCC_PE4302_DAT	  	RCC_AHB1Periph_GPIOC
#define RCC_PE4302_CLK		RCC_AHB1Periph_GPIOC
#define RCC_PE4302_LE		RCC_AHB1Periph_GPIOB



#define PE4302_DAT_GPIO	  	GPIOC
#define PE4302_CLK_GPIO	  	GPIOC
#define PE4302_LE_GPIO		GPIOB


#define PE4302_DAT_Pin	  	GPIO_Pin_13
#define PE4302_CLK_Pin		GPIO_Pin_0
#define PE4302_LE_Pin		GPIO_Pin_13


#define PE4302_DAT         		PCout(13)
#define PE4302_CLK         		PCout(0)
#define PE4302_LE        		PBout(13)


#define PE4302_LE_EN      	PE4302_LE = 1
#define PE4302_LE_DIS     	PE4302_LE = 0

#define PE4302_CLK_0        PE4302_CLK = 0
#define PE4302_CLK_1        PE4302_CLK = 1

#define PE4302_DAT_0        PE4302_DAT = 0
#define PE4302_DAT_1        PE4302_DAT = 1


void PE_GPIO_Init(void);
void PE4302_Set(unsigned char db);

#endif
