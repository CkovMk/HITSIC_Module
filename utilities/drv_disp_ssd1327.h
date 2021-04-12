#ifndef DRV_DISP_SSD1327_H
#define DRV_DISP_SSD1327_H

#include <inc_stdlib.h>
#include "hitsic_common.h"

#if defined(HITSIC_USE_DISP_SSD1327) && (HITSIC_USE_DISP_SSD1327 > 0)
#include <drv_disp_spibus_port.h>
#include <lib_graphic.h>

/** @brief : 软件版本 */
#define DRV_DISP_SSD1327_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))

GRAPHIC_FB_GSCL4B_DEF(disp_ssd1327, DISP_SSD1327, 128, 128)

GRAPHIC_FB_PRINT0608_DEF(disp_ssd1327, DISP_SSD1327)

status_t DISP_SSD1327_Init(void);

void DPSI_SSD1327_DispEnable(bool _state);

void DISP_SSD1327_Fill(disp_ssd1327_fb_pixel_t _color);

void DISP_SSD1327_SetPixel(uint8_t x, uint8_t y, disp_ssd1327_fb_pixel_t _color);

void DISP_SSD1327_BufferUpload(disp_ssd1327_fb_t *_fb);

#endif // ! HITSIC_USE_DISP_SSD1327

#endif // ! DRV_DISP_SSD1327_H

