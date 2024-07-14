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



//ɨƵѡ������
#define 	QUEE					0
#define   ONCE					1
#define		TWICE					2

#define		FIRST         1
#define		INTERVAL			3
#define		SECOND				2


/* �˵�����ȫ�ֱ���  */
extern int 								CURRENT_PAGE;  
extern int								LAST_PAGE;
extern int 								CMD_OPTION;
extern int								EXCHANGEPAGE;
extern double freq_option;
extern double v_option;
extern int								SCAN_PAR_MODE;  //0��ѡ��ֱ���ģʽ  1��ѡ����ʼƵ��ģʽ
/* �˵�����ȫ�ֱ�������  */


/* ɨƵ���� */
extern int Option_Scan;
extern double freq_step;
extern double freq_begin;
extern double freq_end;
extern double freq;
extern int k;
extern  int count;//������
extern 	double 	*p_data;//���ɨƵ���
extern double *p_data_high;//����ָ��
extern double 	temp;//�ݴ�����
extern double 	x;
extern int flag_twice;
extern int finish;

extern u32 x_point;
extern u32 y_point;
/* ɨƵ���� */




/* Ӳ��ȫ�ֱ���  */
extern double freqmid;
extern double v_core;
/* Ӳ��ȫ�ֱ������� */





/*  �˵���Ҫ��ö���ͱ���  */


//��һ���˵�
typedef enum MENU_TOP{
	TOP_SUB0=0,
  TOP_SUB1=1,
	TOP_SUB2=2,
	TOP_SUB3=3,
	TOP_WAIT=4
}MENU_TOP;


//SUB0�ĵڶ����˵�-----��Ƶģʽ
typedef enum MENU_SUB0{
	SUB0_SUB0=0,
	SUB0_SUB1=1,
	SUB0_SUB2=2,
	SUB0_SUB3=3,
	SUB0_WAIT=4
}MENU_SUB0;

//SUB1�ĵڶ����˵�-----ɨƵģʽ
typedef enum MENU_SUB1{
	SUB1_SUB0=0,
	SUB1_SUB1=1,
	SUB1_SUB2=2,
	SUB1_SUB3=3,
	SUB1_WAIT=4
}MENU_SUB1;

//SUB2�ĵڶ����˵�-----����ģʽ
typedef enum MENU_SUB2{
	SUB2_SUB0=0,
	SUB2_SUB1=1,
	SUB2_SUB2=2,
	SUB2_WAIT=3
}MENU_SUB2;

//SUB3�ĵ������˵�-----��������
typedef enum MENU_SUB3{
	SUB3_SUB0=0,
	SUB3_SUB1=1,
	SUB3_SUB2=2,
	SUB3_WAIT=3
}MENU_SUB3;

/*  �˵���Ҫ��ö���ͱ����������  */

/*  �˵�ѡ�����  */
extern MENU_TOP 					OPTION_TOP;
extern MENU_SUB0 					OPTION_SUB0;
extern MENU_SUB1 					OPTION_SUB1;
extern MENU_SUB2 					OPTION_SUB2;
extern MENU_SUB3 					OPTION_SUB3;
/*  �˵�ѡ���������  */




/*  ���ú�����ʼ  */
//����ģ��ѡ����
void Top_Process(void);
//SUBģ��ѡ����
void Sub0_Process(void);
//SUB1ģ��ѡ����
void Sub1_Process(void);
//SUB2ģ��ѡ����
void Sub2_Process(void);
//SUB3ģ��ѡ����
void Sub3_Process(void);

//SUB_SUBģ��ѡ����
//1.------ɨƵģʽ���������
void StartScanOutput(void);
//4.------����ģʽ���������
void StartModuOutput(void);
//2.------�Ѿ������ģʽ���˳�
void Exit_CURRENT(void);
//3.------ɨƵģʽ�¶�λ���
void SetCursor(u32 x,u32 y);
//5.-----ɨƵģʽ����ʾƵ�ʺ�dBmֵ
void Setvalue_option(void);
//6.Ѱ�����ֵ�ĺ���
int find_max(double *a,int n);
//7.ֻɨ��һ�εĺ���
void Scan_Once(void);
//8.������ת��1
u16 Convert_X(u16 k);
//9.������ת��
u16 Convert_Y(double amp);
//10.ɨ�����εĺ���
void Scan_TWICE_1(void);
void Scan_TWICE_2(void);


//˥����PE4302˥������
void KeyProcess_PE4302(double data);
//ADF4351����Ƶ������
void SetStepFreq(double freq_begin,double freq_step,double freq_end);
//�л�����ģʽ
void SwitchMode(void);
//������Ļ��ʾ������Ƶ��
void SetcenterFreq(double freq_center);
//���ƾ�̬UI�����к���
void DrawStatic(void);
void DrawTop_Sub0(void);
void DrawTop_Sub1(void);
void DrawTop_Sub2(void);
void DrawTop_Sub3(void);
//ɨƵģʽ��������
void Scan_Input_Draw(void);



//ɨƵ����
void Freq_Scan(void);




//����������ĺ���
void DrawAxis(void);
//����Ƶ����������
void DrawFreq(u16 freq,u16 amp);
//�Ѳɼ�����32λ����ת���ɶ�Ӧ���ӵĺ���
double Convert(double value);

/*  ���ú�������  */
#endif