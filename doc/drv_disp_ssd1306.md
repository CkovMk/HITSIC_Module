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

- 帧缓存类

  ```c++
  class disp_ssd1306_frameBuffer_t
  {
  public:
      static const uint16_t row = 64, col = 128;
      uint8_t frame[8][128];
  
      void Clear(void);
      void SetPixelColor(uint16_t x, uint16_t y, bool color);
  };
  ```

  `row`和`col`定义了帧缓存的尺寸（像素数，128 * 64）。

  成员函数`void Clear(void);`用于清除缓存区内容。

  成员函数`void SetPixelColor(uint16_t x, uint16_t y, bool color);`用于设置`(x, y)`坐标处像素的颜色（对于此黑白屏幕来说，仅有点亮/熄灭两种颜色）。**注意：该函数未作边界检查 。**

- **初始化函数`void DISP_SSD1306_Init(void);`**

  初始化屏幕。

- **屏幕填充函数`void DISP_SSD1306_Fill(uint8_t dat);`**

  以`dat`填充整块屏幕。`dat == 0x00`时为清屏，`dat == 0xff`时为点亮所有像素。

- **点亮像素`void DISP_SSD1306_PutPixel(uint8_t x,uint8_t y);`**

- **熄灭像素`void DISP_SSD1306_ClrPixel(uint8_t x,uint8_t y);`**

- **设置打印位置`void DISP_SSD1306_SetPos(uint8_t x, uint8_t y);`**

  设置起始打印位置。一般无需调用。

- **字符串打印函数**
	- **`void DISP_SSD1306_Print_F6x8(uint8_t x,uint8_t y,const char* str);`**
  打印6\*8像素的ASCII字符串
- **`void DISP_SSD1306_Print_F8x16(uint8_t x,uint8_t y,const char* str);`**
	打印8\*16像素的ASCII字符串
  - **`void DISP_SSD1306_Printf_F6x8(uint8_t x,uint8_t y,const char* fmt, ...);`**
打印6\*8像素的ASCII格式化字符串
	- **`void DISP_SSD1306_Printf_F8x16(uint8_t x,uint8_t y,const char* fmt, ...);`**
  打印8\*16像素的ASCII格式化字符串

- **帧缓存刷新函数**

  **`void DISP_SSD1306_BufferUpload(uint8_t *buffer);`**

  用于将绘制完成的帧缓存更新到屏幕。也可用于显示预先存储的图片资源。



## 应用指南

### 无缓存



### 字符缓存



### 全缓存





## 移植文档

### 屏幕方向

```c++
#define HITSIC_DISP_SSD1306_FLIP_X (0U)	///< 屏幕左右翻转
#define HITSIC_DISP_SSD1306_FLIP_Y (0U)	///< 屏幕上下翻转
```



### 数字接口

只需实现三个函数：

```c++
/**
 * @brief 设置RST脚电平。
 * 
 * @param x RST脚的电平状态，正逻辑。
 */
inline void DISP_SSD1306_gpioSetRST(uint8_t x);

/**
 * @brief 设置D/C脚电平。
 * 
 * @param x D/C脚的电平状态，正逻辑。
 */
inline void DISP_SSD1306_gpioSetD_C(uint8_t x);

/**
 * @brief 毫秒级阻塞延迟函数。
 * 
 * @param ms 延迟的毫秒数。
 */
inline void DISP_SSD1306_delay_ms(uint32_t ms);

/**
 * @brief SPI接口发送一个字节。
 * 
 * @param data 要发送的数据
 */
inline void DISP_SSD1306_spiWrite(uint8_t data);
```

