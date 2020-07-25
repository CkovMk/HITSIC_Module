/*
 *	@by:		SmartCar SJMC
 *	@name:		drvoled_sssd1306.h
 *	@layer:		driver
 *	@owner:		C.M.@SJMC
 *	@modified:	C.M.@SJMC 2019.01.15 04:48
 */
#pragma once
#ifndef _DISP_SSD1306_H_
#define _DISP_SSD1306_H_
#include "inc_stdlib.h"
#include "hitsic_common.h"
#include "drv_disp_ssd1306_port.hpp"



void DISP_SSD1306_Init(void);
void DISP_SSD1306_Fill(uint8_t dat);
void DISP_SSD1306_PutPixel(uint8_t x,uint8_t y);
void DISP_SSD1306_ClrPixel(uint8_t x,uint8_t y);
void DISP_SSD1306_SetPos(uint8_t x, uint8_t y);
void DISP_SSD1306_Print_F6x8(uint8_t x,uint8_t y,const char* str);
void DISP_SSD1306_Print_F8x16(uint8_t x,uint8_t y,const char* str);
void DISP_SSD1306_Printf_F6x8(uint8_t x,uint8_t y,const char* fmt, ...);
void DISP_SSD1306_Printf_F8x16(uint8_t x,uint8_t y,const char* fmt, ...);
//void DISP_SSD1306_printFrame(uint8_t *p);



#endif // ! _DISP_SSD1306_H_
