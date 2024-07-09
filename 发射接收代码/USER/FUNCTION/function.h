#ifndef __FUNCTION_H
#define __FUNCTION_H
#include "main.h"
#include "sys.h"


#ifdef		SEND
#include "lcd.h"
#include "showhz.h"
#endif


#ifdef		SEND
extern u8 *p_number;
extern u8 *p_element;

void DrawMenu();
void DrawOutput();
void DrawInput();
void Digital_Convert(int digital);
void Base_Convert(u8 *a_data);
void Coding();
#endif


#ifndef		SEND
int Check(u8 *p);
int Counter(u8 *p);
int Binary2Decimal(u8 *p);
#endif

#endif