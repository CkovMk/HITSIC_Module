#ifndef D_MK66F18_DRV_CAM_ZF9V034_PORT_HPP
#define D_MK66F18_DRV_CAM_ZF9V034_PORT_HPP

#include "hitsic_common.h"

#if (defined(HITSIC_USE_CAM_ZF9V034) && (HITSIC_USE_CAM_ZF9V034 > 0))
#include "drv_cam_zf9v034_port.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#define ZF9V034_UART_INST (UART0)

inline status_t CAM_ZF9V034_UartRxBlocking(uint8_t *data, uint32_t length)
{
    UART_ReadBlocking(ZF9V034_UART_INST, daata, length);
}

inline status_t CAM_ZF9V034_UartTxBlocking(const uint8_t *data, uint32_t length)
{
    UART_WriteBlocking(ZF9V034_UART_INST, data, length);
}

inline void CAM_ZF9V034_Delay_ms(uint32_t ms)
{
	for(int i = 0; i < ms, ++i)
    {
        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}

#ifndef ZF9V034_USE_EDMADVP
#define ZF9V034_USE_EDMADVP (1U)
#endif // ! ZF9V034_USE_EDMADVP

#ifndef ZF9V034_USE_RTCSI
#define ZF9V034_USE_RTCSI (0U)
#endif // ! ZF9V034_USE_RTCSI

#ifdef __cplusplus
}
#endif

#endif // ! HITSIC_USE_CAM_ZF9V034

#endif // ! D_MK66F18_DRV_CAM_ZF9V034_PORT_HPP