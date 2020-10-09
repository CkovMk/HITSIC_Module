# DRV_DISP_SSD1306

[TOC]

## 简介

OLED显示屏组件（DRV_DISP_SSD1306）是用于包含SSD1306驱动芯片的OLED显示屏的驱动库。



## 版本说明

### v0.1.1

by beforelight @hitsic 2019.09.22





### v0.1-beta.0

by CkovMk @hitsic 2018.08.12



## API文档

**初始化函数`void DISP_SSD1306_Init(void);`**

初始化屏幕。

**屏幕填充函数`void DISP_SSD1306_Fill(uint8_t dat);`**

以`dat`填充整块屏幕。`dat == 0x00`时为清屏。

**点亮像素`void DISP_SSD1306_PutPixel(uint8_t x,uint8_t y);`**

**熄灭像素`void DISP_SSD1306_ClrPixel(uint8_t x,uint8_t y);`**

**设置打印位置`void DISP_SSD1306_SetPos(uint8_t x, uint8_t y);`**

设置起始打印位置。一般无需调用。

**字符串打印函数**
**`void DISP_SSD1306_Print_F6x8(uint8_t x,uint8_t y,const char* str);`**
打印6\*8像素的ASCII字符串

**`void DISP_SSD1306_Print_F8x16(uint8_t x,uint8_t y,const char* str);`**
打印8\*16像素的ASCII字符串

**`void DISP_SSD1306_Printf_F6x8(uint8_t x,uint8_t y,const char* fmt, ...);`**
打印6\*8像素的ASCII格式化字符串

**`void DISP_SSD1306_Printf_F8x16(uint8_t x,uint8_t y,const char* fmt, ...);`**
打印8\*16像素的ASCII格式化字符串



## 应用指南

### 无缓存



### 字符缓存



### 全缓存





## 移植文档

