#include "timer.h"
#include "led.h"
#include "operation.h"
#include "key_it.h"
#include "ADF4351.h"
#include "adc.h"

/*********************************************************************************

��ʱ�����ʱ����㷽��:Tout=((auto_data+1)*(fractional+1))/Ft(us)  Ft��ʱ��ʱ��
��ʱ��ʱ��Ϊ84M

****************************************************************************/
/*  ��ʱ��2�ж�����  */
/*  ɨƵ��������  */
extern int 		Option_Scan;
/* ɨƵ��������  */
//��ʱ��2����
void TIM2_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      //ʹ��TIM2ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	TIM_Cmd(TIM2,ENABLE);                    //ʹ�ܶ�ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//��ʱ��2�жϷ�����
//k=1---freq=80;k=2---freq=80+step;k=200----freq=80+step*199
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		if(Option_Scan!=QUEE)
		{
			switch(Option_Scan)
			{
				case ONCE:
					Scan_Once();
					break;
				case MODU://������
					Scan_Modu();
					break;
				case ONLY:
					Scan_Only();
					break;
			}
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}

/*  ��ʱ��2����  */



/*  ��ʱ��5�ж�����  */
//��ʱ��5����------���ȼ����ڶ�ʱ��2-------���ڰ������
//��Ҫ��ȫ�ֱ���
int timer5_flag;
void TIM5_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);      //ʹ��TIM2ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
	TIM_Cmd(TIM5,ENABLE);                    //ʹ�ܶ�ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;  //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//��ʱ��5�жϴ�����
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //����ж�
	{
			KEYBOARDPRESS();
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //����жϱ�־λ
}
/*  ��ʱ��5�ж����ý���  */

