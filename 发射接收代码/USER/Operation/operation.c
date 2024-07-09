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


/* ����� */
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
	LCD_DisplayHZstr(60,140,12,"C:���໷����");
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
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
	LCD_DisplayHZstr(90,60,24,"�� �� ģ ʽ");
	LCD_DisplayHZstr(60,100,12,"A:ɨƵ����");
	LCD_DisplayHZstr(190,100,12,"B:ɨƵ����");
	LCD_DisplayHZstr(60,140,12,"C:�˵�����");
	LCD_DisplayHZstr(190,140,12,"D:���˵�");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%.2fMHz",freq_modu_mid);
	LCD_DisplayString(180,199,12,display_str);
	LCD_DisplayHZstr(60,200,12,"�� ǰ �� �� Ƶ ��");
	BRUSH_COLOR=WHITE;
}

//5.���Ʋ������ò˵�����
void DrawTop_Sub3(void)
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
	LCD_DisplayHZstr(90,60,24,"�� �� �� ��");
	LCD_DisplayHZstr(60,100,12,"A:�����������");
	LCD_DisplayHZstr(190,100,12,"B:ɨƵʱ������");
	LCD_DisplayHZstr(60,140,12,"C:�˵�����");
	LCD_DisplayHZstr(190,140,12,"D:���˵�");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%.3fmVpp",v_core);
	LCD_DisplayString(180,219,12,display_str);
	LCD_DisplayHZstr(60,220,12,"�� ǰ �� �� �� ��");
	sprintf((char*)display_str,"%ds",peroid);
	LCD_DisplayHZstr(60,200,12,"�� ǰ ɨ Ƶ ʱ ��");
	LCD_DisplayString(180,199,12,display_str);
	BRUSH_COLOR=WHITE;
}



/* ����� */






/* ����� */
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
double Convert(double data)
{
//	if(data>3143.6)
//		return P1*data-Q1;
//	else if(data>3042.9)
//		return P2*data-Q2;
//	else if(data>2543.9)
//		return P3*data-Q3;
//	else if(data>1840)
//		return P4*data-Q4;
//	else 
//		return -93;
	if(data>3148.1665)
		return PN1*data-QN1;
	else if(data>2000)
		return PN2*data-QN2;
	else return -93;
}

double Convert_high(double data)
{
	if(data>3170.366)
		return SN1*data-TN1;
	else if(data>3100.55)
		return SN2*data-TN2;
	else if(data>2313)
		return SN3*data-TN3;
	else if(data>2040)
		return SN4*data-TN4;
	else return -93;
//	if(data>3143.6)
//		return P1*data-Q1;
//	else if(data>3042.9)
//		return P2*data-Q2;
//	else if(data>2543.9)
//		return P3*data-Q3;
//	else if(data>1840)
//		return P4*data-Q4;
//	else 
//		return -93;
	
}
//���ƺ��η�Χ238
//����x��Χ2--202  ��200��
void DrawFreq(u16 freq,u16 amp)
{
	u16 end=225-amp;
	LCD_DrawLine(freq,225,freq,end,YELLOW);
}

/* ����� */




/* ����� */

//y:20-225
//x:25-225
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
	LCD_DisplayHZstr(250,180,12,"��ǰ��ѹ");
}



void DrawAxis_Modu(void)
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
	LCD_DisplayHZstr(250,26,12,"�� �� ��");
	LCD_DrawLine(226,40,319,40,BLUE);
	LCD_DrawLine(226,80,319,80,BLUE);
	LCD_DisplayHZstr(245,45,12,"A:�˳�ɨƵ");
	LCD_DisplayHZstr(245,65,12,"B:��λ���");
	LCD_DisplayHZstr(228,85,12,"�ز�");
	LCD_DisplayHZstr(228,125,12,"�ϱ�");
	LCD_DisplayHZstr(228,165,12,"�±�");
	LCD_DisplayHZstr(228,205,12,"ָ��");
}

/* ����� */









/* ����� */
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
	OPTION_TOP=TOP_WAIT;
	return;
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
		LCD_DisplayHZstr(60,80,12,"A �� ��");
		ADF4351WriteFreq(freqmid);
		CURRENT_PAGE=CMD;
		CMD_OPTION=RUNSINGLE;
	}
	else if(OPTION_SUB0==SUB0_SUB2)
		CURRENT_PAGE=TOP;
	else if(OPTION_SUB0==SUB0_SUB3)
		CURRENT_PAGE=TOP;
	OPTION_SUB0=SUB0_WAIT;
	return;
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
	{
		Start_OnlyOutput();
		CURRENT_PAGE=CMD;
		CMD_OPTION=NEWCMD;
	}
	else if(OPTION_SUB1==SUB1_SUB3)
		CURRENT_PAGE=TOP;
	OPTION_SUB1=SUB1_WAIT;
}
//SUB2ģ��ѡ����
void Sub2_Process(void)
{
	LAST_PAGE=CURRENT_PAGE;
	if(OPTION_SUB2==SUB2_SUB0)
	{
		freq_step=0.01;
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
		LCD_DisplayHZstr(60,60,12,"�� ѡ �� �� �� Ƶ ��");
		LCD_DisplayHZstr(60,80,12,"0 - 9 Ϊ �� Ӧ �� ��");
		LCD_DisplayHZstr(60,100,12,"* Ϊ С �� ��");
		LCD_DisplayHZstr(60,120,12,"B Ϊ �� �� ��");
		LCD_DisplayHZstr(60,140,12,"�� �� # �� �� �� ��");
		LCD_DisplayString(140,159,12,"MHz");
		CURRENT_PAGE=CMD;
		CMD_OPTION=PUTMODU;
	}
	else if(OPTION_SUB2==SUB2_SUB1)
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
		CURRENT_PAGE=CMD;
		CMD_OPTION=RUNMODU;
		StartModuOutput();
	}
	else if(OPTION_SUB2==SUB2_SUB2)
		CURRENT_PAGE=TOP;
	else if(OPTION_SUB2==SUB2_SUB3)
		CURRENT_PAGE=TOP;
	OPTION_SUB2=SUB2_WAIT;
	return;
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
	{
		LCD_Clear(BLACK);
		LCD_DisplayHZstr(60,0,24,"Ƶ  ��  ��  ��  ��");
		LCD_DisplayHZstr(60,40,12,"�� �� �� ɨ Ƶ ʱ ��");
		LCD_DisplayHZstr(60,60,12,"0 - 9 Ϊ �� Ӧ �� ��");
		LCD_DisplayHZstr(60,80,12,"A C D Ϊ �� �� 0");
		LCD_DisplayHZstr(60,100,12,"* Ϊ С �� ��");
		LCD_DisplayHZstr(60,120,12,"B Ϊ �� �� ��");
		LCD_DisplayHZstr(60,140,12,"�� �� # �� �� �� ��");
		LCD_DisplayString(160,160,16,"s");
		CURRENT_PAGE=CMD;
		CMD_OPTION=SETTIM;
	}
	else if(OPTION_SUB3==SUB3_SUB2)
		CURRENT_PAGE=TOP;
	else if(OPTION_SUB3==SUB3_SUB3)
		CURRENT_PAGE=TOP;
	OPTION_SUB3=SUB3_WAIT;
	return;
}

/* ����� */




double FurConvert(double x)
{
	double y;
	 y=Convert_figure(x-(6.912241066675327*x-2.235346987805799e+02+47)/6.912241066675327);
	return y;
}
















/*�����*/

//1.------ɨƵģʽ���������
void StartScanOutput(void)
{
	DrawAxis();//�Ȼ���������
	freq=freq_begin;
	if(freq_step>0.05)
		test=1;
	ADF4351WriteFreq(freq);
	count=0;
	temp=0;
	finish_once=0;
	disp=0;
	k=0;
	Option_Scan=ONCE;
	
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


/* ����� */



//4.------����ģʽ���������
void StartModuOutput(void)
{
	DrawAxis_Modu();//�Ȼ���������
	freq=freq_modu_begin;
	ADF4351Init();
	ADF4351WriteFreq(freq);
	count=0;
	finish_modu=0;
	disp=0;
	k=0;
	temp=0;
	Option_Scan=MODU;
}
void Start_OnlyOutput(void)
{
	DrawAxis();//�Ȼ���������
	freq=90;
	ADF4351Init();
	ADF4351WriteFreq(freq);
	count=0;
	k=0;
	Option_Scan=ONLY;
}


/* ����� */
//5.-----ɨƵģʽ����ʾƵ�ʺ�dBmֵ
void Setvalue_option(void)
{
	int m;
	double s;
	int j=find_max(p_data,201);
	x_point=(u32)j+25;
	freq_option=j*freq_step+freq_begin;
	//v_option=p_data[j];
	v_option=Convert(p_data[j]);
	if(freq_option<=93.82)
	{
		v_option-=1;
		p_data[j]-=1/PN2;
	}
	y_point=225-Convert_Y(Convert(p_data[j]));
	v_option-=5;
	if(p_data[j]<-47)
	p_data[j]-=5/PN2;
	else 
		p_data[j]-=5/PN1;
}

void Setvalue_option_modu(void)
{
	int m;
	int j=find_max(p_data,201);
	x_point=(u32)j+25;
	v_center=Convert_high(p_data[j])+2;
	v_low=v_high=(p_data[j-10]+p_data[j+10]+p_data[j-11]+p_data[j+11]+p_data[j-9]+p_data[j+9])/6;
	y_point=Convert_Y(v_center+2);
	freq_center=j*0.01+freq_modu_begin;
	freq_low=freq_center-0.1;
	freq_high=freq_center+0.1;
	v_center-=5.5;
	figure_am=40;
//	if(v_center<-37)
//	{
//		figure_am=Convert_figure(v_low*0.01);
		v_low=v_high=Convert_edge(0.01*v_high);
	v_low=v_high=v_low-2.2;
//		v_center+=figure_am*0.03;
//	}
//	else 
//	{
//		figure_am=Convert_figure_2(v_low*0.01);
		//v_low=v_high=2.191520248239798*0.01*v_low-1.069967697583498e+02;
//		v_center+=figure_am*0.03;
//	}
	//v_low=Convert_high(p_data[j-10]);
	//v_high=Convert_high(p_data[j+10]);
	//figure_am=2*pow(10,(v_high-v_center)/20);
	for(m=0;m<j-10;m++)
		p_data[m]=1824;
	for(m=j-9;m<j;m++)
		p_data[m]=1824;
	for(m=j+1;m<j+10;m++)
		p_data[m]=1824;
	for(m=j+11;m<201;m++)
		p_data[m]=1824;
}



void Setvalue_option_high(void)
{
	int m;
	double s;
	int j=find_max(p_data,201);
	x_point=(u32)j+25;
	freq_option=j*freq_step+freq_begin;
	//v_option=p_data[j];
	v_option=Convert_high(p_data[j]);
		if(v_option>-31.5)
		{
			v_option-=5.5;
			p_data[j]-=8/SN1;
		}
		else {
			v_option-=4;
			p_data[j]-=4/SN4;
		}
		
	y_point=225-Convert_Y(Convert_high(p_data[j]));
//		if(v_option<-47&&v_option>=-53.5)
//			v_option+=1.5;
//		if(freq_option>=99.9+10.72&&v_option<-81)
//			v_option-=3;
}
/* ����� */
/* ����� */
//6.Ѱ�����ֵ�ĺ���  n:����
int find_max(double *a,int n)
{
    double max = -1000;
    int k ,j = 0;
    for(k=0;k<n;k++)
    {
        if(a[k]>max)
        {
            max = a[k];
					  j = k ;
        }
    }
    return j ; 
}

/* ����� */
/* ����� */

//7.����ɨ��ĺ���
void Scan_Once(void)
{
	if(k<201)
		{
			if(count<16)
			{
				if(count)
					temp+=(double)Get_Adc();
				count++;
			}
			else
			{
				if(!test)
					ADF4351WriteFreq(freq=freq+freq_step);
				p_data[k]=temp/15;
				count=0;
				temp=0;
				if(!test)
						k++;
				else
				test=0;
			}
		}
	else 
	{
		finish_once=1;
		disp=1;
		Option_Scan=QUEE;
	}
}

//8.������ת��
u16 Convert_X(u16 k)
{
		return k+25;
}

//9.������ת��
u16 Convert_Y(double amp)
{
	if(amp<-92)
		return 3;
	else 
	return (amp+94)*3;
}


/* ����� */
//10.��Ƶģʽ��ɨƵ����
void Scan_Modu(void)
{
	if(k<201)
		{
			if(count<16)
			{
				if(count)
					temp+=(double)Get_Adc();
				count++;
			}
			else
			{
				ADF4351WriteFreq(freq=freq+0.01);
				p_data[k]=temp/15;
				count=0;
				temp=0;
				k++;
			}
		}
	else 
	{
		disp=1;
		finish_modu=1;
		Option_Scan=QUEE;
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


void Draw_Line(void)
{
	int j=0;
	for(j=0;j<201;j++)
		DrawFreq(Convert_X(j),Convert_Y(Convert(p_data[j])));
}


void Draw_Line_high(void)
{
	int j=0;
	for(j=0;j<201;j++)
		DrawFreq(Convert_X(j),Convert_Y(Convert_high(p_data[j])));
}

void Scan_Only(void)
{
		if(k<201)
		{
			ADF4351WriteFreq(freq=freq+0.1);
			count=0;
			k++;
		}
	else 
	{
		Option_Scan=QUEE;
	}
}



double Convert_figure(double x)
{
	//return 4.380716959539005*x*x-2.079029409048162e+02*x+2.477380463379705e+03;
//	return 4.285467168736092*x*x-2.028643067631819e+02*x+2.411303369195047e+03;
//	return 3.992240518000173*x*x-1.873210142855417e+02*x+2.207450422901072e+03;
//	return 4.113997171208329*x*x-1.929872051851720e+02*x+2.273354286866515e+03;
//	return 4.388403558034023*x*x-2.076804500505547e+02*x+2.466505845673204e+03;
//	if(x>25.2)
//	return //4.079859728091232*x*x-1.988680718091704e+02*x+2.441894960504930e+03;
//		//4.285467168736092*x*x-2.028643067631819e+02*x+2.411303369195047e+03;
//4.113997171208329*x*x-1.929872051851720e+02*x+2.273354286866515e+03;
//	else return 4.535147392290249*x-96.802721088435390;
	
	
//	if(x>26.84)return 50.654162696447530*x-1.299670396213135e+03;
//	else return 11.758426323389650*x-2.699110328923293e+02;
	if(x>24.30)
	return 0.055372688886225*x*x*x*x-3.529452973665794*x*x*x+59.329566619681840*x*x+2.000835270794026e+02*x-8.542780373222447e+03;
	else return 9.648784253184127*x-2.189328444615985e+02;
	//return 0.036226727130454*x*x*x*x+-1.528261149657596*x*x*x-19.071877896619874*x*x+1.564564794810894e+03*x-1.744362554092285e+04;
}

double Convert_edge(double x)
{
	return 2.175467425888834*x-1.112594665606405e+02;
}

double Convert_figure_2(double x)
{
	//return 1.268144871663515*x*x*x-1.013984537625716e+02*x*x+2.707441058670845e+03*x-2.412553160701174e+04;
	return 1.268144871663515*x*x*x-1.013984537625716e+02*x*x+2.707441058670845e+03*x-2.412553160701174e+04;
}
