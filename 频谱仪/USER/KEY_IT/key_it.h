#ifndef  __KEY_IT_H
#define  __KEY_IT_H
#include "sys.h"
#include "KeyBoard.h"
#define  ON  								0
#define  OFF								1


#define  WAIT    						1
#define  DOWN      					2
#define  CONFIRMDOWN    		3
#define  WAITUP         		4
#define  CONFIRMUP					5


#define KEYBOARDROW					(ROW0&&ROW1&&ROW2&&ROW3)
#define KEY0 								PFin(9)   
#define KEY1 								PFin(8)		
#define KEY2 								PFin(7)		
#define KEY3 	  						PFin(6)	



/*  ������ʼ��  */
void KEY_Init(void);


/*  ����״̬��  */
extern  int KEYBOARD_16;
void KEYBOARDPRESS(void);
/*  ����״̬������  */


/*  ����KEY0״̬��  */
extern  int KEY0PRESS;
void KEY_0_PRESS(void);

/*  ����KEY1״̬��  */
extern int KEY1PRESS;
void KEY_1_PRESS(void);

/*  ����KEY2״̬��  */
extern int KEY2PRESS;
void KEY_2_PRESS(void);

/*  ����KEY3״̬��  */
extern int KEY3PRESS;
void KEY_3_PRESS(void);
#endif


