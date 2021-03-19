﻿/**
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
 * @brief 陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 * @author xiao qq1761690868
 * @doc drv_imu_invensense.md
 * @version v1.0
 * @date 2020-10-16
 */

#include"drv_imu_invensense.hpp"

#if (defined(HITSIC_USE_DRV_IMU_INV) && (HITSIC_USE_DRV_IMU_INV > 0U))

namespace inv {
    std::string icm20602_t::Report() {
        std::string rtv;
        rtv += "model:icm20602\t";
        rtv += "addr:";
        rtv += std::to_string((int) GetI2cAddr());
        rtv += '\t';
        return rtv;
    }

    int icm20602_t::Convert(float *temp) {
        if (temp) { *temp = (float) ((int16_t) (buf[6] << 8) | buf[7]) / 326.8f + 25.0f; }
        return 0;
    }

    int icm20602_t::SoftReset(void) {
        if (!Detect()) { return -1; }
        int res = 0;
        uint8_t val;
        //复位
        res |= WriteReg((uint8_t) icm20602_RegMap::PWR_MGMT_1, 0x80);
        //等待复位成功
        do {
            ReadReg((uint8_t) icm20602_RegMap::PWR_MGMT_1, &val);
            INV_TRACE("0x%x at PWR_MGMT_1,wait it get 0x41", val);
        } while (val != 0x41);

        //唤起睡眠
        ReadReg((uint8_t) icm20602_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) icm20602_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) icm20602_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) icm20602_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) icm20602_RegMap::PWR_MGMT_1, &val);
        res |= WriteRegVerified((uint8_t) icm20602_RegMap::PWR_MGMT_1, 0x1);

        return res;
    }

    int icm20602_t::Init(config_t _cfg) {
        SetConfig(_cfg);
        ClearIsOpen();
        int res = 0;
        if (!Detect()) { return -1; }
        //软复位
        res |= SoftReset();

        //打开所有传感器
        res |= WriteRegVerified((uint8_t) icm20602_RegMap::PWR_MGMT_2, 0);

        //1khz采样率
        res |= WriteRegVerified((uint8_t) icm20602_RegMap::SMPLRT_DIV, 0);

        //配置陀螺仪lpf
        switch (GetConfig().gyroBandwidth) {
            case config_t::MPU_GBW_250:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::CONFIG, 0);
                break;
            case config_t::MPU_GBW_176:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::CONFIG, 1);
                break;
            case config_t::MPU_GBW_92:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::CONFIG, 2);
                break;
            case config_t::MPU_GBW_41:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::CONFIG, 3);
                break;
            case config_t::MPU_GBW_20:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::CONFIG, 4);
                break;
            case config_t::MPU_GBW_10:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::CONFIG, 5);
                break;
            case config_t::MPU_GBW_5:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::CONFIG, 6);
                break;
            default:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::CONFIG, 0);
                break;
        }

        //配置陀螺仪量程和单位
        switch (GetConfig().gyroUnit) {
            case config_t::MPU_UNIT_RadPerSec:
                gyroUnit = (250.0 / 32768) * (M_PI / 180);
                break;
            case config_t::MPU_UNIT_RevolutionsPerMinute:
                gyroUnit = (250.0 / 32768) * (60 / 360);
                break;
            case config_t::MPU_UNIT_DegPerSec:
            default:
                gyroUnit = 250.0 / 32768;
                break;
        }
        switch (GetConfig().gyroFullScale) {
            case config_t::MPU_FS_250dps:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::GYRO_CONFIG, 0 << 3);
                break;
            case config_t::MPU_FS_500dps:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::GYRO_CONFIG, 1 << 3);
                gyroUnit *= 2;
                break;
            case config_t::MPU_FS_1000dps:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::GYRO_CONFIG, 2 << 3);
                gyroUnit *= 4;
                break;
            case config_t::MPU_FS_2000dps:
            default:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::GYRO_CONFIG, 3 << 3);
                gyroUnit *= 8;
                break;
        }

        //配置加速度计量程和单位
        switch (GetConfig().accelUnit) {
            case config_t::MPU_UNIT_G:
                accelUnit = (2.0 / 32768);
                break;
            case config_t::MPU_UNIT_mG:
                accelUnit = (2000.0 / 32768);
                break;
            case config_t::MPU_UNIT_MetersPerSquareSecond:
            default:
                accelUnit = 2.0 * 9.8 / 32768;
                break;
        }
        switch (GetConfig().accelFullScale) {
            case config_t::MPU_FS_2G:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG, 0 << 3);
                break;
            case config_t::MPU_FS_4G:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG, 1 << 3);
                accelUnit *= 2;
                break;
            case config_t::MPU_FS_8G:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG, 2 << 3);
                accelUnit *= 4;
                break;
            case config_t::MPU_FS_16G:
            default:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG, 3 << 3);
                accelUnit *= 8;
                break;
        }

        //配置加速度计lpf
        switch (GetConfig().accelBandwidth) {
            case config_t::MPU_ABW_218:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG2, 1);
                break;
            case config_t::MPU_ABW_99:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG2, 2);
                break;
            case config_t::MPU_ABW_45:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG2, 3);
                break;
            case config_t::MPU_ABW_21:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG2, 4);
                break;
            case config_t::MPU_ABW_10:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG2, 5);
                break;
            case config_t::MPU_ABW_5:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG2, 6);
                break;
            case config_t::MPU_ABW_420:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG2, 7);
                break;
            default:
                res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG2, 1);
                break;
        }

        //开启数据更新中断
        res |= EnableDataReadyInt();

        if (res == 0) {
            SetIsOpen();
        }
        return res;
    }

    bool icm20602_t::Detect() {
        uint8_t val = 0;
        SetI2cAddr(0x68);
        ReadReg((uint8_t) icm20602_RegMap::WHO_AM_I, &val);
        if (0x12 == val) {
            return true;
        }
        val = 0;
        SetI2cAddr(0x69);
        ReadReg((uint8_t) icm20602_RegMap::WHO_AM_I, &val);
        if (0x12 == val) {
            return true;
        }
        return false;
    }

    int icm20602_t::SelfTest() {
        if (!IsOpen()) { return -1; }
        int res = 0;
        config_t backup_cfg = GetConfig();
        config_t st_cfg;
        st_cfg.gyroFullScale = config_t::MPU_FS_250dps;
        st_cfg.accelFullScale = config_t::MPU_FS_2G;
        st_cfg.accelBandwidth = config_t::MPU_ABW_99;
        st_cfg.gyroBandwidth = config_t::MPU_GBW_92;
        if (0 != Init(st_cfg)) {
            Init(backup_cfg);
            return -1;
        }
        int32_t gyro_bias_st[3], gyro_bias_regular[3];
        int32_t accel_bias_st[3], accel_bias_regular[3];
        int16_t abuf[3];
        int16_t gbuf[3];
        int accel_result = 0;
        int gyro_result = 0;
        uint8_t val;
        memset(gyro_bias_st, 0, sizeof(gyro_bias_st));
        memset(gyro_bias_regular, 0, sizeof(gyro_bias_regular));
        memset(accel_bias_st, 0, sizeof(accel_bias_st));
        memset(accel_bias_regular, 0, sizeof(accel_bias_regular));

        int times;
        times = 20;
        while (times--) { while (!DataReady()) {}}//丢弃前20个数据
        times = 20;
        while (times--) {
            while (!DataReady()) {}
            res |= ReadSensorBlocking();
            Convert(abuf, abuf + 1, abuf + 2, gbuf, gbuf + 1, gbuf + 2);
            for (int i = 0; i < 3; ++i) {
                gyro_bias_regular[i] += gbuf[i];
                accel_bias_regular[i] += abuf[i];
            }
        }

        res |= ReadReg((uint8_t) icm20602_RegMap::GYRO_CONFIG, &val);
        res |= WriteRegVerified((uint8_t) icm20602_RegMap::GYRO_CONFIG, val | (0b111 << 5));//打开陀螺仪自检
        res |= ReadReg((uint8_t) icm20602_RegMap::ACCEL_CONFIG, &val);
        res |= WriteRegVerified((uint8_t) icm20602_RegMap::ACCEL_CONFIG, val | (0b111 << 5));//打开加速度计自检
        times = 20;
        while (times--) { while (!DataReady()) {}}//丢弃前20个数据
        times = 20;
        while (times--) {
            while (!DataReady()) {}
            res |= ReadSensorBlocking();
            Convert(abuf, abuf + 1, abuf + 2, gbuf, gbuf + 1, gbuf + 2);
            for (int i = 0; i < 3; ++i) {
                gyro_bias_st[i] += gbuf[i];
                accel_bias_st[i] += abuf[i];
            }
        }

        for (int i = 0; i < 3; ++i) {
            gyro_bias_regular[i] *= 50;   //(32768/2000)*1000 LSB/mg
            accel_bias_regular[i] *= 50;
            gyro_bias_st[i] *= 50;         //(32768/250)*1000 LSB/dps
            accel_bias_st[i] *= 50;
        }


        //计算加速度计自检结果
        uint8_t regs[3];
        int otp_value_zero = 0;
        int st_shift_prod[3], st_shift_cust[3], st_shift_ratio[3], i;
//    int result;

        res |= (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) icm20602_RegMap::SELF_TEST_X_ACCEL, regs, 3);
        for (i = 0; i < 3; i++) {
            if (regs[i] != 0) {
                st_shift_prod[i] = sSelfTestEquation[regs[i] - 1];
            } else {
                st_shift_prod[i] = 0;
                otp_value_zero = 1;
            }
        }

        if (!otp_value_zero) {
            /* Self Test Pass/Fail Criteria A */
            for (i = 0; i < 3; i++) {
                st_shift_cust[i] = accel_bias_st[i] - accel_bias_regular[i];
                st_shift_ratio[i] = abs(st_shift_cust[i] / st_shift_prod[i] - DEF_ST_PRECISION);
                if (st_shift_ratio[i] > DEF_ACCEL_ST_SHIFT_DELTA) {
                    //加速度计自检未通过
                    accel_result = 1;
                    INV_DEBUG("accel[%d] st fail,result = %d,it demands less than %d", i, st_shift_ratio[i],
                              DEF_ACCEL_ST_SHIFT_DELTA);
                } else {
                    INV_TRACE("accel[%d] st result = %d,it demands less than %d", i, st_shift_ratio[i],
                              DEF_ACCEL_ST_SHIFT_DELTA);
                }
            }
        } else {
            /* Self Test Pass/Fail Criteria B */
            for (i = 0; i < 3; i++) {
                st_shift_cust[i] = abs(accel_bias_st[i] - accel_bias_regular[i]);
                if (st_shift_cust[i] < DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000
                    || st_shift_cust[i] > DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000) {
                    //加速度计自检未通过
                    accel_result = 1;
                    INV_DEBUG("accel[%d] st fail,result = %d,it demands <%d && >%d", i, st_shift_cust[i],
                              DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000, DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000);
                } else {
                    INV_TRACE("accel[%d] st result = %d,it demands <%d && >%d", i, st_shift_cust[i],
                              DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000, DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000);
                }
            }
        }

        //计算陀螺仪自检结果
        res |= (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) icm20602_RegMap::SELF_TEST_X_GYRO, regs, 3);
        for (i = 0; i < 3; i++) {
            if (regs[i] != 0) {
                st_shift_prod[i] = sSelfTestEquation[regs[i] - 1];
            } else {
                st_shift_prod[i] = 0;
                otp_value_zero = 1;
            }
        }

        for (i = 0; i < 3; i++) {
            st_shift_cust[i] = gyro_bias_st[i] - gyro_bias_regular[i];
            if (!otp_value_zero) {
                /* Self Test Pass/Fail Criteria A */
                if (st_shift_cust[i] < DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]) {
                    //陀螺仪自检没过
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]);
                }
            } else {
                /* Self Test Pass/Fail Criteria B */
                if (st_shift_cust[i] < DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION) {
                    //陀螺仪自检没过
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION);
                }
            }
        }

        if (gyro_result == 0) {
            /* Self Test Pass/Fail Criteria C */
            for (i = 0; i < 3; i++) {
                if (abs(gyro_bias_regular[i]) > DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION)
                    //陀螺仪自检没过
                {
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,ift demands less than %d", i, (int) abs(gyro_bias_regular[i]),
                              DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands less than %d", i, (int) abs(gyro_bias_regular[i]),
                              DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION);
                }
            }
        }

        //恢复原来的配置
        res |= Init(backup_cfg);
        return res | (gyro_result << 1) | accel_result;
    }

    bool icm20602_t::DataReady() {
        uint8_t val = 0;
        ReadReg((uint8_t) icm20602_RegMap::INT_STATUS, &val);
        return (val & 0x01) == 0x01;
    }

    int icm20602_t::EnableDataReadyInt() {
        return ModifyReg((uint8_t) icm20602_RegMap::INT_ENABLE, 0x01, 0x01);
    }

    int icm20602_t::ReadSensorBlocking() {
        return (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) icm20602_RegMap::ACCEL_XOUT_H, buf, 14);
    }

    int icm20602_t::ReadSensorNonBlocking() {
        return (*i2c.readNonBlocking)(i2c.context, GetI2cAddr(), (uint8_t) icm20602_RegMap::ACCEL_XOUT_H, buf, 14);
    }

    int icm20602_t::Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y, float *gyro_z) {
        if (acc_x) { *acc_x = accelUnit * ((int16_t) ((buf[0] << 8) | buf[1])); }
        if (acc_y) { *acc_y = accelUnit * ((int16_t) ((buf[2] << 8) | buf[3])); }
        if (acc_z) { *acc_z = accelUnit * ((int16_t) ((buf[4] << 8) | buf[5])); }
        if (gyro_x) { *gyro_x = gyroUnit * ((int16_t) ((buf[8] << 8) | buf[9])); }
        if (gyro_y) { *gyro_y = gyroUnit * ((int16_t) ((buf[10] << 8) | buf[11])); }
        if (gyro_z) { *gyro_z = gyroUnit * ((int16_t) ((buf[12] << 8) | buf[13])); }
        return 0;
    }

    int icm20602_t::Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) {
        if (acc_x) { *acc_x = ((int16_t) ((buf[0] << 8) | buf[1])); }
        if (acc_y) { *acc_y = ((int16_t) ((buf[2] << 8) | buf[3])); }
        if (acc_z) { *acc_z = ((int16_t) ((buf[4] << 8) | buf[5])); }
        if (gyro_x) { *gyro_x = ((int16_t) ((buf[8] << 8) | buf[9])); }
        if (gyro_y) { *gyro_y = ((int16_t) ((buf[10] << 8) | buf[11])); }
        if (gyro_z) { *gyro_z = ((int16_t) ((buf[12] << 8) | buf[13])); }
        return 0;
    }

    int icm20602_t::Convert(float *mag_x, float *mag_y, float *mag_z) {
        return 0;
    }

    int icm20602_t::Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) {
        return 0;
    }

    int mpu6050_t::SelfTest() {
        if (!IsOpen()) { return -1; }
        int res = 0;
        config_t backup_cfg = GetConfig();
        config_t st_cfg;
        st_cfg.gyroFullScale = config_t::MPU_FS_250dps;
        st_cfg.accelFullScale = config_t::MPU_FS_8G;
        if (0 != Init(st_cfg)) {
            Init(backup_cfg);
            return -1;
        }
        int32_t gyro_bias_st[3], gyro_bias_regular[3];
        int32_t accel_bias_st[3], accel_bias_regular[3];
        int16_t abuf[3];
        int16_t gbuf[3];
        int accel_result = 0;
        int gyro_result = 0;
        uint8_t val;
        memset(gyro_bias_st, 0, sizeof(gyro_bias_st));
        memset(gyro_bias_regular, 0, sizeof(gyro_bias_regular));
        memset(accel_bias_st, 0, sizeof(accel_bias_st));
        memset(accel_bias_regular, 0, sizeof(accel_bias_regular));


        int times;
        times = 20;
        while (times--) { while (!DataReady()) {}}//丢弃前20个数据
        times = 20;
        while (times--) {
            while (!DataReady()) {}
            res |= ReadSensorBlocking();
            Convert(abuf, abuf + 1, abuf + 2, gbuf, gbuf + 1, gbuf + 2);
            for (int i = 0; i < 3; ++i) {
                gyro_bias_regular[i] += gbuf[i];
                accel_bias_regular[i] += abuf[i];
            }
        }

        res |= ReadReg((uint8_t) mpu6050_RegMap::GYRO_CONFIG, &val);
        res |= WriteRegVerified((uint8_t) mpu6050_RegMap::GYRO_CONFIG, val | (0b111 << 5));//打开陀螺仪自检
        res |= ReadReg((uint8_t) mpu6050_RegMap::ACCEL_CONFIG, &val);
        res |= WriteRegVerified((uint8_t) mpu6050_RegMap::ACCEL_CONFIG, val | (0b111 << 5));//打开加速度计自检
        times = 20;
        while (times--) { while (!DataReady()) {}}//丢弃前100个数据
        times = 20;
        while (times--) {
            while (!DataReady()) {}
            res |= ReadSensorBlocking();
            Convert(abuf, abuf + 1, abuf + 2, gbuf, gbuf + 1, gbuf + 2);
            for (int i = 0; i < 3; ++i) {
                gyro_bias_st[i] += gbuf[i];
                accel_bias_st[i] += abuf[i];
            }
        }

        for (int i = 0; i < 3; ++i) {
            gyro_bias_regular[i] *= 50;   //(32768/2000)*1000 LSB/mg
            accel_bias_regular[i] *= 50;
            gyro_bias_st[i] *= 50;         //(32768/250)*1000 LSB/dps
            accel_bias_st[i] *= 50;
        }

        //开始计算自检结果
        uint8_t regs[4];
        res |= (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) mpu6050_RegMap::SELF_TEST_X, regs, 4);
        int a_st[3];
        int g_st[3];
        int ft_a[3];
        int ft_g[3];
        a_st[0] = ((0b111 & (regs[0] >> 5)) << 2) | (0b11 & (regs[3] >> 4));
        a_st[1] = ((0b111 & (regs[1] >> 5)) << 2) | (0b11 & (regs[3] >> 2));
        a_st[2] = ((0b111 & (regs[2] >> 5)) << 2) | (0b11 & (regs[3] >> 0));
        g_st[0] = 0b11111 & regs[0];
        g_st[1] = 0b11111 & regs[1];
        g_st[2] = 0b11111 & regs[2];

        a_st[0] &= (32 - 1);
        a_st[1] &= (32 - 1);
        a_st[2] &= (32 - 1);
        g_st[0] &= (32 - 1);
        g_st[1] &= (32 - 1);
        g_st[2] &= (32 - 1);

        ft_a[0] = 1000 * accelSelfTestEquation[a_st[0]];
        ft_a[1] = 1000 * accelSelfTestEquation[a_st[1]];
        ft_a[2] = 1000 * accelSelfTestEquation[a_st[2]];
        ft_g[0] = 1000 * gyroSelfTestEquation[g_st[0]];
        ft_g[1] = -1000 * gyroSelfTestEquation[g_st[1]];
        ft_g[2] = 1000 * gyroSelfTestEquation[g_st[2]];

        for (int i = 0; i < 3; ++i) {
            int str = accel_bias_st[i] - accel_bias_regular[i];
            float Change_from_factory_trim = (float) (str - ft_a[i]) / ft_a[i];
            if (Change_from_factory_trim > 0.14 || Change_from_factory_trim < -0.14) {
                INV_DEBUG("6050 accel[%d] self test fail,result = %f,it demands >-0.14 && <0.14", i,
                          Change_from_factory_trim);
                accel_result = 1;
            } else {
                INV_TRACE("6050 accel[%d] self test result = %f,it demands >-0.14 && <0.14", i,
                          Change_from_factory_trim);
            }
        }

        for (int i = 0; i < 3; ++i) {
            int str = gyro_bias_st[i] - gyro_bias_regular[i];
            float Change_from_factory_trim = (float) (str - ft_g[i]) / ft_g[i];
            if (Change_from_factory_trim > 0.14 || Change_from_factory_trim < -0.14) {
                INV_DEBUG("6050 gryo[%d] self test fail,result = %f,it demands >-0.14 && <0.14", i,
                          Change_from_factory_trim);
                gyro_result = 1;
            } else {
                INV_TRACE("6050 gryo[%d] self test result = %f,it demands >-0.14 && <0.14", i,
                          Change_from_factory_trim);
            }
        }

        //恢复原来的配置
        res |= Init(backup_cfg);
        return (gyro_result << 1) | accel_result | res;
    }


    int mpu6050_t::Convert(float *temp) {
        if (temp) {
            *temp = (float) ((int16_t) ((buf[6] << 8) | buf[7] - 521)) / 340.0f + 35;
        }
        return 0;
    }

    std::string mpu6050_t::Report() {
        std::string rtv;
        rtv += "model:mpu6050\t";
        rtv += "addr:";
        rtv += std::to_string((int) GetI2cAddr());
        rtv += '\t';
        return rtv;
    }

    int mpu6050_t::SoftReset(void) {
        if (!Detect()) { return -1; }
        int res = 0;
        uint8_t val;
        //复位
        res |= WriteReg((uint8_t) mpu6050_RegMap::PWR_MGMT_1, 0x80);
        //等待复位成功
        do {
            ReadReg((uint8_t) mpu6050_RegMap::PWR_MGMT_1, &val);
            INV_TRACE("0x%x at PWR_MGMT_1,wait it get 0x40", val);
        } while (val != 0x40);

        //唤起睡眠
        ReadReg((uint8_t) mpu6050_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) mpu6050_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) mpu6050_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) mpu6050_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) mpu6050_RegMap::PWR_MGMT_1, &val);
        res |= WriteRegVerified((uint8_t) mpu6050_RegMap::PWR_MGMT_1, 0x0);

        return res;
    }
    int mpu6050_t::Init(config_t _cfg) {
        SetConfig(_cfg);
        ClearIsOpen();
        int res = 0;
        if (!Detect()) { return -1; }
        //软复位
        res |= SoftReset();

        //打开所有传感器
        res |= WriteRegVerified((uint8_t) mpu6050_RegMap::PWR_MGMT_2, 0);

        //1khz采样率
        res |= WriteRegVerified((uint8_t) mpu6050_RegMap::SMPLRT_DIV, 0);

        //配置陀螺仪lpf
        switch (GetConfig().gyroBandwidth) {
            case config_t::MPU_GBW_250:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::CONFIG, 0);
                break;
            case config_t::MPU_GBW_176:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::CONFIG, 1);
                break;
            case config_t::MPU_GBW_92:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::CONFIG, 2);
                break;
            case config_t::MPU_GBW_41:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::CONFIG, 3);
                break;
            case config_t::MPU_GBW_20:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::CONFIG, 4);
                break;
            case config_t::MPU_GBW_10:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::CONFIG, 5);
                break;
            case config_t::MPU_GBW_5:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::CONFIG, 6);
                break;
            default:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::CONFIG, 0);
                break;
        }

        //配置陀螺仪量程和单位
        switch (GetConfig().gyroUnit) {
            case config_t::MPU_UNIT_RadPerSec:
                gyroUnit = (250.0 / 32768) * (M_PI / 180);
                break;
            case config_t::MPU_UNIT_RevolutionsPerMinute:
                gyroUnit = (250.0 / 32768) * (60 / 360);
                break;
            case config_t::MPU_UNIT_DegPerSec:
            default:
                gyroUnit = 250.0 / 32768;
                break;
        }
        switch (GetConfig().gyroFullScale) {
            case config_t::MPU_FS_250dps:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::GYRO_CONFIG, 0 << 3);
                break;
            case config_t::MPU_FS_500dps:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::GYRO_CONFIG, 1 << 3);
                gyroUnit *= 2;
                break;
            case config_t::MPU_FS_1000dps:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::GYRO_CONFIG, 2 << 3);
                gyroUnit *= 4;
                break;
            case config_t::MPU_FS_2000dps:
            default:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::GYRO_CONFIG, 3 << 3);
                gyroUnit *= 8;
                break;
        }

        //配置加速度计量程和单位
        switch (GetConfig().accelUnit) {
            case config_t::MPU_UNIT_G:
                accelUnit = (2.0 / 32768);
                break;
            case config_t::MPU_UNIT_mG:
                accelUnit = (2000.0 / 32768);
                break;
            case config_t::MPU_UNIT_MetersPerSquareSecond:
            default:
                accelUnit = 2.0 * 9.8 / 32768;
                break;
        }
        switch (GetConfig().accelFullScale) {
            case config_t::MPU_FS_2G:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::ACCEL_CONFIG, 0 << 3);
                break;
            case config_t::MPU_FS_4G:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::ACCEL_CONFIG, 1 << 3);
                accelUnit *= 2;
                break;
            case config_t::MPU_FS_8G:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::ACCEL_CONFIG, 2 << 3);
                accelUnit *= 4;
                break;
            case config_t::MPU_FS_16G:
            default:
                res |= WriteRegVerified((uint8_t) mpu6050_RegMap::ACCEL_CONFIG, 3 << 3);
                accelUnit *= 8;
                break;
        }

        //开启数据更新中断
        res |= EnableDataReadyInt();

        if (res == 0) {
            SetIsOpen();
        }
        return res;
    }
    bool mpu6050_t::Detect() {
        uint8_t val = 0;
        SetI2cAddr(0x68);
        ReadReg((uint8_t) mpu6050_RegMap::WHO_AM_I, &val);
        if (0x68 == val) {
            return true;
        }
        val = 0;
        SetI2cAddr(0x69);
        ReadReg((uint8_t) mpu6050_RegMap::WHO_AM_I, &val);
        if (0x68 == val) {
            return true;
        }
        return false;
    }

    bool mpu6050_t::DataReady() {
        uint8_t val = 0;
        ReadReg((uint8_t) mpu6050_RegMap::INT_STATUS, &val);
        return (val & 0x01) == 0x01;
    }

    int mpu6050_t::EnableDataReadyInt() {
        return ModifyReg((uint8_t) mpu6050_RegMap::INT_ENABLE, 0x01, 0x01);
    }

    int mpu6050_t::ReadSensorBlocking() {
        return (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) mpu6050_RegMap::ACCEL_XOUT_H, buf, 14);
    }

    int mpu6050_t::ReadSensorNonBlocking() {
        return (*i2c.readNonBlocking)(i2c.context, GetI2cAddr(), (uint8_t) mpu6050_RegMap::ACCEL_XOUT_H, buf, 14);
    }

    int mpu6050_t::Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) {
        return 0;
    }

    int mpu6050_t::Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y, float *gyro_z) {
        if (acc_x) { *acc_x = accelUnit * ((int16_t) ((buf[0] << 8) | buf[1])); }
        if (acc_y) { *acc_y = accelUnit * ((int16_t) ((buf[2] << 8) | buf[3])); }
        if (acc_z) { *acc_z = accelUnit * ((int16_t) ((buf[4] << 8) | buf[5])); }
        if (gyro_x) { *gyro_x = gyroUnit * ((int16_t) ((buf[8] << 8) | buf[9])); }
        if (gyro_y) { *gyro_y = gyroUnit * ((int16_t) ((buf[10] << 8) | buf[11])); }
        if (gyro_z) { *gyro_z = gyroUnit * ((int16_t) ((buf[12] << 8) | buf[13])); }
        return 0;
    }

    int mpu6050_t::Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) {
        if (acc_x) { *acc_x = ((int16_t) ((buf[0] << 8) | buf[1])); }
        if (acc_y) { *acc_y = ((int16_t) ((buf[2] << 8) | buf[3])); }
        if (acc_z) { *acc_z = ((int16_t) ((buf[4] << 8) | buf[5])); }
        if (gyro_x) { *gyro_x = ((int16_t) ((buf[8] << 8) | buf[9])); }
        if (gyro_y) { *gyro_y = ((int16_t) ((buf[10] << 8) | buf[11])); }
        if (gyro_z) { *gyro_z = ((int16_t) ((buf[12] << 8) | buf[13])); }
        return 0;
    }

    int mpu6050_t::Convert(float *mag_x, float *mag_y, float *mag_z) {
        return 0;
    }


    int mpu9250_t::Init(config_t _cfg) {
        SetConfig(_cfg);
        ClearIsOpen();
        int res = 0;
        if (!Detect()) { return -1; }
        //软复位
        res |= SoftReset();

        //打开所有传感器
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::PWR_MGMT_2, 0);

        //1khz采样率
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::SMPLRT_DIV, 0);

        //配置陀螺仪lpf
        switch (GetConfig().gyroBandwidth) {
            case config_t::MPU_GBW_250:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::CONFIG, 0);
                break;
            case config_t::MPU_GBW_176:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::CONFIG, 1);
                break;
            case config_t::MPU_GBW_92:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::CONFIG, 2);
                break;
            case config_t::MPU_GBW_41:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::CONFIG, 3);
                break;
            case config_t::MPU_GBW_20:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::CONFIG, 4);
                break;
            case config_t::MPU_GBW_10:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::CONFIG, 5);
                break;
            case config_t::MPU_GBW_5:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::CONFIG, 6);
                break;
            default:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::CONFIG, 0);
                break;
        }

        //配置陀螺仪量程和单位
        switch (GetConfig().gyroUnit) {
            case config_t::MPU_UNIT_RadPerSec:
                gyroUnit = (250.0 / 32768) * (M_PI / 180);
                break;
            case config_t::MPU_UNIT_RevolutionsPerMinute:
                gyroUnit = (250.0 / 32768) * (60 / 360);
                break;
            case config_t::MPU_UNIT_DegPerSec:
            default:
                gyroUnit = 250.0 / 32768;
                break;
        }
        switch (GetConfig().gyroFullScale) {
            case config_t::MPU_FS_250dps:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::GYRO_CONFIG, 0 << 3);
                break;
            case config_t::MPU_FS_500dps:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::GYRO_CONFIG, 1 << 3);
                gyroUnit *= 2;
                break;
            case config_t::MPU_FS_1000dps:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::GYRO_CONFIG, 2 << 3);
                gyroUnit *= 4;
                break;
            case config_t::MPU_FS_2000dps:
            default:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::GYRO_CONFIG, 3 << 3);
                gyroUnit *= 8;
                break;
        }

        //配置加速度计量程和单位
        switch (GetConfig().accelUnit) {
            case config_t::MPU_UNIT_G:
                accelUnit = (2.0 / 32768);
                break;
            case config_t::MPU_UNIT_mG:
                accelUnit = (2000.0 / 32768);
                break;
            case config_t::MPU_UNIT_MetersPerSquareSecond:
            default:
                accelUnit = 2.0 * 9.8 / 32768;
                break;
        }
        switch (GetConfig().accelFullScale) {
            case config_t::MPU_FS_2G:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG, 0 << 3);
                break;
            case config_t::MPU_FS_4G:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG, 1 << 3);
                accelUnit *= 2;
                break;
            case config_t::MPU_FS_8G:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG, 2 << 3);
                accelUnit *= 4;
                break;
            case config_t::MPU_FS_16G:
            default:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG, 3 << 3);
                accelUnit *= 8;
                break;
        }

        //配置加速度计lpf
        switch (GetConfig().accelBandwidth) {
            case config_t::MPU_ABW_218:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG2, 1);
                break;
            case config_t::MPU_ABW_99:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG2, 2);
                break;
            case config_t::MPU_ABW_45:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG2, 3);
                break;
            case config_t::MPU_ABW_21:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG2, 4);
                break;
            case config_t::MPU_ABW_10:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG2, 5);
                break;
            case config_t::MPU_ABW_5:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG2, 6);
                break;
            case config_t::MPU_ABW_420:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG2, 7);
                break;
            default:
                res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG2, 1);
                break;
        }

        //开启数据更新中断
        res |= EnableDataReadyInt();

        if (res != 0) { return res; }
        ClearIsOpen();

        uint8_t val;
        //设置9250内部i2c
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::I2C_MST_CTRL, 1 << 4 | 9);//500khz，连续读模式
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::USER_CTRL, 1 << 5);//开启i2c主模式

        //开始设置ak8963
        //读取id
        res |= SubI2cRead(MPU9250_AK8963_I2C_ADDR, (uint8_t) ak8963_RegMap::WIA, &ak8963DeviceId, 1);
        if (res != 0) { return res; }
        res |= SubI2cRead(MPU9250_AK8963_I2C_ADDR, (uint8_t) ak8963_RegMap::INFO, &ak8963Information, 1);

        //复位并且校准磁力计
        val = MPU9250_AK8963_CNTL2_SRST;
        res |= SubI2cWrite(MPU9250_AK8963_I2C_ADDR, (uint8_t) ak8963_RegMap::CNTL2, &val, 1);
        val = MPU9250_AK8963_POWER_DOWN;
        res |= SubI2cWrite(MPU9250_AK8963_I2C_ADDR, (uint8_t) ak8963_RegMap::CNTL, &val, 1);
        val = MPU9250_AK8963_FUSE_ROM_ACCESS;
        res |= SubI2cWrite(MPU9250_AK8963_I2C_ADDR, (uint8_t) ak8963_RegMap::CNTL, &val, 1);
        //AK8963 get calibration data
        uint8_t response[3] = {0, 0, 0};
        res |= SubI2cRead(MPU9250_AK8963_I2C_ADDR, (uint8_t) ak8963_RegMap::ASAX, response, 3);
        INV_TRACE("0x%x 0x%x 0x%x at ak8963_RegMap::ASAX", response[0], response[1], response[2]);
        //AK8963_SENSITIVITY_SCALE_FACTOR
        //ak8963Asa[i++] = (s16)((data - 128.0f) / 256.0f + 1.0f) ;
        //ak8963Asa[i++] = (s16)((data - 128.0f) *0.00390625f + 1.0f) ;
        ak8963Asa[0] = (1.0f + 0.00390625f * ((int16_t) (response[0]) - 128));
        ak8963Asa[1] = (1.0f + 0.00390625f * ((int16_t) (response[1]) - 128));
        ak8963Asa[2] = (1.0f + 0.00390625f * ((int16_t) (response[2]) - 128));

        INV_TRACE("%f %f %f at ak8963Asa", ak8963Asa[0], ak8963Asa[1], ak8963Asa[2]);
        val = MPU9250_AK8963_POWER_DOWN;
        res |= SubI2cWrite(MPU9250_AK8963_I2C_ADDR, (uint8_t) ak8963_RegMap::CNTL, &val, 1);

        //设置连续读ak8963到fifo
        val = 0x5D;
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::I2C_MST_CTRL, val);

        val = MPU9250_AK8963_I2C_ADDR | 0x80;
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::I2C_SLV0_ADDR, val);

        val = (uint8_t) (uint8_t) ak8963_RegMap::ST1;
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::I2C_SLV0_REG, val);

        val = 0x88;
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::I2C_SLV0_CTRL, val);

        val = MPU9250_AK8963_CONTINUOUS_MEASUREMENT;
        res |= SubI2cWrite(MPU9250_AK8963_I2C_ADDR, (uint8_t) ak8963_RegMap::CNTL, &val, 1);

        val = 0x09;
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::I2C_SLV4_CTRL, val);

        val = 0x81;
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::I2C_MST_DELAY_CTRL, val);

        if (res == 0) {
            SetIsOpen();
            return 0;
        } else {
            return res;
        }
    }

    std::string mpu9250_t::Report() {
        std::string rtv;
        rtv += "model:mpu9250\t";
        rtv += "addr:";
        rtv += std::to_string((int) GetI2cAddr());
        rtv += '\t';

        rtv += "magnetometer:ak8963\t";
        rtv += "ID:";
        rtv += std::to_string((int) ak8963DeviceId);
        rtv += '\t';
        rtv += "INF:";
        rtv += std::to_string((int) ak8963Information);
        rtv += '\t';
        return rtv;
    }

    int mpu9250_t::SubI2cRead(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len) {
        uint8_t index = 0;
        uint8_t status = 0;
        uint32_t timeout = 0;
        uint8_t tmp = 0;
        int res = 0;
        tmp = addr | 0x80;
        res |= WriteReg((uint8_t) mpu9250_RegMap::I2C_SLV4_ADDR, tmp);
        while (index < len) {
            tmp = reg + index;
            res |= WriteReg((uint8_t) mpu9250_RegMap::I2C_SLV4_REG, tmp);
            tmp = MPU9250_I2C_SLV4_EN;
            res |= WriteReg((uint8_t) mpu9250_RegMap::I2C_SLV4_CTRL, tmp);
            do {
                if (timeout++ > 5000) {
                    INV_DEBUG("SubI2cRead time out");
                    return -2;
                }
                res |= ReadReg((uint8_t) mpu9250_RegMap::I2C_MST_STATUS, &status);
            } while ((status & MPU9250_I2C_SLV4_DONE) == 0);
            res |= ReadReg((uint8_t) mpu9250_RegMap::I2C_SLV4_DI, val + index);
            index++;
        }
        return res;
    }

    int mpu9250_t::SubI2cWrite(uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len) {
        uint32_t timeout = 0;
        uint8_t status = 0;
        uint8_t tmp = 0;
        uint8_t index = 0;
        int res = 0;
        tmp = addr;
        res |= WriteReg((uint8_t) mpu9250_RegMap::I2C_SLV4_ADDR, tmp);
        while (index < len) {
            tmp = reg + index;
            res |= WriteReg((uint8_t) mpu9250_RegMap::I2C_SLV4_REG, tmp);
            tmp = val[index];
            res |= WriteReg((uint8_t) mpu9250_RegMap::I2C_SLV4_DO, tmp);
            tmp = MPU9250_I2C_SLV4_EN;
            res |= WriteReg((uint8_t) mpu9250_RegMap::I2C_SLV4_CTRL, tmp);
            do {
                if (timeout++ > 5000) {
                    INV_DEBUG("SubI2cWrite time out");
                    return -2;
                }
                res |= ReadReg((uint8_t) mpu9250_RegMap::I2C_MST_STATUS, &status);
            } while ((status & MPU9250_I2C_SLV4_DONE) == 0);
            if (status & MPU9250_I2C_SLV4_NACK) {
                INV_DEBUG("SubI2cWrite no ack");
                return -3;
            }
            index++;
        }
        return res;
    }

    int mpu9250_t::Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y,
                           float *gyro_z) {
        if (acc_x) { *acc_x = accelUnit * ((int16_t) (buf[0] << 8) | buf[1]); }
        if (acc_y) { *acc_y = accelUnit * ((int16_t) (buf[2] << 8) | buf[3]); }
        if (acc_z) { *acc_z = accelUnit * ((int16_t) (buf[4] << 8) | buf[5]); }
        if (gyro_x) { *gyro_x = gyroUnit * ((int16_t) (buf[8] << 8) | buf[9]); }
        if (gyro_y) { *gyro_y = gyroUnit * ((int16_t) (buf[10] << 8) | buf[11]); }
        if (gyro_z) { *gyro_z = gyroUnit * ((int16_t) (buf[12] << 8) | buf[13]); }
        return 0;
    }

    int mpu9250_t::Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x,
                           int16_t *gyro_y, int16_t *gyro_z) {
        if (acc_x) { *acc_x = ((int16_t) (buf[0] << 8) | buf[1]); }
        if (acc_y) { *acc_y = ((int16_t) (buf[2] << 8) | buf[3]); }
        if (acc_z) { *acc_z = ((int16_t) (buf[4] << 8) | buf[5]); }
        if (gyro_x) { *gyro_x = ((int16_t) (buf[8] << 8) | buf[9]); }
        if (gyro_y) { *gyro_y = ((int16_t) (buf[10] << 8) | buf[11]); }
        if (gyro_z) { *gyro_z = ((int16_t) (buf[12] << 8) | buf[13]); }
        return 0;
    }

    int mpu9250_t::Convert(float *mag_x, float *mag_y, float *mag_z) {
        if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY) || (buf[14 + 0] & MPU9250_AK8963_DATA_OVERRUN)) {
//            INV_TRACE("0x%x at buf[14 + 0]", (int) buf[14 + 0]);
            return -1;
        }
        if (buf[14 + 7] & MPU9250_AK8963_OVERFLOW) {
//            INV_TRACE("0x%x at buf[14 + 7]", (int) buf[14 + 7]);
            return -1;
        }
        if (mag_x) { *mag_x = magUnit * ak8963Asa[0] * ((int16_t) (buf[14 + 2] << 8) | buf[14 + 1]); }
        if (mag_y) { *mag_y = magUnit * ak8963Asa[1] * ((int16_t) (buf[14 + 4] << 8) | buf[14 + 3]); }
        if (mag_z) { *mag_z = magUnit * ak8963Asa[2] * ((int16_t) (buf[14 + 6] << 8) | buf[14 + 5]); }
        return 0;
    }

    int mpu9250_t::Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) {
        if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY) || (buf[14 + 0] & MPU9250_AK8963_DATA_OVERRUN)) {
//            INV_TRACE("0x%x at buf[14 + 0]", (int) buf[14 + 0]);
            return -1;
        }
        if (buf[14 + 7] & MPU9250_AK8963_OVERFLOW) {
//            INV_TRACE("0x%x at buf[14 + 7]", (int) buf[14 + 7]);
            return -1;
        }
        if (mag_x) { *mag_x = ((int16_t) (buf[14 + 2] << 8) | buf[14 + 1]); }
        if (mag_y) { *mag_y = ((int16_t) (buf[14 + 4] << 8) | buf[14 + 3]); }
        if (mag_z) { *mag_z = ((int16_t) (buf[14 + 6] << 8) | buf[14 + 5]); }
        return 0;
    }

    int mpu9250_t::Convert(float *temp) {
        if (temp) { *temp = (float) ((int16_t) (buf[6] << 8) | buf[7]) / 333.87f + 21.0f; }
        return 0;
    }

    int mpu9250_t::ReadSensorBlocking() {
        return (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) mpu9250_RegMap::ACCEL_XOUT_H, buf, 22);
    }

    int mpu9250_t::ReadSensorNonBlocking() {
        return (*i2c.readNonBlocking)(i2c.context, GetI2cAddr(), (uint8_t) mpu9250_RegMap::ACCEL_XOUT_H, buf, 22);
    }

    int mpu9250_t::SoftReset(void) {
        if (!Detect()) { return -1; }
        int res = 0;
        uint8_t val;
        //复位
        res |= WriteReg((uint8_t) mpu9250_RegMap::PWR_MGMT_1, 0x80);
        //等待复位成功
        do {
            ReadReg((uint8_t) mpu9250_RegMap::PWR_MGMT_1, &val);
            INV_TRACE("0x%x at PWR_MGMT_1,wait it get 0x1", val);
        } while (val != 0x1);

        //唤起睡眠
        ReadReg((uint8_t) mpu9250_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) mpu9250_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) mpu9250_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) mpu9250_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint8_t) mpu9250_RegMap::PWR_MGMT_1, &val);
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::PWR_MGMT_1, 0x1);

        return res;
    }
    bool mpu9250_t::Detect() {
        uint8_t val = 0;
        SetI2cAddr(0x68);
        ReadReg((uint8_t) mpu6050_RegMap::WHO_AM_I, &val);
        if (0x71 == val) {
            return true;
        }
        val = 0;
        SetI2cAddr(0x69);
        ReadReg((uint8_t) mpu6050_RegMap::WHO_AM_I, &val);
        if (0x71 == val) {
            return true;
        }
        return false;
    }
    int mpu9250_t::SelfTest() {
        if (!IsOpen()) { return -1; }
        int res = 0;
        config_t backup_cfg = GetConfig();
        config_t st_cfg;
        st_cfg.gyroFullScale = config_t::MPU_FS_250dps;
        st_cfg.accelFullScale = config_t::MPU_FS_2G;
        st_cfg.accelBandwidth = config_t::MPU_ABW_99;
        st_cfg.gyroBandwidth = config_t::MPU_GBW_92;
        if (0 != Init(st_cfg)) {
            Init(backup_cfg);
            return -1;
        }
        int32_t gyro_bias_st[3], gyro_bias_regular[3];
        int32_t accel_bias_st[3], accel_bias_regular[3];
        int16_t abuf[3];
        int16_t gbuf[3];
        int accel_result = 0;
        int gyro_result = 0;
        uint8_t val;
        memset(gyro_bias_st, 0, sizeof(gyro_bias_st));
        memset(gyro_bias_regular, 0, sizeof(gyro_bias_regular));
        memset(accel_bias_st, 0, sizeof(accel_bias_st));
        memset(accel_bias_regular, 0, sizeof(accel_bias_regular));

        int times;
        times = 20;
        while (times--) { while (!DataReady()) {}}//丢弃前20个数据
        times = 20;
        while (times--) {
            while (!DataReady()) {}
            res |= ReadSensorBlocking();
            Convert(abuf, abuf + 1, abuf + 2, gbuf, gbuf + 1, gbuf + 2);
            for (int i = 0; i < 3; ++i) {
                gyro_bias_regular[i] += gbuf[i];
                accel_bias_regular[i] += abuf[i];
            }
        }

        res |= ReadReg((uint8_t) mpu9250_RegMap::GYRO_CONFIG, &val);
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::GYRO_CONFIG, val | (0b111 << 5));//打开陀螺仪自检
        res |= ReadReg((uint8_t) mpu9250_RegMap::ACCEL_CONFIG, &val);
        res |= WriteRegVerified((uint8_t) mpu9250_RegMap::ACCEL_CONFIG, val | (0b111 << 5));//打开加速度计自检
        times = 20;
        while (times--) { while (!DataReady()) {}}//丢弃前20个数据
        times = 20;
        while (times--) {
            while (!DataReady()) {}
            res |= ReadSensorBlocking();
            Convert(abuf, abuf + 1, abuf + 2, gbuf, gbuf + 1, gbuf + 2);
            for (int i = 0; i < 3; ++i) {
                gyro_bias_st[i] += gbuf[i];
                accel_bias_st[i] += abuf[i];
            }
        }

        for (int i = 0; i < 3; ++i) {
            gyro_bias_regular[i] *= 50;   //(32768/2000)*1000 LSB/mg
            accel_bias_regular[i] *= 50;
            gyro_bias_st[i] *= 50;         //(32768/250)*1000 LSB/dps
            accel_bias_st[i] *= 50;
        }


        //计算加速度计自检结果
        uint8_t regs[3];
        int otp_value_zero = 0;
        int st_shift_prod[3], st_shift_cust[3], st_shift_ratio[3], i;
//    int result;

        res |= (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) mpu9250_RegMap::SELF_TEST_X_ACCEL, regs, 3);
        for (i = 0; i < 3; i++) {
            if (regs[i] != 0) {
                st_shift_prod[i] = sSelfTestEquation[regs[i] - 1];
            } else {
                st_shift_prod[i] = 0;
                otp_value_zero = 1;
            }
        }

        if (!otp_value_zero) {
            /* Self Test Pass/Fail Criteria A */
            for (i = 0; i < 3; i++) {
                st_shift_cust[i] = accel_bias_st[i] - accel_bias_regular[i];
                st_shift_ratio[i] = abs(st_shift_cust[i] / st_shift_prod[i] - DEF_ST_PRECISION);
                if (st_shift_ratio[i] > DEF_ACCEL_ST_SHIFT_DELTA) {
                    //加速度计自检未通过
                    accel_result = 1;
                    INV_DEBUG("accel[%d] st fail,result = %d,it demands less than %d", i, st_shift_ratio[i],
                              DEF_ACCEL_ST_SHIFT_DELTA);
                } else {
                    INV_TRACE("accel[%d] st result = %d,it demands less than %d", i, st_shift_ratio[i],
                              DEF_ACCEL_ST_SHIFT_DELTA);
                }
            }
        } else {
            /* Self Test Pass/Fail Criteria B */
            for (i = 0; i < 3; i++) {
                st_shift_cust[i] = abs(accel_bias_st[i] - accel_bias_regular[i]);
                if (st_shift_cust[i] < DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000
                    || st_shift_cust[i] > DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000) {
                    //加速度计自检未通过
                    accel_result = 1;
                    INV_DEBUG("accel[%d] st fail,result = %d,it demands <%d && >%d", i, st_shift_cust[i],
                              DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000, DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000);
                } else {
                    INV_TRACE("accel[%d] st result = %d,it demands <%d && >%d", i, st_shift_cust[i],
                              DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000, DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000);
                }
            }
        }

        //计算陀螺仪自检结果
        res |= (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) mpu9250_RegMap::SELF_TEST_X_GYRO, regs, 3);
        for (i = 0; i < 3; i++) {
            if (regs[i] != 0) {
                st_shift_prod[i] = sSelfTestEquation[regs[i] - 1];
            } else {
                st_shift_prod[i] = 0;
                otp_value_zero = 1;
            }
        }

        for (i = 0; i < 3; i++) {
            st_shift_cust[i] = gyro_bias_st[i] - gyro_bias_regular[i];
            if (!otp_value_zero) {
                /* Self Test Pass/Fail Criteria A */
                if (st_shift_cust[i] < DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]) {
                    //陀螺仪自检没过
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]);
                }
            } else {
                /* Self Test Pass/Fail Criteria B */
                if (st_shift_cust[i] < DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION) {
                    //陀螺仪自检没过
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION);
                }
            }
        }

        if (gyro_result == 0) {
            /* Self Test Pass/Fail Criteria C */
            for (i = 0; i < 3; i++) {
                if (abs(gyro_bias_regular[i]) > DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION)
                    //陀螺仪自检没过
                {
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,ift demands less than %d", i, (int) abs(gyro_bias_regular[i]),
                              DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands less than %d", i, (int) abs(gyro_bias_regular[i]),
                              DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION);
                }
            }
        }

        //恢复原来的配置
        res |= Init(backup_cfg);
        return res | (gyro_result << 1) | accel_result;
    }

    bool mpu9250_t::DataReady() {
        uint8_t val = 0;
        ReadReg((uint8_t) mpu9250_RegMap::INT_STATUS, &val);
        return (val & 0x01) == 0x01;
    }

    int mpu9250_t::EnableDataReadyInt() {
        return ModifyReg((uint8_t) mpu9250_RegMap::INT_ENABLE, 0x01, 0x01);
    }

    int imuPtr_t::Load(i2cInterface_t &_i2c, uint8_t _addr) {
        if (icm20602_t(_i2c, _addr).Detect()) {
            INV_TRACE("icm20602 detected");
            reset(new icm20602_t(_i2c, _addr));
            return 0;
        } else if (mpu6050_t(_i2c, _addr).Detect()) {
            INV_TRACE("mpu6050 detected");
            reset(new mpu6050_t(_i2c, _addr));
            return 0;
        } else if (mpu9250_t(_i2c, _addr).Detect()) {
            INV_TRACE("mpu9250 detected");
            reset(new mpu9250_t(_i2c, _addr));
            return 0;
        } else if (icm20600_t(_i2c, _addr).Detect()) {
            INV_TRACE("icm20600 detected");
            reset(new icm20600_t(_i2c, _addr));
            return 0;
        } else if (icm20948_t(_i2c, _addr).Detect()) {
            INV_TRACE("icm20948 detected");
            reset(new icm20948_t(_i2c, _addr));
            return 0;
        }
        return -1;
    }

    int imu_t::WriteReg(uint8_t reg, const uint8_t val) {
        int res = (*i2c.writeBlocking)(i2c.context, addr, reg, &val, 1);
        if (res != 0) {
            INV_DEBUG("i2c write return code = %d", res);
        }
        return res;
    }

    int imu_t::ReadReg(uint8_t reg, uint8_t *val) {
        int res = (*i2c.readBlocking)(i2c.context, addr, reg, val, 1);
        if (res != 0) {
            INV_DEBUG("i2c read return code = %d", res);
        }
        return res;
    }

    int imu_t::ModifyReg(uint8_t reg, const uint8_t val, const uint8_t mask) {
        uint8_t regVal;
        int res = 0;
        res |= ReadReg(reg, &regVal);
        res |= WriteRegVerified(reg, (regVal & (~mask)) | (val & mask));
        res |= ReadReg(reg, &regVal);
        if ((regVal & mask) != (val & mask)) {
            INV_DEBUG("i2c rw error");
            res |= -1;
        }
        return res;
    }

    int imu_t::WriteRegVerified(uint8_t reg, const uint8_t val) {
        uint8_t regVal;
        int res = 0;
        res |= WriteReg(reg, val);
        res |= ReadReg(reg, &regVal);
        if (res == 0 && val != regVal) {
            res |= WriteReg(reg, val);
            res |= ReadReg(reg, &regVal);
            if (res == 0 && val != regVal) {
                INV_DEBUG("i2c rw error");
                res |= -1;
            }
        }
        return res;
    }
    bool icm20600_t::Detect() {
        uint8_t val = 0;
        SetI2cAddr(0x68);
        ReadReg((uint8_t) icm20602_RegMap::WHO_AM_I, &val);
        if (0x11 == val) {
            return true;
        }
        val = 0;
        SetI2cAddr(0x69);
        ReadReg((uint8_t) icm20602_RegMap::WHO_AM_I, &val);
        if (0x11 == val) {
            return true;
        }
        return false;
    }
    std::string icm20600_t::Report() {
        std::string rtv;
        rtv += "model:icm20600\t";
        rtv += "addr:";
        rtv += std::to_string((int) GetI2cAddr());
        rtv += '\t';
        return rtv;
    }

    int icm20948_t::SoftReset() {
        if (!Detect()) { return -1; }
        int res = 0;
        uint8_t val;
        //先选择bank0
        SwitchBank(0);
        //复位
        res |= WriteReg((uint16_t) icm20948_RegMap::PWR_MGMT_1, 0x80);
        //等待复位成功
        do {
            ReadReg((uint16_t) icm20948_RegMap::PWR_MGMT_1, &val);
            INV_TRACE("0x%x at PWR_MGMT_1,wait it get 0x41", val);
        } while (val != 0x41);

        //唤起睡眠
        ReadReg((uint16_t) icm20948_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint16_t) icm20948_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint16_t) icm20948_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint16_t) icm20948_RegMap::PWR_MGMT_1, &val);
        ReadReg((uint16_t) icm20948_RegMap::PWR_MGMT_1, &val);
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::PWR_MGMT_1, 0x1);
        return res;
    }
    bool icm20948_t::Detect() {
        uint8_t val = 0;
        SetI2cAddr(0x68);
        SwitchBank(0);
        ReadReg((uint16_t) icm20948_RegMap::WHO_AM_I, &val);
        if (0xEA == val) {
            return true;
        }
        val = 0;
        SetI2cAddr(0x69);
        SwitchBank(0);
        ReadReg((uint16_t) icm20948_RegMap::WHO_AM_I, &val);
        if (0xEA == val) {
            return true;
        }
        return false;
    }

    int icm20948_t::SwitchBank(int _bank) {
        bank = _bank;
        return imu_t::WriteRegVerified((uint8_t) icm20948_RegMap::REG_BANK_SEL, _bank << 4);
    }
    int icm20948_t::WriteReg(uint16_t reg, const uint8_t val) {
        if (bank != reg >> 8) {
            SwitchBank(reg >> 8);
        }
        return imu_t::WriteReg(reg, val);
    }
    int icm20948_t::WriteRegVerified(uint16_t reg, const uint8_t val) {
        if (bank != reg >> 8) {
            SwitchBank(reg >> 8);
        }
        return imu_t::WriteRegVerified(reg, val);
    }
    int icm20948_t::ReadReg(uint16_t reg, uint8_t *val) {
        if (bank != reg >> 8) {
            SwitchBank(reg >> 8);
        }
        return imu_t::ReadReg(reg, val);
    }
    int icm20948_t::ModifyReg(uint16_t reg, const uint8_t val, const uint8_t mask) {
        if (bank != reg >> 8) {
            SwitchBank(reg >> 8);
        }
        return imu_t::ModifyReg(reg, val, mask);
    }

    int icm20948_t::Init(config_t _cfg) {
        SetConfig(_cfg);
        ClearIsOpen();
        int res = 0;
        if (!Detect()) { return -1; }
        //软复位
        res |= SoftReset();

        //打开所有传感器
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::PWR_MGMT_2, 0);

        //1.125khz采样率
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_SMPLRT_DIV, 0);
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_SMPLRT_DIV_1, 0);
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_SMPLRT_DIV_2, 0);

        //配置陀螺仪lpf
//        res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 0);
        switch (GetConfig().gyroBandwidth) {
            case config_t::MPU_GBW_250:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 | (0 << 3));
                break;
            case config_t::MPU_GBW_176:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 | (1 << 3));
                break;
            case config_t::MPU_GBW_92:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 | (2 << 3));
                break;
            case config_t::MPU_GBW_41:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 | (3 << 3));
                break;
            case config_t::MPU_GBW_20:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 | (4 << 3));
                break;
            case config_t::MPU_GBW_10:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 | (5 << 3));
                break;
            case config_t::MPU_GBW_5:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 | (6 << 3));
                break;
            default:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 | (1 << 3));
                break;
        }

        //配置陀螺仪量程和单位
        switch (GetConfig().gyroUnit) {
            case config_t::MPU_UNIT_RadPerSec:
                gyroUnit = (250.0 / 32768) * (M_PI / 180);
                break;
            case config_t::MPU_UNIT_RevolutionsPerMinute:
                gyroUnit = (250.0 / 32768) * (60 / 360);
                break;
            case config_t::MPU_UNIT_DegPerSec:
            default:
                gyroUnit = 250.0 / 32768;
                break;
        }

        switch (GetConfig().gyroFullScale) {
            case config_t::MPU_FS_250dps:
                res |= ModifyReg((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 0 << 1, 3 << 1);
                break;
            case config_t::MPU_FS_500dps:
                res |= ModifyReg((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 1 << 1, 3 << 1);
                gyroUnit *= 2;
                break;
            case config_t::MPU_FS_1000dps:
                res |= ModifyReg((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 2 << 1, 3 << 1);
                gyroUnit *= 4;
                break;
            case config_t::MPU_FS_2000dps:
            default:
                res |= ModifyReg((uint16_t) icm20948_RegMap::GYRO_CONFIG_1, 3 << 1, 3 << 1);
                gyroUnit *= 8;
                break;
        }

        //配置加速度计lpf
        switch (GetConfig().accelBandwidth) {
            case config_t::MPU_ABW_218:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 | (1 << 3));
                break;
            case config_t::MPU_ABW_99:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 | (2 << 3));
                break;
            case config_t::MPU_ABW_45:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 | (3 << 3));
                break;
            case config_t::MPU_ABW_21:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 | (4 << 3));
                break;
            case config_t::MPU_ABW_10:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 | (5 << 3));
                break;
            case config_t::MPU_ABW_5:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 | (6 << 3));
                break;
            case config_t::MPU_ABW_420:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 | (7 << 3));
                break;
            default:
                res |= WriteRegVerified((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 | (1 << 3));
                break;
        }

        //配置加速度计量程和单位
        switch (GetConfig().accelUnit) {
            case config_t::MPU_UNIT_G:
                accelUnit = (2.0 / 32768);
                break;
            case config_t::MPU_UNIT_mG:
                accelUnit = (2000.0 / 32768);
                break;
            case config_t::MPU_UNIT_MetersPerSquareSecond:
            default:
                accelUnit = 2.0 * 9.8 / 32768;
                break;
        }
        switch (GetConfig().accelFullScale) {
            case config_t::MPU_FS_2G:
                res |= ModifyReg((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 0 << 1, 3 << 1);
                break;
            case config_t::MPU_FS_4G:
                res |= ModifyReg((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 1 << 1, 3 << 1);
                accelUnit *= 2;
                break;
            case config_t::MPU_FS_8G:
                res |= ModifyReg((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 2 << 1, 3 << 1);
                accelUnit *= 4;
                break;
            case config_t::MPU_FS_16G:
            default:
                res |= ModifyReg((uint16_t) icm20948_RegMap::ACCEL_CONFIG, 3 << 1, 3 << 1);
                accelUnit *= 8;
                break;
        }

        //设置温度输出lpf
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::TEMP_CONFIG, 6); //8hz

        //开启数据更新中断
        res |= EnableDataReadyInt();

        if (res != 0) { return res; }
        ClearIsOpen();

        //设置内部i2c
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::I2C_MST_CTRL, 1 << 4 | 12);//471khz，连续读模式
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::USER_CTRL, 1 << 5);//开启i2c主模式

        //开始设置ak8963
        res |= SubI2cRead(ICM20948_AK09916_I2C_ADDR, (uint8_t) ak09916_RegMap::WIA2, &ak09916DeviceId, 1);
        if (res != 0) { return res; }

        //复位
        uint8_t val = 0x01;
        res |= SubI2cWrite(ICM20948_AK09916_I2C_ADDR, (uint8_t) ak09916_RegMap::CNTL3, &val, 1);
        val = 0;
        res |= SubI2cWrite(ICM20948_AK09916_I2C_ADDR, (uint8_t) ak09916_RegMap::CNTL2, &val, 1);

        //设置连续读ak8963到fifo
        val = 0x5D;
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::I2C_MST_CTRL, val);

        val = ICM20948_AK09916_I2C_ADDR | 0x80;
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::I2C_SLV0_ADDR, val);

        val = (uint8_t) ak09916_RegMap::ST1;
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::I2C_SLV0_REG, val);


        //唤醒ak09916
        val = (1 << 3);
        res |= SubI2cWrite(ICM20948_AK09916_I2C_ADDR, (uint8_t) ak09916_RegMap::CNTL2, &val, 1);

        val = 0x89;
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::I2C_SLV0_CTRL, val);

        val = 0x09;
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::I2C_SLV4_CTRL, val);

        val = 0x81;
        res |= WriteRegVerified((uint16_t) icm20948_RegMap::I2C_MST_DELAY_CTRL, val);


        if (res == 0) {
            SetIsOpen();
            return 0;
        } else {
            return res;
        }
    }
    int icm20948_t::SelfTest() {
        if (!IsOpen()) { return -1; }
        int res = 0;
        config_t backup_cfg = GetConfig();
        config_t st_cfg;
        st_cfg.gyroFullScale = config_t::MPU_FS_250dps;
        st_cfg.accelFullScale = config_t::MPU_FS_2G;
        st_cfg.accelBandwidth = config_t::MPU_ABW_99;
        st_cfg.gyroBandwidth = config_t::MPU_GBW_92;
        if (0 != Init(st_cfg)) {
            Init(backup_cfg);
            return -1;
        }
        int32_t gyro_bias_st[3], gyro_bias_regular[3];
        int32_t accel_bias_st[3], accel_bias_regular[3];
        int16_t abuf[3];
        int16_t gbuf[3];
        int accel_result = 0;
        int gyro_result = 0;
        uint8_t val;
        memset(gyro_bias_st, 0, sizeof(gyro_bias_st));
        memset(gyro_bias_regular, 0, sizeof(gyro_bias_regular));
        memset(accel_bias_st, 0, sizeof(accel_bias_st));
        memset(accel_bias_regular, 0, sizeof(accel_bias_regular));

        int times;
        times = 20;
        while (times--) { while (!DataReady()) {}}//丢弃前20个数据
        times = 20;
        while (times--) {
            while (!DataReady()) {}
            res |= ReadSensorBlocking();
            Convert(abuf, abuf + 1, abuf + 2, gbuf, gbuf + 1, gbuf + 2);
            for (int i = 0; i < 3; ++i) {
                gyro_bias_regular[i] += gbuf[i];
                accel_bias_regular[i] += abuf[i];
            }
        }

        //打开自检输出
        res |= ModifyReg((uint16_t) icm20948_RegMap::GYRO_CONFIG_2, 0b111 << 3, 0b111 << 3);
        res |= ModifyReg((uint16_t) icm20948_RegMap::ACCEL_CONFIG_2, 0b111 << 3, 0b111 << 3);

        times = 20;
        while (times--) { while (!DataReady()) {}}//丢弃前20个数据
        times = 20;
        while (times--) {
            while (!DataReady()) {}
            res |= ReadSensorBlocking();
            Convert(abuf, abuf + 1, abuf + 2, gbuf, gbuf + 1, gbuf + 2);
            for (int i = 0; i < 3; ++i) {
                gyro_bias_st[i] += gbuf[i];
                accel_bias_st[i] += abuf[i];
            }
        }

        for (int i = 0; i < 3; ++i) {
            gyro_bias_regular[i] *= 50;   //(32768/2000)*1000 LSB/mg
            accel_bias_regular[i] *= 50;
            gyro_bias_st[i] *= 50;         //(32768/250)*1000 LSB/dps
            accel_bias_st[i] *= 50;
        }


        //计算加速度计自检结果
        uint8_t regs[3];
        int otp_value_zero = 0;
        int st_shift_prod[3], st_shift_cust[3], st_shift_ratio[3], i;
//    int result;
        res |= ReadReg((uint16_t) icm20948_RegMap::SELF_TEST_X_ACCEL, regs);
        res |= ReadReg((uint16_t) icm20948_RegMap::SELF_TEST_Y_ACCEL, regs + 1);
        res |= ReadReg((uint16_t) icm20948_RegMap::SELF_TEST_Z_ACCEL, regs + 2);
        for (i = 0; i < 3; i++) {
            if (regs[i] != 0) {
                st_shift_prod[i] = sSelfTestEquation[regs[i] - 1];
            } else {
                st_shift_prod[i] = 0;
                otp_value_zero = 1;
            }
        }

        if (!otp_value_zero) {
            /* Self Test Pass/Fail Criteria A */
            for (i = 0; i < 3; i++) {
                st_shift_cust[i] = accel_bias_st[i] - accel_bias_regular[i];
                st_shift_ratio[i] = abs(st_shift_cust[i] / st_shift_prod[i] - DEF_ST_PRECISION);
                if (st_shift_ratio[i] > DEF_ACCEL_ST_SHIFT_DELTA) {
                    //加速度计自检未通过
                    accel_result = 1;
                    INV_DEBUG("accel[%d] st fail,result = %d,it demands less than %d", i, st_shift_ratio[i],
                              DEF_ACCEL_ST_SHIFT_DELTA);
                } else {
                    INV_TRACE("accel[%d] st result = %d,it demands less than %d", i, st_shift_ratio[i],
                              DEF_ACCEL_ST_SHIFT_DELTA);
                }
            }
        } else {
            /* Self Test Pass/Fail Criteria B */
            for (i = 0; i < 3; i++) {
                st_shift_cust[i] = abs(accel_bias_st[i] - accel_bias_regular[i]);
                if (st_shift_cust[i] < DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000
                    || st_shift_cust[i] > DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000) {
                    //加速度计自检未通过
                    accel_result = 1;
                    INV_DEBUG("accel[%d] st fail,result = %d,it demands <%d && >%d", i, st_shift_cust[i],
                              DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000, DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000);
                } else {
                    INV_TRACE("accel[%d] st result = %d,it demands <%d && >%d", i, st_shift_cust[i],
                              DEF_ACCEL_ST_AL_MAX * (32768 / 2000) * 1000, DEF_ACCEL_ST_AL_MIN * (32768 / 2000) * 1000);
                }
            }
        }

        //计算陀螺仪自检结果
        res |= ReadReg((uint16_t) icm20948_RegMap::SELF_TEST_X_GYRO, regs);
        res |= ReadReg((uint16_t) icm20948_RegMap::SELF_TEST_Y_GYRO, regs + 1);
        res |= ReadReg((uint16_t) icm20948_RegMap::SELF_TEST_Z_GYRO, regs + 2);
        for (i = 0; i < 3; i++) {
            if (regs[i] != 0) {
                st_shift_prod[i] = sSelfTestEquation[regs[i] - 1];
            } else {
                st_shift_prod[i] = 0;
                otp_value_zero = 1;
            }
        }

        for (i = 0; i < 3; i++) {
            st_shift_cust[i] = gyro_bias_st[i] - gyro_bias_regular[i];
            if (!otp_value_zero) {
                /* Self Test Pass/Fail Criteria A */
                if (st_shift_cust[i] < DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]) {
                    //陀螺仪自检没过
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_CT_SHIFT_DELTA * st_shift_prod[i]);
                }
            } else {
                /* Self Test Pass/Fail Criteria B */
                if (st_shift_cust[i] < DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION) {
                    //陀螺仪自检没过
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands greater than %d", i, st_shift_cust[i],
                              DEF_GYRO_ST_AL * (32768 / 250) * DEF_ST_PRECISION);
                }
            }
        }

        if (gyro_result == 0) {
            /* Self Test Pass/Fail Criteria C */
            for (i = 0; i < 3; i++) {
                if (abs(gyro_bias_regular[i]) > DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION)
                    //陀螺仪自检没过
                {
                    gyro_result = 1;
                    INV_DEBUG("gyro[%d] st fail,result = %d,ift demands less than %d", i, (int) abs(gyro_bias_regular[i]),
                              DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION);
                } else {
                    INV_TRACE("gyro[%d] st result = %d,it demands less than %d", i, (int) abs(gyro_bias_regular[i]),
                              DEF_GYRO_OFFSET_MAX * (32768 / 250) * DEF_ST_PRECISION);
                }
            }
        }

        //恢复原来的配置
        res |= Init(backup_cfg);
        return res | (gyro_result << 1) | accel_result;
    }
    bool icm20948_t::DataReady() {
        uint8_t val = 0;
        ReadReg((uint16_t) icm20948_RegMap::INT_STATUS_1, &val);
        return (val & 0x01) == 0x01;
    }
    int icm20948_t::EnableDataReadyInt() {
        return ModifyReg((uint16_t) icm20948_RegMap::INT_ENABLE_1, 0x01, 0x01);
    }
    int icm20948_t::SubI2cRead(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len) {
        uint8_t index = 0;
        uint8_t status = 0;
        uint32_t timeout = 0;
        uint8_t tmp = 0;
        int res = 0;
        tmp = addr | 0x80;
        res |= WriteReg((uint16_t) icm20948_RegMap::I2C_SLV4_ADDR, tmp);
        while (index < len) {
            tmp = reg + index;
            res |= WriteReg((uint16_t) icm20948_RegMap::I2C_SLV4_REG, tmp);
            tmp = MPU9250_I2C_SLV4_EN;
            res |= WriteReg((uint16_t) icm20948_RegMap::I2C_SLV4_CTRL, tmp);
            do {
                if (timeout++ > 5000) {
                    INV_DEBUG("SubI2cRead time out");
                    return -2;
                }
                res |= ReadReg((uint16_t) icm20948_RegMap::I2C_MST_STATUS, &status);
            } while ((status & MPU9250_I2C_SLV4_DONE) == 0);
            if (status & MPU9250_I2C_SLV4_NACK) {
                INV_DEBUG("SubI2cRead no ack");
                return -3;
            }
            res |= ReadReg((uint16_t) icm20948_RegMap::I2C_SLV4_DI, val + index);
            index++;
        }
        return res;
    }
    int icm20948_t::SubI2cWrite(uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len) {
        uint32_t timeout = 0;
        uint8_t status = 0;
        uint8_t tmp = 0;
        uint8_t index = 0;
        int res = 0;
        tmp = addr;
        res |= WriteReg((uint16_t) icm20948_RegMap::I2C_SLV4_ADDR, tmp);
        while (index < len) {
            tmp = reg + index;
            res |= WriteReg((uint16_t) icm20948_RegMap::I2C_SLV4_REG, tmp);
            tmp = val[index];
            res |= WriteReg((uint16_t) icm20948_RegMap::I2C_SLV4_DO, tmp);
            tmp = MPU9250_I2C_SLV4_EN;
            res |= WriteReg((uint16_t) icm20948_RegMap::I2C_SLV4_CTRL, tmp);
            do {
                if (timeout++ > 5000) {
                    INV_DEBUG("SubI2cWrite time out");
                    return -2;
                }
                res |= ReadReg((uint16_t) icm20948_RegMap::I2C_MST_STATUS, &status);
            } while ((status & MPU9250_I2C_SLV4_DONE) == 0);
            if (status & MPU9250_I2C_SLV4_NACK) {
                INV_DEBUG("SubI2cWrite no ack");
                return -3;
            }
            index++;
        }
        return res;
    }

    int icm20948_t::ReadSensorBlocking() {
        if (bank != 0) {
            SwitchBank(0);
        }
        return (*i2c.readBlocking)(i2c.context, GetI2cAddr(), (uint8_t) icm20948_RegMap::ACCEL_XOUT_H, buf, 14 + 9);
    }

    int icm20948_t::ReadSensorNonBlocking() {
        if (bank != 0) {
            SwitchBank(0);
        }
        return (*i2c.readNonBlocking)(i2c.context, GetI2cAddr(), (uint8_t) icm20948_RegMap::ACCEL_XOUT_H, buf, 14 + 9);
    }

    int icm20948_t::Convert(float *acc_x, float *acc_y, float *acc_z, float *gyro_x, float *gyro_y, float *gyro_z) {
        if (acc_x) { *acc_x = accelUnit * ((int16_t) ((buf[0] << 8) | buf[1])); }
        if (acc_y) { *acc_y = accelUnit * ((int16_t) ((buf[2] << 8) | buf[3])); }
        if (acc_z) { *acc_z = accelUnit * ((int16_t) ((buf[4] << 8) | buf[5])); }
        if (gyro_x) { *gyro_x = gyroUnit * ((int16_t) ((buf[6] << 8) | buf[7])); }
        if (gyro_y) { *gyro_y = gyroUnit * ((int16_t) ((buf[8] << 8) | buf[9])); }
        if (gyro_z) { *gyro_z = gyroUnit * ((int16_t) ((buf[10] << 8) | buf[11])); }
        return 0;
    }
    int icm20948_t::Convert(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z, int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) {
        if (acc_x) { *acc_x = ((int16_t) ((buf[0] << 8) | buf[1])); }
        if (acc_y) { *acc_y = ((int16_t) ((buf[2] << 8) | buf[3])); }
        if (acc_z) { *acc_z = ((int16_t) ((buf[4] << 8) | buf[5])); }
        if (gyro_x) { *gyro_x = ((int16_t) ((buf[6] << 8) | buf[7])); }
        if (gyro_y) { *gyro_y = ((int16_t) ((buf[8] << 8) | buf[9])); }
        if (gyro_z) { *gyro_z = ((int16_t) ((buf[10] << 8) | buf[11])); }
        return 0;
    }

    int icm20948_t::Convert(float *mag_x, float *mag_y, float *mag_z) {
//        if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY) || (buf[14 + 0] & MPU9250_AK8963_DATA_OVERRUN)) {
        if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY)) {
//            INV_TRACE("0x%x at buf[14 + 0]", (int) buf[14 + 0]);
            return -1;
        }
        if (buf[14 + 8] & MPU9250_AK8963_OVERFLOW) {
            INV_TRACE("0x%x at buf[14 + 7]", (int) buf[14 + 7]);
            return -1;
        }
        if (mag_x) { *mag_x = magUnit * ((int16_t) (buf[14 + 2] << 8) | buf[14 + 1]); }
        if (mag_y) { *mag_y = magUnit * ((int16_t) (buf[14 + 4] << 8) | buf[14 + 3]); }
        if (mag_z) { *mag_z = magUnit * ((int16_t) (buf[14 + 6] << 8) | buf[14 + 5]); }
        return 0;
    }
    int icm20948_t::Convert(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) {
        if (!(buf[14 + 0] & MPU9250_AK8963_DATA_READY) || (buf[14 + 0] & MPU9250_AK8963_DATA_OVERRUN)) {
//            INV_TRACE("0x%x at buf[14 + 0]", (int) buf[14 + 0]);
            return -1;
        }
        if (buf[14 + 8] & MPU9250_AK8963_OVERFLOW) {
//            INV_TRACE("0x%x at buf[14 + 7]", (int) buf[14 + 7]);
            return -1;
        }
        if (mag_x) { *mag_x = ((int16_t) (buf[14 + 2] << 8) | buf[14 + 1]); }
        if (mag_y) { *mag_y = ((int16_t) (buf[14 + 4] << 8) | buf[14 + 3]); }
        if (mag_z) { *mag_z = ((int16_t) (buf[14 + 6] << 8) | buf[14 + 5]); }
        return 0;
    }
    int icm20948_t::Convert(float *temp) {
        if (temp) { *temp = (float) ((int16_t) ((buf[12] << 8) | buf[13])) / 333.87f + 21; }
        return 0;
    }

    std::string icm20948_t::Report() {
        std::string rtv;
        rtv += "model:icm20948\t";
        rtv += "addr:";
        rtv += std::to_string((int) GetI2cAddr());
        rtv += '\t';

        rtv += "magnetometer:ak09916\t";
        rtv += "ID:";
        rtv += std::to_string((int) ak09916DeviceId);
        return rtv;
    }

}

#endif // ! HITSIC_USE_DRV_IMU_INV
