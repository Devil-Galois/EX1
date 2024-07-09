#include "timer.h"
#include "led.h"
#include "operation.h"
#include "key_it.h"
#include "ADF4351.h"
#include "adc.h"

/*********************************************************************************

定时器溢出时间计算方法:Tout=((auto_data+1)*(fractional+1))/Ft(us)  Ft定时器时钟
定时器时钟为84M

****************************************************************************/
/*  定时器2中断配置  */
/*  扫频变量设置  */
extern int 		Option_Scan;
/* 扫频变量结束  */
//定时器2配置
void TIM2_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      //使能TIM2时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	TIM_Cmd(TIM2,ENABLE);                    //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//定时器2中断服务函数
//k=1---freq=80;k=2---freq=80+step;k=200----freq=80+step*199
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		if(Option_Scan!=QUEE)
		{
			switch(Option_Scan)
			{
				case ONCE:
					Scan_Once();
					break;
				case MODU://调幅波
					Scan_Modu();
					break;
				case ONLY:
					Scan_Only();
					break;
			}
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

/*  定时器2结束  */



/*  定时器5中断配置  */
//定时器5配置------优先级高于定时器2-------用于按键检测
//需要的全局变量
int timer5_flag;
void TIM5_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);      //使能TIM2时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM5,ENABLE);                    //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;  //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//定时器5中断处理函数
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //溢出中断
	{
			KEYBOARDPRESS();
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
}
/*  定时器5中断配置结束  */

