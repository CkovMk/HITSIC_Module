/*********************************************************************************************************************
Smartcar OLED显示底层
制作人：王学嘉
联系方式：18846446962
qq：512924543
备注：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露，感谢逐飞科技
	  OLED接线有多种方式，想要自己开发的同学请自行查询芯片手册
					OLED接线定义：
					------------------------------------
					OLED液晶      单片机
						PTD0          A15
						PTD1          A16
						RES         A14
						DC          B16
					------------------------------------
 ********************************************************************************************************************/

#pragma once
#ifndef DRV_DISP_SSD1306_H
#define DRV_DISP_SSD1306_H
#include "stdafx.h" 
#include "hitsic_common.h"

#define OLED_HARD_SPI_MODE 0U//培训板接线接错了，用不了硬件spi模式
#if defined(OLED_HARD_SPI_MODE)&&OLED_HARD_SPI_MODE==1u
#include "spi.h"
#define BOARD_OLED_SPI SPI2
#define BOARD_OLED_SPI_BAUDRATE 15*1000*1000
#define BOARD_OLED_SPI_PCS 0U
#endif 

 //OLED引脚宏定义
#define BOARD_OLED_SCL_GPIO GPIOD
#define BOARD_OLED_SCL_PORT PORTD
#define BOARD_OLED_SCL_GPIO_PIN 12U
#define BOARD_OLED_SDA_GPIO GPIOD   
#define BOARD_OLED_SDA_PORT PORTD
#define BOARD_OLED_SDA_GPIO_PIN 14U 
#define BOARD_OLED_RST_GPIO GPIOD   
#define BOARD_OLED_RST_PORT PORTD
#define BOARD_OLED_RST_GPIO_PIN 13U 
#define BOARD_OLED_DC_GPIO GPIOD
#define BOARD_OLED_DC_PORT PORTD
#define BOARD_OLED_DC_GPIO_PIN 15U 
//#define BOARD_OLED_CS_GPIO GPIOE   
//#define BOARD_OLED_CS_PORT PORTE
//#define BOARD_OLED_CS_GPIO_PIN 4U 

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef int16_t int16;
void OLED_Init(void);
void OLED_Fill(uint8 dat);//填充颜色选择0x00 or 0xff
void OLED_PutPixel(uint8 x, uint8 y);
void OLED_ClrPixel(uint8 x, uint8 y);
void OLED_Set_Pos(uint8 x, uint8 y);
void OLED_PrintStr_F6x8(uint8 x, uint8 y, uint8 ch[]);
void OLED_PrintStr_F8x16(uint8 x, uint8 y, uint8 ch[]);
void OLED_HEXACSII(uint16 hex, uint8* Print);
void OLED_PrintNum_F8x16(uint8 x, uint8 y, uint16 num);
void OLED_PrintNum_F6x8(uint8 x, uint8 y, int16 num);
void OLED_DispBmp(uint16 high, uint16 width, uint8* p, uint8 value);
void image_size_half_forOLED(uint8* src, uint8* dst, const int row, const int col);
int OLED_Printf(uint16 Row, uint16 off, const char* Format, ...);


/********************************************************************/

#endif // ! DRV_DISP_SSD1306_H
