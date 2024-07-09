#include "si5351.h"
// 初始化IIC的IO口
void I2C2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;   // 定义GPIO结构体
	
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // 打开GPIOB口时钟
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; // 开漏
    GPIO_InitStructure.GPIO_Pin = Pin_SCL | Pin_SDA ; // IIC对应IO口
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed ; // 50MHZ
    GPIO_Init(GPIOF, &GPIO_InitStructure); // 初始化GPIO
	
    I2C2_Stop();
}
 
// 发送IIC起始信号
bool I2C2_Start(void)
{
    Pin_SCL_H; // 拉高时钟线
    Pin_SDA_H; // 拉高信号线
    delay_us(1);
    if(!Read_SDA_Pin)		return false;
    Pin_SDA_L;
    delay_us(1);
    Pin_SDA_L;
    delay_us(1);
    return true;
}
 
// 发送IIC停止信号
bool I2C2_Stop(void)
{
    Pin_SCL_H;
    Pin_SDA_L;
    delay_us(1);
    if(Read_SDA_Pin)	return false;
    Pin_SDA_H;
    delay_us(1);
    if(!Read_SDA_Pin) return false;
    Pin_SDA_H;
    delay_us(1);	
    return true;
}
 
// IIC发送ACK信号
void I2C2_Ack(void)
{
    Pin_SCL_L;
    delay_us(1);
    Pin_SDA_L;	
    Pin_SCL_H;
    delay_us(1);
    Pin_SCL_L;
    Pin_SDA_H;
    delay_us(1);
}
 
// IIC不发送ACK信号
void I2C2_NAck(void)
{
    Pin_SCL_L;
    delay_us(1);	
    Pin_SDA_H;
    Pin_SCL_H;
    delay_us(1);
    Pin_SCL_L;
    delay_us(1);
}
 
// IIC等待ACK信号
uint8_t I2C2_Wait_Ack(void)
{
    Pin_SCL_L;
    delay_us(1);	
    Pin_SDA_H;
    Pin_SCL_H;
    delay_us(1);	
    if(Read_SDA_Pin)
    {
	Pin_SCL_L;
	delay_us(1);
	return false;
    }
    Pin_SCL_L;
    delay_us(1);
    return true;
}
 
// IIC发送一个字节
void I2C2_Send_Byte(uint8_t txd)
{
	int i;
    for(i=0; i<8; i++)
    {
	Pin_SCL_L;
	delay_us(1);
	if(txd & 0x80)
	    Pin_SDA_H;
	else
	    Pin_SDA_L;
	    txd <<= 1;
	    Pin_SCL_H;
	    delay_us(1);
    }
}
 
// IIC读取一个字节
uint8_t	I2C2_Read_Byte(void)
{
	int i;
    uint8_t rxd = 0;
    for(i=0; i<8; i++)
    {
	rxd <<= 1;
	Pin_SCL_L;
	delay_us(1);
	Pin_SCL_H;	
	delay_us(1);		
	if(Read_SDA_Pin)
	{
	    rxd |= 0x01;
	}
    }
    return rxd;
}
 
// 向从机指定地址写数据
bool I2C_Write_REG(uint8_t SlaveAddress, uint8_t REG_Address,uint8_t REG_data)
{
    if(!I2C2_Start())		return false;
    I2C2_Send_Byte(SlaveAddress);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    I2C2_Send_Byte(REG_Address);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    I2C2_Send_Byte(REG_data);
    if(!I2C2_Wait_Ack()) { I2C2_Stop(); return false;	}
    if(!I2C2_Stop()) return false;
    return true;
}
 
// 从设备中读取数据
uint8_t I2C2_Read_REG(uint8_t SlaveAddress,uint8_t REG_Address)
{
    uint8_t data;
    if(!I2C2_Start())	return false;
    I2C2_Send_Byte(SlaveAddress);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    I2C2_Send_Byte(REG_Address);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    if(!I2C2_Start())	return false;
    I2C2_Send_Byte(SlaveAddress + 1);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    data = I2C2_Read_Byte();
    I2C2_NAck();
    if(!I2C2_Stop())	return false;	
    return data;
}
 
// 连续写N个字节
bool I2C2_Write_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t* buf, uint8_t len)
{
	int i;
	if(!I2C2_Start())return false;
	I2C2_Send_Byte(SlaveAddress);  //发送设备地址+写信号
	if(!I2C2_Wait_Ack()){I2C2_Stop(); return false;}
	I2C2_Send_Byte(REG_Address);   
	if(!I2C2_Wait_Ack()){I2C2_Stop(); return false;}
	for(i=0; i<len; i++)
	{
		I2C2_Send_Byte(buf[i]);
		if(i<len-1)
		{
			if(!I2C2_Wait_Ack()){I2C2_Stop(); return false;}
		}
	}
	I2C2_Stop();
	return true;
}

bool I2C2_CheckDevice(uint8_t SlaveAddress)
{
    if(!I2C2_Start())	return false;
    I2C2_Send_Byte(SlaveAddress);
    if(!I2C2_Wait_Ack())
    {
	I2C2_Stop();
	return false;		
    }
    if(!I2C2_Stop())	return false;	
    return true;	
}


bool I2C_sendREG(uint8_t REG_Address,uint8_t REG_data)
{
    if(!I2C2_Start())		return false;
    I2C2_Send_Byte(0xC0);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    I2C2_Send_Byte(REG_Address);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    I2C2_Send_Byte(REG_data);
    if(!I2C2_Wait_Ack()) { I2C2_Stop(); return false;	}
    if(!I2C2_Stop()) return false;
    return true;
}

uint8_t my_I2C2_Read_REG(uint8_t REG_Address)
{
    uint8_t data;
    if(!I2C2_Start())	return false;
    I2C2_Send_Byte(0xC0);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    I2C2_Send_Byte(REG_Address);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    if(!I2C2_Start())	return false;
    I2C2_Send_Byte(0xC1);
    if(!I2C2_Wait_Ack()) { I2C2_Stop();	return false;	}
    data = I2C2_Read_Byte();
    I2C2_NAck();
    if(!I2C2_Stop())	return false;	
    return data;
}

void setupPLL(uint8_t pll, uint8_t mult, uint32_t num, uint32_t denom)
{
  uint32_t P1;					// PLL config register P1
  uint32_t P2;					// PLL config register P2
  uint32_t P3;					// PLL config register P3

  P1 = (uint32_t)(128 * ((float)num / (float)denom));
  P1 = (uint32_t)(128 * (uint32_t)(mult) + P1 - 512);
  P2 = (uint32_t)(128 * ((float)num / (float)denom));
  P2 = (uint32_t)(128 * num - denom * P2);
  P3 = denom;

  I2C_sendREG(pll + 0, (P3 & 0x0000FF00) >> 8);
  I2C_sendREG(pll + 1, (P3 & 0x000000FF));
  I2C_sendREG(pll + 2, (P1 & 0x00030000) >> 16);
  I2C_sendREG(pll + 3, (P1 & 0x0000FF00) >> 8);
  I2C_sendREG(pll + 4, (P1 & 0x000000FF));
  I2C_sendREG(pll + 5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
  I2C_sendREG(pll + 6, (P2 & 0x0000FF00) >> 8);
  I2C_sendREG(pll + 7, (P2 & 0x000000FF));
}


void setupMultisynth(uint8_t synth,uint32_t divider,uint8_t rDiv)
{
  uint32_t P1;					// Synth config register P1
  uint32_t P2;					// Synth config register P2
  uint32_t P3;					// Synth config register P3

  P1 = 128 * divider - 512;
  P2 = 0;							// P2 = 0, P3 = 1 forces an integer value for the divider
  P3 = 1;

  I2C_sendREG(synth + 0,   (P3 & 0x0000FF00) >> 8);
  I2C_sendREG(synth + 1,   (P3 & 0x000000FF));
  I2C_sendREG(synth + 2,   ((P1 & 0x00030000) >> 16) | rDiv);
  I2C_sendREG(synth + 3,   (P1 & 0x0000FF00) >> 8);
  I2C_sendREG(synth + 4,   (P1 & 0x000000FF));
  I2C_sendREG(synth + 5,   ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
  I2C_sendREG(synth + 6,   (P2 & 0x0000FF00) >> 8);
  I2C_sendREG(synth + 7,   (P2 & 0x000000FF));
}


void si5351aSetFrequency(uint32_t frequency , uint8_t Chanal )
{
  uint32_t pllFreq;
  uint32_t xtalFreq = XTAL_FREQ;// Crystal frequency
  uint32_t l;
  float f;
  uint8_t mult;
  uint32_t num;
  uint32_t denom;
  uint32_t divider;

  divider = 900000000 / frequency;// Calculate the division ratio. 900,000,000 is the maximum internal 
                                                                  // PLL frequency: 900MHz
  if (divider % 2) divider--;		// Ensure an even integer division ratio

  pllFreq = divider * frequency;	// Calculate the pllFrequency: the divider * desired output frequency

  mult = pllFreq / xtalFreq;		// Determine the multiplier to get to the required pllFrequency
  l = pllFreq % xtalFreq;			// It has three parts:
  f = l;							// mult is an integer that must be in the range 15..90
  f *= 1048575;					// num and denom are the fractional parts, the numerator and denominator
  f /= xtalFreq;					// each is 20 bits (range 0..1048575)
  num = f;						// the actual multiplier is  mult + num / denom
  denom = 1048575;				// For simplicity we set the denominator to the maximum 1048575
  // Set up PLL A with the calculated multiplication ratio
  setupPLL(SI_SYNTH_PLL_A, mult, num, denom);
                                                                  // Set up MultiSynth divider 0, with the calculated divider. 
                                                                  // The final R division stage can divide by a power of two, from 1..128. 
                                                                  // reprented by constants SI_R_DIV1 to SI_R_DIV128 (see si5351a.h header file)
                                                                  // If you want to output frequencies below 1MHz, you have to use the 
                                                                  // final R division stage
  if( Chanal == 0 ){
		setupMultisynth(SI_SYNTH_MS_0,divider,SI_R_DIV_1);
                                                                  // Reset the PLL. This causes a glitch in the output. For small changes to 
                                                                  // the parameters, you don't need to reset the PLL, and there is no glitch
		I2C_sendREG(SI_PLL_RESET,0xA0);	
                                                                  // Finally switch on the CLK0 output (0x4F)
                                                                  // and set the MultiSynth0 input to be PLL A
		I2C_sendREG(SI_CLK0_CONTROL, 0x4F|SI_CLK_SRC_PLL_A);
	}
	else if ( Chanal == 1 ){
		setupMultisynth(SI_SYNTH_MS_1,divider,SI_R_DIV_1);
		I2C_sendREG(SI_PLL_RESET,0xA0);	
		I2C_sendREG(SI_CLK1_CONTROL, 0x4F|SI_CLK_SRC_PLL_A);
	}
		else if ( Chanal == 2 ){
		setupMultisynth(SI_SYNTH_MS_2,divider,SI_R_DIV_1);
		I2C_sendREG(SI_PLL_RESET,0xA0);	
		I2C_sendREG(SI_CLK2_CONTROL, 0x4F|SI_CLK_SRC_PLL_A);
		}
}

