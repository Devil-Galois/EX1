键盘引脚：
1-----PB7-----COL3
2-----PA4-----COL2
3-----PG15----COL1
4-----PC7-----COL0
5-----PC9-----ROW3
6-----PB6-----ROW2
7-----PE6-----ROW1
8-----PA8-----ROW0
自行修改





SPI驱动AD9851/AD9850

PB3	SCL
PB4	MISO
PB5	MOSI
GND

根据开发板自行修改



PE4302

PE4302_DAT         		PC13
PE4302_CLK         		PC0
PE4302_LE        		PB13



LCD引脚：

PF10,11
PE7~15
PG2
PG12
PD0 1 4 5 8 9 10 14 15

exti引脚  用于键盘触发
PF0
PG0

自行选用定时器触发
ADC3_IN14	PF4	用于测量AD637有效值检波结果
ADC1_IN7	PA7	用于测量150k低通接AGC输出结果			建议使用DMA

TIMXX:用于变频和分析ADC1_IN7结果