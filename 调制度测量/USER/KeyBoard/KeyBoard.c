#include "KeyBoard.h"
#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "operation.h"
#include "key_it.h"
#include "led.h"
#define TFTFontSize 16			//字体大小
KeyBoradCode KBCode;
int ENSURE_INPUT=0;
int RNSURE_INPUT_SINGLE=0;
/*  开启GIPO  */
void Keyboard_GPIO_Config(void)
{
		int i=0;
		//开启时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
		ROW_H_COL_L();
}
/*  开启GPIO结束  */

/*  初始化键盘码  */
void InitRowColValue(void)
{
    KBCode.Col0Val=0x70;
    KBCode.Col1Val=0xb0;
    KBCode.Col2Val=0xd0;
    KBCode.Col3Val=0xe0;
    KBCode.Row0Val=0x07;
    KBCode.Row1Val=0x0b;
    KBCode.Row2Val=0x0d;
    KBCode.Row3Val=0x0e;
}
/*  初始化键盘码结束  */



/*  键盘扫描  */
char ReadKeyBoard(void)
{
	char ch_keyboard=0;
  int Code=0;
	if(KEYBOARD_16)
	{
		Code =TestRow();													//检测所在行
		ROW_L_COL_H();														//列输入上拉，行输出下拉
		delay_us(100);                          	//延时后检测所在列
		Code|=TestCol();                       		//行和列的结果按位或
		switch(Code)
		{
		case 126:
			LED0=!LED0;
			ch_keyboard='*';
			break;
		case 190:
			ch_keyboard='0';
			break;
		case 222:
			ch_keyboard='#';
			break;
		case 238:
			ch_keyboard='D';
			break;
		case 125:
			ch_keyboard='7';
			break;
		case 189:
			ch_keyboard='8';
			break;
		case 221:
			ch_keyboard='9';
			break;
		case 237:
			ch_keyboard='C';
			break;
		case 123:
			ch_keyboard='4';
			break;
		case 187:
			ch_keyboard='5';
			break;
		case 219:
			ch_keyboard='6';
			break;
		case 235:
			ch_keyboard='B';
			break;
		case 119:  
			ch_keyboard='1';
			break;
		case 183:
			ch_keyboard='2';
			break;
		case 215:
			ch_keyboard='3';
			break;
		case 231:
			ch_keyboard='A';
			break;
		default:
			break;
		}
		KEYBOARD_16=0;
	}
  ROW_H_COL_L();
	return ch_keyboard;
}
/*  键盘扫描结束  */



/*  字符串转浮点数  */
double Char_2_Double(char *CHAR_temp)
{
  double temp_double=0;
  long long int temp_=0;
  int i=0,j=0;
  for(j=0,temp_=0; j<17&&CHAR_temp[j]!='\0'; j=j+1)
  {
    if(CHAR_temp[j]!='.')
    {
      temp_=temp_*10+(CHAR_temp[j]-48);
    }
  }
  temp_double=temp_;
  i=0;
  while(CHAR_temp[i])
  {
    if(CHAR_temp[i]=='.')
    {
      i++;
      while(CHAR_temp[i])
      {
        temp_double=temp_double/10;
        i++;
      }
      break;
    }
    i++;
  }
  return temp_double;
}

/*  字符串转浮点数结束  */



/*  行输出下拉，列输入上拉  */
void ROW_L_COL_H(void)
{
	int i=0;
  GPIO_InitTypeDef ROW_GPIO[4];
  GPIO_InitTypeDef COL_GPIO[4];
  for(i=0; i<4; i++)
  {
    COL_GPIO[i].GPIO_Mode=GPIO_Mode_IN;
    COL_GPIO[i].GPIO_Speed=GPIO_Speed_100MHz;
    COL_GPIO[i].GPIO_PuPd=GPIO_PuPd_UP;
  }
  COL_GPIO[0].GPIO_Pin=COL0_GPIO_BIT;
  COL_GPIO[1].GPIO_Pin=COL1_GPIO_BIT;
  COL_GPIO[2].GPIO_Pin=COL2_GPIO_BIT;
  COL_GPIO[3].GPIO_Pin=COL3_GPIO_BIT;
  GPIO_Init(COL0_GPIO, &COL_GPIO[0]);
  GPIO_Init(COL1_GPIO, &COL_GPIO[1]);
  GPIO_Init(COL2_GPIO, &COL_GPIO[2]);
  GPIO_Init(COL3_GPIO, &COL_GPIO[3]);
  for(i=0; i<4; i++)
  {
    ROW_GPIO[i].GPIO_Mode=GPIO_Mode_OUT;
    ROW_GPIO[i].GPIO_Speed=GPIO_Speed_100MHz;
    ROW_GPIO[i].GPIO_OType = GPIO_OType_PP;
    ROW_GPIO[i].GPIO_PuPd=GPIO_PuPd_DOWN;
  }
  ROW_GPIO[0].GPIO_Pin=ROW0_GPIO_BIT;
  ROW_GPIO[1].GPIO_Pin=ROW1_GPIO_BIT;
  ROW_GPIO[2].GPIO_Pin=ROW2_GPIO_BIT;
  ROW_GPIO[3].GPIO_Pin=ROW3_GPIO_BIT;
  GPIO_Init(ROW0_GPIO, &ROW_GPIO[0]);
  GPIO_Init(ROW1_GPIO, &ROW_GPIO[1]);
  GPIO_Init(ROW2_GPIO, &ROW_GPIO[2]);
  GPIO_Init(ROW3_GPIO, &ROW_GPIO[3]);
  ROW_L;
}
/*  行输出下拉，列输入上拉结束  */

/*  列输出下拉，行输入上拉  */
void ROW_H_COL_L(void)
{
		int i=0;
		GPIO_InitTypeDef ROW_GPIO[4];
		GPIO_InitTypeDef COL_GPIO[4];
		//行输入上拉初始化
		for(i=0; i<4; i++)
		{
			ROW_GPIO[i].GPIO_Mode=GPIO_Mode_IN;
			ROW_GPIO[i].GPIO_Speed=GPIO_Speed_100MHz;
			ROW_GPIO[i].GPIO_PuPd=GPIO_PuPd_UP;
		}
		ROW_GPIO[0].GPIO_Pin=ROW0_GPIO_BIT;
		ROW_GPIO[1].GPIO_Pin=ROW1_GPIO_BIT;
		ROW_GPIO[2].GPIO_Pin=ROW2_GPIO_BIT;
		ROW_GPIO[3].GPIO_Pin=ROW3_GPIO_BIT;
		GPIO_Init(ROW0_GPIO, &ROW_GPIO[0]);
		GPIO_Init(ROW1_GPIO, &ROW_GPIO[1]);
		GPIO_Init(ROW2_GPIO, &ROW_GPIO[2]);
		GPIO_Init(ROW3_GPIO, &ROW_GPIO[3]);
		//列输出下拉初始化
		for(i=0; i<4; i++)
		{
			COL_GPIO[i].GPIO_Mode=GPIO_Mode_OUT;
			COL_GPIO[i].GPIO_OType = GPIO_OType_PP;
			COL_GPIO[i].GPIO_Speed=GPIO_Speed_100MHz;
			COL_GPIO[i].GPIO_PuPd=GPIO_PuPd_DOWN;
		}
		COL_GPIO[0].GPIO_Pin=COL0_GPIO_BIT;
		COL_GPIO[1].GPIO_Pin=COL1_GPIO_BIT;
		COL_GPIO[2].GPIO_Pin=COL2_GPIO_BIT;
		COL_GPIO[3].GPIO_Pin=COL3_GPIO_BIT;
		GPIO_Init(COL0_GPIO, &COL_GPIO[0]);
		GPIO_Init(COL1_GPIO, &COL_GPIO[1]);
		GPIO_Init(COL2_GPIO, &COL_GPIO[2]);
		GPIO_Init(COL3_GPIO, &COL_GPIO[3]);
		COL_L;
}
/*  列输出下拉，行输入上拉结束  */

/*  行输入、列输出的检测函数 */
int TestRow(void)
{
  int nTemp=0;
  if(ROW0==0)
    nTemp=KBCode.Row0Val;		//0x70;
  else if(ROW1==0)
    nTemp=KBCode.Row1Val;		//0xb0;
  else if(ROW2==0)
    nTemp=KBCode.Row2Val;		//0xd0;
  else if(ROW3==0)
    nTemp=KBCode.Row3Val;		//0xe0;
  else
    nTemp=0x0f;		//0xf0;
  return nTemp;
}

int TestCol(void)
{
  int nTemp=0;
  if(COL0==0)
    nTemp=KBCode.Col0Val;		//0x07;
  else if(COL1==0)
    nTemp=KBCode.Col1Val;		//0x0b;
  else if(COL2==0)
    nTemp=KBCode.Col2Val;		//0x0d;
  else if(COL3==0)
    nTemp=KBCode.Col3Val;		//0x0e;
  else
    nTemp=0x00;
  return nTemp;
}

/*  行输入、列输出的检测函数结束 */

/*  浮点数据输入函数----只进入一次中断，一次中断输入所有位并确认  */
double input_double_single(u16 x,u16 y)
{
  double value=0;
  char ch=0;
  static int i;
  static char CHAR_temp[8];
	RNSURE_INPUT_SINGLE=0;
	//清空输入数据
  LCD_DisplayString(x,y,TFTFontSize,(u8*)"        ");	
  while(1)
  {
    ch=ReadKeyBoard();
    if('0'<=ch&&ch<='9'&&i<7)
    {
      CHAR_temp[i]=ch;
      CHAR_temp[i+1]='\0';
      LCD_DisplayString(x,y,TFTFontSize,(u8*)CHAR_temp);
      i=i+1;
    }
    else if(ch=='#')//按下enter键---#
    {
			RNSURE_INPUT_SINGLE=1;
      break;
    }
    else if(ch=='B')//按退格键---B
    {
      if(i>0)
      {
        i=i-1;
        CHAR_temp[i]='\0';
        LCD_DisplayString(x,y,TFTFontSize,(u8*)"        ");	
       	LCD_DisplayString(x,y,TFTFontSize,(u8*)CHAR_temp);
      }
    }
    else if(ch=='*')//按下‘*’（小数点）键
    {
      CHAR_temp[i]='.';
      CHAR_temp[i+1]='\0';
      LCD_DisplayString(x,y,TFTFontSize,(u8*)CHAR_temp);
      i=i+1;
    }
    else if(i<7)//其他都输入0
    {
			CHAR_temp[i]='0';
			CHAR_temp[i+1]='\0';
			LCD_DisplayString(x,y,TFTFontSize,(u8*)"        ");	
			LCD_DisplayString(x,y,TFTFontSize,(u8*)CHAR_temp);
			i++;
		}
	}
	value=Char_2_Double(CHAR_temp);
	i=0;
	return value;
}
/*  输入函数结束  */


/*  输入浮点数据的函数----多次进入中断每次只写一个位  */

double input_double(u16 x,u16 y)
{
  double value;
  char ch;
  static int i;
  static char CHAR_temp[8];
  ENSURE_INPUT = 0;
  value=0;
	ch=0;
	//读取键盘值
  ch=ReadKeyBoard();
	
	//如果键盘值是数字
  if('0'<=ch&&ch<='9'&&i<7)
  {
    CHAR_temp[i]=ch;
    CHAR_temp[i+1]='\0';
    LCD_DisplayString(x,y,TFTFontSize,(u8*)"        ");	
    LCD_DisplayString(x,y,TFTFontSize,(u8*)CHAR_temp);
    i=i+1;
  }
	//按下enter键也就是'#'
  else if(ch=='#')
  {
    ENSURE_INPUT = 1;
  }
	//按下B键也就是'backspace'
  else if(ch=='B')
  {
    if(i>0)
    {
      i--;
      CHAR_temp[i]='\0';
      LCD_DisplayString(x,y,TFTFontSize,(u8*)"        ");	
      LCD_DisplayString(x,y,TFTFontSize,(u8*)CHAR_temp);
    }
  }
	//按下‘*'键也就是'.'
  else if(ch=='*'&&i<7)
  {
    CHAR_temp[i]='.';
    CHAR_temp[i+1]='\0';
    LCD_DisplayString(x,y,TFTFontSize,(u8*)"        ");	
    LCD_DisplayString(x,y,TFTFontSize,(u8*)CHAR_temp);
    i++;
  }
	//其他情况都输入0
	else if(i<7)
	{
    CHAR_temp[i]='0';
    CHAR_temp[i+1]='\0';
    LCD_DisplayString(x,y,TFTFontSize,(u8*)"        ");	
    LCD_DisplayString(x,y,TFTFontSize,(u8*)CHAR_temp);
    i++;
  }
	//确认输入完成转换数组为double数据并把光标定位到字符串开头
	//否则只进行转换而不重置光标
  if(ENSURE_INPUT==1)
  {
    i=0;
    value=Char_2_Double(CHAR_temp);
		LCD_Clear(BLACK);
    return value;
  }
  else
  {
    value=Char_2_Double(CHAR_temp);
    return value;
  }
}
/*  输入浮点数据结束  */
