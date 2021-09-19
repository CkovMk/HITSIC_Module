#include <drv_disp_st7789.h>

#if defined(HITSIC_USE_DISP_ST7789) && (HITSIC_USE_DISP_ST7789 > 0)

void DISP_ST7789_WriteDat_8Bit(uint8_t data)
{
    DISP_SPIBUS_gpioSetD_C(1);
    DISP_SPIBUS_spiWrite(&data, 1U);
}

void DISP_ST7789_WriteDat_16Bit(uint16_t data)
{
    uint8_t spi_data[2];
    spi_data[0] = data >> 8;
    spi_data[1] = (uint8_t)data;
    DISP_SPIBUS_gpioSetD_C(1);
    DISP_SPIBUS_spiWrite(spi_data, 2U);
}

void DISP_ST7789_WriteDat_24Bit(uint32_t data)
{
    uint8_t spi_data[3];
    spi_data[0] = (uint8_t)(data >> 24);
    spi_data[1] = (uint8_t)(data >> 16);
    spi_data[2] = (uint8_t)(data >> 8);
    DISP_SPIBUS_gpioSetD_C(1);
    DISP_SPIBUS_spiWrite(spi_data, 3U);
}

void DISP_ST7789_WriteCmd(uint8_t cmd)
{
    DISP_SPIBUS_gpioSetD_C(0);
    DISP_SPIBUS_spiWrite(&cmd, 1U);
}

//void DISP_ST7789_SetColAddress(uint16_t Addr1, uint16_t Addr2)
//{
//#if defined(HITSIC_DISP_USE_IPS_154) && (HITSIC_DISP_USE_IPS_154 > 0)
//    DISP_ST7789_WriteCmd(0x2a);
//    DISP_ST7789_WriteDat_16Bit(Addr1);  //送列地址开始
//    DISP_ST7789_WriteDat_16Bit(Addr2);      //送列地址结束
//    DISP_ST7789_WriteCmd(0x2c);
//
//#elif defined(HITSIC_DISP_USE_IPS_114) && (HITSIC_DISP_USE_IPS_114 > 0)
//
//#else
//#error DISP_ST7789: Please select screen type !
//#endif
//}
//
////=======写行地址==============
//void DISP_ST7789_SetRowAddress(uint16_t Addr1, uint16_t Addr2)
//{
//#if defined(HITSIC_DISP_USE_IPS_154) && (HITSIC_DISP_USE_IPS_154 > 0)
//    DISP_ST7789_WriteCmd(0x2b);
//    DISP_ST7789_WriteDat_16Bit(Addr1);  //送行地址开始
//    DISP_ST7789_WriteDat_16Bit(Addr2);      //送行地址结束
//    DISP_ST7789_WriteCmd(0x2c);
//
//#elif defined(HITSIC_DISP_USE_IPS_114) && (HITSIC_DISP_USE_IPS_114 > 0)
//
//#else
//#error DISP_ST7789: Please select screen type !
//#endif
//}

void DISP_ST7789_SetRegion(uint16_t col_addr1, uint16_t row_addr1, uint16_t col_addr2, uint16_t row_addr2)
{
#if defined(HITSIC_DISP_USE_IPS_154) && (HITSIC_DISP_USE_IPS_154 > 0)



#if defined(HITSIC_DISP_ST7789_VERTICAL) && (HITSIC_DISP_ST7789_VERTICAL > 0)
#if defined(HITSIC_DISP_ST7789_REVERSE) && (HITSIC_DISP_ST7789_REVERSE > 0)
    DISP_ST7789_WriteCmd(0x2a);//列地址设置
    DISP_ST7789_WriteDat_16Bit(col_addr1);
    DISP_ST7789_WriteDat_16Bit(col_addr2);
    DISP_ST7789_WriteCmd(0x2b);//行地址设置
    DISP_ST7789_WriteDat_16Bit(row_addr1+80);
    DISP_ST7789_WriteDat_16Bit(row_addr2+80);
    DISP_ST7789_WriteCmd(0x2c);//储存器写
#else // ! HITSIC_DISP_ST7789_REVERSE
    DISP_ST7789_WriteCmd(0x2a);//列地址设置
    DISP_ST7789_WriteDat_16Bit(col_addr1);
    DISP_ST7789_WriteDat_16Bit(col_addr2);
    DISP_ST7789_WriteCmd(0x2b);//行地址设置
    DISP_ST7789_WriteDat_16Bit(row_addr1);
    DISP_ST7789_WriteDat_16Bit(row_addr2);
    DISP_ST7789_WriteCmd(0x2c);//储存器写
#endif // ! HITSIC_DISP_ST7789_REVERSE
#else// ! HITSIC_DISP_ST7789_VERTICAL
#if defined(HITSIC_DISP_ST7789_REVERSE) && (HITSIC_DISP_ST7789_REVERSE > 0)
    DISP_ST7789_WriteCmd(0x2a);//列地址设置
    DISP_ST7789_WriteDat_16Bit(col_addr1+80);
    DISP_ST7789_WriteDat_16Bit(col_addr2+80);
    DISP_ST7789_WriteCmd(0x2b);//行地址设置
    DISP_ST7789_WriteDat_16Bit(row_addr1);
    DISP_ST7789_WriteDat_16Bit(row_addr2);
    DISP_ST7789_WriteCmd(0x2c);//储存器写
#else // ! HITSIC_DISP_ST7789_REVERSE
    DISP_ST7789_WriteCmd(0x2a);//列地址设置
    DISP_ST7789_WriteDat_16Bit(col_addr1);
    DISP_ST7789_WriteDat_16Bit(col_addr2);
    DISP_ST7789_WriteCmd(0x2b);//行地址设置
    DISP_ST7789_WriteDat_16Bit(row_addr1);
    DISP_ST7789_WriteDat_16Bit(row_addr2);
    DISP_ST7789_WriteCmd(0x2c);//储存器写
#endif // ! HITSIC_DISP_ST7789_REVERSE
#endif // ! HITSIC_DISP_ST7789_VERTICAL
#endif // ! HITSIC_DISP_USE_IPS_154


#if defined(HITSIC_DISP_USE_IPS_114) && (HITSIC_DISP_USE_IPS_114 > 0)



#if defined(HITSIC_DISP_ST7789_VERTICAL) && (HITSIC_DISP_ST7789_VERTICAL > 0)
#if defined(HITSIC_DISP_ST7789_REVERSE) && (HITSIC_DISP_ST7789_REVERSE > 0)
    DISP_ST7789_WriteCmd(0x2a);//列地址设置
    DISP_ST7789_WriteDat_16Bit(col_addr1+53);
    DISP_ST7789_WriteDat_16Bit(col_addr2+53);
    DISP_ST7789_WriteCmd(0x2b);//行地址设置
    DISP_ST7789_WriteDat_16Bit(row_addr1+40);
    DISP_ST7789_WriteDat_16Bit(row_addr2+40);
    DISP_ST7789_WriteCmd(0x2c);//储存器写
#else // ! HITSIC_DISP_ST7789_REVERSE
    DISP_ST7789_WriteCmd(0x2a);//列地址设置
    DISP_ST7789_WriteDat_16Bit(col_addr1+52);
    DISP_ST7789_WriteDat_16Bit(col_addr2+52);
    DISP_ST7789_WriteCmd(0x2b);//行地址设置
    DISP_ST7789_WriteDat_16Bit(row_addr1+40);
    DISP_ST7789_WriteDat_16Bit(row_addr2+40);
    DISP_ST7789_WriteCmd(0x2c);//储存器写
#endif // ! HITSIC_DISP_ST7789_REVERSE
#else // ! HITSIC_DISP_ST7789_VERTICAL
#if defined(HITSIC_DISP_ST7789_REVERSE) && (HITSIC_DISP_ST7789_REVERSE > 0)
    DISP_ST7789_WriteCmd(0x2a);//列地址设置
    DISP_ST7789_WriteDat_16Bit(col_addr1+40);
    DISP_ST7789_WriteDat_16Bit(col_addr2+40);
    DISP_ST7789_WriteCmd(0x2b);//行地址设置
    DISP_ST7789_WriteDat_16Bit(row_addr1+52);
    DISP_ST7789_WriteDat_16Bit(row_addr2+52);
    DISP_ST7789_WriteCmd(0x2c);//储存器写
#else // ! HITSIC_DISP_ST7789_REVERSE
    DISP_ST7789_WriteCmd(0x2a);//列地址设置
    DISP_ST7789_WriteDat_16Bit(col_addr1+40);
    DISP_ST7789_WriteDat_16Bit(col_addr2+40);
    DISP_ST7789_WriteCmd(0x2b);//行地址设置
    DISP_ST7789_WriteDat_16Bit(row_addr1+53);
    DISP_ST7789_WriteDat_16Bit(row_addr2+53);
    DISP_ST7789_WriteCmd(0x2c);//储存器写
#endif // ! HITSIC_DISP_ST7789_REVERSE
#endif // ! HITSIC_DISP_ST7789_VERTICAL
#endif // ! HITSIC_DISP_USE_IPS_114
}

disp_st7789_fb_pixel_t DISP_ST7789_ColorCoding(disp_st7789_fb_pixel_t r, disp_st7789_fb_pixel_t g, disp_st7789_fb_pixel_t b)
{
    disp_st7789_fb_pixel_t color = 0;
    color |= (r & 0xfcU) << 24;
    color |= (g & 0xfcU) << 16;
    color |= (g & 0xfcU) << 8;
    return color;
}

void DISP_ST7789_Fill(disp_st7789_fb_pixel_t _color)
{
    DISP_ST7789_SetRegion(0, 0, disp_st7789_fb_col - 1, disp_st7789_fb_row - 1);
	for(uint16_t i=0;i<disp_st7789_fb_row;i++)
	{													   	 	
		for(uint16_t j=disp_st7789_fb_col;j<disp_st7789_fb_col;j++)
		{
			DISP_ST7789_WriteDat_24Bit(_color);
		}
	} 
}

void DISP_ST7789_Init(void)
{
    DISP_SPIBUS_gpioSetRST(0);
    DISP_SPIBUS_delay_ms(100);
    
    DISP_SPIBUS_gpioSetRST(1);
    DISP_SPIBUS_delay_ms(100);

    DISP_SPIBUS_gpioSetBLK(1);
    DISP_SPIBUS_delay_ms(100);

    DISP_ST7789_WriteCmd(0x11); //Sleep out
    DISP_SPIBUS_delay_ms(100);  //Delay 100ms
	
    DISP_ST7789_WriteCmd(0x36);
    

#if defined(HITSIC_DISP_ST7789_VERTICAL) && (HITSIC_DISP_ST7789_VERTICAL > 0)
#if defined(HITSIC_DISP_ST7789_REVERSE) && (HITSIC_DISP_ST7789_REVERSE > 0)
    DISP_ST7789_WriteDat_8Bit(0xC0);
#else 
    DISP_ST7789_WriteDat_8Bit(0x00);    
#endif // ! HITSIC_DISP_ST7789_REVERSE
#else // ! HITSIC_DISP_ST7789_VERTICAL
#if defined(HITSIC_DISP_ST7789_REVERSE) && (HITSIC_DISP_ST7789_REVERSE > 0)
    DISP_ST7789_WriteDat_8Bit(0xA0);
#else
    DISP_ST7789_WriteDat_8Bit(0x70);
#endif // ! HITSIC_DISP_ST7789_REVERSE

#endif // ! HITSIC_DISP_ST7789_VERTICAL
    DISP_ST7789_WriteCmd(0x3A); 
    DISP_ST7789_WriteDat_8Bit(0x06);

    DISP_ST7789_WriteCmd(0xB2);
    DISP_ST7789_WriteDat_8Bit(0x0C);
    DISP_ST7789_WriteDat_8Bit(0x0C);
    DISP_ST7789_WriteDat_8Bit(0x00);
    DISP_ST7789_WriteDat_8Bit(0x33);
    DISP_ST7789_WriteDat_8Bit(0x33); 

    DISP_ST7789_WriteCmd(0xB7); 
    DISP_ST7789_WriteDat_8Bit(0x35);  

    DISP_ST7789_WriteCmd(0xBB);
    DISP_ST7789_WriteDat_8Bit(0x32);

    DISP_ST7789_WriteCmd(0xC0);
    DISP_ST7789_WriteDat_8Bit(0x2C);

    DISP_ST7789_WriteCmd(0xC2);
    DISP_ST7789_WriteDat_8Bit(0x01);

    DISP_ST7789_WriteCmd(0xC3);
    DISP_ST7789_WriteDat_8Bit(0x15);   

    DISP_ST7789_WriteCmd(0xC4);
    DISP_ST7789_WriteDat_8Bit(0x20);  

    DISP_ST7789_WriteCmd(0xC6); 
    DISP_ST7789_WriteDat_8Bit(0x0F);    

    DISP_ST7789_WriteCmd(0xD0); 
    DISP_ST7789_WriteDat_8Bit(0xA4);
    DISP_ST7789_WriteDat_8Bit(0xA1);

#if defined(HITSIC_DISP_USE_IPS_154) && (HITSIC_DISP_USE_IPS_154 > 0)
	DISP_ST7789_WriteCmd(0xE0);
	DISP_ST7789_WriteDat_8Bit(0xD0);   
	DISP_ST7789_WriteDat_8Bit(0x08);   
	DISP_ST7789_WriteDat_8Bit(0x0E);   
	DISP_ST7789_WriteDat_8Bit(0x09);   
	DISP_ST7789_WriteDat_8Bit(0x09);   
	DISP_ST7789_WriteDat_8Bit(0x05);   
	DISP_ST7789_WriteDat_8Bit(0x31);   
	DISP_ST7789_WriteDat_8Bit(0x33);   
	DISP_ST7789_WriteDat_8Bit(0x48);   
	DISP_ST7789_WriteDat_8Bit(0x17);   
	DISP_ST7789_WriteDat_8Bit(0x14);   
	DISP_ST7789_WriteDat_8Bit(0x15);   
	DISP_ST7789_WriteDat_8Bit(0x31);   
	DISP_ST7789_WriteDat_8Bit(0x34);   

	DISP_ST7789_WriteCmd(0xE1);     
	DISP_ST7789_WriteDat_8Bit(0xD0);   
	DISP_ST7789_WriteDat_8Bit(0x08);   
	DISP_ST7789_WriteDat_8Bit(0x0E);   
	DISP_ST7789_WriteDat_8Bit(0x09);   
	DISP_ST7789_WriteDat_8Bit(0x09);   
	DISP_ST7789_WriteDat_8Bit(0x15);   
	DISP_ST7789_WriteDat_8Bit(0x31);   
	DISP_ST7789_WriteDat_8Bit(0x33);   
	DISP_ST7789_WriteDat_8Bit(0x48);   
	DISP_ST7789_WriteDat_8Bit(0x17);   
	DISP_ST7789_WriteDat_8Bit(0x14);   
	DISP_ST7789_WriteDat_8Bit(0x15);   
	DISP_ST7789_WriteDat_8Bit(0x31);   
	DISP_ST7789_WriteDat_8Bit(0x34);
#endif // ! HITSIC_DISP_USE_IPS_154

#if defined(HITSIC_DISP_USE_IPS_114) && (HITSIC_DISP_USE_IPS_114 > 0)
	DISP_ST7789_WriteCmd(0xE0);
	DISP_ST7789_WriteDat_8Bit(0xD0);
	DISP_ST7789_WriteDat_8Bit(0x04);
	DISP_ST7789_WriteDat_8Bit(0x0D);
	DISP_ST7789_WriteDat_8Bit(0x11);
	DISP_ST7789_WriteDat_8Bit(0x13);
	DISP_ST7789_WriteDat_8Bit(0x2B);
	DISP_ST7789_WriteDat_8Bit(0x3F);
	DISP_ST7789_WriteDat_8Bit(0x54);
	DISP_ST7789_WriteDat_8Bit(0x4C);
	DISP_ST7789_WriteDat_8Bit(0x18);
	DISP_ST7789_WriteDat_8Bit(0x0D);
	DISP_ST7789_WriteDat_8Bit(0x0B);
	DISP_ST7789_WriteDat_8Bit(0x1F);
	DISP_ST7789_WriteDat_8Bit(0x23);

	DISP_ST7789_WriteCmd(0xE1);
	DISP_ST7789_WriteDat_8Bit(0xD0);
	DISP_ST7789_WriteDat_8Bit(0x04);
	DISP_ST7789_WriteDat_8Bit(0x0C);
	DISP_ST7789_WriteDat_8Bit(0x11);
	DISP_ST7789_WriteDat_8Bit(0x13);
	DISP_ST7789_WriteDat_8Bit(0x2C);
	DISP_ST7789_WriteDat_8Bit(0x3F);
	DISP_ST7789_WriteDat_8Bit(0x44);
	DISP_ST7789_WriteDat_8Bit(0x51);
	DISP_ST7789_WriteDat_8Bit(0x2F);
	DISP_ST7789_WriteDat_8Bit(0x1F);
	DISP_ST7789_WriteDat_8Bit(0x1F);
	DISP_ST7789_WriteDat_8Bit(0x20);
	DISP_ST7789_WriteDat_8Bit(0x23);
#endif // ! HITSIC_DISP_USE_IPS_114

	DISP_ST7789_WriteCmd(0x21); 
    DISP_ST7789_WriteCmd(0x29);
    DISP_ST7789_Fill(0);
}

void DISP_ST7789_BufferUpload(disp_st7789_fb_t *_fb)
{
    DISP_ST7789_SetRegion(0, 0, disp_st7789_fb_col - 1, disp_st7789_fb_row - 1);
    DISP_SPIBUS_gpioSetD_C(1);
    DISP_SPIBUS_spiWrite((uint8_t*)_fb, sizeof(disp_st7789_fb_t));
}

#endif // ! HITSIC_USE_DISP_ST7789
