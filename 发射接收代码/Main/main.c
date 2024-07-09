#include "main.h"
#include "timer.h"
#include "function.h"

#ifdef		SEND
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "sinx.h"												//���Ҳ�����
/*	�ַ���ʾ���  */
#include "malloc.h" 
#include "tfcard_sdio.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h" 
#include "updatefont.h"  								//����HanZiUse.lib�и����ֿ⺯�� ����Ҽ�ѡ���ͷ�ļ��ɲ鿴��ͷ�ļ�
#include "showhz.h"	     								//����HanZiUse.lib����ʾ���ֺ���
																

#include "KeyBoard.h"										//�����������
#include "math.h"
#include "exti.h"												//�ⲿ�ж����ڽ����������ֵ��ж�
#endif



#ifndef		SEND
#include "tm1637.h"											//�����
#include "si5351.h"											//��������
#include "key_it.h"
#include "ADF4351.h"
#endif




#ifdef		SEND
/* �˵������ */
#define TOP 1
#define SUB1 2
#define SUB2 3
#endif

#ifndef		SEND
#define FREQ_1M	1000000									//����SI5351����Ƶ������   1*FREQ_1M=1M�������
#endif





#ifdef		SEND
u16  wave_sin[]={												//���Ҳ�����
	1237,1286,1335,1383,1430,1476,1521,1564,1607,1648,1687,1725,1761,1795,1828,
	1858,1886,1913,1937,1959,1978,1995,2010,2023,2033,2040,2045,2048,2048,2045,
	2041,2033,2023,2011,1996,1979,1959,1938,1914,1887,1859,1829,1796,1762,1726,
	1688,1649,1608,1566,1522,1477,1431,1385,1337,1288,1239,1189,1139,1088,1038,
	987,937,886,836,787,738,689,642,595,550,506,463,421,381,342,305,270,237,205,
	176,149,123,100,80,61,45,32,20,12,5,2,0,1,5,11,20,31,45,61,79,100,123,148,175,
	204,236,269,304,341,379,420,461,504,548,594,640,688,736,785,835,885,935,985,
	1036,1087,1137,1187,1237
};
u8 NUMBER[4]={0,0,0,0};								//��ż���������λʮ��������
u8 ELEMENT[32]={0};										//32λ��������������

u16 * p=wave_sin;											//ָ�����������ָ��
u8  * p_number=NUMBER;								//ָ����������λ�����������ָ��
u8  * p_element=ELEMENT;							//ָ��������32λ�����������ָ��
double board_number;									//������̶�ȡֵ
int CURRENTPAGE=TOP;									//����˵������ʶ
int CHANGE=0;													//��ʾ�����Ƿ��л�  �л���ˢ�½���
int STOP=0;														//�����Ƿ�ֹͣ			ֹͣ��0
int __index=0;												//����32λ�����������±�
#endif


char segmentMap[] = {									//�����ӳ��0-F
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 
    0x00
};
#ifndef		SEND
int IC2Value;													//���ն˶�ȡ�ߵ�ƽ�����ڼ��ڵļ���ֵ
int index_re;													//���ն˴�Ŷ������������±�
u8 RECEIVE[24]={0};										//���ڴ�Ž��ն˳�8��1����Ķ���������
int RECEIVE_Finish=0;									//������24����Ч�������1��ʾ�������
int RECEIVE_Start=0;									//���յ�8��1���ʾ��ʼ����
int cnt=0;														//������յ���0�ĸ���  ���յ�500��0��Ϩ�������
double freq_current=35.276;
double freq_before=35.7;
#endif



int main(void)
{  
#ifndef		SEND
	int __Receive[5];										//�ѽ��յ���ǰ20������4��һ�����Ϊʮ���ƴ����������
	double temp_freq;
#endif
	
#ifdef		SEND
	int option=0;												//����˵�ѡ��AorB
#endif
	
	
	
	
	/*  ��ʼ������  */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	delay_init();		  															//��ʼ����ʱ����
	
	
	
#ifdef		SEND
	LED_Init();		    															//��ʼ��LED�˿�
	LCD_Init();																			//��ʼ��LCD
	KEY_Init();																			//��ʼ�����ذ���
	Keyboard_GPIO_Config();													//��ʼ�����̶˿�
	TIM5_Init(99,8399);															//��ʱ��5ʱ��84M����Ƶϵ��8400��84M/8400=10K ���Լ���100��Ϊ10ms   ��ÿ10ms����һ�ΰ�����⺯��
	EXTIX_Init();																		//�ⲿ�жϳ�ʼ��
	font_init();																		//��ʼ�������ֿ�
	InitRowColValue();															//��ʼ�������б�
	Wave_Gen();																			//�������Ҳ�
	Init_tim8();																		//��ʼ����ʱ��8����PWM��
#endif


#ifndef		SEND
	I2C2_Init();
	TM1637_Init();																	//��ʼ�������
	Init_tim3();																		//��ʼ����ʱ��3���ڲ������
	ADF4351Init();
	Init_KEY_RECEIVE();
#endif
  /*  ��ʼ������  */
	
	
	
	/* д��Ƶ�� */
#ifndef		SEND
	si5351aSetFrequency(34*FREQ_1M , 0);						//CLK0--34M
	si5351aSetFrequency(35*FREQ_1M , 1);						//CLK1--35M
	si5351aSetFrequency(36*FREQ_1M , 2);						//CLK0--36M
#endif

#ifdef		SEND
	/*  UI����  */
	DrawMenu();  																		//�˵�����
	/*  UI���ƽ���  */
#endif

/*	��Բ�˲���ʱ��Դ  */
#ifdef		SEND
	Tim4_CLK_Init_B7((u16)8400,(u16)100);								//���Ƶ��CLK=84M/(PSC*ARR)    ������װ��Tim4_CLK_Init((u16)PSC,(u16)ARR);---PB7
	Tim2_CLK_Init_A1((u16)8400,(u16)100);								//PA1
#endif
#ifndef		SEND
	Tim4_CLK_Init_B6((u16)8400,(u16)100);			
	Tim2_CLK_Init_A3((u16)8400,(u16)100);
	ADF4351WriteFreq(freq_current);
#endif
	/*  ��ѭ��  */
	while(1){
		
		
#ifdef		SEND
		/*������º���*/
		if(CHANGE)																		//��⵽������ľ�ˢ��
		{
			switch(CURRENTPAGE){
				case TOP:	DrawMenu();break;
				case SUB1:DrawInput();break;
				case SUB2:DrawOutput();break;
				}
			CHANGE=0;
		}
		/* ���㰴����� */
		if(KEYBOARD_16&&CURRENTPAGE==TOP)							
		{
			option=ReadKeyBoard();
			if(option=='A')
			{
				CURRENTPAGE=SUB1;
				CHANGE=1;
			}
			else if(option=='B')												//����������STOP��0   ��ʱ����Coding������32λ�������PWM
			{
				CURRENTPAGE=SUB2;
				CHANGE=1;
				STOP=0;
				Coding();
			}
			else CURRENTPAGE=TOP;
		}
		/* ��������ģʽ */
		if(CURRENTPAGE==SUB1)
		{
			if(KEYBOARD_16)
			{
				board_number=0;
				GPIO_ResetBits(GPIOG,GPIO_Pin_0);					//�����ⲿ�жϴ�������������
			}
			if(ENSURE_INPUT)														//�����������
			{
				Digital_Convert((int)board_number);
				CURRENTPAGE=TOP;
				CHANGE=1;
				ENSURE_INPUT=0;
			}
		}
		/* �������ģʽ */
		if(KEYBOARD_16&&CURRENTPAGE==SUB2)
		{
			option=ReadKeyBoard();											//�����������ʱ����A   STOP��λ1    ��ʱ���ȫ0
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
		/* ���� */
		if(RECEIVE_Finish)																				//���յ�ȫ��24λ��Чλ��
		{
			__Receive[0]=Binary2Decimal(RECEIVE);										//������תʮ����
			__Receive[1]=Binary2Decimal(RECEIVE+4);
			__Receive[2]=Binary2Decimal(RECEIVE+8);
			__Receive[3]=Binary2Decimal(RECEIVE+12);
			__Receive[4]=Binary2Decimal(RECEIVE+16);
			if(Counter(RECEIVE)==__Receive[4]&&Check(RECEIVE+20))		//У��ɹ�������ʾ�����
			{
				TM1637ShowOne(0,__Receive[0]);
				TM1637ShowOne(1,__Receive[1]);
				TM1637ShowOne(2,__Receive[2]);
				TM1637ShowOne(3,__Receive[3]);
			}
			else 																										//У��ʧ��START����Ϊ0   ���²ɼ�
			{
				RECEIVE_Start=0;
				index_re=0;
			}
		}

		if(cnt>500)																								//������⵽500��0��ʾ��ʱ�Ѿ�ֹͣ����5s    �����Ϩ��
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
	/*  ��ѭ������  */
}
