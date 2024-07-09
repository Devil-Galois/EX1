#ifndef __TM1637_H
#define __TM1637_H
#include "main.h"
#include "sys.h"
#include "delay.h"
#define TM1637_PORT			 GPIOB

#ifdef		SEND
#define TM1637_SCLK_PIN  GPIO_Pin_13
#define TM1637_DIN_PIN   GPIO_Pin_12		
#endif



#ifndef		SEND
#define TM1637_SCLK_PIN  GPIO_Pin_10
#define TM1637_DIN_PIN   GPIO_Pin_11	
#endif
extern char segmentMap[];
void TM1637_Init(void);
void TM1637SetBrightness(char ch,char brightness);
void TM1637Start(void);
void TM1637Stop(void);
void TM1637WriteByte(unsigned char b);
void TM1637ReadResult(void);
void TM1637_Display_Byte(unsigned char addr, unsigned char segments);
void TM1637ShowOne(unsigned char index, unsigned char digit);
void TM1637_Display4(u16 data);
void TM1637_Clear(void);
#endif
