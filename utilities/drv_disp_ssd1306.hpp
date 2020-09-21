/**
 * Copyright 2018 - 2019 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @file 	:	drv_disp_ssd1306.h
 * @author  :	beforelight/肖日涛(qq:1761690868)
 * @author  :   CkovMk/马奇科(qq:905497173)
 * @version :	v0.1.1
 *
 * @date 	:	v0.1-beta.0 2018.08.12 马奇科
 * @date 	:	v0.1.1		2019.09.22 肖日涛
 *
 * @brief   :   SSD1306 OLED屏幕驱动组件
 */

#pragma once
#ifndef _DISP_SSD1306_H_
#define _DISP_SSD1306_H_
#include "inc_stdlib.h"
#include "hitsic_common.h"

#if defined(HITSIC_USE_DISP_SSD1306) && (HITSIC_USE_DISP_SSD1306 > 0)

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

#endif // ! HITSIC_USE_DISP_SSD1306

#endif // ! _DISP_SSD1306_H_
