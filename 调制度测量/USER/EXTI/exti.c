#include "exti.h"
#include "key_it.h"
#include "KeyBoard.h"
#include "led.h"
#include "operation.h"

/*  外部中断初始化    PF0为中断输入接口  */

void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	Input_exit_Init();
	Output_exti_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource0);//PF0---- 连接到中断线0
	
	// 配置外部中断线 EXTI_Line0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE; //中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //外部中断0  刚好 PF0在 EXTI0_IRQn 里面
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置   
}


/*  外部中断初始化结束  */

/*  中断处理程序  */
void EXTI0_IRQHandler(void)
{
		EXTI_ClearITPendingBit(EXTI_Line0);
		GPIO_SetBits(GPIOG,GPIO_Pin_0);
}
/*  中断处理程序结束  */


/*  中断接口初始化  使用接口为PF0*/
void Input_exit_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);    //使能GPIOF时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;             //普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/*  中断接口初始化结束  */

/*  触发源输入接口  使用PG0  */
void Output_exti_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

  //PG0初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                  //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;             //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                   //上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);                         //初始化GPIO
	GPIO_SetBits(GPIOG,GPIO_Pin_0);
}
/*  触发源输入接口结束  */