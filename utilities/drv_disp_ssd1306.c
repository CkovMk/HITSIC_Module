/*
 *	@by:		SmartCar SJMC
 *	@name:		drvDISP_SSD1306_ssd1306.cpp
 *	@layer:		driver
 *	@owner:		C.M.@SJMC
 *	@modified:	C.M.@SJMC 2019.01.15 04:48
 */
#include <drv_disp_ssd1306.h>

#if defined(HITSIC_USE_DISP_SSD1306) && (HITSIC_USE_DISP_SSD1306 > 0)

#define SYSLOG_TAG  ("DISP_1306")
#define SYSLOG_LVL  (3U)
#include <inc_syslog.h>

//#include "drv_disp_font.cpp"

/*********************************************************************************************************************
宏定义
*********************************************************************************************************************/

//#define OLED_SSD1306_Brightness 0xCF

void DISP_SSD1306_WriteDat(uint8_t data)
{
	DISP_SPIBUS_gpioSetD_C(1);
	DISP_SPIBUS_spiWrite(&data, 1U);
}
void DISP_SSD1306_WriteCmd(uint8_t cmd)
{
	DISP_SPIBUS_gpioSetD_C(0);
	DISP_SPIBUS_spiWrite(&cmd, 1U);
}


void DISP_SSD1306_SetPos(uint8_t x, uint8_t y)
{
	DISP_SSD1306_WriteCmd(0xb0 + y);
	DISP_SSD1306_WriteCmd(((x & 0xf0) >> 4) | 0x10);
	DISP_SSD1306_WriteCmd((x & 0x0f) | 0x01);
}


void DISP_SSD1306_Fill(uint8_t bmp_data)
{
	uint8_t y, x;

	for (y = 0; y < 8; y++)
	{
		DISP_SSD1306_WriteCmd(0xb0 + y);
		DISP_SSD1306_WriteCmd(0x01);
		DISP_SSD1306_WriteCmd(0x10);
		for (x = 0; x < 128; x++)
			DISP_SSD1306_WriteDat(bmp_data);
	}
}


void DISP_SSD1306_Init(void)
{
    SYSLOG_I("Init Begin. v%d.%d.%d",HITSIC_VERSION_MAJOR(DRV_DISP_SSD1306_VERSION),
                HITSIC_VERSION_MINOR(DRV_DISP_SSD1306_VERSION), HITSIC_VERSION_PATCH(DRV_DISP_SSD1306_VERSION));
	DISP_SPIBUS_gpioSetRST(1);
	DISP_SPIBUS_delay_ms(1);
	DISP_SPIBUS_gpioSetRST(0);
	DISP_SPIBUS_delay_ms(50);
	DISP_SPIBUS_gpioSetRST(1);

	DISP_SSD1306_WriteCmd(0xae); //--turn off oled panel
	DISP_SSD1306_WriteCmd(0x00); //---set low column address
	DISP_SSD1306_WriteCmd(0x10); //---set high column address
	DISP_SSD1306_WriteCmd(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	DISP_SSD1306_WriteCmd(0x81); //--set contrast control register
	DISP_SSD1306_WriteCmd(0xcf); // Set SEG Output Current OLED_SSD1306_Brightness
#if defined(HITSIC_DISP_SSD1306_FLIP_X) && (HITSIC_DISP_SSD1306_FLIP_X > 0)
	DISP_SSD1306_WriteCmd(0xa0); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	SYSLOG_D("X axis flipped.");
#else // ! HITSIC_DISP_SSD1306_FLIP_X
	DISP_SSD1306_WriteCmd(0xa1); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	SYSLOG_D("X axis normal.");
#endif// ! HITSIC_DISP_SSD1306_FLIP_X
#if defined(HITSIC_DISP_SSD1306_FLIP_Y) && (HITSIC_DISP_SSD1306_FLIP_Y > 0)
	DISP_SSD1306_WriteCmd(0xc0); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	SYSLOG_D("Y axis flipped.");
#else // ! HITSIC_DISP_SSD1306_FLIP_Y
	DISP_SSD1306_WriteCmd(0xc8); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	SYSLOG_D("Y axis normal.");
#endif// ! HITSIC_DISP_SSD1306_FLIP_Y
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
	//DISP_SSD1306_WriteCmd(0x02); //
	DISP_SSD1306_WriteCmd(0x00);//@ C.M. change to page address mode. enable frame buffer.
	DISP_SSD1306_WriteCmd(0x8d); //--set Charge Pump enable/disable
	DISP_SSD1306_WriteCmd(0x14); //--set(0x10) disable
	DISP_SSD1306_WriteCmd(0xa4); // Disable Entire Display On (0xa4/0xa5)
	DISP_SSD1306_WriteCmd(0xa6); // Disable Inverse Display On (0xa6/a7)
	DISP_SSD1306_WriteCmd(0xaf); //--turn on oled panel
	DISP_SSD1306_Fill(0x00);  //初始清屏
	DISP_SSD1306_SetPos(0, 0);

#if defined(HITSIC_DISP_SSD1306_DMA) && (HITSIC_DISP_SSD1306_DMA > 0U)
	DISP_SPIBUS_spiDmaInit();
	SYSLOG_D("DMA API enabled.");
#else
	SYSLOG_D("DMA API disbled.");
#endif // ! HITSIC_DISP_SSD1306_DMA

	SYSLOG_I("Init Comlpete.");
}


void DISP_SSD1306_SetPixel(uint8_t x, uint8_t y, bool value)
{
	uint8_t data1; //data1当前点的数据

	DISP_SSD1306_SetPos(x, y);
	if (value)
	{
		data1 = 0x01 << (y % 8);
	}
	else
	{
		data1 = 0x00 << (y % 8);
	}
	DISP_SSD1306_WriteCmd(0xb0 + (y >> 3));
	DISP_SSD1306_WriteCmd(((x & 0xf0) >> 4) | 0x10);
	DISP_SSD1306_WriteCmd((x & 0x0f) | 0x00);
	DISP_SSD1306_WriteDat(data1);
}


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


//void DISP_SSD1306_Printf_F6x8(uint8_t x,uint8_t y,const char* fmt, ...)
//{
//	va_list args;
//	va_start(args, fmt);
//	static char* buf = new char[64];
//	vsnprintf(buf, 64, fmt, args);
//	DISP_SSD1306_Print_F6x8(x, y, buf);
//}
//
//void DISP_SSD1306_Printf_F8x16(uint8_t x,uint8_t y,const char* fmt, ...)
//{
//	va_list args;
//	va_start(args, fmt);
//	static char* buf = new char[64];
//	vsnprintf(buf, 64, fmt, args);
//	DISP_SSD1306_Print_F8x16(x, y, buf);
//}


void DISP_SSD1306_BufferUpload(uint8_t *buffer)
{
    DISP_SSD1306_SetPos(0, 0);
    DISP_SPIBUS_gpioSetD_C(1);
    DISP_SPIBUS_spiWrite(buffer, sizeof(disp_ssd1306_fb_t));
}


#if defined(HITSIC_DISP_SSD1306_DMA) && (HITSIC_DISP_SSD1306_DMA > 0U)

void DISP_SSD1306_BufferUploadDMA(uint8_t *buffer)
{
    DISP_SSD1306_SetPos(0, 0);
    DISP_SSD1306_gpioSetD_C(1);
    DISP_SSD1306_spiDmaWrite(buffer, sizeof(disp_ssd1306_fb_t));
}

#endif // ! HITSIC_DISP_SSD1306_DMA

#endif // ! HITSIC_USE_DISP_SSD1306

