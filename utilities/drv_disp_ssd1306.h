/**
 * Copyright 2018 - 2020 HITSIC
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
 * @date 	:	v0.1-beta.0 2018.08.12 CkovMk
 * @date 	:	v0.1.1		2019.09.22 beforelight
 * @date    :   v0.2.0      2020.10.29 CkovMk
 * @date    :   v0.2.1      2020.11.09 CkovMk
 *
 * @brief   :   SSD1306 OLED屏幕驱动组件
 */

#pragma once
#ifndef UTILITIES_DRV_DISP_SSD1306_HPP
#define UTILITIES_DRV_DISP_SSD1306_HPP
#include <inc_stdlib.h>
#include "hitsic_common.h"

#if defined(HITSIC_USE_DISP_SSD1306) && (HITSIC_USE_DISP_SSD1306 > 0)
#include <drv_disp_ssd1306_port.h>

/** @brief : 软件版本 */
#define DRV_DISP_SSD1306_VERSION (HITSIC_MAKE_VERSION(0U, 2U, 1U))

extern const uint8_t DISP_font_6x8[][6];
extern const uint8_t DISP_font_8x16[][16];

class disp_ssd1306_frameBuffer_t
{
public:
    typedef bool pixel_t;
    static const uint16_t row = 64, col = 128;
    uint8_t frame[8][128];

    void Clear(void)
    {
        static_assert(sizeof(disp_ssd1306_frameBuffer_t) == 8U * 128U);
        memset(&frame, 0U, sizeof(disp_ssd1306_frameBuffer_t));
    }
    void SetPixelColor(uint16_t x, uint16_t y, pixel_t color)
    {
        color ? (frame[y >> 3][x] |= (1U << (y & 7U))) : (frame[y >> 3][x] &= ~(1U << (y & 7U)));
    }
};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED初始化函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_Init(void);

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED清屏函数
//  @param      bmp_data	填充颜色选着(0x00 or 0xff)
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_Fill(uint8_t dat);

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED点亮或清除一个像素
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      value       像素状态
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_SetPixel(uint8_t x, uint8_t y, bool value);

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示坐标设置
//  @param      x			x轴坐标设置
//  @param      y			y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_SetPos(uint8_t x, uint8_t y);

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(6*8字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      str         字符串
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_Print_F6x8(uint8_t x,uint8_t y,const char* str);

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(8*16字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      str         字符串
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_Print_F8x16(uint8_t x,uint8_t y,const char* str);

//void DISP_SSD1306_Printf_F6x8(uint8_t x,uint8_t y,const char* fmt, ...);
//
//void DISP_SSD1306_Printf_F8x16(uint8_t x,uint8_t y,const char* fmt, ...);

/**
 * @brief 将帧缓存上传至屏幕显示
 *
 * @param buffer 指向帧缓存的指针
 */
void DISP_SSD1306_BufferUpload(uint8_t *buffer);

#if defined(HITSIC_DISP_SSD1306_DMA) && (HITSIC_DISP_SSD1306_DMA > 0U)

/**
 * @brief 将帧缓存上传至屏幕显示,DMA方式
 *
 * @param buffer 指向帧缓存的指针
 */
void DISP_SSD1306_BufferUploadDMA(uint8_t *buffer);

#endif // ! HITSIC_DISP_SSD1306_DMA

#endif // ! HITSIC_USE_DISP_SSD1306

#endif // ! UTILITIES_DRV_DISP_SSD1306_HPP
