#include "exti.h"
#include "key_it.h"
#include "KeyBoard.h"
#include "led.h"
#include "operation.h"
#include "ADF4351.h"
//�ⲿ�������ڶ�ȡ����ֵ����������
extern double readvalue_keyboard;
//�ⲿ��������ɨƵ����
extern double freq_step;
//�ⲿ��������ɨƵ�Ƿ����
extern int Start_Scan;
extern int finish;
/*  �ⲿ�жϳ�ʼ��    PF6Ϊ�ж�����ӿ�  */

void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	Input_exit_Init();
	Output_exti_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource0);//PF0---- ���ӵ��ж���0
	
	// �����ⲿ�ж��� EXTI_Line0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE; //�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //�ⲿ�ж�0  �պ� PF0�� EXTI0_IRQn ����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����   
}


/*  �ⲿ�жϳ�ʼ������  */

/*  �жϴ������  */
void EXTI0_IRQHandler(void)
{
		switch(CMD_OPTION)
		{
			case PUTSINGLE://��Ƶģʽ������ΪƵ��
				readvalue_keyboard=input_double(60,160);
				break;
			case PUTSCAN://ɨƵģʽ��Ϊѡ������ģʽ
				if(!SCAN_PAR_MODE)
				{
					switch(ReadKeyBoard())
					{
						case 'A':
						freq_step=0.01;
						SCAN_PAR_MODE=1;//����Ϊ��������ģʽ
						Scan_Input_Draw();
						break;
						case 'C':
						Exit_CURRENT();
						freq_step=0.1;
						break;
					}
				}
				else 
					readvalue_keyboard=input_double(60,160);
					break;
			case SETVALUE://��������ģʽ������ֵΪ�������
				readvalue_keyboard=input_double(60,160);
				break;
			case RUNSINGLE:
				switch(ReadKeyBoard())
				{
					case 'A'://��Ƶ����°���A��ʾ��ͣ
					//ADF4351_Disable();
					break;
					case 'B'://��Ƶģʽ�°���B��ʾֹͣ������˳���һ��
					//ADF4351_Disable();
					Exit_CURRENT();
					//freqmid=100;
					break;
					case 'C'://��Ƶģʽ�°���C��ʾ�������
					//ADF4351_Enable();
					ADF4351WriteFreq(freqmid);
					break;
				}
				break;
			case RUNSCAN:
				if(Option_Scan==QUEE)
					switch(ReadKeyBoard())
					{
						case 'A':
							finish=0;
							Exit_CURRENT();
							break;
						case 'B':
							if(finish)
								SetCursor(x_point,y_point);//��λ��꣬Ӧ���޸Ĵ���
							break;
					}
		}	
		EXTI_ClearITPendingBit(EXTI_Line0);
		GPIO_SetBits(GPIOG,GPIO_Pin_0);
}
/*  �жϴ���������  */


/*  �жϽӿڳ�ʼ��  ʹ�ýӿ�ΪPF0*/
void Input_exit_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);    //ʹ��GPIOFʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;             //��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/*  �жϽӿڳ�ʼ������  */

/*  ����Դ����ӿ�  ʹ��PG0  */
void Output_exti_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��

  //PG0��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                  //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;             //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                   //����
  GPIO_Init(GPIOG, &GPIO_InitStructure);                         //��ʼ��GPIO
	GPIO_SetBits(GPIOG,GPIO_Pin_0);
}
/*  ����Դ����ӿڽ���  */