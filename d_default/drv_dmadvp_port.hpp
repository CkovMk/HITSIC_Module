#ifndef D_DEFAULT_DRV_DMADVP_PORT_HPP
#define D_DEFAULT_DRV_DMADVP_PORT_HPP

#include "hitsic_common.h"

#if defined(HITSIC_USE_DMADVP) && (HITSIC_USE_DMADVP > 0)
#include "sys_extint.hpp"

/*!
 * @addtogroup dmadvp
 * @{
 */

#define kStatusGroup_DMADVP (200U)

struct DMADVP_Type
{
    INTC_Type* vsnc_intc;               /*!< 场中断中断设备地址 */
    uint32_t vsnc_pin;                  /*!< 场中断引脚号 */
    INTC_Type* pclk_intc;               /*!< 像素时钟中断设备地址 */
    uint32_t pclk_pin;                  /*!< 像素时钟引脚号 */

    uint32_t dmaDataAddress;            /*!< DMA数据源地址 */
    uint32_t dmaChannel;                /*!< 使用的DMA通道*/
    uint32_t dmauxChannel;              /*!< 使用的DMAMUX通道号 */
    uint32_t dmamuxRequestSrc;          /*!< DMAMUX请求源 */
    IRQn_Type dmaIrqn;                  /*!< DMA中断号 */
    uint32_t dmaIrqPrio;                /*!< DMA中断优先级 */

    uint32_t imgSize;                   /*!< 图像大小，单位：字节，无需配置 */

    port_interrupt_t vsncInterruptCfg;  /*!< 场中断外部中断方式设置，无需配置 */
    port_interrupt_t pclkInterruptCfg;  /*!< 像素时钟DMA触发方式设置，无需配置 */
};

#define DMADVP_CNT (1U)

#define DMADVP0_DAT0_GPIO          RTEPIN_DIGITAL_CAM_D_0_GPIO
#define DMADVP0_DAT0_PORT          RTEPIN_DIGITAL_CAM_D_0_PORT
#define DMADVP0_DAT0_PIN           RTEPIN_DIGITAL_CAM_D_0_PIN
#define DMADVP0_DMA_INST           (DMA0)
#define DMADVP0_DMAMUX_INST        (DMAMUX0)
#define DMADVP0_DATA_DMA_ADDR      (((uint32_t)DMADVP0_DAT0_GPIO)+0x10+DMADVP0_DAT0_PIN/8)
extern DMADVP_Type __DMADVP0;
#define DMADVP0 (&__DMADVP0)

inline void DMADVP_Delay_ms(uint32_t ms)
{
	for(uint32_t i = 0; i < ms; ++i)
    {
        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}

inline uint32_t DMADVP_GetInstance(DMADVP_Type* base)
{
    if((uint32_t)base == (uint32_t)DMADVP0)
    {
        return 0;
    }
    else
    {
        assert(0);//should NEVER end up in here!
    }
}

/* @} */

#endif // ! HITSIC_USE_DMADVP

#endif // ! D_DEFAULT_DRV_DMADVP_PORT_HPP
