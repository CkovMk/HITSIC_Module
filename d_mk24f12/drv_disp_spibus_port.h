#ifndef D_MK24F12_DRVOLED_H
#define D_MK24F12_DRVOLED_H
#include <sys_pitmgr.h>
#include <inc_stdlib.h>
#include "hitsic_common.h"
#include "pin_mux.h"

#define HITSIC_DISP_SSD1306_FLIP_X (0U)	///< 屏幕左右翻转
#define HITSIC_DISP_SSD1306_FLIP_Y (0U)	///< 屏幕上下翻转

#define HITSIC_DISP_SSD1327_REVERSE 	(0U) 	///< 屏幕旋转180度

#define OLED_SPI_BASE			DISP_SPI_PERIPHERAL
#define OLED_SPI_CLKFREQ 		DISP_SPI_CLK_FREQ
#define OLED_SPI_Pcsn		    kDSPI_Pcs1
#define OLED_SPI_MasterPcsn		kDSPI_MasterPcs1
#define OLED_SPI_Ctarn			kDSPI_Ctar0
#define OLED_SPI_MasterCtarn	kDSPI_MasterCtar0

#if defined(HITSIC_DISP_SSD1306_DMA) && (HITSIC_DISP_SSD1306_DMA > 0U)
#define OLED_SPI_DMA_RX_Chnl    (5U)
#define OLED_SPI_DMA_IM_Chnl    (6U)
#define OLED_SPI_DMA_TX_Chnl    (7U)
#define OLED_SPI_DMA_RX_REQSRC kDmaRequestMux0SPI0Rx
#define OLED_SPI_DMA_TX_REQSRC  kDmaRequestMux0SPI2Tx
#endif // ! HITSIC_DISP_SSD1306_DMA
/**
 * @brief 设置RST脚电平。
 * 
 * @param x RST脚的电平状态，正逻辑。
 */
inline void DISP_SPIBUS_gpioSetRST(uint8_t x)
{
	GPIO_PinWrite(RTEPIN_COMMUNICATION_DISP_RST_GPIO, RTEPIN_COMMUNICATION_DISP_RST_PIN, x);
}

/**
 * @brief 设置D/C脚电平。
 * 
 * @param x D/C脚的电平状态，正逻辑。
 */
inline void DISP_SPIBUS_gpioSetD_C(uint8_t x)
{
	GPIO_PinWrite(RTEPIN_COMMUNICATION_DISP_DC_GPIO, RTEPIN_COMMUNICATION_DISP_DC_PIN, x);
}

/**
 * @brief 毫秒级阻塞延迟函数。
 * 
 * @param ms 延迟的毫秒数。
 */
inline void DISP_SPIBUS_delay_ms(uint32_t ms)
{
	for(uint32_t i = 0; i < ms; ++i)
	{
        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	}
}

/**
 * @brief 微秒级阻塞延迟函数。
 *
 * @param ms 延迟的微秒数。
 */
inline void DISP_SPIBUS_delay_us(uint32_t us)
{
	for(uint32_t i = 0; i < us; ++i)
	{
        SDK_DelayAtLeastUs(1, CLOCK_GetFreq(kCLOCK_CoreSysClk));
	}
}

/**
 * @brief SPI接口发送多个字节。
 * 
 * @param data 要发送的数据
 * @param size 数据大小
 */
status_t DISP_SPIBUS_spiWrite(uint8_t *data, uint32_t size);

/**
 * @name DMA传输
 * @ {
 */
/// 启用DMA传输API
#ifndef HITSIC_DISP_SSD1306_DMA
#define HITSIC_DISP_SSD1306_DMA (0U)
#endif // ! HITSIC_DISP_SSD1306_DMA

#if defined(HITSIC_DISP_SSD1306_DMA) && (HITSIC_DISP_SSD1306_DMA > 0U)
/**
 * @brief 初始化SPI DMA传输
 */
status_t DISP_SPIBUS_spiDmaInit(void);


/**
 * @brief SPI接口DMA发送。
 *
 * @param data 要发送的数据
 * @param size 数据长度
 */
status_t DISP_SPIBUS_spiDmaWrite(uint8_t* data, uint32_t size);

#endif // ! HITSIC_DISP_SSD1306_DMA

/* @ } */

#endif // ! D_MK24F12_DRVOLED_H
