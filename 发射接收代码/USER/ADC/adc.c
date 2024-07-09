#include "adc.h"	 

//��ʼ��ADC															   
void  ADC1_Init(void)
{    
  GPIO_InitTypeDef  		GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

  //�ȳ�ʼ��ADC1ͨ��6 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA6  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
// 
//	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ  ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_T2_TRGO;//ʹ�ö�ʱ��2����
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Falling;//�����ش���
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��  21M/480+12=23.2us
//	//ADC�����������5ʱ��=0.23us��ÿ��adc����Ҫ23.5us������adc��ȡ�������24us����Ҫ��ͬһ��ֵ�����ɼ�10��,���Ա�ƵӦ�ô���0.24ms,ѡȡ��Ƶ1ms�㹻
//	//��Ƶ1ms---��ʱ��1/10=0.1ms���,���ǳ�����ʱ,��ʱ�����0.1ms=100us����---��Ҳ��adc��������Ĵ��ֵ
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

 
}			

/****************************************************************************
* ��    ��: u16 Get_Adc(u8 ch) 
* ��    �ܣ����ADCֵ
* ��ڲ�����ch: ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
* ���ز�����12λADC��Чֵ
* ˵    ����       
****************************************************************************/
u16 Get_Adc(void)   
{	
	return ADC_GetConversionValue(ADC1);	 //�������һ��ADC1�������ת�����
}


	 









