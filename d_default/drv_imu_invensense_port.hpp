/**
 * @brief 陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 * @author xiao qq1761690868
 * @doc drv_imu_invensense.md
 * @version v1.0
 * @date 2020-10-16
 */

#ifndef DRV_IMU_INVENSENSE_PORT_HPP
#define DRV_IMU_INVENSENSE_PORT_HPP

#define HITSIC_INV_IMU_DEBUG 1
#define HITSIC_INV_YES_TRACE 0
#define HITSIC_INV_NO_DEBUG 1
#include <cstdio>
#define INV_PRINTF printf


#include <cstdint>

int MyImuI2cRead(void *context,
                    uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);

int MyImuI2cWrite(void *context,
                     uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len);

int MyImuI2cReadNonBlocking(void *context,
                 uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);


#endif //DRV_IMU_INVENSENSE_PORT_HPP
