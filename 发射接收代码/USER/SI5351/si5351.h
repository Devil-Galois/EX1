#ifndef __I2C_H
#define __I2C_H
#include "sys.h"
#include "delay.h"
#include "stdbool.h"

typedef unsigned  short int   uint;


#define I2C2_GPIOx  GPIOB
#define Pin_SCL     GPIO_Pin_1
#define Pin_SDA     GPIO_Pin_0
 
#define Pin_SCL_L       I2C2_GPIOx->ODR &= ~Pin_SCL
#define Pin_SCL_H       I2C2_GPIOx->ODR |= Pin_SCL
 
#define Pin_SDA_L       I2C2_GPIOx->ODR &= ~Pin_SDA
#define Pin_SDA_H       I2C2_GPIOx->ODR |= Pin_SDA
 
#define Read_SDA_Pin    I2C2_GPIOx->IDR & Pin_SDA


#define SI_CLK0_CONTROL 16          // Register definitions
#define SI_CLK1_CONTROL 17
#define SI_CLK2_CONTROL 18
#define SI_SYNTH_PLL_A  26
#define SI_SYNTH_PLL_B  34
#define SI_SYNTH_MS_0       42
#define SI_SYNTH_MS_1       50
#define SI_SYNTH_MS_2       58
#define SI_PLL_RESET        177

#define SI_R_DIV_1      0x00            // R-division ratio definitions
#define SI_R_DIV_2      0b00010000
#define SI_R_DIV_4      0b00100000
#define SI_R_DIV_8      0b00110000
#define SI_R_DIV_16     0b01000000
#define SI_R_DIV_32     0b01010000
#define SI_R_DIV_64     0b01100000
#define SI_R_DIV_128        0b01110000

#define SI_CLK_SRC_PLL_A    0x00
#define SI_CLK_SRC_PLL_B    0b00100000
#define XTAL_FREQ   25000000            // Crystal frequency
 

 
void I2C2_Init(void);         
bool I2C2_Start(void);                     
bool I2C2_Stop(void);                  
void I2C2_Send_Byte(uint8_t txd);
uint8_t I2C2_Read_Byte(void);
uint8_t I2C2_Wait_Ack(void);       
void I2C2_Ack(void);                           
void I2C2_NAck(void);                      
 
bool I2C2_Write_REG(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
uint8_t I2C2_Read_REG(uint8_t SlaveAddress,uint8_t REG_Address);
bool I2C2_Write_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t* buf, uint8_t len);
bool I2C2_Read_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t* buf, uint8_t len);
//建议使用这个函数来测试是否有通信
bool I2C2_CheckDevice(uint8_t SlaveAddress);

//这里的代码和硬件i2c重复了，下面会给出
void si5351aSetFrequency(uint32_t frequency , uint8_t Chanal );
void setupPLL(uint8_t pll, uint8_t mult, uint32_t num, uint32_t denom);
void setupMultisynth(uint8_t synth,uint32_t divider,uint8_t rDiv);

#endif
