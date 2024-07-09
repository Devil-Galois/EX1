#include "function.h"



#ifdef		SEND
//���ƾ�̬����
void DrawMenu()
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(120,60,24,"��  ��");
	LCD_DisplayHZstr(110,105,16,"A.��������");
	LCD_DisplayHZstr(110,160,16,"B.��������");
	BRUSH_COLOR=BLUE;
	sprintf((char*)display_str,"%d %d %d %d",p_number[0],p_number[1],p_number[2],p_number[3]);
	LCD_DisplayHZstr(70,220,12,"�� ǰ �� ��");
	LCD_DisplayString(180,219,12,display_str);
	BRUSH_COLOR=WHITE;
}


//�����������
void DrawOutput()
{
	u8 display_str[8];
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(110,60,16,"A.��ͣ������");
	sprintf((char*)display_str,"%d %d %d %d",p_number[0],p_number[1],p_number[2],p_number[3]);
	LCD_DisplayHZstr(80,120,12,"�� �� �� ��");
	LCD_DisplayString(180,119,12,display_str);
	BRUSH_COLOR=WHITE;
}

//���������������
void DrawInput()
{
	LCD_Clear(BLACK);
	LCD_DisplayHZstr(100,40,16,"���� # �������");
	LCD_DisplayHZstr(100,70,16,"���� B ����ɾ��");
}


//���ַ�����
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

//����ת��
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


//���ֱ���
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

//������תʮ����
int Binary2Decimal(u8 *p)
{
	return 8*p[0]+4*p[1]+2*p[2]+p[3];
}

//���㳤��16������1�ĸ���
int Counter(u8 *p)
{
	int i=0;
	int count=0;
	for(i=0;i<16;i++)
		if(p[i])
			count++;
	return count;
}
//У����β
int Check(u8 *p)
{
	return p[0]&&p[1]&&p[2]&&(!p[3]);
}


#endif
