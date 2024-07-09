#ifndef __OPERATION_H
#define __OPERATION_H
#include "sys.h"
#include "delay.h"
#include "lcd.h"

#define 	P1		0.085477134784586
#define		Q1		3.055651049714763e+02
#define		P2		0.067483362814151
#define		Q2		2.477575334201479e+02
#define		P3		0.041313768488978
#define		Q3		1.681844891251666e+02
#define		P4    0.040610961108169
#define   Q4    1.652307865605224e+02



#define		PN1		0.093799091566123
#define		QN1		3.372110390094615e+02
#define		PN2		0.039680559198663
#define		QN2		1.680027921174069e+02

#define   S1		0.092190843076246
#define   T1		310.5141794547449
#define		S2		0.099143864368484
#define		T2		331.5004776273928
#define		S3		0.044018480319645
#define		T3		173.5676909616054
#define   S4		0.040670733547216
#define		T4		163.5924464842182



#define		SN1		0.102194962553205
#define		TN1		3.642265166117326e+02
#define		SN2		0.121208537811151
#define		TN2		4.240614911606450e+02
#define		SN3		0.036632325977877
#define		TN3		1.627345504998497e+02
#define		SN4		0.037860835688787
#define		TN4		1.659435046305352e+02


#define MORE(a,b)    ((a)>(b)?(a):(b))
#define MAX(a,b,c)   (MORE((a),(b))>(c)?MORE((a),(b)):(c))

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
#define		SETTIM      14
#define   PUTMODU			12
#define		RUNMODU			13
#define 	CMD					10
#define		NONE				11

#define   NEWCMD			16
//扫频选项设置
#define 	QUEE					0
#define   ONCE					1
#define		MODU					2
#define   ONLY					3

/* 菜单界面全局变量  */
extern int 								CURRENT_PAGE;  
extern int								LAST_PAGE;
extern int 								CMD_OPTION;
extern int								EXCHANGEPAGE;



extern double 									freq_option;
extern double 									v_option;
extern double										freq_center;
extern double 										freq_high;
extern double										freq_low;
extern double										v_center;
extern double										v_high;
extern double										v_low;
extern double										figure_am;

extern int								SCAN_PAR_MODE;  //0：选择分辨率模式  1：选择起始频率模式
/* 菜单界面全局变量结束  */


/* 扫频变量 */
extern int Option_Scan;
extern double freq_step;
extern double freq_begin;
extern double freq_end;
extern double freq;
extern double freq_modu_begin;
extern double freq_modu_end;
extern double freq_modu_mid;

extern int peroid;
extern int test;
extern int disp;


extern 	int k;
extern  int count;//计数器
extern 	double 	*p_data;//存放扫频结果


extern double 	temp;//暂存数据
extern double 	x;
extern int finish_modu;
extern int finish_once;

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
	SUB2_SUB3=3,
	SUB2_WAIT=4
}MENU_SUB2;

//SUB3的第三级菜单-----参数设置
typedef enum MENU_SUB3{
	SUB3_SUB0=0,
	SUB3_SUB1=1,
	SUB3_SUB2=2,
	SUB3_SUB3=3,
	SUB3_WAIT=4
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
void Setvalue_option_modu(void);
void Setvalue_option_high(void);

//6.寻找最大值的函数
int find_max(double *a,int n);
//7.只扫描一次的函数
void Scan_Once(void);
void Start_OnlyOutput(void);
//8.横坐标转换1
u16 Convert_X(u16 k);
//9.纵坐标转换
u16 Convert_Y(double amp);
//10.调频模式下扫频函数
void Scan_Modu(void);
void DrawStatic(void);
void DrawTop_Sub0(void);
void DrawTop_Sub1(void);
void DrawTop_Sub2(void);
void DrawTop_Sub3(void);
//扫频模式数字输入
void Scan_Input_Draw(void);
void Scan_Only(void);


//绘制坐标轴的函数
void DrawAxis(void);
void DrawAxis_Modu(void);
//绘制频谱线条函数
void DrawFreq(u16 freq,u16 amp);
void Draw_Line(void);
void Draw_Line_high(void);
//把采集到的32位数据转换成对应格子的函数
double Convert(double value);
double Convert_high(double data);
double FurConvert(double x);
//时域里预处理
void PreFilter(void);
double Convert_figure(double x);
double Convert_edge(double x);
double Convert_figure_2(double x);
/*  调用函数结束  */
#endif