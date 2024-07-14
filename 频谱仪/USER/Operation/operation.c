#include "sys.h"
#include "operation.h"
#include "ADF4351.h"
#include "PE4302.h"
#include "lcd.h"
#include "showhz.h"	     //调用HanZiUse.lib中显示汉字函数
#include "led.h"
#include "math.h"
#include "adc.h"
#include "timer.h"

//绘制静态UI的所有函数
//1.绘制不变的界面元素
void DrawStatic(void)
{
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
	LCD_DisplayHZstr(120,60,24,"M E N U");
	LCD_DisplayHZstr(60,100,12,"A:单频模式");
	LCD_DisplayHZstr(190,100,12,"B:扫频模式");
	LCD_DisplayHZstr(60,140,12,"C:调幅模式");
	LCD_DisplayHZstr(190,140,12,"D:参数设置");
}

//2.绘制单频的菜单界面
void DrawTop_Sub0(void)
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
	LCD_DisplayHZstr(90,60,24,"单 频 模 式");
	LCD_DisplayHZstr(60,100,12,"A:频率设置");
	LCD_DisplayHZstr(190,100,12,"B:输出启动");
	LCD_DisplayHZstr(60,140,12,"C:菜单回退");
	LCD_DisplayHZstr(190,140,12,"D:主菜单");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%.3fMHz",freqmid);
	LCD_DisplayHZstr(60,220,12,"当 前 输 出 频 率");
	LCD_DisplayString(180,219,12,display_str);
	BRUSH_COLOR=WHITE;
}

//3.绘制扫频模式菜单界面
void DrawTop_Sub1(void)
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
	LCD_DisplayHZstr(90,60,24,"扫 频 模 式");
	LCD_DisplayHZstr(60,100,12,"A:扫频设置");
	LCD_DisplayHZstr(190,100,12,"B:扫频启动");
	LCD_DisplayHZstr(60,140,12,"C:菜单回退");
	LCD_DisplayHZstr(190,140,12,"D:主菜单");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%.2f-%.2fMHz",freq_begin-10.72,freq_end-10.72);
	LCD_DisplayString(150,199,12,display_str);
	LCD_DisplayHZstr(60,200,12,"当 前 扫 频");
	sprintf((char*)display_str,"%dkHz",(int)(freq_step*1000));
	LCD_DisplayString(150,219,12,display_str);
	LCD_DisplayHZstr(60,220,12,"分 辨 率");
	BRUSH_COLOR=WHITE;
}

//4.绘制调幅模式菜单界面
void DrawTop_Sub2(void)
{
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
	LCD_DisplayHZstr(90,60,24,"调 幅 模 式");
	LCD_DisplayHZstr(60,100,12,"A:扫频启动");
	LCD_DisplayHZstr(190,100,12,"B:菜单回退");
	LCD_DisplayHZstr(60,140,12,"C:主菜单");
}

//5.绘制参数设置菜单界面
void DrawTop_Sub3(void)
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
	LCD_DisplayHZstr(90,60,24,"参 数 设 置");
	LCD_DisplayHZstr(60,100,12,"A:本振幅度设置");
	LCD_DisplayHZstr(190,100,12,"B:菜单回退");
	LCD_DisplayHZstr(60,140,12,"C:主菜单");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%.3fmVpp",v_core);
	LCD_DisplayHZstr(60,220,12,"当 前 本 振 幅 度");
	LCD_DisplayString(180,219,12,display_str);
	BRUSH_COLOR=WHITE;
}


//绘制频谱的函数
//把采集到的32位数据转换成对应格子的函数
//dbm=10log10(Vrms^2/50*10^-3)
//10mVrms--- -26.99dBm
//10uVrms---- -86.99dBm
//5uVrms=----- -93.01dBm
//差值：66---需要3*66=198格子
//因此每dBm增3格子---  -26.99dBm=-27dBm---
//<-93dBm---<5uVrms---238,237,236
//此后每增加1dBm----格子数增加三格----因此最终dBm数值是   （待测量海拔-3）/3-93  同理可以由dBm算出最后海拔
double Convert(double value)
{
	if(value<2.06)
	return _10_LOG2_10*(2*(-0.3409*pow(value,2)+6.3953*value-12.1263)-LOG2_50000);
	else
	return _10_LOG2_10*(2*(-50.0855*pow(value,3)+342.8234*pow(value,2)-771.3388*value+571.6222)-LOG2_50000);
}
//绘制海拔范围238
//绘制x范围2--202  共200格
void DrawFreq(u16 freq,u16 amp)
{
	u16 i;
	u16 end=226-amp;
	//LCD_DrawLine(freq1,end1,freq2,end2,YELLOW);
	for(i=226;i>end;i--)
	LCD_Color_DrawPoint(freq,end,YELLOW);
}
//绘制坐标轴的函数
void DrawAxis(void)
{
	u8 display_str[8];
	u16 j,i;
	double step;
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(80,0,16,"频  谱  分  析  仪");
	LCD_DrawLine(26,226,319,226,BLUE);//x轴
	LCD_DrawLine(1,19,319,19,BLUE);//顶栏边界
	LCD_DrawLine(24,19,24,226,BLUE);//y轴
	LCD_DrawLine(226,19,226,226,BLUE);//侧边栏左边界
	LCD_DrawLine(319,19,319,226,BLUE);//侧边栏右边界
	for(j=1;j<5;j++)
	{
		for(i=0;i<20;i++)
   {
		 LCD_DrawLine(i*10+26,205,i*10+31,205,BLUE);
		 LCD_DisplayString(3,205,12,"-87");
		 LCD_DrawLine(i*10+26,145,i*10+31,145,BLUE);
		 LCD_DisplayString(3,145,12,"-67");
		 LCD_DrawLine(i*10+26,85,i*10+31,85,BLUE);
		 LCD_DisplayString(3,85,12,"-47");
		 LCD_DrawLine(i*10+26,25,i*10+31,25,BLUE);
		 LCD_DisplayString(3,25,12,"-27");
		 LCD_DrawLine(j*40+26,i*10+26,j*40+26,i*10+31,BLUE);//网格y轴
	 }
	}
	step=(freq_end-freq_begin)/5;
	for(j =0;j<6;j++)
	{
		sprintf((char*)display_str,"%.1f",j*step+freq_begin-10.72);
	  LCD_DisplayString(10+40*j,227,12,display_str);
	}
	LCD_DisplayHZstr(250,30,12,"命 令 栏");
	LCD_DrawLine(226,56,319,56,BLUE);
	LCD_DrawLine(226,123,319,123,BLUE);
	LCD_DisplayHZstr(245,65,12,"A:退出扫频");
	LCD_DisplayHZstr(245,105,12,"B:定位光标");
	LCD_DisplayHZstr(250,140,12,"当前频率");
	LCD_DisplayHZstr(250,175,12,"当前电压");
}


//顶层模块选择函数
void Top_Process(void)
{
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_TOP==TOP_SUB0)
		CURRENT_PAGE=SUB0;
	else if(OPTION_TOP==TOP_SUB1)
		CURRENT_PAGE=SUB1;
	else if(OPTION_TOP==TOP_SUB2)
		CURRENT_PAGE=SUB2;
	else if(OPTION_TOP==TOP_SUB3)
		CURRENT_PAGE=SUB3;
	else return;
}
//SUB0模块选择函数
void Sub0_Process(void)
{
	u8 display_str[8];
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB0==SUB0_SUB0)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
		LCD_DisplayHZstr(60,40,12,"请 输 入 本 振 频 率");
		LCD_DisplayHZstr(60,60,12,"0 - 9 为 相 应 数 字");
		LCD_DisplayHZstr(60,80,12,"A C D 为 数 字 0");
		LCD_DisplayHZstr(60,100,12,"* 为 小 数 点");
		LCD_DisplayHZstr(60,120,12,"B 为 退 格 键");
		LCD_DisplayHZstr(60,140,12,"按 下 # 输 入 完 成");
		LCD_DisplayString(160,160,16,"MHz");
		CURRENT_PAGE=CMD;
		CMD_OPTION=PUTSINGLE;
	}
	else if(OPTION_SUB0==SUB0_SUB1)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
		sprintf((char*)display_str,"%.3fMHz",freqmid); //浮点型数据
		LCD_DisplayString(180,39,12,display_str);
		LCD_DisplayHZstr(60,40,12,"当 前 本 振 频 率");
		LCD_DisplayHZstr(60,60,12,"A 停 止 输 出");
		LCD_DisplayHZstr(60,80,12,"B 停 止 输 出 并 退 出");
		LCD_DisplayHZstr(60,100,12,"C 继 续 输 出");
		ADF4351WriteFreq(freqmid);
		CURRENT_PAGE=CMD;
		CMD_OPTION=RUNSINGLE;
	}
	else if(OPTION_SUB0==SUB0_SUB2)
		CURRENT_PAGE=TOP;
	else if(OPTION_SUB0==SUB0_SUB3)
		CURRENT_PAGE=TOP;
	else return;
}
	
//SUB1模块选择函数
void Sub1_Process(void)
{
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB1==SUB1_SUB0)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
		LCD_DisplayHZstr(60,60,12,"请 选 择 扫 频 步 长");
		LCD_DisplayHZstr(60,80,12,"A 扫 频 步 长 10k");
		LCD_DisplayHZstr(60,100,12,"C 扫 频 步 长 100k");
		SCAN_PAR_MODE=0;
		CURRENT_PAGE=CMD;
		CMD_OPTION=PUTSCAN;
	}
	else if(OPTION_SUB1==SUB1_SUB1)
	{
		
		StartScanOutput();
		CURRENT_PAGE=CMD;
		CMD_OPTION=RUNSCAN;
	}
	else if(OPTION_SUB1==SUB1_SUB2)
		CURRENT_PAGE=TOP;
	else if(OPTION_SUB1==SUB1_SUB3)
		CURRENT_PAGE=TOP;
	else return;
}
//SUB2模块选择函数
void Sub2_Process(void)
{
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB2==SUB2_SUB0)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
		CURRENT_PAGE=CMD;
		CMD_OPTION=RUNSCAN;
		StartModuOutput();
	}
	else if(OPTION_SUB2==SUB2_SUB1)
		CURRENT_PAGE=TOP;
	else if(OPTION_SUB2==SUB2_SUB2)
		CURRENT_PAGE=TOP;
	else return;
}
//SUB3模块选择函数
void Sub3_Process(void)
{
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB3==SUB3_SUB0)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
		LCD_DisplayHZstr(60,40,12,"请 输 入 本 振 幅 度");
		LCD_DisplayHZstr(60,60,12,"0 - 9 为 相 应 数 字");
		LCD_DisplayHZstr(60,80,12,"A C D 为 数 字 0");
		LCD_DisplayHZstr(60,100,12,"* 为 小 数 点");
		LCD_DisplayHZstr(60,120,12,"B 为 退 格 键");
		LCD_DisplayHZstr(60,140,12,"按 下 # 输 入 完 成");
		LCD_DisplayString(160,160,16,"mVpp");
		CURRENT_PAGE=CMD;
		CMD_OPTION=SETVALUE;
	}
	else if(OPTION_SUB3==SUB3_SUB1)
		CURRENT_PAGE=TOP;
	else if(OPTION_SUB3==SUB3_SUB2)
		CURRENT_PAGE=TOP;
	else return;
}


//1.------扫频模式下启动输出
void StartScanOutput(void)
{
	DrawAxis();//先绘制坐标轴
	freq=freq_begin;
	ADF4351Init();
	ADF4351WriteFreq(freq);
	count=0;
	finish=0;
	if(freq_step>0.05)
	{
		Option_Scan=ONCE;
		k=1;
		p_data[0]=-93;
	}
	else 
	{
		TIM2->ARR=799;//定时器2时钟84M，分频系数84，84M/84=1M 所以计数500次为0.5ms---10s+8s=18s
		Option_Scan=TWICE;
		flag_twice=FIRST;
		k=0;
	}
}


//2.------已经输出的模式下退出
void Exit_CURRENT(void)
{
		CMD_OPTION=NONE;
		EXCHANGEPAGE=CURRENT_PAGE;
		CURRENT_PAGE=LAST_PAGE;
		LAST_PAGE=EXCHANGEPAGE;
}

//3.------扫频模式下定位光标
void SetCursor(u32 x,u32 y)
{
	u32 i = 0;
	for(i=0;i<20;i++)
	{
		LCD_DrawLine(x,i*10+19,x,i*10+24,RED);
		LCD_DrawLine(i*10+26,y,i*10+31,y,RED);
	}
	LCD_DrawLine(x,i*10+19,x,i*10+24,RED);
}

//4.------调幅模式下输出启动
void StartModuOutput(void)
{
}

//5.-----扫频模式下显示频率和dBm值
void Setvalue_option(void)
{
	int j=find_max(p_data,202);
	x_point=(u32)j+24;
	freq_option=(j-1)*freq_step+freq_begin;
	v_option=p_data[j];
	y_point=-56-3*v_option;
}

//6.寻找最大值的函数
int find_max(double *a,int n)
{
    double max = -100;
    int k ,j = 0;
    for(k=0;k<n;k++)
    {
        if(a[k]>max)
        {
            max = a[k];
					  j = k ;
        }
        k++;
    }
    return j ; 
}


//7.只扫描一次的函数
void Scan_Once(void)
{
	if(k<=201)
		{
			if(count<10)
			{
				temp+=(double)Get_Adc();
				count++;
			}
			else
			{
				ADF4351WriteFreq(freq=freq+freq_step);
				x=temp/10000;
				x=p_data[k]=Convert(x);    //存放dB值
				count=0;
				DrawFreq(Convert_X(k),Convert_Y(x));//绘图
				temp=0;
				k++;
			}
		}
	else 
	{
		finish=1;
		Option_Scan=QUEE;
	}
}

//8.横坐标转换
u16 Convert_X(u16 k)
{
	return k+24;
}

//9.纵坐标转换
u16 Convert_Y(double amp)
{
	if(amp<-92)
		return 3;
	else 
	return (amp+94)*3;
}

//10.扫描两次的函数
void Scan_TWICE_1(void)
{
	if(k<=2000)
	{
		if(count<10)
		{
			temp+=(double)Get_Adc();
			count++;
		}
		else
		{
			ADF4351WriteFreq(freq=freq+freq_step);
			p_data_high[k]=temp/10000;
			count=0;
			temp=0;
			k++;
		}
	}
	else
		flag_twice=INTERVAL;
}

void Scan_TWICE_2(void)
{
		if(k<=201)
		{
			if(count<10)
			{
				temp+=(double)Get_Adc();
				count++;
			}
			else
			{
				ADF4351WriteFreq(freq=freq+freq_step);
				x=temp/10000;
				x=p_data[k]=Convert(x);    //存放dB值
				count=0;
				DrawFreq(Convert_X(k),Convert_Y(x));//绘图
				temp=0;
				k++;
			}
		}
	else 
	{
		Option_Scan=QUEE;
		finish=1;
	}
}

//扫频模式数字输入
void Scan_Input_Draw(void)
{
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"频  谱  分  析  仪");
	LCD_DisplayHZstr(60,60,12,"请 选 择 扫 频 起 点");
	LCD_DisplayHZstr(60,80,12,"0 - 9 为 相 应 数 字");
	LCD_DisplayHZstr(60,100,12,"* 为 小 数 点");
	LCD_DisplayHZstr(60,120,12,"B 为 退 格 键");
	LCD_DisplayHZstr(60,140,12,"按 下 # 输 入 完 成");
	LCD_DisplayString(140,159,12,"MHz");
}



