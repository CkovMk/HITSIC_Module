#ifndef D_MK66F18_DRV_CAM_ZF9V034_PORT_H
#define D_MK66F18_DRV_CAM_ZF9V034_PORT_H

#include "hitsic_common.h"

#if (defined(HITSIC_USE_CAM_ZF9V034) && (HITSIC_USE_CAM_ZF9V034 > 0))

/*!
 * @addtogroup cam_zf9v034
 * @{
 */

#define HITSIC_ZF9V034_LOG_LVL  (3U)

#define ZF9V034_UART_INST (UART3)

/**
 * @brief 阻塞同步串口接收函数。
 * 
 * @param data 数据指针
 * @param length 数据长度
 * @return status_t 返回状态，成功时返回kStatus_Success。
 */
inline status_t CAM_ZF9V034_UartRxBlocking(uint8_t *data, uint32_t length)
{
    return UART_ReadBlocking(ZF9V034_UART_INST, data, length);
}

/**
 * @brief 阻塞同步串口发送函数。
 * 
 * @param data 数据指针
 * @param length 数据长度
 * @return status_t 返回状态，成功时返回kStatus_Success。
 */
inline status_t CAM_ZF9V034_UartTxBlocking(const uint8_t *data, uint32_t length)
{
    return UART_WriteBlocking(ZF9V034_UART_INST, data, length);
}

/**
 * @brief 毫秒级延迟函数
 * 
 * @param ms 要延迟的毫秒数
 */
inline void CAM_ZF9V034_Delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; ++i)
    {
        SDK_DelayAtLeastUs(1000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}

#ifndef ZF9V034_USE_DMADVP
#define ZF9V034_USE_DMADVP (1U)
#endif // ! ZF9V034_USE_DMADVP

#ifndef ZF9V034_USE_RTCSI
#define ZF9V034_USE_RTCSI (0U)
#endif // ! ZF9V034_USE_RTCSI

/* @} */

#endif // ! HITSIC_USE_CAM_ZF9V034

#endif // ! D_MK66F18_DRV_CAM_ZF9V034_PORT_H
