键盘引脚：

1-----PD6-----COL3
2-----PD7-----COL2
3-----PC6----COL1
4-----PC8-----COL0
5-----PC11-----ROW3
6-----PE5-----ROW2
7-----PA6-----ROW1
8-----PD3-----ROW0


row1   1 2 3 A
row2   4 5 6 B
row3   7 8 9 C
row4   * 0 # D

D6
D7
C6
C8
C11
E5
A6
D3






LCD引脚：

PF10,11
PE7~15
PG2
PG12
PD0 1 4 5 8 9 10 14 15

exti引脚
PF0
PG0


PWM波：
TIM3  CH2  PA8



码：
0-7：11111111
8-11：数字1
12-15：数字2
16-19：数字3
20-23：数字4
24-27：校验位
28-31：1110


I2C:
PB1
PB2


数码管：
发射机	引脚		接收机
PB11	--SDA--		PB12
PB10	--SCL--		PB13





输入捕获定时器TIM3
PB4--CH1


Note:SI5351必须提前写好频率  否则引脚和时钟输出引脚冲突
SI5351引脚
SCL---PB1
SDA---PB0



中断管理
发送端		外部中断<按键检测中断=PWM中断			子优先级		按键检测中断<PWM中断
接收端		边沿检测中断

已用定时器
发射机
TIM8		PWM
TIM5		按键检测
TIM6		输出正弦波
TIM4		输出数字信号--PB7
TIM2		输出数字信号--PA1
接收机
TIM3		码元检测
TIM4		输出数字信号--PB6
TIM2		输出数字信号--PA3



ADF4351

PE14--CE
PE12--LE
PE11--DAT
PD13--CLK



接收机拉低PF7切换ADF4351频率
