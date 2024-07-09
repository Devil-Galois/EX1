#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "sys.h"
#include "key_it.h"
#include "delay.h"
//键盘引脚：
//1-----PD6-----COL3
//2-----PD7-----COL2
//3-----PC6----COL1
//4-----PC8-----COL0
//5-----PC11-----ROW3
//6-----PE5-----ROW2
//7-----PA6-----ROW1
//8-----PD3-----ROW0
#define 	COL0_GPIO	GPIOC
#define		COL1_GPIO	GPIOC
#define 	COL2_GPIO	GPIOD
#define		COL3_GPIO	GPIOD

#define 	ROW0_GPIO	GPIOD
#define 	ROW1_GPIO	GPIOA
#define 	ROW2_GPIO	GPIOE
#define 	ROW3_GPIO	GPIOC

#define 	COL0_GPIO_BIT	GPIO_Pin_8
#define		COL1_GPIO_BIT	GPIO_Pin_6
#define 	COL2_GPIO_BIT	GPIO_Pin_7
#define		COL3_GPIO_BIT	GPIO_Pin_6

#define 	ROW0_GPIO_BIT	GPIO_Pin_3
#define 	ROW1_GPIO_BIT	GPIO_Pin_6
#define 	ROW2_GPIO_BIT	GPIO_Pin_5
#define 	ROW3_GPIO_BIT	GPIO_Pin_11

#define	RCC_COL0_GPIO	RCC_AHB1Periph_GPIOC
#define	RCC_COL1_GPIO	RCC_AHB1Periph_GPIOC
#define	RCC_COL2_GPIO	RCC_AHB1Periph_GPIOD
#define	RCC_COL3_GPIO	RCC_AHB1Periph_GPIOD

#define RCC_ROW0_GPIO	RCC_AHB1Periph_GPIOD
#define RCC_ROW1_GPIO	RCC_AHB1Periph_GPIOA
#define RCC_ROW2_GPIO	RCC_AHB1Periph_GPIOE
#define RCC_ROW3_GPIO	RCC_AHB1Periph_GPIOC

#define ROW_H	{ROW0_H;ROW1_H;ROW2_H;ROW3_H;}
#define ROW_L	{ROW0_L;ROW1_L;ROW2_L;ROW3_L;}

#define COL_H	{COL0_H;COL1_H;COL2_H;COL3_H;}
#define COL_L	{COL0_L;COL1_L;COL2_L;COL3_L;}

#define ROW0_H 	GPIO_SetBits(ROW0_GPIO,ROW0_GPIO_BIT)
#define ROW1_H 	GPIO_SetBits(ROW1_GPIO,ROW1_GPIO_BIT)
#define ROW2_H 	GPIO_SetBits(ROW2_GPIO,ROW2_GPIO_BIT)
#define ROW3_H 	GPIO_SetBits(ROW3_GPIO,ROW3_GPIO_BIT)

#define ROW0_L 	GPIO_ResetBits(ROW0_GPIO,ROW0_GPIO_BIT)
#define ROW1_L 	GPIO_ResetBits(ROW1_GPIO,ROW1_GPIO_BIT)
#define ROW2_L 	GPIO_ResetBits(ROW2_GPIO,ROW2_GPIO_BIT)
#define ROW3_L 	GPIO_ResetBits(ROW3_GPIO,ROW3_GPIO_BIT)

#define COL0_H	GPIO_SetBits(COL0_GPIO,COL0_GPIO_BIT)
#define COL1_H	GPIO_SetBits(COL1_GPIO,COL1_GPIO_BIT)
#define COL2_H	GPIO_SetBits(COL2_GPIO,COL2_GPIO_BIT)
#define COL3_H	GPIO_SetBits(COL3_GPIO,COL3_GPIO_BIT)

#define COL0_L	GPIO_ResetBits(COL0_GPIO,COL0_GPIO_BIT)
#define COL1_L	GPIO_ResetBits(COL1_GPIO,COL1_GPIO_BIT)
#define COL2_L	GPIO_ResetBits(COL2_GPIO,COL2_GPIO_BIT)
#define COL3_L	GPIO_ResetBits(COL3_GPIO,COL3_GPIO_BIT)

#define ROW0	GPIO_ReadInputDataBit(ROW0_GPIO,ROW0_GPIO_BIT) 
#define ROW1	GPIO_ReadInputDataBit(ROW1_GPIO,ROW1_GPIO_BIT) 
#define ROW2	GPIO_ReadInputDataBit(ROW2_GPIO,ROW2_GPIO_BIT)
#define ROW3	GPIO_ReadInputDataBit(ROW3_GPIO,ROW3_GPIO_BIT) 

#define COL0	GPIO_ReadInputDataBit(COL0_GPIO,COL0_GPIO_BIT)
#define COL1	GPIO_ReadInputDataBit(COL1_GPIO,COL1_GPIO_BIT)
#define COL2	GPIO_ReadInputDataBit(COL2_GPIO,COL2_GPIO_BIT)
#define COL3	GPIO_ReadInputDataBit(COL3_GPIO,COL3_GPIO_BIT)

typedef struct
{
	u8 Row0Val;
	u8 Row1Val;
	u8 Row2Val;
	u8 Row3Val;
	u8 Col0Val;
	u8 Col1Val;
	u8 Col2Val;
	u8 Col3Val;
}KeyBoradCode;
  
extern KeyBoradCode KBCode;
extern int ENSURE_INPUT;
//字符串转浮点型
double Char_2_Double(char *CHAR_temp);

//键盘扫描函数
char ReadKeyBoard(void);
//获取键盘输入值并动态显示函数
double input_double(u16 x,u16 y);
double input_double_single(u16 x,u16 y);
//获取键盘行列信息
int TestRow(void);
int TestCol(void);
//初始化键盘接口
void Keyboard_GPIO_Config(void);
//行输入上拉，列输出下拉
void ROW_H_COL_L(void);
void ROW_L_COL_H(void);
//初始化键盘值
void InitRowColValue();

#endif

