#include "dac.h"

/*	DAC初始化函数 */
void Init_dac()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);		

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel_1, ENABLE);
}