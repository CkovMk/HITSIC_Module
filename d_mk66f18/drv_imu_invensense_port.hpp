/**
 * @brief 陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 * @author xiao qq1761690868
 * @doc drv_imu_invensense.md
 * @version v1.0
 * @date 2020-10-16
 */

#ifndef D_MK66F18_DRV_IMU_INVENSENSE_PORT_HPP
#define D_MK66F18_DRV_IMU_INVENSENSE_PORT_HPP

#define HITSIC_INV_IMU_DEBUG 1
#define HITSIC_INV_YES_TRACE 0
#define HITSIC_INV_NO_DEBUG 1
#include "hitsic_common.h"
#define INV_PRINTF PRINTF   // FIXME: HITSICC_INV_PRINTF(...)

#if (defined(HITSIC_USE_DRV_IMU_INV) && (HITSIC_USE_DRV_IMU_INV > 0U))

//TODO:port this file. change the names.
int IMU_INV_I2cRxBlocking(void *userData,
                    uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);

int IMU_INV_I2cTxBlocking(void *userData,
                     uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len);

int IMU_INV_I2cTxNonBlocking(void *userData,
                 uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);

#endif // ! HITSIC_USE_DRV_IMU_INV

#endif //D_MK66F18_DRV_IMU_INVENSENSE_PORT_HPP
