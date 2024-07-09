#include "timer.h"
#include "key_it.h"
#ifdef	SEND
#include "led.h"
#endif


#ifdef	SEND
/*	��ʱ��6��ʼ������������Ҳ� */
void Init_tim6()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	DMA_InitTypeDef DMA_InitStructure;

	TIM_Cmd(TIM6, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 56;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;	
	
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* ����DMA1 Stream 5 channel 7����DAC1 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_7;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1; 
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)p;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;		
	DMA_InitStructure.DMA_BufferSize = 128;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream5, ENABLE);
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
}


/* ��ʱ��8��ʼ�����PWM�� */
void Init_tim8()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);      		//TIM8ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 								//GPIOA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//���츴�����
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM8); 
	TIM_DeInit(TIM8);
	TIM_TimeBaseStructure.TIM_Prescaler=16799;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; 					//���¼���
	TIM_TimeBaseStructure.TIM_Period=99;   																//��������
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
 
 
	TIM_ClearFlag(TIM8,TIM_FLAG_Update);
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
 
 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;			//ʹ�÷�����ͨ���Դ����ഫ�书��
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  													//����Tָ���Ĳ�����ʼ������TIM8
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  									//ʹ��TIM8��CCR�ϵ�Ԥװ�ؼĴ���
	TIM_CCxCmd(TIM8,  TIM_Channel_1,  TIM_CCx_Enable);
	TIM_ARRPreloadConfig(TIM8,ENABLE);  
	TIM_Cmd(TIM8, ENABLE);  																						//ʹ��TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_SetCompare1(TIM8, 70); 	
}

#endif


/* ��ʱ��5���ڰ������*/
void TIM5_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);      					//ʹ��TIM5ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     					//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      					//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 		//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);								//��ʼ��TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); 													//����ʱ��5�����ж�
	TIM_Cmd(TIM5,ENABLE);                   													//ʹ�ܶ�ʱ��5
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; 										//��ʱ��5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;				//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;  							//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) 				//����ж�
	{
#ifdef		SEND
			KEYBOARDPRESS();																//��ⱡĤ����
#endif
		
#ifndef		SEND
			KEY_RECEIVE_PRESS();
#endif
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  				//����жϱ�־λ
}
/*  ��ʱ��5�ж����ý���  */

#ifdef		SEND
/* ��ʱ��8�жϴ��� 		*/
void TIM8_UP_TIM13_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) 				//����ж�
	{			
		if(STOP)TIM8->CCR1=70;														//ֹͣ����ģʽ�´�0
		else 
		{
			TIM8->CCR1=p_element[__index++]?30:70;					//����ģʽ�¸��ݶ���������ֵ���ô���ռ�ձ�
			__index=__index%32;															//ֻ��32λ   mod12
		}
	}
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  				//����жϱ�־λ
}
#endif



#ifndef		SEND
/* ��ʱ��3��ʼ�����ڲ������� */
void Init_tim3(void)
{
        GPIO_InitTypeDef                 GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef          TIM_TimeBaseStructure;
        NVIC_InitTypeDef                 NVIC_InitStructure;
        TIM_ICInitTypeDef                TIM3_ICInitStructure;
        
        
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);					//ʱ��ʹ��
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);				//ʹ��GPIOBʱ��
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                   //GPIOB4
        GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;                 //���ù���
        GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;        		//�ٶ�100MHz
        GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;               //���츴�����
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOB,&GPIO_InitStructure);                     	//��ʼ��
        
        // ��ʱ����������
        GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);

        TIM_TimeBaseStructure.TIM_Prescaler = 8399;                 //��ʱ����Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
        TIM_TimeBaseStructure.TIM_Period = 199;                    	//�Զ���װ��ֵ
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //��Ƶ���� ��������ʱ���õ�
        
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);


        //��ʼ��TIM3���벶�����
        TIM3_ICInitStructure.TIM_Channel  = TIM_Channel_1;           				//ͨ��1
        TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
        TIM3_ICInitStructure.TIM_ICSelection= TIM_ICSelection_DirectTI; 		//ӳ�䵽TI1��
        TIM3_ICInitStructure.TIM_ICPrescaler= TIM_ICPSC_DIV1;               //���������Ƶ,����Ƶ 
        TIM3_ICInitStructure.TIM_ICFilter = 0x05;                                                
        // ��ʼ�� PWM ����ģʽ
        TIM_PWMIConfig(TIM3, &TIM3_ICInitStructure);
                
        

        // �������� PWM ����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
        // ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������
        
        // ѡ�����벶��Ĵ����ź�
        TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);

        // ѡ���ģʽ: ��λģʽ
        // PWM ����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������ CNT �ᱻ��λ
        TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
        TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
        
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x01;                
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                      
        NVIC_Init(&NVIC_InitStructure);      
        
        TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);                                      
        
        TIM_Cmd(TIM3,ENABLE );       
}


 void TIM3_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
	IC2Value = TIM_GetCapture2(TIM3);
	if(IC2Value!=0)												//�ߵ�ƽ���ڼ���ֵ��0
	{
		if(!RECEIVE_Start)									//δ��⵽����8��1
		{
			if(index_re==8)										//��⵽����8��1
			{
				RECEIVE_Start=1;								//��ʼ������Ч��
				RECEIVE_Finish=0;								//��������Ϊδ���
				index_re=0;
			}
			else if(IC2Value>50)							//������Ԫ1
			{
				index_re++;
				cnt=0;													//ÿ���յ�1��Ҫ����cnt
			}
			else
			{
				index_re=0;											//ÿ���յ�0��Ҫ����index_re
				cnt++;
			}
		}
		if(index_re<24&&RECEIVE_Start)
			RECEIVE[index_re++]=IC2Value>50?1:0;		//����ռ�ձȴ�Ŷ�������
		else if(index_re==24)
			RECEIVE_Finish=1;
	}
}

#endif


/*  �������ʱ���źŵĶ�ʱ��TIM4  */
#ifdef		SEND
void Tim4_CLK_Init_B7(u16 __PSC,u16 __ARR)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);      		//TIM4ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 								//GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//���츴�����
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	TIM_TimeBaseInitStructure.TIM_Prescaler=__PSC-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=__ARR-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=__ARR/2;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_Cmd(TIM4,ENABLE);	
}


/*  �������ʱ���źŵĶ�ʱ��TIM2  */

void Tim2_CLK_Init_A1(u16 __PSC,u16 __ARR)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      		//TIM2ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 								//GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//���츴�����
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
	TIM_TimeBaseInitStructure.TIM_Prescaler=__PSC-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=__ARR-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=__ARR/2;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_Cmd(TIM2,ENABLE);	
}
#endif



/*  �������ʱ���źŵĶ�ʱ��TIM4  */
#ifndef		SEND
void Tim4_CLK_Init_B6(u16 __PSC,u16 __ARR)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);      		//TIM4ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 								//GPIOB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//���츴�����
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
	TIM_TimeBaseInitStructure.TIM_Prescaler=__PSC-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=__ARR-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=__ARR/2;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_Cmd(TIM4,ENABLE);	
}


/*  �������ʱ���źŵĶ�ʱ��TIM2  */

void Tim2_CLK_Init_A3(u16 __PSC,u16 __ARR)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      		//TIM2ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 								//GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//���츴�����
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2);
	TIM_TimeBaseInitStructure.TIM_Prescaler=__PSC-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=__ARR-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=__ARR/2;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_Cmd(TIM2,ENABLE);	
}
#endif




