#include "led.h"
#include "lcd.h"
#include "key.h"
#include "malloc.h" 
#include "tfcard_sdio.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h" 
#include "ADF4351.h"
#include "PE4302.h"
#include "adc.h"
#include "updatefont.h"  //调用HanZiUse.lib中更新字库函数 鼠标右键选择打开头文件可查看该头文件
#include "showhz.h"	     //调用HanZiUse.lib中显示汉字函数
#include "KeyBoard.h"
#include "math.h"
#include "timer.h"
#include "operation.h"
#include "exti.h"
#include "math.h"


#define _10_LOG2_10    3.0103
#define	LOG2_50000     15.60964
//当前菜单
#define		TOP	 0
#define		SUB0 1
#define		SUB1 2
#define		SUB2 3
#define		SUB3 4
#define   PUTSINGLE  	5
#define 	PUTSCAN			6
#define		RUNSINGLE		7
#define		RUNSCAN			8
#define		SETVALUE		9
#define 	CMD					10
#define		NONE				11

/*  菜单处理全局变量  */
int 											CURRENT_PAGE=TOP;  
int												LAST_PAGE=TOP;
int												CMD_OPTION=NONE;
int												EXCHANGEPAGE;
MENU_TOP 					OPTION_TOP=TOP_WAIT;
MENU_SUB0 				OPTION_SUB0=SUB0_WAIT;
MENU_SUB1 				OPTION_SUB1=SUB1_WAIT;
MENU_SUB2 				OPTION_SUB2=SUB2_WAIT;
MENU_SUB3 				OPTION_SUB3=SUB3_WAIT;
double freq_option=0;
double v_option=0;
int												SCAN_PAR_MODE=0;

/*  菜单处理全局变量结束  */

/*  扫频变量设置  */
int 	 Option_Scan=QUEE;
double freq_step=0.1;
double freq_begin=90.72;
double freq_end=110.72;
double freq;
int flag_twice;
int finish=0;


u32 x_point;
u32 y_point;
/* 扫频变量结束  */


/*  采样变量设置  */
double am_data[201]={0};//存放采样结果的数组
double am_data_high[2001]={0};//存放第一遍扫频结果的数组
double *p_data_high=am_data_high;//传入指针用于存放第一次扫描结果
double *p_data=am_data;//传入指针
int 		k=1;									//扫频下标
int 		count=0;//计数器
/*  采样变量设置结束  */


/* 键盘全局变量 */
double readvalue_keyboard;
/* 键盘全局变量结束 */


/* 硬件全局变量  */
double freqmid=100;
double v_core=100;
/* 硬件全局变量结束 */
extern int k;
int main(void)
{  
	u8 display_str[8];
	int subscript=0;
	int i=0;
	int j;
	char option=0;
	double step;
	//double test=0;
	
	
	/*  初始化部分  */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();		  															//初始化延时函数
	LED_Init();		    															//初始化LED端口
	LCD_Init();																			//初始化LCD
	KEY_Init();																			//初始化板载按键
	ADC1_Init();        														//初始化ADC
	Keyboard_GPIO_Config();													//初始化键盘端口
	ADF4351Init();     															//初始化锁相环
	PE_GPIO_Init();																	//初始化衰减器
	TIM2_Init(3999,83);															//定时器2时钟84M，分频系数84，84M/84=1M 所以计数4000次为4ms---8s扫完
	TIM5_Init(99,8399);															//定时器5时钟84M，分频系数8400，84M/8400=10K 所以计数100次为10ms   即每10ms调用一次按键检测函数
	EXTIX_Init();																		//外部中断初始化
	font_init();																		//初始化汉字字库
	InitRowColValue();															//初始化键盘列表
	ADC_Cmd(ADC1, ENABLE);													//开启ADC
	PE4302_Set(8);																	//value是外部变量用于设置PE4302的衰减倍数
  /*  初始化结束  */
	//ADF4351WriteFreq(90);
	
	/*  UI绘制  */
	DrawStatic();																		//复位界面显示
	/*  UI绘制结束  */
	/*  死循环  */
	while(1){
	/*  键盘读取处理 ---- 菜单模式 */
		if(CURRENT_PAGE!=CMD)
		{
			option=ReadKeyBoard();
	/*  菜单绘制处理  */
			if(LAST_PAGE!=CURRENT_PAGE)
			{
				switch(CURRENT_PAGE){
					case 	TOP:
						DrawStatic();
						break;
					case 	SUB0:
						DrawTop_Sub0();
						break;
					case	SUB1:
						DrawTop_Sub1();
						break;
					case  SUB2:
						DrawTop_Sub2();
						break;
					case  SUB3:
						DrawTop_Sub3();
						break;
				}
				LAST_PAGE=CURRENT_PAGE;
			}
			//按键控制     当有按键按下再执行菜单选择界面    按键按下优先级  A>B>C>D
			else	if(option)
			{
				switch(CURRENT_PAGE){
					case	TOP:
						if(option=='A')																	
							OPTION_TOP=TOP_SUB0;
						else if(option=='B')
							OPTION_TOP=TOP_SUB1;
						else if(option=='C')
							OPTION_TOP=TOP_SUB2;
						else if(option=='D')
							OPTION_TOP=TOP_SUB3;
						Top_Process();
						break;
					case SUB0:
						if(option=='A')
							OPTION_SUB0=SUB0_SUB0;
						else if(option=='B')
							OPTION_SUB0=SUB0_SUB1;
						else if(option=='C')
							OPTION_SUB0=SUB0_SUB2;
						else if(option=='D')
							OPTION_SUB0=SUB0_SUB3;
						Sub0_Process();
						break;	
					case SUB1:
						if(option=='A')
							OPTION_SUB1=SUB1_SUB0;
						else if(option=='B')
							OPTION_SUB1=SUB1_SUB1;
						else if(option=='C')
							OPTION_SUB1=SUB1_SUB2;
						else if(option=='D')
							OPTION_SUB1=SUB1_SUB3;
						Sub1_Process();
						break;	
					case SUB2:					
						if(option=='A')
							OPTION_SUB2=SUB2_SUB0;
						else if(option=='B')
							OPTION_SUB2=SUB2_SUB1;
						else if(option=='C')
							OPTION_SUB2=SUB2_SUB2;
						Sub2_Process();
						break;	
					case SUB3:					
						if(option=='A')
							OPTION_SUB3=SUB3_SUB0;
						else if(option=='B')
							OPTION_SUB3=SUB3_SUB1;
						else if(option=='C')
							OPTION_SUB3=SUB3_SUB2;
						Sub3_Process();
						break;	
				}
				option=0;
			}
		}
		/*  数字输入模式 CURRENT_PAGE==CMD */
		else 
		{
			if(KEYBOARD_16)
			{
				readvalue_keyboard=0;
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);
			}
			//数字输入处理
			if(ENSURE_INPUT)
			{
				switch(CMD_OPTION)
				{
					case PUTSINGLE:
							freqmid=readvalue_keyboard;
							break;
					case SETVALUE:
							v_core=readvalue_keyboard;
							PE4302_Set(-40.71*log10(v_core)+97.19);//设置衰减器倍数
							break;
					case PUTSCAN:
							freq_begin=10.72+readvalue_keyboard;
							SCAN_PAR_MODE=0;
							break;
				}
				CMD_OPTION=NONE;
				EXCHANGEPAGE=CURRENT_PAGE;
				CURRENT_PAGE=LAST_PAGE;
				LAST_PAGE=EXCHANGEPAGE;
				ENSURE_INPUT=0;
			}
		}
		/*  数字输入模式结束  */
	
	/*  菜单绘制处理结束  */
	
		
	/*  扫频处理开始  */
		if(flag_twice==INTERVAL)
		{
			TIM2->ARR=3999;
			subscript=find_max(am_data_high,2001);
			freq=freq_begin=89.73+0.01*subscript;
			freq_end=91.72+0.01*subscript;
			am_data[0]=-93;
			ADF4351Init();
			ADF4351WriteFreq(freq);
			count=0;
			k=1;
			flag_twice=SECOND;
		}
		if(finish)
		{
			Setvalue_option();
			sprintf((char*)display_str,"%.3fMHz",freq_option); //浮点型数据
			LCD_DisplayString(255,155,12,display_str);
			sprintf((char*)display_str,"%.3fdBm",v_option); //浮点型数据
			LCD_DisplayString(255,190,12,display_str);
			step=(freq_end-freq_begin)/5;
			for(j =0;j<6;j++)
			{
				sprintf((char*)display_str,"%.1f",j*step+freq_begin-10.72);
				LCD_DisplayString(10+40*j,227,12,display_str);
			}
		}
			
	/* 扫频处理结束 */
	}
	/*  死循环结束  */
}






