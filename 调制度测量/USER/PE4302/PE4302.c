#include "PE4302.h"
/**************************************************************** 
��������: PE_GPIO_Init 
��    ��: �ܽų�ʼ�� 
��    ��: ��
����ֵ  : �� 
*****************************************************************/ 

void PE_GPIO_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_PE4302_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_PE4302_DAT, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_PE4302_LE, ENABLE);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
	
	GPIO_InitStruct.GPIO_Pin=PE4302_CLK_Pin;
	GPIO_Init(PE4302_CLK_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=PE4302_DAT_Pin;
	GPIO_Init(PE4302_DAT_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_InitStruct.GPIO_Pin=PE4302_LE_Pin;
	GPIO_Init(PE4302_LE_GPIO, &GPIO_InitStruct);
	
	PE4302_LE_DIS;
	PE4302_CLK_0;
	PE4302_DAT_0;
	
	
}
/**************************************************************** 
��������: PE4302_0_Set 
��    ��: ˥��ֵ���� 
��    ��: db����Χ��0~63����Ӧ˥��0~31.5dB
����ֵ  : �� 
*****************************************************************/ 
void PE4302_Set(unsigned char db)
{
  unsigned int i;
  unsigned int W_DB;
	W_DB = db;
	PE4302_LE_DIS;
	PE4302_CLK_0;
	for(i = 0; i < 6;i++)
	{
		if (0x20 == (W_DB & 0X20))
		{	
		  PE4302_DAT_1;
		}
		else
		{
			PE4302_DAT_0;
		}
		  delay_us(500);
		  PE4302_CLK_1;
			delay_us(500);
			PE4302_CLK_0;
			delay_us(500);
		  W_DB = W_DB << 1;
     		
  }
	   PE4302_LE_EN;
     delay_us(500);
	   PE4302_LE_DIS;
     PE4302_DAT_0;
}