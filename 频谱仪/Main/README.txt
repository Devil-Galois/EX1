PE4302引脚：

GND
PC13--DAT
PC0---DCLK
PB13--LE


ADF4351引脚：

CE---PF14
LE---PF12
DAT---PB10
CLK---PB11
GND

键盘引脚：

1-----PB7-----COL3
2-----PA4-----COL2
3-----PG15----COL1
4-----PC7-----COL0
5-----PC9-----ROW3
6-----PB6-----ROW2
7-----PE6-----ROW1
8-----PA8-----ROW0


row1   1 2 3 A
row2   4 5 6 B
row3   7 8 9 C
row4   * 0 # D






ADC引脚：

ADC1----IN12----PA6





LCD引脚：

PF10
PE7~15
PG2
PG12
PD0 1 4 5 8 9 10 14 15

exti引脚
PF0
PG0



模块衰减：
石英晶体滤波器： -30dB
OPA847:         40dB
低噪声运放倍数
8.757




衰减器  100M						
0----92.7mV---262.1556			
10---53mV---149.884
20---30mV---84.84
30---17.1mV---48.3588
40---9.6mV---27.1488
50---5.5mV---15.554
60---3.1mV---8.7668

80M--
0---81mV---
50---4.9mV
90M
0---89mV



整级回路
滑变最大
OPA695
输入		输出
10mVrms		2.675V
5mVrms		2.622V
1mVrms		2.452V
354uVrms	2.26V	
113uVrms--4.2mVrms--2.017V
20uVrms--790uVrms--1.65V

滑变最小
OPA695
输入		输出
10mVrms		2.394V
5mVrms		2.32V
1mVrms		2.115V
354uVrms	1.908V	
113uVrms--4.2mVrms--1.65V
20uVrms--790uVrms--1.3V

滑变最大
THS3001
输入		输出
10mVrms		1.696V
5mVrms		1.618V
1mVrms		1.413V
354uVrms	1.211V	
//113uVrms--4.2mVrms--1.65V
20uVrms--790uVrms--0.65V