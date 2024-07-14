#include "sys.h"
#include "operation.h"
#include "ADF4351.h"
#include "PE4302.h"
#include "lcd.h"
#include "showhz.h"	     //����HanZiUse.lib����ʾ���ֺ���
#include "led.h"
#include "math.h"
#include "adc.h"
#include "timer.h"

//���ƾ�̬UI�����к���
//1.���Ʋ���Ľ���Ԫ��
void DrawStatic(void)
{
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
	LCD_DisplayHZstr(120,60,24,"M E N U");
	LCD_DisplayHZstr(60,100,12,"A:��Ƶģʽ");
	LCD_DisplayHZstr(190,100,12,"B:ɨƵģʽ");
	LCD_DisplayHZstr(60,140,12,"C:����ģʽ");
	LCD_DisplayHZstr(190,140,12,"D:��������");
}

//2.���Ƶ�Ƶ�Ĳ˵�����
void DrawTop_Sub0(void)
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
	LCD_DisplayHZstr(90,60,24,"�� Ƶ ģ ʽ");
	LCD_DisplayHZstr(60,100,12,"A:Ƶ������");
	LCD_DisplayHZstr(190,100,12,"B:�������");
	LCD_DisplayHZstr(60,140,12,"C:�˵�����");
	LCD_DisplayHZstr(190,140,12,"D:���˵�");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%.3fMHz",freqmid);
	LCD_DisplayHZstr(60,220,12,"�� ǰ �� �� Ƶ ��");
	LCD_DisplayString(180,219,12,display_str);
	BRUSH_COLOR=WHITE;
}

//3.����ɨƵģʽ�˵�����
void DrawTop_Sub1(void)
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
	LCD_DisplayHZstr(90,60,24,"ɨ Ƶ ģ ʽ");
	LCD_DisplayHZstr(60,100,12,"A:ɨƵ����");
	LCD_DisplayHZstr(190,100,12,"B:ɨƵ����");
	LCD_DisplayHZstr(60,140,12,"C:�˵�����");
	LCD_DisplayHZstr(190,140,12,"D:���˵�");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%.2f-%.2fMHz",freq_begin-10.72,freq_end-10.72);
	LCD_DisplayString(150,199,12,display_str);
	LCD_DisplayHZstr(60,200,12,"�� ǰ ɨ Ƶ");
	sprintf((char*)display_str,"%dkHz",(int)(freq_step*1000));
	LCD_DisplayString(150,219,12,display_str);
	LCD_DisplayHZstr(60,220,12,"�� �� ��");
	BRUSH_COLOR=WHITE;
}

//4.���Ƶ���ģʽ�˵�����
void DrawTop_Sub2(void)
{
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
	LCD_DisplayHZstr(90,60,24,"�� �� ģ ʽ");
	LCD_DisplayHZstr(60,100,12,"A:ɨƵ����");
	LCD_DisplayHZstr(190,100,12,"B:�˵�����");
	LCD_DisplayHZstr(60,140,12,"C:���˵�");
}

//5.���Ʋ������ò˵�����
void DrawTop_Sub3(void)
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
	LCD_DisplayHZstr(90,60,24,"�� �� �� ��");
	LCD_DisplayHZstr(60,100,12,"A:�����������");
	LCD_DisplayHZstr(190,100,12,"B:�˵�����");
	LCD_DisplayHZstr(60,140,12,"C:���˵�");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%.3fmVpp",v_core);
	LCD_DisplayHZstr(60,220,12,"�� ǰ �� �� �� ��");
	LCD_DisplayString(180,219,12,display_str);
	BRUSH_COLOR=WHITE;
}


//����Ƶ�׵ĺ���
//�Ѳɼ�����32λ����ת���ɶ�Ӧ���ӵĺ���
//dbm=10log10(Vrms^2/50*10^-3)
//10mVrms--- -26.99dBm
//10uVrms---- -86.99dBm
//5uVrms=----- -93.01dBm
//��ֵ��66---��Ҫ3*66=198����
//���ÿdBm��3����---  -26.99dBm=-27dBm---
//<-93dBm---<5uVrms---238,237,236
//�˺�ÿ����1dBm----��������������----�������dBm��ֵ��   ������������-3��/3-93  ͬ�������dBm�����󺣰�
double Convert(double value)
{
	if(value<2.06)
	return _10_LOG2_10*(2*(-0.3409*pow(value,2)+6.3953*value-12.1263)-LOG2_50000);
	else
	return _10_LOG2_10*(2*(-50.0855*pow(value,3)+342.8234*pow(value,2)-771.3388*value+571.6222)-LOG2_50000);
}
//���ƺ��η�Χ238
//����x��Χ2--202  ��200��
void DrawFreq(u16 freq,u16 amp)
{
	u16 i;
	u16 end=226-amp;
	//LCD_DrawLine(freq1,end1,freq2,end2,YELLOW);
	for(i=226;i>end;i--)
	LCD_Color_DrawPoint(freq,end,YELLOW);
}
//����������ĺ���
void DrawAxis(void)
{
	u8 display_str[8];
	u16 j,i;
	double step;
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(80,0,16,"Ƶ  ��  ��  ��  ��");
	LCD_DrawLine(26,226,319,226,BLUE);//x��
	LCD_DrawLine(1,19,319,19,BLUE);//�����߽�
	LCD_DrawLine(24,19,24,226,BLUE);//y��
	LCD_DrawLine(226,19,226,226,BLUE);//�������߽�
	LCD_DrawLine(319,19,319,226,BLUE);//������ұ߽�
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
		 LCD_DrawLine(j*40+26,i*10+26,j*40+26,i*10+31,BLUE);//����y��
	 }
	}
	step=(freq_end-freq_begin)/5;
	for(j =0;j<6;j++)
	{
		sprintf((char*)display_str,"%.1f",j*step+freq_begin-10.72);
	  LCD_DisplayString(10+40*j,227,12,display_str);
	}
	LCD_DisplayHZstr(250,30,12,"�� �� ��");
	LCD_DrawLine(226,56,319,56,BLUE);
	LCD_DrawLine(226,123,319,123,BLUE);
	LCD_DisplayHZstr(245,65,12,"A:�˳�ɨƵ");
	LCD_DisplayHZstr(245,105,12,"B:��λ���");
	LCD_DisplayHZstr(250,140,12,"��ǰƵ��");
	LCD_DisplayHZstr(250,175,12,"��ǰ��ѹ");
}


//����ģ��ѡ����
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
//SUB0ģ��ѡ����
void Sub0_Process(void)
{
	u8 display_str[8];
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB0==SUB0_SUB0)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
		LCD_DisplayHZstr(60,40,12,"�� �� �� �� �� Ƶ ��");
		LCD_DisplayHZstr(60,60,12,"0 - 9 Ϊ �� Ӧ �� ��");
		LCD_DisplayHZstr(60,80,12,"A C D Ϊ �� �� 0");
		LCD_DisplayHZstr(60,100,12,"* Ϊ С �� ��");
		LCD_DisplayHZstr(60,120,12,"B Ϊ �� �� ��");
		LCD_DisplayHZstr(60,140,12,"�� �� # �� �� �� ��");
		LCD_DisplayString(160,160,16,"MHz");
		CURRENT_PAGE=CMD;
		CMD_OPTION=PUTSINGLE;
	}
	else if(OPTION_SUB0==SUB0_SUB1)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
		sprintf((char*)display_str,"%.3fMHz",freqmid); //����������
		LCD_DisplayString(180,39,12,display_str);
		LCD_DisplayHZstr(60,40,12,"�� ǰ �� �� Ƶ ��");
		LCD_DisplayHZstr(60,60,12,"A ͣ ֹ �� ��");
		LCD_DisplayHZstr(60,80,12,"B ͣ ֹ �� �� �� �� ��");
		LCD_DisplayHZstr(60,100,12,"C �� �� �� ��");
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
	
//SUB1ģ��ѡ����
void Sub1_Process(void)
{
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB1==SUB1_SUB0)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
		LCD_DisplayHZstr(60,60,12,"�� ѡ �� ɨ Ƶ �� ��");
		LCD_DisplayHZstr(60,80,12,"A ɨ Ƶ �� �� 10k");
		LCD_DisplayHZstr(60,100,12,"C ɨ Ƶ �� �� 100k");
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
//SUB2ģ��ѡ����
void Sub2_Process(void)
{
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB2==SUB2_SUB0)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
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
//SUB3ģ��ѡ����
void Sub3_Process(void)
{
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB3==SUB3_SUB0)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
		LCD_DisplayHZstr(60,40,12,"�� �� �� �� �� �� ��");
		LCD_DisplayHZstr(60,60,12,"0 - 9 Ϊ �� Ӧ �� ��");
		LCD_DisplayHZstr(60,80,12,"A C D Ϊ �� �� 0");
		LCD_DisplayHZstr(60,100,12,"* Ϊ С �� ��");
		LCD_DisplayHZstr(60,120,12,"B Ϊ �� �� ��");
		LCD_DisplayHZstr(60,140,12,"�� �� # �� �� �� ��");
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


//1.------ɨƵģʽ���������
void StartScanOutput(void)
{
	DrawAxis();//�Ȼ���������
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
		TIM2->ARR=799;//��ʱ��2ʱ��84M����Ƶϵ��84��84M/84=1M ���Լ���500��Ϊ0.5ms---10s+8s=18s
		Option_Scan=TWICE;
		flag_twice=FIRST;
		k=0;
	}
}


//2.------�Ѿ������ģʽ���˳�
void Exit_CURRENT(void)
{
		CMD_OPTION=NONE;
		EXCHANGEPAGE=CURRENT_PAGE;
		CURRENT_PAGE=LAST_PAGE;
		LAST_PAGE=EXCHANGEPAGE;
}

//3.------ɨƵģʽ�¶�λ���
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

//4.------����ģʽ���������
void StartModuOutput(void)
{
}

//5.-----ɨƵģʽ����ʾƵ�ʺ�dBmֵ
void Setvalue_option(void)
{
	int j=find_max(p_data,202);
	x_point=(u32)j+24;
	freq_option=(j-1)*freq_step+freq_begin;
	v_option=p_data[j];
	y_point=-56-3*v_option;
}

//6.Ѱ�����ֵ�ĺ���
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


//7.ֻɨ��һ�εĺ���
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
				x=p_data[k]=Convert(x);    //���dBֵ
				count=0;
				DrawFreq(Convert_X(k),Convert_Y(x));//��ͼ
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

//8.������ת��
u16 Convert_X(u16 k)
{
	return k+24;
}

//9.������ת��
u16 Convert_Y(double amp)
{
	if(amp<-92)
		return 3;
	else 
	return (amp+94)*3;
}

//10.ɨ�����εĺ���
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
				x=p_data[k]=Convert(x);    //���dBֵ
				count=0;
				DrawFreq(Convert_X(k),Convert_Y(x));//��ͼ
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

//ɨƵģʽ��������
void Scan_Input_Draw(void)
{
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
	LCD_DisplayHZstr(60,60,12,"�� ѡ �� ɨ Ƶ �� ��");
	LCD_DisplayHZstr(60,80,12,"0 - 9 Ϊ �� Ӧ �� ��");
	LCD_DisplayHZstr(60,100,12,"* Ϊ С �� ��");
	LCD_DisplayHZstr(60,120,12,"B Ϊ �� �� ��");
	LCD_DisplayHZstr(60,140,12,"�� �� # �� �� �� ��");
	LCD_DisplayString(140,159,12,"MHz");
}



