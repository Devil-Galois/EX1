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
#include "updatefont.h"  //����HanZiUse.lib�и����ֿ⺯�� ����Ҽ�ѡ���ͷ�ļ��ɲ鿴��ͷ�ļ�
#include "showhz.h"	     //����HanZiUse.lib����ʾ���ֺ���
#include "KeyBoard.h"
#include "math.h"
#include "timer.h"
#include "operation.h"
#include "exti.h"
#include "math.h"


#define _10_LOG2_10    3.0103
#define	LOG2_50000     15.60964
//��ǰ�˵�
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

/*  �˵�����ȫ�ֱ���  */
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

/*  �˵�����ȫ�ֱ�������  */

/*  ɨƵ��������  */
int 	 Option_Scan=QUEE;
double freq_step=0.1;
double freq_begin=90.72;
double freq_end=110.72;
double freq;
int flag_twice;
int finish=0;


u32 x_point;
u32 y_point;
/* ɨƵ��������  */


/*  ������������  */
double am_data[201]={0};//��Ų������������
double am_data_high[2001]={0};//��ŵ�һ��ɨƵ���������
double *p_data_high=am_data_high;//����ָ�����ڴ�ŵ�һ��ɨ����
double *p_data=am_data;//����ָ��
int 		k=1;									//ɨƵ�±�
int 		count=0;//������
/*  �����������ý���  */


/* ����ȫ�ֱ��� */
double readvalue_keyboard;
/* ����ȫ�ֱ������� */


/* Ӳ��ȫ�ֱ���  */
double freqmid=100;
double v_core=100;
/* Ӳ��ȫ�ֱ������� */
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
	
	
	/*  ��ʼ������  */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	delay_init();		  															//��ʼ����ʱ����
	LED_Init();		    															//��ʼ��LED�˿�
	LCD_Init();																			//��ʼ��LCD
	KEY_Init();																			//��ʼ�����ذ���
	ADC1_Init();        														//��ʼ��ADC
	Keyboard_GPIO_Config();													//��ʼ�����̶˿�
	ADF4351Init();     															//��ʼ�����໷
	PE_GPIO_Init();																	//��ʼ��˥����
	TIM2_Init(3999,83);															//��ʱ��2ʱ��84M����Ƶϵ��84��84M/84=1M ���Լ���4000��Ϊ4ms---8sɨ��
	TIM5_Init(99,8399);															//��ʱ��5ʱ��84M����Ƶϵ��8400��84M/8400=10K ���Լ���100��Ϊ10ms   ��ÿ10ms����һ�ΰ�����⺯��
	EXTIX_Init();																		//�ⲿ�жϳ�ʼ��
	font_init();																		//��ʼ�������ֿ�
	InitRowColValue();															//��ʼ�������б�
	ADC_Cmd(ADC1, ENABLE);													//����ADC
	PE4302_Set(8);																	//value���ⲿ������������PE4302��˥������
  /*  ��ʼ������  */
	//ADF4351WriteFreq(90);
	
	/*  UI����  */
	DrawStatic();																		//��λ������ʾ
	/*  UI���ƽ���  */
	/*  ��ѭ��  */
	while(1){
	/*  ���̶�ȡ���� ---- �˵�ģʽ */
		if(CURRENT_PAGE!=CMD)
		{
			option=ReadKeyBoard();
	/*  �˵����ƴ���  */
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
			//��������     ���а���������ִ�в˵�ѡ�����    �����������ȼ�  A>B>C>D
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
		/*  ��������ģʽ CURRENT_PAGE==CMD */
		else 
		{
			if(KEYBOARD_16)
			{
				readvalue_keyboard=0;
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);
			}
			//�������봦��
			if(ENSURE_INPUT)
			{
				switch(CMD_OPTION)
				{
					case PUTSINGLE:
							freqmid=readvalue_keyboard;
							break;
					case SETVALUE:
							v_core=readvalue_keyboard;
							PE4302_Set(-40.71*log10(v_core)+97.19);//����˥��������
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
		/*  ��������ģʽ����  */
	
	/*  �˵����ƴ������  */
	
		
	/*  ɨƵ����ʼ  */
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
			sprintf((char*)display_str,"%.3fMHz",freq_option); //����������
			LCD_DisplayString(255,155,12,display_str);
			sprintf((char*)display_str,"%.3fdBm",v_option); //����������
			LCD_DisplayString(255,190,12,display_str);
			step=(freq_end-freq_begin)/5;
			for(j =0;j<6;j++)
			{
				sprintf((char*)display_str,"%.1f",j*step+freq_begin-10.72);
				LCD_DisplayString(10+40*j,227,12,display_str);
			}
		}
			
	/* ɨƵ������� */
	}
	/*  ��ѭ������  */
}






