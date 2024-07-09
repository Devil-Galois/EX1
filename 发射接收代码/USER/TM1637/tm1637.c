#include "tm1637.h"

void TM1637_Init(void)
{
	// TM1640接口初始化
	GPIO_InitTypeDef GPIO_Initure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_Initure.GPIO_Pin = TM1637_SCLK_PIN;
	GPIO_Initure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Initure.GPIO_OType = GPIO_OType_PP;
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_Initure);

	GPIO_Initure.GPIO_Pin = TM1637_DIN_PIN; 
	GPIO_Initure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Initure.GPIO_OType = GPIO_OType_PP;
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_Initure);
	TM1637SetBrightness(1,2);
	TM1637_Clear();
}

void TM1637SetBrightness(char ch,char brightness)
{
	TM1637Start();
  TM1637WriteByte(0x80+(ch?0x08 : 0x00)+ brightness);
  TM1637ReadResult();
  TM1637Stop();
}

//开启数码管传输
void TM1637Start(void)
{
	GPIO_SetBits(TM1637_PORT,TM1637_SCLK_PIN);
	GPIO_SetBits(TM1637_PORT,TM1637_DIN_PIN);
  delay_us(2);
	GPIO_ResetBits(TM1637_PORT,TM1637_DIN_PIN);
}
//停止数码管传输
void TM1637Stop(void)
{
	GPIO_ResetBits(TM1637_PORT,TM1637_SCLK_PIN);
	delay_us(2);
	GPIO_ResetBits(TM1637_PORT,TM1637_DIN_PIN);
	delay_us(2);
	GPIO_SetBits(TM1637_PORT,TM1637_SCLK_PIN);
	delay_us(2);
	GPIO_SetBits(TM1637_PORT,TM1637_DIN_PIN);
}
//写一个字节
void TM1637WriteByte(unsigned char b)
{
	int i;
	for (i = 0; i < 8; ++i) 
	{
		GPIO_ResetBits(TM1637_PORT,TM1637_SCLK_PIN);
		 if (b & 0x01) 
				GPIO_SetBits(TM1637_PORT,TM1637_DIN_PIN);
			else 
				GPIO_ResetBits(TM1637_PORT,TM1637_DIN_PIN);
			delay_us(3);
			b >>= 1;
			GPIO_SetBits(TM1637_PORT,TM1637_SCLK_PIN);
			delay_us(3);
	}
}
//读取结果
void TM1637ReadResult(void)
{
	GPIO_ResetBits(TM1637_PORT,TM1637_SCLK_PIN);
  delay_us(5);
	GPIO_SetBits(TM1637_PORT,TM1637_SCLK_PIN);
  delay_us(2);
  GPIO_ResetBits(TM1637_PORT,TM1637_SCLK_PIN);
}


//显示字节
void TM1637_Display_Byte(unsigned char addr, unsigned char segments)
{
    TM1637Start();
    TM1637WriteByte(0x40 + addr);
    TM1637ReadResult();
		TM1637Stop();

    TM1637Start();
    TM1637WriteByte(0xC0 + addr);
    TM1637ReadResult();

    TM1637WriteByte(segments);
    TM1637ReadResult();
    TM1637Stop();
}

//显示一位数字
void TM1637ShowOne(unsigned char index, unsigned char digit)
{
    TM1637_Display_Byte(index, segmentMap[digit] |0x00);
}

void TM1637_Display4(u16 data)
{
	TM1637ShowOne(0, (u8)(data / 1000));//千位
	TM1637ShowOne(1, (u8)(data / 100 % 10));//百位
	TM1637ShowOne(2, (u8)(data % 100 / 10));//十位
	TM1637ShowOne(3, (u8)(data % 10));//个位
}

//清屏
void TM1637_Clear(void)
{
	u8 i;
	for (i = 0; i < 4; i++)
	{
			TM1637_Display_Byte(i, 0x00);
	}
}

