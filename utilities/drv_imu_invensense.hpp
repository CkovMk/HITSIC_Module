/**
 * Copyright 2018 - 2020 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @file    :   drv_imu_invensense.hpp
 * @author  :   beforelight/肖日涛(qq:1761690868)
 * @version :   v0.1.0
 *
 * @date    :   v0.1-beta.0 2019.10.16 肖日涛
 *
 * @brief   :   陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 */

#ifndef UTILITIES_DRV_IMU_INVENSENSE_HPP
#define UTILITIES_DRV_IMU_INVENSENSE_HPP
#include "inc_stdlib.hpp"
#include "hitsic_common.h"

#if (defined(HITSIC_USE_DRV_IMU_INV) && (HITSIC_USE_DRV_IMU_INV > 0U))
#include "drv_imu_invensense_port.hpp"
#include "drv_imu_invensense_def.hpp"

/** @brief : 软件版本 */
#define DRV_IMU_INVENSENSE_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))

#ifdef INV_PRINTF

#if (defined(HITSIC_INV_IMU_DEBUG) && (HITSIC_INV_IMU_DEBUG > 0))

#if (defined(HITSIC_INV_YES_TRACE) && (HITSIC_INV_YES_TRACE > 0))
#define INV_TRACE_(fmt, ...) \
    INV_PRINTF("[I]imu_inv line:%d:trace: " fmt "%s\r\n",  __LINE__, __VA_ARGS__)
#define INV_TRACE(...) INV_TRACE_(__VA_ARGS__, "")
#else
#define INV_TRACE(...)
#endif//(defined(HITSIC_INV_YES_TRACE)&&(HITSIC_INV_YES_TRACE>0))

#if !(defined(HITSIC_INV_NO_DEBUG) && (HITSIC_INV_NO_DEBUG > 0))
#define INV_DEBUG_(fmt, ...) \
    INV_PRINTF("[E]imu_inv line:%d:debug: " fmt "%s\r\n",  __LINE__, __VA_ARGS__)
#define INV_DEBUG(...) INV_DEBUG_(__VA_ARGS__, "")
#else
#define INV_DEBUG(...)
#endif// ! HITSIC_INV_NO_DEBUG

#endif// ! HITSIC_INV_IMU_DEBUG
#endif // ! INV_PRINTF


#if defined(__linux__)
#include<iostream>
#undef INV_TRACE_
#undef INV_TRACE
#undef INV_DEBUG_
#undef INV_DEBUG
#undef HITSIC_INV_IMU_DEBUG
#define HITSIC_INV_IMU_DEBUG 1
#define INV_TRACE_(fmt, ...) \
    printf("%s:%d:trace: " fmt "%s\r\n", __FILE__, __LINE__, __VA_ARGS__)
#define INV_TRACE(...) INV_TRACE_(__VA_ARGS__, "")
#define INV_DEBUG_(fmt, ...) \
    printf("%s:%d:debug: " fmt "%s\r\n", __FILE__, __LINE__, __VA_ARGS__)
#define INV_DEBUG(...) INV_DEBUG_(__VA_ARGS__, "")
#endif//defined(__linux__)


namespace inv {


    //i2c接口
    class i2cInterface_t {
    public:
        /**
         * @param  _context          :调用函数指针传入的用户参数
         * @param  _readBlocking     :约定如下，阻塞读
         *  **************************************************
         *  * @brief   这里的函数指针的参数约定
         *  * @param  {void*}                : 用户参数
         *  * @param  {uint8_t}        : iic从机地址
         *  * @param  {uint8_t}        : 从机寄存器地址
         *  * @param  {const unsigned* char} : 缓存地址
         *  * @param  {unsigned int}         : 数据长度
         *  * @return {int}                  : 错误码
         *  **************************************************
         * @param  _writeBlocking    :约定同上，阻塞写
         * @param  _readNonBlocking  :约定同上，非阻塞读
         */
        i2cInterface_t(void *_context,
                       int (*_readBlocking)(void *context,
                                            uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len),
                       int (*_writeBlocking)(void *context,
                                             uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len),
                       int (*_readNonBlocking)(void *context,
                                               uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len))
                : context(_context), readBlocking(_readBlocking), writeBlocking(_writeBlocking),
                  readNonBlocking(_readNonBlocking) {}

        i2cInterface_t(void *_context,
                       int (*_readBlocking)(void *context,
                                            uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len),
                       int (*_writeBlocking)(void *context,
                                             uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len)) :
                i2cInterface_t(_context, _readBlocking, _writeBlocking, _readBlocking) {}


        void *context;
        int (*readBlocking)(void *context,
                            uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);
        int (*writeBlocking)(void *context,
                             uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len);
        int (*readNonBlocking)(void *context,
                               uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);
    };

#if (__cplusplus > 201103L)
    class i2cif_t : public i2cInterface_t {
    private:
        std::function<int(uint8_t, uint8_t, uint8_t *, unsigned int)> read;
        std::function<int(uint8_t, uint8_t, uint8_t *, unsigned int)> readNB;
        std::function<int(uint8_t, uint8_t, const uint8_t *, unsigned int)> write;
        static int ReadBlocking(void *context,
                                uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len) {
            return static_cast<i2cif_t *>(context)->read(addr, reg, val, len);
        }
        static int WriteBlocking(void *context,
                                 uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len) {
            return static_cast<i2cif_t *>(context)->write(addr, reg, val, len);
        }
        static int ReadNonBlocking(void *context,
                                   uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len) {
            return static_cast<i2cif_t *>(context)->readNB(addr, reg, val, len);
        }
    public:
        i2cif_t(std::function<int(uint8_t, uint8_t, uint8_t *, unsigned int)> _read,
                std::function<int(uint8_t, uint8_t, const uint8_t *, unsigned int)> _write,
                std::function<int(uint8_t, uint8_t, uint8_t *, unsigned int)> _readNonBlocking) :
                read(_read), readNB(_readNonBlocking), write(_write),
                i2cInterface_t(this, ReadBlocking, WriteBlocking, ReadNonBlocking) {}
        i2cif_t(std::function<int(uint8_t, uint8_t, uint8_t *, unsigned int)> _read,
                std::function<int(uint8_t, uint8_t, const uint8_t *, unsigned int)> _write)
                : i2cif_t(_read, _write, _read) {}
    };
    class spiif_t : public i2cInterface_t {
    private:
//        void *realloc(void *mem_address, unsigned int newsize);
        uint8_t *txbuff;
        uint8_t *rxbuff;
        int *shareCounter;
        int buffsize;
        std::function<int(const uint8_t *, uint8_t *, unsigned int)> transfer;
        static int ReadBlocking(void *context,
                                uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len) {
            spiif_t *spi = static_cast<spiif_t *>(context);
            if (len + 1 > spi->buffsize) {
                if ((*spi->shareCounter) == 0) {
                    free(spi->txbuff);
                    free(spi->rxbuff);
                } else {
                    (*spi->shareCounter)--;
                }
                spi->txbuff = (uint8_t *) malloc(len + 30);
                spi->rxbuff = (uint8_t *) malloc(len + 30);
                spi->buffsize = len + 30;
            }
            spi->txbuff[0] = (1 << 8) | (reg & 0x7f);
            int rtv = spi->transfer(spi->txbuff, spi->rxbuff, len + 1);
            memcpy(val, &spi->rxbuff[1], len);
            return rtv;
        }
        static int WriteBlocking(void *context,
                                 uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len) {
            spiif_t *spi = static_cast<spiif_t *>(context);
            if (len + 1 > spi->buffsize) {
                if ((*spi->shareCounter) == 0) {
                    free(spi->txbuff);
                    free(spi->rxbuff);
                } else {
                    (*spi->shareCounter)--;
                }
                spi->txbuff = (uint8_t *) malloc(len + 30);
                spi->rxbuff = (uint8_t *) malloc(len + 30);
                spi->buffsize = len + 30;
            }
            spi->txbuff[0] = (reg & 0x7f);
            memcpy(&spi->txbuff[1], val, len);
            int rtv = spi->transfer(spi->txbuff, spi->rxbuff, len + 1);
            return rtv;
        }
    public:
        spiif_t(std::function<int(const uint8_t *, uint8_t *, unsigned int)> transfer_)
                : transfer(transfer_), shareCounter(new int(0)), i2cInterface_t(this, ReadBlocking, WriteBlocking) {
            txbuff = (uint8_t *) malloc(30);
            rxbuff = (uint8_t *) malloc(30);
            buffsize = 30;
        }
        spiif_t(const spiif_t &r) : i2cInterface_t(this, ReadBlocking, WriteBlocking) {
            rxbuff = r.rxbuff;
            txbuff = r.txbuff;
            shareCounter = r.shareCounter;
            buffsize = r.buffsize;
            transfer = r.transfer;
            (*shareCounter)++;
        };
        ~spiif_t() {
            if (*shareCounter == 0) {
                free(txbuff);
                free(rxbuff);
                delete shareCounter;
            } else {
                (*shareCounter)--;
            }
        }
    };
#endif
    struct config_t {
        enum mpu_accel_fs {    // In the ACCEL_CONFIG (0x1C) register, the full scale select  bits are :
            MPU_FS_2G = 0,    // 00 = 2G
            MPU_FS_4G,        // 01 = 4
            MPU_FS_8G,        // 10 = 8
            MPU_FS_16G,        // 11 = 16
            NUM_MPU_AFS
        } accelFullScale;

        /** @brief Allowed value for accel DLPF bandwidth (ACCEL_CONFIG2 (0x1D) register) */
        enum mpu_accel_bw {        // In the ACCEL_CONFIG2 (0x1D) register, the BW setting bits are :
            MPU_ABW_218 = 1,    ///< 001 = 218 Hz
            MPU_ABW_99,            ///< 010 = 99 Hz
            MPU_ABW_45,            ///< 011 = 45 Hz
            MPU_ABW_21,            ///< 100 = 21 Hz
            MPU_ABW_10,            ///< 101 = 10 Hz
            MPU_ABW_5,            ///< 110 = 5 Hz
            MPU_ABW_420,        ///< 111 = 420 Hz
            NUM_MPU_ABW
        } accelBandwidth;

        enum mpu_gyro_fs {        // In the GYRO_CONFIG register, the fS_SEL bits are :
            MPU_FS_250dps = 0,    // 00 = 250
            MPU_FS_500dps,        // 01 = 500
            MPU_FS_1000dps,        // 10 = 1000
            MPU_FS_2000dps,        // 11 = 2000
            NUM_MPU_GFS
        } gyroFullScale;

        /** @brief Allowed value for gyro DLPF bandwidth (CONFIG (0x1A) register) */
        enum mpu_gyro_bw {   // In the CONFIG register, the  BW setting bits are :
            MPU_GBW_250 = 0, ///< 000 = 250 Hz
            MPU_GBW_176 = 1, ///< 001 = 176 Hz
            MPU_GBW_92,         ///< 010 = 92 Hz
            MPU_GBW_41,         ///< 011 = 41 Hz
            MPU_GBW_20,         ///< 100 = 20 Hz
            MPU_GBW_10,         ///< 101 = 10 Hz
            MPU_GBW_5,         ///< 110 = 5 Hz
            NUM_MPU_GBW
        } gyroBandwidth;

        enum mpu_gyro_unit {
            MPU_UNIT_DegPerSec = 0,
            MPU_UNIT_RadPerSec = 1,
            MPU_UNIT_RevolutionsPerMinute,
        } gyroUnit;

        enum mpu_accel_unit {
            MPU_UNIT_MetersPerSquareSecond = 0,
            MPU_UNIT_G,
            MPU_UNIT_mG
        } accelUnit;

        config_t(mpu_accel_fs _accel_fs = MPU_FS_8G, mpu_accel_bw _accel_bw = MPU_ABW_99,
                 mpu_accel_unit mpuAccelUnit = MPU_UNIT_MetersPerSquareSecond,
                 mpu_gyro_fs _gyro_gs = MPU_FS_2000dps, mpu_gyro_bw _gyro_bw = MPU_GBW_92,
                 mpu_gyro_unit mpuGyroUnit = MPU_UNIT_DegPerSec)
                : accelFullScale(_accel_fs), accelBandwidth(_accel_bw), accelUnit(mpuAccelUnit),
                  gyroFullScale(_gyro_gs), gyroBandwidth(_gyro_bw), gyroUnit(mpuGyroUnit) {}
    };


    class imu_t {
    public:
        virtual ~imu_t() {}
        virtual int Init(config_t _cfg = config_t()) = 0;
        virtual bool Detect() = 0;
        virtual int SelfTest() = 0;
        virtual std::string Report() = 0;
        virtual bool DataReady() = 0;
        virtual int EnableDataReadyInt() = 0;
        virtual int SoftReset() = 0;
        virtual int ReadSensorBlocking() = 0;
        virtual int ReadSensorNonBlocking() = 0;
        virtual int Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y, float *gyro_z) = 0;
        virtual int Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) = 0;
        virtual int Convert(float *mag_x, float *mag_y, float *mag_z) = 0;
        virtual int Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) = 0;
        virtual int Convert(float *temp) = 0;
    public:
        static constexpr const uint8_t AddrAutoDetect = 0;
        imu_t(i2cInterface_t &_i2c, uint8_t _addr = AddrAutoDetect) : i2c(_i2c), isOpen(false), addr(_addr), cfg(config_t()) {
            if (addr == AddrAutoDetect) {
                addrAutoDetect = 0;
            } else {
                addrAutoDetect = 1;
            }
        }
        bool IsOpen() { return isOpen; };
        void SetConfig(config_t _cfg) { cfg = _cfg; }
        constexpr const config_t &GetConfig() { return cfg; }
        void SetI2cAddr(uint8_t _addr) { if (addrAutoDetect == 0) { addr = _addr; }}
        constexpr const uint8_t &GetI2cAddr() { return addr; }
        int WriteReg(uint8_t reg, const uint8_t val);
        int WriteRegVerified(uint8_t reg, const uint8_t val);
        int ReadReg(uint8_t reg, uint8_t *val);
        int ModifyReg(uint8_t reg, const uint8_t val, const uint8_t mask);
    protected:
        void SetIsOpen() { isOpen = true; }
        void ClearIsOpen() { isOpen = false; }
        i2cInterface_t &i2c;
    private:
        int addrAutoDetect;
        uint8_t addr;
        bool isOpen;
        config_t cfg;
    };

    class mpu6050_t : public imu_t {
    public:
        ~mpu6050_t() {}
        mpu6050_t(i2cInterface_t &_i2c, uint8_t _addr = AddrAutoDetect) : imu_t(_i2c, _addr) {}

        int Init(config_t _cfg = config_t()) override;
        bool Detect() override;
        int SelfTest() override;
        std::string Report() override;
        bool DataReady() override;
        int EnableDataReadyInt() override;
        int SoftReset() override;
        int ReadSensorBlocking() override;
        int ReadSensorNonBlocking() override;
        int Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y, float *gyro_z) override;
        int Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) override;
        int Convert(float *mag_x, float *mag_y, float *mag_z) override;
        int Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) override;
        int Convert(float *temp) override;

    private:
        float gyroUnit;
        float accelUnit;
        uint8_t buf[14];
    };

    class icm20602_t : public imu_t {
    public:
        virtual ~icm20602_t() {}
        icm20602_t(i2cInterface_t &_i2c, uint8_t _addr = AddrAutoDetect) : imu_t(_i2c, _addr) {}

        int Init(config_t _cfg = config_t()) override;
        bool Detect() override;
        int SelfTest() override;
        std::string Report() override;
        bool DataReady() override;
        int EnableDataReadyInt() override;
        int SoftReset() override;
        int ReadSensorBlocking() override;
        int ReadSensorNonBlocking() override;
        int Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y, float *gyro_z) override;
        int Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) override;
        int Convert(float *mag_x, float *mag_y, float *mag_z) override;
        int Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) override;
        int Convert(float *temp) override;
    private:
        float gyroUnit;
        float accelUnit;
        uint8_t buf[14];
    };

    class icm20600_t : public icm20602_t {
    public:
    public:
        virtual ~icm20600_t() {}
        icm20600_t(i2cInterface_t &_i2c, uint8_t _addr = AddrAutoDetect) : icm20602_t(_i2c, _addr) {}
        bool Detect() override;
        std::string Report() override;
    };

    class mpu9250_t : public imu_t {
    public:
        virtual ~mpu9250_t() {}

        mpu9250_t(i2cInterface_t &_i2c, uint8_t _addr = AddrAutoDetect) : imu_t(_i2c, _addr) {}

        int Init(config_t _cfg = config_t()) override;
        bool Detect() override;
        int SelfTest() override;
        std::string Report() override;
        bool DataReady() override;
        int EnableDataReadyInt() override;
        int SoftReset() override;
        int ReadSensorBlocking() override;
        int ReadSensorNonBlocking() override;
        int Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y, float *gyro_z) override;
        int Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) override;
        int Convert(float *mag_x, float *mag_y, float *mag_z) override;
        int Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) override;
        int Convert(float *temp) override;
    public:
        int SubI2cRead(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len = 1);
        int SubI2cWrite(uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len = 1);
    private:
        uint8_t buf[22];
        float gyroUnit;
        float accelUnit;
        uint8_t ak8963DeviceId;
        uint8_t ak8963Information;
        constexpr static float magUnit = 0.15f;;//固定量程4900uT 0.15µT/LSB
        float ak8963Asa[3];
    };

    class icm20948_t : public imu_t {
    public:
        ~icm20948_t() {}
        icm20948_t(i2cInterface_t &_i2c, uint8_t _addr = AddrAutoDetect) : imu_t(_i2c, _addr), bank(0) {}

        int Init(config_t _cfg = config_t()) override;
        bool Detect() override;
        int SelfTest() override;
        std::string Report() override;
        bool DataReady() override;
        int EnableDataReadyInt() override;
        int SoftReset() override;
        int ReadSensorBlocking() override;
        int ReadSensorNonBlocking() override;
        int Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y, float *gyro_z) override;
        int Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) override;
        int Convert(float *mag_x, float *mag_y, float *mag_z) override;
        int Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) override;
        int Convert(float *temp) override;
    public:
        int SubI2cRead(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len = 1);
        int SubI2cWrite(uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len = 1);
        int WriteReg(uint16_t reg, const uint8_t val);
        int WriteRegVerified(uint16_t reg, const uint8_t val);
        int ReadReg(uint16_t reg, uint8_t *val);
        int ModifyReg(uint16_t reg, const uint8_t val, const uint8_t mask);
    private:
        int SwitchBank(int _bank);
    private:
        int bank;
        float gyroUnit;
        float accelUnit;
        uint8_t ak09916DeviceId;
        uint8_t buf[14 + 9];
        constexpr static float magUnit = 0.15f;;//固定量程4900uT 0.15µT/LSB
    };


    class imuPtr_t : public std::shared_ptr<imu_t> {
    public:
        int Load(i2cInterface_t &_i2c, uint8_t _addr = imu_t::AddrAutoDetect);
    };
}
#endif // ! HITSIC_USE_DRV_IMU_INV

#endif // ! UTILITIES_DRV_IMU_INVENSENSE_HPP
