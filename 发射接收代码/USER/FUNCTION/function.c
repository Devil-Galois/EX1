#include "function.h"



#ifdef		SEND
//绘制静态界面
void DrawMenu()
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(120,60,24,"菜  单");
	LCD_DisplayHZstr(110,105,16,"A.设置数码");
	LCD_DisplayHZstr(110,160,16,"B.启动传输");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%d %d %d %d",p_number[0],p_number[1],p_number[2],p_number[3]);
	LCD_DisplayHZstr(70,220,12,"当 前 数 码");
	LCD_DisplayString(180,219,12,display_str);
	BRUSH_COLOR=WHITE;
}


//绘制输出界面
void DrawOutput()
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(110,60,16,"A.暂停并返回");
	sprintf((char*)display_str,"%d %d %d %d",p_number[0],p_number[1],p_number[2],p_number[3]);
	LCD_DisplayHZstr(80,120,12,"正 在 传 输");
	LCD_DisplayString(180,119,12,display_str);
	BRUSH_COLOR=WHITE;
}

//绘制数字输入界面
void DrawInput()
{
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(100,40,16,"按下 # 完成输入");
	LCD_DisplayHZstr(100,70,16,"按下 B 进行删除");
}


//数字分离器
void Digital_Convert(int digital)
{
	int temp=digital;
	int i;
	for(i=0;i<4;i++)
	{
		p_number[3-i]=temp%10;
		temp/=10;
	}
}

//进制转换
void Base_Convert(u8 *a_data)
{
	int temp;
	int i=0,j=0;
	for(i=0;i<4;i++)
	{
		temp=a_data[3-i];
		for(j=0;j<4;j++)
		{
			p_element[23-i*4-j]=temp%2;
			temp/=2;
		}
	}
}


//数字编码
void Coding()
{
	int i=0;
	int count=0;
	int temp;
	for(i=0;i<8;i++)
		p_element[i]=1;
	Base_Convert(p_number);
	for(i=8;i<24;i++)
		if(p_element[i])
			count++;
	temp=count;
	for(i=0;i<4;i++)
	{
		p_element[27-i]=temp%2;
		temp/=2;
	}
	p_element[28]=1;
	p_element[29]=1;
	p_element[30]=1;
	p_element[31]=0;
}
#endif


#ifndef		SEND

//二进制转十进制
int Binary2Decimal(u8 *p)
{
	return 8*p[0]+4*p[1]+2*p[2]+p[3];
}

//计算长度16数组中1的个数
int Counter(u8 *p)
{
	int i=0;
	int count=0;
	for(i=0;i<16;i++)
		if(p[i])
			count++;
	return count;
}
//校验码尾
int Check(u8 *p)
{
	return p[0]&&p[1]&&p[2]&&(!p[3]);
}


#endif
