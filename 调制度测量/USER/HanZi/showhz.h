#ifndef __SHOWHZ_H__
#define __SHOWHZ_H__	 
#include "sys.h"
#include "delay.h"
#include "updatefont.h"

/*********************************************************************************
*************************MCU启明 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: showhz.h                                                             *
* 文件简述：汉字显示调用的函数                                                   *
* 创建日期：2015.03.14                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/ 
 					     
/****************************************************************************
* 名    称: void LCD_Display1HZ(u16 x,u16 y,u8 *font,u8 size)
* 功    能：在指定位置显示一个汉字
* 入口参数：x,y :汉字的坐标 
            font:汉字GBK码
            size:字体大小
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void LCD_Display1HZ(u16 x,u16 y,u8 *font,u8 size);	  

/****************************************************************************
* 名    称: void LCD_DisplayHZstr(u16 x,u16 y,u8*str,u8 size)
* 功    能：在指定位置显示一个汉字
* 入口参数：x,y:起始坐标
            str  :字符串
            size :字体大小
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void LCD_DisplayHZstr(u16 x,u16 y,u8 size,u8*str);	    //在指定位置显示一个字符串 

#endif
