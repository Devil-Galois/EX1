#include "exti.h"
#include "key_it.h"
#include "KeyBoard.h"
#include "led.h"
#include "operation.h"

/*  �ⲿ�жϳ�ʼ��    PF0Ϊ�ж�����ӿ�  */

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