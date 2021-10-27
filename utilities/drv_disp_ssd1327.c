#include <drv_disp_ssd1327.h>

#if defined(HITSIC_USE_DISP_SSD1327) && (HITSIC_USE_DISP_SSD1327 > 0)

#define SYSLOG_TAG  ("DISP_1327")
#define SYSLOG_LVL  (3U)
#include <inc_syslog.h>

void DISP_SSD1327_WriteDat(uint8_t data)
{
    DISP_SPIBUS_gpioSetD_C(1);
    DISP_SPIBUS_spiWrite(&data, 1U);
}
void DISP_SSD1327_WriteCmd(uint8_t cmd)
{
    DISP_SPIBUS_gpioSetD_C(0);
    DISP_SPIBUS_spiWrite(&cmd, 1U);
}

void DISP_SSD1327_SetColAddress(uint8_t Addr1, uint8_t Addr2)
{
    DISP_SSD1327_WriteCmd(0x15);
    DISP_SSD1327_WriteCmd(Addr1);  //送列地址开始
    DISP_SSD1327_WriteCmd(Addr2);      //送列地址结束
}

//=======写行地址==============
void DISP_SSD1327_SetRowAddress(uint8_t Addr1, uint8_t Addr2)
{
    DISP_SSD1327_WriteCmd(0x75);
    DISP_SSD1327_WriteCmd(Addr1);  //送行地址开始
    DISP_SSD1327_WriteCmd(Addr2);      //送行地址结束
}


void DISP_SSD1327_Init(void)
{
    SYSLOG_I("Init Begin. v%d.%d.%d",HITSIC_VERSION_MAJOR(DRV_DISP_SSD1327_VERSION),
                HITSIC_VERSION_MINOR(DRV_DISP_SSD1327_VERSION), HITSIC_VERSION_PATCH(DRV_DISP_SSD1327_VERSION));
    DISP_SPIBUS_gpioSetRST(1);
    DISP_SPIBUS_delay_ms(1);
    DISP_SPIBUS_gpioSetRST(0);
    DISP_SPIBUS_delay_ms(50);
    DISP_SPIBUS_gpioSetRST(1);

    DISP_SSD1327_WriteCmd(0xae);//Set display off
    DISP_SSD1327_WriteCmd(0xa0);//Set re-map

#if defined(HITSIC_DISP_SSD1327_REVERSE) && (HITSIC_DISP_SSD1327_REVERSE > 0)
    DISP_SSD1327_WriteCmd(0x40); //0x66旋转180度 0x55正常
#else // ! HITSIC_DISP_SSD1327_REVERSE
    DISP_SSD1327_WriteCmd(0x53); //0x66旋转180度 0x55正常
#endif// ! HITSIC_DISP_SSD1327_REVERSE

    DISP_SSD1327_WriteCmd(0xa1);//Set display start line
    DISP_SSD1327_WriteCmd(0x00);
    DISP_SSD1327_WriteCmd(0xa2);//Set display offset
    DISP_SSD1327_WriteCmd(0x00);
    DISP_SSD1327_WriteCmd(0xa4);//Normal Display
    DISP_SSD1327_WriteCmd(0xa8);//Set multiplex ratio
    DISP_SSD1327_WriteCmd(0x7f);
    DISP_SSD1327_WriteCmd(0xab);//Function Selection A
    DISP_SSD1327_WriteCmd(0x01);//Enable internal VDD regulator
    DISP_SSD1327_WriteCmd(0x81);//Set contrast
    DISP_SSD1327_WriteCmd(0x77);
    DISP_SSD1327_WriteCmd(0xb1);//Set Phase Length
    DISP_SSD1327_WriteCmd(0x31);
    DISP_SSD1327_WriteCmd(0xb3);//Set Front Clock Divider /Oscillator Frequency
    DISP_SSD1327_WriteCmd(0xb1);
    DISP_SSD1327_WriteCmd(0xb4); //For brightness enhancement
    DISP_SSD1327_WriteCmd(0xb5);
    DISP_SSD1327_WriteCmd(0x03);
    DISP_SSD1327_WriteCmd(0xb6);//Set Second pre-charge Period
    DISP_SSD1327_WriteCmd(0x0d);
    DISP_SSD1327_WriteCmd(0xbc);//Set Pre-charge voltage
    DISP_SSD1327_WriteCmd(0x07);
    DISP_SSD1327_WriteCmd(0xbe);//Set VCOMH
    DISP_SSD1327_WriteCmd(0x07);
    DISP_SSD1327_WriteCmd(0xd5);//Function Selection B
    DISP_SSD1327_WriteCmd(0x02);//Enable second pre-charge
    DISP_SSD1327_WriteCmd(0xaf);//Display on

#if defined(HITSIC_DISP_SSD1306_DMA) && (HITSIC_DISP_SSD1306_DMA > 0U)
    DISP_SPIBUS_spiDmaInit();
    SYSLOG_D("DMA API enabled.");
#else
    SYSLOG_D("DMA API disbled.");
#endif // ! HITSIC_DISP_SSD1306_DMA

    SYSLOG_I("Init Comlpete.");

}

void DPSI_SSD1327_DispEnable(bool _state)
{
    if(_state)
    {
        DISP_SSD1327_WriteCmd(0xAF);//OLED开户推出休眠模?
        DISP_SSD1327_WriteCmd(0xb5);//通过设置SSD1327 GPIO来控制升压电路的使能管脚
        DISP_SSD1327_WriteCmd(0x03);//0X03 GPIO输出为高电平，打开使能
        DISP_SSD1327_Fill(0);
    }
    else
    {
        DISP_SSD1327_WriteCmd(0xAE);//OLED关闭进入休眠模式
        DISP_SSD1327_WriteCmd(0xb5);//通过设置SSD1327 GPIO来控制升压电路的使能管脚
        DISP_SSD1327_WriteCmd(0x02);//0X02 GPIO输出为低电平平，禁止使能
    }
}

void DISP_SSD1327_Fill(disp_ssd1327_fb_pixel_t _color)
{
    uint8_t data = _color | (_color << 4U);

    //TODO
    uint8_t j,i;
    //Column Address
    DISP_SSD1327_SetColAddress(0,63);
    // Row Address
    DISP_SSD1327_SetRowAddress(0,127);
    for (j=0;j<128;j++)        /* 64 row */
    {
        for (i=0;i<64;i++)    /* 64*2=128 column  a nibble of command is a dot*/
        {
            DISP_SPIBUS_delay_us(1);
            DISP_SSD1327_WriteDat(data);
        }
    }

}

void DISP_SSD1327_SetPixel(uint8_t x, uint8_t y, disp_ssd1327_fb_pixel_t _color)
{
    //Column Address
    //DISP_SSD1327_SetColAddress(x >> 1,x >> 1);
    // Row Address
    //DISP_SSD1327_SetRowAddress(y,y);
}

void DISP_SSD1327_BufferUpload(disp_ssd1327_fb_t *_fb)
{
    // Column Address
    DISP_SSD1327_SetColAddress(0,63);
    // Row Address
    DISP_SSD1327_SetRowAddress(0,127);

    DISP_SPIBUS_gpioSetD_C(1);
    DISP_SPIBUS_spiWrite((uint8_t*)_fb, sizeof(disp_ssd1327_fb_t));
}


#endif // ! HITSIC_USE_DISP_SSD1327


