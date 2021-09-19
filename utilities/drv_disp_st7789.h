/**
 * Copyright 2018 - 2021 HITSIC
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
 * @file    :   drv_disp_ssd7789.h
 * @author  :   benjamin
 *
 * @date    :   v0.1-beta.0 2021.08.05 benjamin
 *
 * @brief   :   ST7789 IPS屏幕驱动组件
 */
#ifndef DRV_DISP_ST7789_H
#define DRV_DISP_ST7789_H

#include "hitsic_common.h"

#if defined(HITSIC_USE_DISP_ST7789) && (HITSIC_USE_DISP_ST7789 > 0)
#include <inc_stdlib.h>
#include <drv_disp_spibus_port.h>
#include <lib_graphic.h>

/** @brief : 软件版本 */
#define DRV_DISP_ST7789_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))


#if defined(HITSIC_DISP_USE_IPS_114) && (HITSIC_DISP_USE_IPS_114 > 0)
#if defined(HITSIC_DISP_ST7789_VERTICAL) && (HITSIC_DISP_ST7789_VERTICAL > 0)
GRAPHIC_FB_RGB18B_DEF(disp_st7789, DISP_ST7789, 240, 135)
#else // ! HITSIC_DISP_ST7789_VERTICAL
GRAPHIC_FB_RGB18B_DEF(disp_st7789, DISP_ST7789, 135, 240)
#endif // ! HITSIC_DISP_ST7789_VERTICAL
#endif // ! HITSIC_DISP_USE_IPS_114


#if defined(HITSIC_DISP_USE_IPS_154) && (HITSIC_DISP_USE_IPS_154 > 0)
GRAPHIC_FB_RGB18B_DEF(disp_st7789, DISP_ST7789, 240, 240)
#endif // ! HITSIC_DISP_USE_IPS_154

GRAPHIC_FB_PRINT1215_DEF(disp_st7789, DISP_ST7789)

void DISP_ST7789_Init(void);

void DISP_ST7789_Fill(disp_st7789_fb_pixel_t _color);

void DISP_ST7789_SetPixel(uint8_t x, uint8_t y, disp_st7789_fb_pixel_t _color);

void DISP_ST7789_BufferUpload(disp_st7789_fb_t *_fb);

#endif // ! HITSIC_USE_DISP_ST7789

#endif // ! DRV_DISP_ST7789_H
