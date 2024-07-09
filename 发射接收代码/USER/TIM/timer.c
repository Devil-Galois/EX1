#include "timer.h"
#include "key_it.h"
#ifdef	SEND
#include "led.h"
#endif


#ifdef	SEND
/*	定时器6初始化用于输出正弦波 */
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

	/* 配置DMA1 Stream 5 channel 7用于DAC1 */
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


/* 定时器8初始化输出PWM波 */
void Init_tim8()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);      		//TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 								//GPIOA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//无上下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM8); 
	TIM_DeInit(TIM8);
	TIM_TimeBaseStructure.TIM_Prescaler=16799;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; 					//向下计数
	TIM_TimeBaseStructure.TIM_Period=99;   																//计数周期
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
 
 
	TIM_ClearFlag(TIM8,TIM_FLAG_Update);
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
 
 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;			//使用反向传输通道自带反相传输功能
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  													//根据T指定的参数初始化外设TIM8
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  									//使能TIM8在CCR上的预装载寄存器
	TIM_CCxCmd(TIM8,  TIM_Channel_1,  TIM_CCx_Enable);
	TIM_ARRPreloadConfig(TIM8,ENABLE);  
	TIM_Cmd(TIM8, ENABLE);  																						//使能TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_SetCompare1(TIM8, 70); 	
}

#endif


/* 定时器5用于按键检测*/
void TIM5_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);      					//使能TIM5时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     					//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 		//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);								//初始化TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); 													//允许定时器5更新中断
	TIM_Cmd(TIM5,ENABLE);                   													//使能定时器5
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; 										//定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;				//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;  							//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) 				//溢出中断
	{
#ifdef		SEND
			KEYBOARDPRESS();																//检测薄膜键盘
#endif
		
#ifndef		SEND
			KEY_RECEIVE_PRESS();
#endif
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  				//清除中断标志位
}
/*  定时器5中断配置结束  */

#ifdef		SEND
/* 定时器8中断处理 		*/
void TIM8_UP_TIM13_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) 				//溢出中断
	{			
		if(STOP)TIM8->CCR1=70;														//停止传输模式下传0
		else 
		{
			TIM8->CCR1=p_element[__index++]?30:70;					//传输模式下根据二进制数组值设置传输占空比
			__index=__index%32;															//只有32位   mod12
		}
	}
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  				//清除中断标志位
}
#endif



#ifndef		SEND
/* 定时器3初始化用于捕获输入 */
void Init_tim3(void)
{
        GPIO_InitTypeDef                 GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef          TIM_TimeBaseStructure;
        NVIC_InitTypeDef                 NVIC_InitStructure;
        TIM_ICInitTypeDef                TIM3_ICInitStructure;
        
        
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);					//时钟使能
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);				//使能GPIOB时钟
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                   //GPIOB4
        GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;                 //复用功能
        GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;        		//速度100MHz
        GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;               //推挽复用输出
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOB,&GPIO_InitStructure);                     	//初始化
        
        // 定时器复用引脚
        GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);

        TIM_TimeBaseStructure.TIM_Prescaler = 8399;                 //定时器分频
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
        TIM_TimeBaseStructure.TIM_Period = 199;                    	//自动重装载值
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //分频因子 配置死区时会用到
        
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);


        //初始化TIM3输入捕获参数
        TIM3_ICInitStructure.TIM_Channel  = TIM_Channel_1;           				//通道1
        TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
        TIM3_ICInitStructure.TIM_ICSelection= TIM_ICSelection_DirectTI; 		//映射到TI1上
        TIM3_ICInitStructure.TIM_ICPrescaler= TIM_ICPSC_DIV1;               //配置输入分频,不分频 
        TIM3_ICInitStructure.TIM_ICFilter = 0x05;                                                
        // 初始化 PWM 输入模式
        TIM_PWMIConfig(TIM3, &TIM3_ICInitStructure);
                
        

        // 当工作做 PWM 输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
        // 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
        
        // 选择输入捕获的触发信号
        TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);

        // 选择从模式: 复位模式
        // PWM 输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器 CNT 会被复位
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
	if(IC2Value!=0)												//高电平周期计数值非0
	{
		if(!RECEIVE_Start)									//未检测到连续8个1
		{
			if(index_re==8)										//检测到连续8个1
			{
				RECEIVE_Start=1;								//开始接收有效码
				RECEIVE_Finish=0;								//接收重置为未完成
				index_re=0;
			}
			else if(IC2Value>50)							//接收码元1
			{
				index_re++;
				cnt=0;													//每次收到1都要重置cnt
			}
			else
			{
				index_re=0;											//每次收到0都要重置index_re
				cnt++;
			}
		}
		if(index_re<24&&RECEIVE_Start)
			RECEIVE[index_re++]=IC2Value>50?1:0;		//根据占空比存放二进制码
		else if(index_re==24)
			RECEIVE_Finish=1;
	}
}

#endif


/*  用于输出时钟信号的定时器TIM4  */
#ifdef		SEND
void Tim4_CLK_Init_B7(u16 __PSC,u16 __ARR)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);      		//TIM4时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 								//GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//无上下拉
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


/*  用于输出时钟信号的定时器TIM2  */

void Tim2_CLK_Init_A1(u16 __PSC,u16 __ARR)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      		//TIM2时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 								//GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//无上下拉
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



/*  用于输出时钟信号的定时器TIM4  */
#ifndef		SEND
void Tim4_CLK_Init_B6(u16 __PSC,u16 __ARR)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);      		//TIM4时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 								//GPIOB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//无上下拉
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


/*  用于输出时钟信号的定时器TIM2  */

void Tim2_CLK_Init_A3(u16 __PSC,u16 __ARR)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      		//TIM2时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 								//GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   		 	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 						//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;					//无上下拉
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




