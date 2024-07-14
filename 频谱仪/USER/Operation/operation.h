#ifndef __OPERATION_H
#define __OPERATION_H
#include "sys.h"
#include "delay.h"
#include "lcd.h"

#define _10_LOG2_10    3.0103
#define	LOG2_50000     15.60964

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



//扫频选项设置
#define 	QUEE					0
#define   ONCE					1
#define		TWICE					2

#define		FIRST         1
#define		INTERVAL			3
#define		SECOND				2


/* 菜单界面全局变量  */
extern int 								CURRENT_PAGE;  
extern int								LAST_PAGE;
extern int 								CMD_OPTION;
extern int								EXCHANGEPAGE;
extern double freq_option;
extern double v_option;
extern int								SCAN_PAR_MODE;  //0：选择分辨率模式  1：选择起始频率模式
/* 菜单界面全局变量结束  */


/* 扫频变量 */
extern int Option_Scan;
extern double freq_step;
extern double freq_begin;
extern double freq_end;
extern double freq;
extern int k;
extern  int count;//计数器
extern 	double 	*p_data;//存放扫频结果
extern double *p_data_high;//传入指针
extern double 	temp;//暂存数据
extern double 	x;
extern int flag_twice;
extern int finish;

extern u32 x_point;
extern u32 y_point;
/* 扫频变量 */




/* 硬件全局变量  */
extern double freqmid;
extern double v_core;
/* 硬件全局变量结束 */





/*  菜单需要的枚举型变量  */


//第一级菜单
typedef enum MENU_TOP{
	TOP_SUB0=0,
  TOP_SUB1=1,
	TOP_SUB2=2,
	TOP_SUB3=3,
	TOP_WAIT=4
}MENU_TOP;


//SUB0的第二级菜单-----单频模式
typedef enum MENU_SUB0{
	SUB0_SUB0=0,
	SUB0_SUB1=1,
	SUB0_SUB2=2,
	SUB0_SUB3=3,
	SUB0_WAIT=4
}MENU_SUB0;

//SUB1的第二级菜单-----扫频模式
typedef enum MENU_SUB1{
	SUB1_SUB0=0,
	SUB1_SUB1=1,
	SUB1_SUB2=2,
	SUB1_SUB3=3,
	SUB1_WAIT=4
}MENU_SUB1;

//SUB2的第二级菜单-----调幅模式
typedef enum MENU_SUB2{
	SUB2_SUB0=0,
	SUB2_SUB1=1,
	SUB2_SUB2=2,
	SUB2_WAIT=3
}MENU_SUB2;

//SUB3的第三级菜单-----参数设置
typedef enum MENU_SUB3{
	SUB3_SUB0=0,
	SUB3_SUB1=1,
	SUB3_SUB2=2,
	SUB3_WAIT=3
}MENU_SUB3;

/*  菜单需要的枚举型变量定义结束  */

/*  菜单选择变量  */
extern MENU_TOP 					OPTION_TOP;
extern MENU_SUB0 					OPTION_SUB0;
extern MENU_SUB1 					OPTION_SUB1;
extern MENU_SUB2 					OPTION_SUB2;
extern MENU_SUB3 					OPTION_SUB3;
/*  菜单选择变量结束  */




/*  调用函数开始  */
//顶层模块选择函数
void Top_Process(void);
//SUB模块选择函数
void Sub0_Process(void);
//SUB1模块选择函数
void Sub1_Process(void);
//SUB2模块选择函数
void Sub2_Process(void);
//SUB3模块选择函数
void Sub3_Process(void);

//SUB_SUB模块选择函数
//1.------扫频模式下启动输出
void StartScanOutput(void);
//4.------调幅模式下输出启动
void StartModuOutput(void);
//2.------已经输出的模式下退出
void Exit_CURRENT(void);
//3.------扫频模式下定位光标
void SetCursor(u32 x,u32 y);
//5.-----扫频模式下显示频率和dBm值
void Setvalue_option(void);
//6.寻找最大值的函数
int find_max(double *a,int n);
//7.只扫描一次的函数
void Scan_Once(void);
//8.横坐标转换1
u16 Convert_X(u16 k);
//9.纵坐标转换
u16 Convert_Y(double amp);
//10.扫描两次的函数
void Scan_TWICE_1(void);
void Scan_TWICE_2(void);


//衰减器PE4302衰减设置
void KeyProcess_PE4302(double data);
//ADF4351步进频率设置
void SetStepFreq(double freq_begin,double freq_step,double freq_end);
//切换键盘模式
void SwitchMode(void);
//设置屏幕显示的中心频率
void SetcenterFreq(double freq_center);
//绘制静态UI的所有函数
void DrawStatic(void);
void DrawTop_Sub0(void);
void DrawTop_Sub1(void);
void DrawTop_Sub2(void);
void DrawTop_Sub3(void);
//扫频模式数字输入
void Scan_Input_Draw(void);



//扫频函数
void Freq_Scan(void);




//绘制坐标轴的函数
void DrawAxis(void);
//绘制频谱线条函数
void DrawFreq(u16 freq,u16 amp);
//把采集到的32位数据转换成对应格子的函数
double Convert(double value);

/*  调用函数结束  */
#endif