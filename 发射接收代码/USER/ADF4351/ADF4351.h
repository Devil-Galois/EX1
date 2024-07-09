/*------------------------------------------------------
Modify Author: yusnows
Modify Date  : 2017.8.2
File         : ADF4351.h
version      : v0.0
-------------------------------------------------------*/


#ifndef _ADF4351_H_
#define _ADF4351_H_

#include "sys.h" 


//PE14--CE
//PE12--LE
//PE11--DAT
//PD13--CLK

#define RCC_ADF4351_CLK		RCC_AHB1Periph_GPIOD
#define RCC_ADF4351_DOUT	RCC_AHB1Periph_GPIOF
#define RCC_ADF4351_DIN		RCC_AHB1Periph_GPIOE
#define RCC_ADF4351_LE		RCC_AHB1Periph_GPIOE
#define RCC_ADF4351_CE		RCC_AHB1Periph_GPIOE

#define ADF4351_CLK_GPIO	GPIOD
#define ADF4351_DIN_GPIO	GPIOE
#define ADF4351_DOUT_GPIO	GPIOF
#define ADF4351_LE_GPIO		GPIOE
#define ADF4351_CE_GPIO		GPIOE

#define ADF4351_CLK_Pin		GPIO_Pin_13
#define ADF4351_DOUT_Pin	GPIO_Pin_1
#define ADF4351_DIN_Pin		GPIO_Pin_11
#define ADF4351_LE_Pin		GPIO_Pin_12
#define ADF4351_CE_Pin		GPIO_Pin_14

#define ADF4351_CLK         PDout(13)
#define ADF4351_DIN         PEout(11)
#define ADF4351_LE          PEout(12)
#define ADF4351_CE          PEout(14)

#define ADF4351_DOUT        PFin(1)

#ifndef kHz
#define kHz		            1000
#endif
#ifndef MHz
#define MHz		            1000*kHz
#endif

#define FREQREFIN	        100*MHz

#define R0ADDR		        0
#define R1ADDR		        1
#define R2ADDR		        2
#define R3ADDR	`	        3
#define R4ADDR		        4
#define R5ADDR		        5

typedef struct
{
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R4;
	uint32_t R5;
	double Fpfd;		//鉴相器频率;Ffpd=FREQREFIN*((1+D)/(R*(1+T))),一般设置：D=0，T=1
	uint32_t RFDiv;	//RF输出分频
    uint32_t INT;
    uint32_t MOD;
    uint32_t FRAC;
	uint32_t BandCLKSelDiv;	//锁定定时器时钟分频
	double CurrentFreq;	    //当前ADF4351的输出频率
    double CurrentVCOFreq;      //当前AF4351VCO频率,单位 MHz
	
}ADF4351MSG;

extern ADF4351MSG ADF4351;

void ADF4351Init(void);
void ReadToADF4351(uint8_t count, uint8_t *buf);
void WriteToADF4351(uint8_t count, uint8_t *buf);
void WriteOneRegToADF4351(uint32_t Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(double Fre);		//	(xx.x) M Hz

void ADF4351MEGInit(void);
void TransformInt32ToU8(uint32_t value,uint8_t buf[4]);

#endif

