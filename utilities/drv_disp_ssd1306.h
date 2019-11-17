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
#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"
#include "drv_cam_def.h"
     


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
void OLED_ImgGetHalfResolution(uint8_t *src, uint8_t *dst, const uint16_t row, const uint16_t col);


/**
 * @example :
 *              OLED_Printf(1,6,"hello, %d + %d  = %d", 1, 2, 1+2);
 */
int OLED_Printf(uint16 Row, uint16 off, const char* Format, ...);

/**
 * @brief 打印图像
 * 
 * @param {camera_img_t*} image : 图像采集结构体
 * @param {uint8_t} mono_th     : 二值化阈值 
 */
void OLED_PrintHalfImg(camera_img_t* image, uint8_t mono_th);




/********************************************************************/

#endif // ! DRV_DISP_SSD1306_H
