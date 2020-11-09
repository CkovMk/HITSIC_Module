/**
 * @brief 陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 * @author xiao qq1761690868
 * @doc drv_imu_invensense.md
 * @version v1.0
 * @date 2020-10-16
 */

#ifndef D_MK66F18_DRV_IMU_INVENSENSE_PORT_HPP
#define D_MK66F18_DRV_IMU_INVENSENSE_PORT_HPP


#include "hitsic_common.h"

#if (defined(HITSIC_USE_DRV_IMU_INV) && (HITSIC_USE_DRV_IMU_INV > 0U))

#define HITSIC_INV_IMU_DEBUG 1
#define HITSIC_INV_YES_TRACE 0
#define HITSIC_INV_NO_DEBUG 1

#define INV_PRINTF PRINTF

inline int IMU_INV_I2cRxBlocking(void *userData,
                    uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len)
{
    return HAL_I2C_Mem_ReadBlocking(I2C0, addr, reg, 1U, val, len);
}
inline int IMU_INV_I2cTxBlocking(void *userData,
                     uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len)
{
    return HAL_I2C_Mem_WriteBlocking(I2C0, addr, reg, 1U, val, len);
}

inline int IMU_INV_I2cTxNonBlocking(void *userData,
                 uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len)
{
    return kStatus_Fail;
}

inline void IMU_DelayUs(uint32_t delay_ms)
{
    SDK_DelayAtLeastUs(delay_ms,CLOCK_GetFreq(kCLOCK_CoreSysClk));
}
#endif // ! HITSIC_USE_DRV_IMU_INV

#endif //! D_MK66F18_DRV_IMU_INVENSENSE_PORT_HPP
