#include "exti.h"
#include "key_it.h"
#include "KeyBoard.h"
#include "led.h"
#include "operation.h"
#include "ADF4351.h"
//外部变量用于读取键盘值传给主函数
extern double readvalue_keyboard;
//外部变量设置扫频步长
extern double freq_step;
//外部变量决定扫频是否完成
extern int Start_Scan;
extern int finish;
/*  外部中断初始化    PF6为中断输入接口  */

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
		switch(CMD_OPTION)
		{
			case PUTSINGLE://单频模式下输入为频率
				readvalue_keyboard=input_double(60,160);
				break;
			case PUTSCAN://扫频模式下为选项输入模式
				if(!SCAN_PAR_MODE)
				{
					switch(ReadKeyBoard())
					{
						case 'A':
						freq_step=0.01;
						SCAN_PAR_MODE=1;//设置为数字输入模式
						Scan_Input_Draw();
						break;
						case 'C':
						Exit_CURRENT();
						freq_step=0.1;
						break;
					}
				}
				else 
					readvalue_keyboard=input_double(60,160);
					break;
			case SETVALUE://参数设置模式下输入值为本振幅度
				readvalue_keyboard=input_double(60,160);
				break;
			case RUNSINGLE:
				switch(ReadKeyBoard())
				{
					case 'A'://单频输出下按下A表示暂停
					//ADF4351_Disable();
					break;
					case 'B'://单频模式下按下B表示停止输出并退出上一级
					//ADF4351_Disable();
					Exit_CURRENT();
					//freqmid=100;
					break;
					case 'C'://单频模式下按下C表示继续输出
					//ADF4351_Enable();
					ADF4351WriteFreq(freqmid);
					break;
				}
				break;
			case RUNSCAN:
				if(Option_Scan==QUEE)
					switch(ReadKeyBoard())
					{
						case 'A':
							finish=0;
							Exit_CURRENT();
							break;
						case 'B':
							if(finish)
								SetCursor(x_point,y_point);//定位光标，应该修改传参
							break;
					}
		}	
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