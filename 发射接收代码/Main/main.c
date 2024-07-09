#include "main.h"
#include "timer.h"
#include "function.h"

#ifdef		SEND
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "sinx.h"												//正弦波产生
/*	字符显示相关  */
#include "malloc.h" 
#include "tfcard_sdio.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h" 
#include "updatefont.h"  								//调用HanZiUse.lib中更新字库函数 鼠标右键选择打开头文件可查看该头文件
#include "showhz.h"	     								//调用HanZiUse.lib中显示汉字函数
																

#include "KeyBoard.h"										//键盘输入相关
#include "math.h"
#include "exti.h"												//外部中断用于进入输入数字的中断
#endif



#ifndef		SEND
#include "tm1637.h"											//数码管
#include "si5351.h"											//方波振荡器
#include "key_it.h"
#include "ADF4351.h"
#endif




#ifdef		SEND
/* 菜单界面宏 */
#define TOP 1
#define SUB1 2
#define SUB2 3
#endif

#ifndef		SEND
#define FREQ_1M	1000000									//用于SI5351方波频率设置   1*FREQ_1M=1M方波输出
#endif





#ifdef		SEND
u16  wave_sin[]={												//正弦波数组
	1237,1286,1335,1383,1430,1476,1521,1564,1607,1648,1687,1725,1761,1795,1828,
	1858,1886,1913,1937,1959,1978,1995,2010,2023,2033,2040,2045,2048,2048,2045,
	2041,2033,2023,2011,1996,1979,1959,1938,1914,1887,1859,1829,1796,1762,1726,
	1688,1649,1608,1566,1522,1477,1431,1385,1337,1288,1239,1189,1139,1088,1038,
	987,937,886,836,787,738,689,642,595,550,506,463,421,381,342,305,270,237,205,
	176,149,123,100,80,61,45,32,20,12,5,2,0,1,5,11,20,31,45,61,79,100,123,148,175,
	204,236,269,304,341,379,420,461,504,548,594,640,688,736,785,835,885,935,985,
	1036,1087,1137,1187,1237
};
u8 NUMBER[4]={0,0,0,0};								//存放键盘输入四位十进制数组
u8 ELEMENT[32]={0};										//32位二进制数码数组

u16 * p=wave_sin;											//指向正弦数组的指针
u8  * p_number=NUMBER;								//指向待传存放四位二进制数组的指针
u8  * p_element=ELEMENT;							//指向待传存放32位二进制数码的指针
double board_number;									//保存键盘读取值
int CURRENTPAGE=TOP;									//保存菜单界面标识
int CHANGE=0;													//表示界面是否切换  切换则刷新界面
int STOP=0;														//传输是否停止			停止则传0
int __index=0;												//待传32位二进制数组下标
#endif


char segmentMap[] = {									//数码管映射0-F
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 
    0x00
};
#ifndef		SEND
int IC2Value;													//接收端读取高电平持续期间内的计数值
int index_re;													//接收端存放二进制数数组下标
u8 RECEIVE[24]={0};										//用于存放接收端除8个1以外的二进制数码
int RECEIVE_Finish=0;									//接收完24个有效数码后置1表示接收完成
int RECEIVE_Start=0;									//接收到8个1后表示开始接收
int cnt=0;														//计算接收到的0的个数  接收到500个0就熄灭数码管
double freq_current=35.276;
double freq_before=35.7;
#endif



int main(void)
{  
#ifndef		SEND
	int __Receive[5];										//把接收到的前20个数码4个一组解码为十进制存放在数组里
	double temp_freq;
#endif
	
#ifdef		SEND
	int option=0;												//保存菜单选项AorB
#endif
	
	
	
	
	/*  初始化部分  */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();		  															//初始化延时函数
	
	
	
#ifdef		SEND
	LED_Init();		    															//初始化LED端口
	LCD_Init();																			//初始化LCD
	KEY_Init();																			//初始化板载按键
	Keyboard_GPIO_Config();													//初始化键盘端口
	TIM5_Init(99,8399);															//定时器5时钟84M，分频系数8400，84M/8400=10K 所以计数100次为10ms   即每10ms调用一次按键检测函数
	EXTIX_Init();																		//外部中断初始化
	font_init();																		//初始化汉字字库
	InitRowColValue();															//初始化键盘列表
	Wave_Gen();																			//生成正弦波
	Init_tim8();																		//初始化定时器8生成PWM波
#endif


#ifndef		SEND
	I2C2_Init();
	TM1637_Init();																	//初始化数码管
	Init_tim3();																		//初始化定时器3用于捕获边沿
	ADF4351Init();
	Init_KEY_RECEIVE();
#endif
  /*  初始化结束  */
	
	
	
	/* 写入频率 */
#ifndef		SEND
	si5351aSetFrequency(34*FREQ_1M , 0);						//CLK0--34M
	si5351aSetFrequency(35*FREQ_1M , 1);						//CLK1--35M
	si5351aSetFrequency(36*FREQ_1M , 2);						//CLK0--36M
#endif

#ifdef		SEND
	/*  UI绘制  */
	DrawMenu();  																		//菜单绘制
	/*  UI绘制结束  */
#endif

/*	椭圆滤波器时钟源  */
#ifdef		SEND
	Tim4_CLK_Init_B7((u16)8400,(u16)100);								//输出频率CLK=84M/(PSC*ARR)    函数封装：Tim4_CLK_Init((u16)PSC,(u16)ARR);---PB7
	Tim2_CLK_Init_A1((u16)8400,(u16)100);								//PA1
#endif
#ifndef		SEND
	Tim4_CLK_Init_B6((u16)8400,(u16)100);			
	Tim2_CLK_Init_A3((u16)8400,(u16)100);
	ADF4351WriteFreq(freq_current);
#endif
	/*  死循环  */
	while(1){
		
		
#ifdef		SEND
		/*界面更新函数*/
		if(CHANGE)																		//检测到界面更改就刷新
		{
			switch(CURRENTPAGE){
				case TOP:	DrawMenu();break;
				case SUB1:DrawInput();break;
				case SUB2:DrawOutput();break;
				}
			CHANGE=0;
		}
		/* 顶层按键检测 */
		if(KEYBOARD_16&&CURRENTPAGE==TOP)							
		{
			option=ReadKeyBoard();
			if(option=='A')
			{
				CURRENTPAGE=SUB1;
				CHANGE=1;
			}
			else if(option=='B')												//输出启动后把STOP置0   此时根据Coding出来的32位数码控制PWM
			{
				CURRENTPAGE=SUB2;
				CHANGE=1;
				STOP=0;
				Coding();
			}
			else CURRENTPAGE=TOP;
		}
		/* 数码输入模式 */
		if(CURRENTPAGE==SUB1)
		{
			if(KEYBOARD_16)
			{
				board_number=0;
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);					//进入外部中断处理函数输入数字
			}
			if(ENSURE_INPUT)														//数字输入完成
			{
				Digital_Convert((int)board_number);
				CURRENTPAGE=TOP;
				CHANGE=1;
				ENSURE_INPUT=0;
			}
		}
		/* 输出启动模式 */
		if(KEYBOARD_16&&CURRENTPAGE==SUB2)
		{
			option=ReadKeyBoard();											//在输出进行中时按下A   STOP置位1    此时输出全0
			if(option=='A')
			{
				CURRENTPAGE=TOP;
				CHANGE=1;
				STOP=1;
			}
			else CURRENTPAGE=SUB2;
		}
#endif

		
		
#ifndef		SEND
		/* 解码 */
		if(RECEIVE_Finish)																				//接收到全部24位有效位后
		{
			__Receive[0]=Binary2Decimal(RECEIVE);										//二进制转十进制
			__Receive[1]=Binary2Decimal(RECEIVE+4);
			__Receive[2]=Binary2Decimal(RECEIVE+8);
			__Receive[3]=Binary2Decimal(RECEIVE+12);
			__Receive[4]=Binary2Decimal(RECEIVE+16);
			if(Counter(RECEIVE)==__Receive[4]&&Check(RECEIVE+20))		//校验成功进行显示数码管
			{
				TM1637ShowOne(0,__Receive[0]);
				TM1637ShowOne(1,__Receive[1]);
				TM1637ShowOne(2,__Receive[2]);
				TM1637ShowOne(3,__Receive[3]);
			}
			else 																										//校验失败START重置为0   重新采集
			{
				RECEIVE_Start=0;
				index_re=0;
			}
		}

		if(cnt>500)																								//连续检测到500个0表示此时已经停止传输5s    数码管熄灭
		{
			TM1637_Clear();
			cnt=0;
		}
		if(KEY_RECEIVE)
		{
			temp_freq=freq_before;
			freq_before=freq_current;
			freq_current=temp_freq;
			ADF4351WriteFreq(freq_current);
		}
#endif

	}
	/*  死循环结束  */
}
