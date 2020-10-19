#ifndef D_MK66F18_DRV_EDMADVP_PORT_HPP
#define D_MK66F18_DRV_EDMADVP_PORT_HPP

#include "hitsic_common.h"

/** port */

#if defined(HITSIC_USE_EDMADVP) && (HITSIC_USE_EDMADVP > 0)

#define EDMADVP_CNT (1U);

#define EDMADVP0_DAT0_GPIO		RTEPIN_BOARD_CAM_D0_GPIO
#define EDMADVP0_DAT0_PORT		RTEPIN_BOARD_CAM_D0_PORT
#define EDMADVP0_DAT0_PIN		RTEPIN_BOARD_CAM_D0_PIN

#define EDMADVP0_DMA_INST        (DMA0)
#define EDMADVP0_DMAMUX_INST        (DMA0)
#define EDMADVP0_DATA_DMA_ADDR		(((uint32_t)RTEPIN_BOARD_CAM_D0_GPIO)+0x10+RTEPIN_BOARD_CAM_D0_PIN/8)

#define kStatusGroup_EDMADVP (200U)

struct EDMADVP_Type
{
    INTC_Type* vsnc_intc;
    uint32_t vsnc_pin
    INTC_Type* pclk_intc;
    uint32_t pclk_pin;

    uint32_t dmaDataAddress;
    uint32_t dmaChannel;
    uint32_t dmauxChannel;
    uint32_t dmamuxRequestSrc;
    uint32_t dmaIrqn;
    uint32_t dmaIrqPrio;

    uint32_t imgSize;

    port_interrupt_t vsncInterruptCfg;
    port_interrupt_t pclkInterruptCfg;
};

EDMADVP_Type __EDMADVP0 = 
{
    .vsnc_intc = RTEPIN_BOARD_CAM_VSNC_PORT,
    .vsnc_pin = RTEPIN_BOARD_CAM_VSNC_PIN,
    .pclk_intc = RTEPIN_BOARD_CAM_PCLK_PORT,
    .pclk_pin = RTEPIN_BOARD_CAM_PCLK_PIN,

    .dmaDataAddress = EDMADVP0_DATA_DMA_ADDR,
    .dmaChannel = 0U,
    .dmauxChannel = 0U,
    .dmamuxRequestSrc = 0U;
    .dmaIrqn = DMA0_DMA16_IRQn,
    .dmaIrqPrio = 2U,

    .imgSize = 0U,

    .vsncInterruptCfg = kPORT_InterruptFallingEdge;
    .pclkInterruptCfg = kPORT_DMARisingEdge;
};

static const EDMADVP_Type* EDMADVP0 = &__EDMADVP0;

inline void EDMADVP_Delay_ms(uint32_t ms)
{
	for(int i = 0; i < ms, ++i)
    {
        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}

inline uint32_t EDMADVP_GetInstance(EDMADVP_Type* base)
{
    switch(base)
    {
    case EDMADVP0:
        return 0;
    default:
        assert(0);//should NEVER end up in here!
    }
}

#endif // ! HITSIC_USE_EDMADVP
/** ! port */

#endif // ! D_MK66F18_DRV_EDMADVP_PORT_HPP
