/*
 *	@by:		SmartCar SJMC
 *	@name:		drvDISP_SSD1306_ssd1306.cpp
 *	@layer:		driver
 *	@owner:		C.M.@SJMC
 *	@modified:	C.M.@SJMC 2019.01.15 04:48
 */
#include "drv_disp_ssd1306.hpp"

#if defined(HITSIC_USE_DISP_SSD1306) && (HITSIC_USE_DISP_SSD1306 > 0)

#include "drv_disp_font.cpp"

/*********************************************************************************************************************
宏定义
*********************************************************************************************************************/
//#define OLED_SSD1306_XLevelL 0x00
//#define OLED_SSD1306_XLevelH 0x10
//#define OLED_SSD1306_XLevel ((OLED_SSD1306_XLevelH & 0x0F) * 16 + OLED_SSD1306_XLevelL)
//#define OLED_SSD1306_Column 128
//#define OLED_SSD1306_Row 64
//#define OLED_SSD1306_Brightness 0xCF
#define OLED_SSD1306_X_WIDTH 128
#define OLED_SSD1306_Y_WIDTH 64

//extern const uint8_t DISP_font_6x8[][6];
//extern const uint8_t DISP_font_8x16[][16];

#ifdef __cplusplus
extern "C"{
#endif

void DISP_SSD1306_WriteDat(uint8_t data)
{
	DISP_SSD1306_gpioSetD_C(1);
	DISP_SSD1306_spiWrite(data);
}
void DISP_SSD1306_WriteCmd(uint8_t cmd)
{
	DISP_SSD1306_gpioSetD_C(0);
	DISP_SSD1306_spiWrite(cmd);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示坐标设置
//  @param      x			x轴坐标设置
//  @param      y			y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_SetPos(uint8_t x, uint8_t y)
{
	DISP_SSD1306_WriteCmd(0xb0 + y);
	DISP_SSD1306_WriteCmd(((x & 0xf0) >> 4) | 0x10);
	DISP_SSD1306_WriteCmd((x & 0x0f) | 0x01);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED清屏函数
//  @param      bmp_data	填充颜色选着(0x00 or 0xff)
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_Fill(uint8_t bmp_data)
{
	uint8_t y, x;

	for (y = 0; y < 8; y++)
	{
		DISP_SSD1306_WriteCmd(0xb0 + y);
		DISP_SSD1306_WriteCmd(0x01);
		DISP_SSD1306_WriteCmd(0x10);
		for (x = 0; x < OLED_SSD1306_X_WIDTH; x++)
			DISP_SSD1306_WriteDat(bmp_data);
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED初始化函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_Init(void)
{
	DISP_SSD1306_gpioSetRST(1);
	DISP_SSD1306_delay_ms(1);
	DISP_SSD1306_gpioSetRST(0);
	DISP_SSD1306_delay_ms(50);
	DISP_SSD1306_gpioSetRST(1);

	DISP_SSD1306_WriteCmd(0xae); //--turn off oled panel
	DISP_SSD1306_WriteCmd(0x00); //---set low column address
	DISP_SSD1306_WriteCmd(0x10); //---set high column address
	DISP_SSD1306_WriteCmd(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	DISP_SSD1306_WriteCmd(0x81); //--set contrast control register
	DISP_SSD1306_WriteCmd(0xcf); // Set SEG Output Current OLED_SSD1306_Brightness
	DISP_SSD1306_WriteCmd(0xa0); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	DISP_SSD1306_WriteCmd(0xc0); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	DISP_SSD1306_WriteCmd(0xa6); //--set normal display
	DISP_SSD1306_WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	DISP_SSD1306_WriteCmd(0x3f); //--1/64 duty
	DISP_SSD1306_WriteCmd(0xd3); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	DISP_SSD1306_WriteCmd(0x00); //-not offset
	DISP_SSD1306_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	DISP_SSD1306_WriteCmd(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
	DISP_SSD1306_WriteCmd(0xd9); //--set pre-charge period
	DISP_SSD1306_WriteCmd(0xf1); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	DISP_SSD1306_WriteCmd(0xda); //--set com pins hardware configuration
	DISP_SSD1306_WriteCmd(0x12);
	DISP_SSD1306_WriteCmd(0xdb); //--set vcomh
	DISP_SSD1306_WriteCmd(0x40); //Set VCOM Deselect Level
	DISP_SSD1306_WriteCmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
	DISP_SSD1306_WriteCmd(0x02); //
	DISP_SSD1306_WriteCmd(0x8d); //--set Charge Pump enable/disable
	DISP_SSD1306_WriteCmd(0x14); //--set(0x10) disable
	DISP_SSD1306_WriteCmd(0xa4); // Disable Entire Display On (0xa4/0xa5)
	DISP_SSD1306_WriteCmd(0xa6); // Disable Inverse Display On (0xa6/a7)
	DISP_SSD1306_WriteCmd(0xaf); //--turn on oled panel
	DISP_SSD1306_Fill(0x00);  //初始清屏
	DISP_SSD1306_SetPos(0, 0);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED点亮一个点
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_PutPixel(uint8_t x, uint8_t y)
{
	uint8_t data1; //data1当前点的数据

	DISP_SSD1306_SetPos(x, y);
	data1 = 0x01 << (y % 8);
	DISP_SSD1306_WriteCmd(0xb0 + (y >> 3));
	DISP_SSD1306_WriteCmd(((x & 0xf0) >> 4) | 0x10);
	DISP_SSD1306_WriteCmd((x & 0x0f) | 0x00);
	DISP_SSD1306_WriteDat(data1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED清除一个点
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_ClrPixel(uint8_t x, uint8_t y)
{
	uint8_t data1; //data1当前点的数据

	DISP_SSD1306_SetPos(x, y);
	data1 = 0x00 << (y % 8);
	DISP_SSD1306_WriteCmd(0xb0 + (y >> 3));
	DISP_SSD1306_WriteCmd(((x & 0xf0) >> 4) | 0x10);
	DISP_SSD1306_WriteCmd((x & 0x0f) | 0x00);
	DISP_SSD1306_WriteDat(data1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(6*8字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      ch[]        字符串
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_Print_F6x8(uint8_t x,uint8_t y,const char* str)
{
	uint8_t c = 0, i = 0, j = 0;
	while (str[j] != '\0')
	{
		if(str[j] == '\n')
		{
			x = 0;
			++y;
			continue;
		}
		c = str[j] - 32;
		if (x > 125)
		{
			x = 0;
			++y;
		}
		if(y > 7){break;}
		DISP_SSD1306_SetPos(x, y);
		for (i = 0; i < 6; i++)
			DISP_SSD1306_WriteDat((uint8_t)(DISP_font_6x8[c][i]));
		x += 6;
		j++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(8*16字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      ch[]        字符串
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DISP_SSD1306_Print_F8x16(uint8_t x,uint8_t y,const char* str)
{
	uint8_t c = 0, i = 0, j = 0;

	while (str[j] != '\0')
	{
		if(str[j] == '\n')
		{
			x = 0;
			y += 2;
			continue;
		}
		c = str[j] - 32;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		if(y > 6){break;}
		DISP_SSD1306_SetPos(x, y);
		for (i = 0; i < 8; i++)
			DISP_SSD1306_WriteDat((uint8_t)(DISP_font_8x16[c][i]));

		DISP_SSD1306_SetPos(x, y + 1);
		for (i = 0; i < 8; i++)
			DISP_SSD1306_WriteDat((uint8_t)(DISP_font_8x16[c][i + 8]));

		x += 8;
		j++;
	}
}


void DISP_SSD1306_Printf_F6x8(uint8_t x,uint8_t y,const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	static char* buf = new char[64];
	vsnprintf(buf, 64, fmt, args);
	DISP_SSD1306_Print_F6x8(x, y, buf);
}

void DISP_SSD1306_Printf_F8x16(uint8_t x,uint8_t y,const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	static char* buf = new char[64];
	vsnprintf(buf, 64, fmt, args);
	DISP_SSD1306_Print_F8x16(x, y, buf);
}

#ifdef __cplusplus
}
#endif


#endif // ! HITSIC_USE_DISP_SSD1306

