#include "sys.h"
#include "key_it.h"
#include "KeyBoard.h"
//KEYBOARD_16,KEY0PRESS,KEY1PRESS,KEY2PRESS,KEY3PRESS��Ҫ�ֶ���0
/*  KEY������ʼ������  */
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);    //ʹ��GPIOFʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //KEY0 KEY1 KEY2 KEY3��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;             //��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
  GPIO_Init(GPIOF, &GPIO_InitStructure);                   //��ʼ��GPIOF6,7,8,9
}
void	Init_KEY_RECEIVE()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);    //ʹ��GPIOFʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;             //��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/*  ������ʼ������  */


/*  ����״̬��  */
//������Ҫ��ȫ�ֱ���
int CNT_KEYBOARD=0;
int STATE_KEYBOARD=WAIT;
int KEYBOARD_16=0;   //Ϊ1ʱ��ʾ�м�����
//�������Ƿ���
void KEYBOARDPRESS(void)
{
	switch(STATE_KEYBOARD){
		case WAIT: 
				CNT_KEYBOARD=0;
				if(KEYBOARDROW==ON)
					STATE_KEYBOARD=DOWN;
				break;
		case DOWN:
				if(CNT_KEYBOARD==5)
				{
					CNT_KEYBOARD=0;
					STATE_KEYBOARD=CONFIRMDOWN;
				}
				else if(CNT_KEYBOARD<5&&KEYBOARDROW==ON)
					CNT_KEYBOARD++;
				else 
					STATE_KEYBOARD=WAIT;
				break;
		case CONFIRMDOWN:
				KEYBOARD_16=1;								//��ʾĳһ���м�������
				STATE_KEYBOARD=WAITUP;
				break;
		case WAITUP:
				CNT_KEYBOARD=0;
				if(KEYBOARDROW==OFF)
				STATE_KEYBOARD=CONFIRMUP;
				break;
		case CONFIRMUP:
				if(CNT_KEYBOARD==5)
					STATE_KEYBOARD=WAIT;
				else if(KEYBOARDROW==ON)
					STATE_KEYBOARD=WAITUP;
				else CNT_KEYBOARD++;
				break;
		default: STATE_KEYBOARD=WAIT;
				break;
	}
}
		

/*  ����״̬������  */

/*  ����0״̬����ʼ  */
//��Ҫ��ȫ�ֱ���
int CNT_KEY0=0;
int STATE_KEY0=WAIT;
int KEY0PRESS=0;   //Ϊ1ʱ��ʾ�м�����
void KEY_0_PRESS(void)
{
		switch(STATE_KEY0){
		case WAIT: 
				CNT_KEY0=0;
				if(KEY0==ON)
					STATE_KEY0=DOWN;
				break;
		case DOWN:
				if(CNT_KEY0==5)
				{
					CNT_KEY0=0;
					STATE_KEY0=CONFIRMDOWN;
				}
				else if(CNT_KEY0<5&&KEY0==ON)
					CNT_KEY0++;
				else 
					STATE_KEY0=WAIT;
				break;
		case CONFIRMDOWN:
				KEY0PRESS=1;								//��ʾĳһ���м�������
				STATE_KEY0=WAITUP;
				break;
		case WAITUP:
				CNT_KEY0=0;
				if(KEY0==OFF)
				STATE_KEY0=CONFIRMUP;
				break;
		case CONFIRMUP:
				if(CNT_KEY0==5)
					STATE_KEY0=WAIT;
				else if(KEY0==ON)
					STATE_KEY0=WAITUP;
				else CNT_KEY0++;
				break;
		default: STATE_KEY0=WAIT;
				break;
	}
}

/*  ����0״̬������  */


/*  ����1״̬����ʼ  */
//��Ҫ��ȫ�ֱ���
int CNT_KEY1=0;
int STATE_KEY1=WAIT;
int KEY1PRESS=0;   //Ϊ1ʱ��ʾ�м�����
void KEY_1_PRESS(void)
{
		switch(STATE_KEY1){
		case WAIT: 
				CNT_KEY1=0;
				if(KEY1==ON)
					STATE_KEY1=DOWN;
				break;
		case DOWN:
				if(CNT_KEY1==5)
				{
					CNT_KEY1=0;
					STATE_KEY1=CONFIRMDOWN;
				}
				else if(CNT_KEY1<5&&KEY1==ON)
					CNT_KEY1++;
				else 
					STATE_KEY1=WAIT;
				break;
		case CONFIRMDOWN:
				KEY1PRESS=1;								//��ʾĳһ���м�������
				STATE_KEY1=WAITUP;
				break;
		case WAITUP:
				CNT_KEY1=0;
				if(KEY1==OFF)
				STATE_KEY1=CONFIRMUP;
				break;
		case CONFIRMUP:
				if(CNT_KEY1==5)
					STATE_KEY1=WAIT;
				else if(KEY1==ON)
					STATE_KEY1=WAITUP;
				else CNT_KEY1++;
				break;
		default: STATE_KEY1=WAIT;
				break;
	}
}

/*  ����1״̬������  */


/*  ����2״̬����ʼ  */
//��Ҫ��ȫ�ֱ���
int CNT_KEY2=0;
int STATE_KEY2=WAIT;
int KEY2PRESS=0;   //Ϊ1ʱ��ʾ�м�����
void KEY_2_PRESS(void)
{
	switch(STATE_KEY2){
		case WAIT: 
				CNT_KEY2=0;
				if(KEY2==ON)
					STATE_KEY2=DOWN;
				break;
		case DOWN:
				if(CNT_KEY2==5)
				{
					CNT_KEY2=0;
					STATE_KEY2=CONFIRMDOWN;
				}
				else if(CNT_KEY2<5&&KEY2==ON)
					CNT_KEY2++;
				else 
					STATE_KEY2=WAIT;
				break;
		case CONFIRMDOWN:
				KEY2PRESS=1;								//��ʾĳһ���м�������
				STATE_KEY2=WAITUP;
				break;
		case WAITUP:
				CNT_KEY2=0;
				if(KEY2==OFF)
				STATE_KEY2=CONFIRMUP;
				break;
		case CONFIRMUP:
				if(CNT_KEY2==5)
					STATE_KEY2=WAIT;
				else if(KEY2==ON)
					STATE_KEY2=WAITUP;
				else CNT_KEY2++;
				break;
		default: STATE_KEY2=WAIT;
				break;
	}
}


/*  ����2״̬������  */




/*  ����3״̬����ʼ  */
//��Ҫ��ȫ�ֱ���
int CNT_KEY3=0;
int STATE_KEY3=WAIT;
int KEY3PRESS=0;   //Ϊ1ʱ��ʾ�м�����
void KEY_3_PRESS(void)
{
	switch(STATE_KEY3){
		case WAIT: 
				CNT_KEY3=0;
				if(KEY3==ON)
					STATE_KEY3=DOWN;
				break;
		case DOWN:
				if(CNT_KEY3==5)
				{
					CNT_KEY3=0;
					STATE_KEY3=CONFIRMDOWN;
				}
				else if(CNT_KEY3<5&&KEY3==ON)
					CNT_KEY3++;
				else 
					STATE_KEY3=WAIT;
				break;
		case CONFIRMDOWN:
				KEY3PRESS=1;								//��ʾĳһ���м�������
				STATE_KEY3=WAITUP;
				break;
		case WAITUP:
				CNT_KEY3=0;
				if(KEY3==OFF)
				STATE_KEY3=CONFIRMUP;
				break;
		case CONFIRMUP:
				if(CNT_KEY3==5)
					STATE_KEY3=WAIT;
				else if(KEY3==ON)
					STATE_KEY3=WAITUP;
				else CNT_KEY3++;
				break;
		default: STATE_KEY3=WAIT;
				break;
	}
}


/*  ����3״̬������  */

/*  ���ջ��������� */
int KEY_RPRESS=0;
int CNT_RECEIVE=0;
int STATE_RECEIVE=WAIT;
void KEY_RECEIVE_PRESS()
{
	switch(STATE_RECEIVE){
		case WAIT: 
				CNT_RECEIVE=0;
				if(KEY_RECEIVE==ON)
					STATE_RECEIVE=DOWN;
				break;
		case DOWN:
				if(CNT_RECEIVE==5)
				{
					CNT_RECEIVE=0;
					STATE_RECEIVE=CONFIRMDOWN;
				}
				else if(CNT_RECEIVE<5&&KEY_RECEIVE==ON)
					CNT_RECEIVE++;
				else 
					STATE_RECEIVE=WAIT;
				break;
		case CONFIRMDOWN:
				KEY_RPRESS=1;								//��ʾĳһ���м�������
				STATE_RECEIVE=WAITUP;
				break;
		case WAITUP:
				CNT_RECEIVE=0;
				if(KEY_RECEIVE==OFF)
				STATE_RECEIVE=CONFIRMUP;
				break;
		case CONFIRMUP:
				if(CNT_RECEIVE==5)
					STATE_RECEIVE=WAIT;
				else if(KEY_RECEIVE==ON)
					STATE_RECEIVE=WAITUP;
				else CNT_RECEIVE++;
				break;
		default: STATE_RECEIVE=WAIT;
				break;
	}
}
