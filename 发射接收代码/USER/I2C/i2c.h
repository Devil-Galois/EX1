#ifndef __I2C1_H
#define __I2C1_H
#include "sys.h" 
 
void I2C1_Init(void);// I2C1��ʼ������
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_SendData(uint8_t slaveAddr, uint8_t* data, uint8_t len);
void I2C1_ReceiveData(uint8_t slaveAddr, uint8_t* data, uint8_t len);
 
#endif
