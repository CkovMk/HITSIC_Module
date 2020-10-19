/**
 * @brief 陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 * @author xiao qq1761690868
 * @doc drv_imu_invensense.md
 * @version v1.0
 * @date 2020-10-16
 */

#ifndef UTILITIES_DRV_IMU_INVENSENSE_HPP
#define UTILITIES_DRV_IMU_INVENSENSE_HPP

#include <cstdint>
#include <string>
#include <memory>
#include "drv_imu_invensense_def.hpp"
#include "drv_imu_invensense_port.hpp"

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
#endif//!(defined(HITSIC_INV_NO_DEBUG)&&(HITSIC_INV_NO_DEBUG>0))

#endif//(defined(HITSIC_INV_IMU_DEBUG)&&(HITSIC_INV_IMU_DEBUG>0))

#endif //INV_PRINTF

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
    class i2cInterface_t;//i2c接口
    struct config_t;//设置量程和数字低通滤波器

    class imu_t;//imu接口类，抽象出初始化/数据转换/SelfTest/Detect/IO的欸皮埃

    class mpuSeries_t;//基类，抽象出invensense的mpu系列以及部分icm系列imu的软复位/中断/温度转换/WhoAmI的api，实现初始化/数据转换/Detect/IO的方法
    class mpu6050_t;//mpu6050驱动

    class mpu6500Series_t;//基类，实现mpu6500系列以及部分icm系列imu的自检方法

    class icm20602_t;//icm20602驱动
    class mpu9250_t;//mpu9250驱动

    class imuPtr_t;//imu的智能指针类，用于实例化imu对象


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
                                             uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len)):
                i2cInterface_t(_context,_readBlocking,_writeBlocking,_readBlocking){}


        void *context;
        int (*readBlocking)(void *context,
                            uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);
        int (*writeBlocking)(void *context,
                             uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len);
        int (*readNonBlocking)(void *context,
                               uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);
    };

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

        config_t(mpu_accel_fs _accel_fs = MPU_FS_8G, mpu_accel_bw _accel_bw = MPU_ABW_99,
                 mpu_gyro_fs _gyro_gs = MPU_FS_2000dps, mpu_gyro_bw _gyro_bw = MPU_GBW_92) :
                accelFullScale(_accel_fs), accelBandwidth(_accel_bw),
                gyroFullScale(_gyro_gs), gyroBandwidth(_gyro_bw) {}
    };

    class imu_t {
    public:
        virtual ~imu_t() {}

        /**
         * @brief   初始化imu，初始化之后才能使用其他方法
         * @param  {config_t} _cfg : 量程等配置信息
         * @return {int}           : 错误码
         */
        virtual int Init(config_t _cfg = config_t()) = 0;

        /**
         * @brief   可以在初始化之前执行，检查是否有imu，并且设置正确的iic地址
         * @return {bool}  : true为成功
         */
        virtual bool Detect() = 0;

        /**
         * @brief 自检，自检过程中保持传感器静止
         * @return {int}  : 错误码
         */
        virtual int SelfTest() = 0;

        /**
         * @brief   转换！！！缓存！！!中加速度和陀螺仪的数据到指定的地方，单位如下
         * @param  {float*} acc_x  :可以等于NULL，米每平方妙
         * @param  {float*} acc_y  :可以等于NULL，米每平方妙
         * @param  {float*} acc_z  :可以等于NULL，米每平方妙
         * @param  {float*} gyro_x :可以等于NULL， dps(degree per second)
         * @param  {float*} gyro_y :可以等于NULL， dps(degree per second)
         * @param  {float*} gyro_z :可以等于NULL， dps(degree per second)
         * @return {int}           :错误码
         */
        virtual int Converter(float *acc_x, float *acc_y, float *acc_z,
                              float *gyro_x, float *gyro_y, float *gyro_z) = 0;

        /**
         * @brief   转换！！！缓存！！!中磁力计的数据到指定的地方，单位为uT
         * @param  {float*} mag_x : 可以等于NULL
         * @param  {float*} mag_y : 可以等于NULL
         * @param  {float*} mag_z : 可以等于NULL
         * @return {int}          : 错误码
         */
        virtual int Converter(float *mag_x, float *mag_y, float *mag_z) = 0;

        /**
         * @brief   调用阻塞IO读取传感器数据到缓存，当读取完成或者发送错误时返回
         * @return {int}  : 错误码
         */
        virtual int ReadSensorBlocking() = 0;

        /**
         * @brief   调用非阻塞IO读取传感器数据到缓存，立即返回，请自行确定非阻塞读取是否完成
         * @return {int}  : 错误码
         */
        virtual int ReadSensorNonBlocking() = 0;

        /**
         * @brief   返回字符串告诉用户传感器的模组信息
         * @return {std::string}  : 字符串
         */
        virtual std::string Report() = 0;

    public:

        /**
         * @brief   返回是否初始化
         * @return {bool}  : true为是
         */
        bool IsOpen() { return isOpen; };

        /**
         * @brief   设置内存中的量程等配置信息，不会对传感器生效
         * @param  {config_t} _cfg : 
         */
        void SetConfig(config_t _cfg) { cfg = _cfg; }

        /**
         * @return {config_t}  :量程等配置信息
         */
        constexpr const config_t &GetConfig() { return cfg; }

        /**
         * @brief   设置通讯中的iic从机地址
         * @param  {uint8_t} _addr : 从机地址
         */
        void SetI2cAddr(uint8_t _addr) { addr = _addr; }

        /**
         * @return {uint8_t}  : 从机地址
         */
        constexpr const uint8_t &GetI2cAddr() { return addr; }

    protected:
        void SetIsOpen() { isOpen = true; }

        void ClearIsOpen() { isOpen = false; }

        /**
         * imu_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        imu_t(i2cInterface_t &_i2c) : i2c(_i2c), isOpen(false), addr(0), cfg(config_t()) {}

        i2cInterface_t &i2c;

        int WriteReg(uint8_t reg, const uint8_t val);;

        int ReadReg(uint8_t reg, uint8_t *val);;

        int ModifyReg(uint8_t reg, const uint8_t val, const uint8_t mask) {
            uint8_t regVal;
            int res = 0;
            res |= ReadReg(reg, &regVal);
            res |= WriteReg(reg, (regVal & (~mask)) | (val & mask));
            return res;
        }

    private:
        uint8_t addr;
        bool isOpen;
        config_t cfg;
    };

    class mpuSeries_t : public imu_t {
    public:
        virtual ~mpuSeries_t() {}

        int Init(config_t _cfg = config_t()) override;
        bool Detect() override;
        int Converter(float *acc_x, float *acc_y, float *acc_z,
                      float *gyro_x, float *gyro_y, float *gyro_z) override;
        int Converter(float *mag_x, float *mag_y, float *mag_z) override;
        int ReadSensorBlocking() override;
        int ReadSensorNonBlocking() override;
    public:
        /**
         * @brief   软复位，可以在初始化之前执行
         * @return {int}  : 错误码
         */
        virtual int SoftReset() = 0;

        /**
         * @brief   转换！！！缓存！！!中加速度和陀螺仪的数据到指定的地方，单位为LSB
         * @param  {int16_t*} acc_x  : 可以等于NULL
         * @param  {int16_t*} acc_y  : 可以等于NULL
         * @param  {int16_t*} acc_z  : 可以等于NULL
         * @param  {int16_t*} gyro_x : 可以等于NULL
         * @param  {int16_t*} gyro_y : 可以等于NULL
         * @param  {int16_t*} gyro_z : 可以等于NULL
         * @return {int}             : 错误码
         */
        virtual int Converter(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z,
                              int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z);
        /**
         * @brief   转换！！！缓存！！!中磁力计的数据到指定的地方，单位为LSB
         * @param  {int16_t*} mag_x : 可以等于NULL
         * @param  {int16_t*} mag_y : 可以等于NULL
         * @param  {int16_t*} mag_z : 可以等于NULL
         * @return {int}            : 错误码
         */
        virtual int Converter(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z);

        /**
         * @brief   转换缓冲中的温度到其他地方，单位为摄氏度
         * @param  {float*} temp :可以等于NULL
         * @return {int}         :错误码
         */
        virtual int Converter(float *temp) = 0;

        /**
         * @brief   使能传感器的DataReady中断
         * @return {int}  : 
         */
        virtual int EnableDataReadyInt();

        /**
         * @brief   返回传感器的DataReady中断状态
         * @return {bool}  :
         */
        virtual bool DataReady();

        /**
         * @brief   返回传感器WHO_AM_I寄存器的出厂默认值
         * @return {uint8_t}  : 
         */
        virtual uint8_t WhoAmI() = 0;

        /**
         * @brief   返回传感器WHO_AM_I寄存器的寄存器地址
         * @return {uint8_t}           :
         */
        virtual uint8_t RegWhoAmI() { return (uint8_t) icm20602_RegMap::WHO_AM_I; }

    protected:
        /**
         * mpuSeries_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        mpuSeries_t(i2cInterface_t &_i2c);
        float accelUnit;
        float gyroUnit;
        uint8_t buf[14];
    };


    class mpu6050_t : public mpuSeries_t {
    public:
        virtual ~mpu6050_t() {}

        /**
         * mpu6050_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        mpu6050_t(i2cInterface_t &_i2c) : mpuSeries_t(_i2c) {}

        int SelfTest() override;
        using mpuSeries_t::Converter;
        int Converter(float *temp) override;
        std::string Report() override;
        int SoftReset(void) override;

        uint8_t WhoAmI() override { return 0x68; }

        constexpr static const uint16_t accelSelfTestEquation[32] = {
                1347, 1393, 1440, 1488, 1538, 1590, 1644, 1699,
                1757, 1816, 1877, 1941, 2006, 2074, 2144, 2216,
                2291, 2368, 2448, 2531, 2616, 2704, 2795, 2890,
                2987, 3088, 3192, 3300, 3411, 3526, 3645, 3768,
        };
        constexpr static const uint16_t gyroSelfTestEquation[32] = {
                3131, 3275, 3426, 3583, 3748, 3920, 4101, 4289,
                4487, 4693, 4909, 5135, 5371, 5618, 5877, 6147,
                6430, 6725, 7035, 7358, 7697, 8051, 8421, 8809,
                9214, 9638, 10081, 10545, 11030, 11537, 12068, 12623};
    };

    class mpu6500Series_t : public mpuSeries_t {
    protected:
        virtual ~mpu6500Series_t() {}

        /**
         * mpu6500Series_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        mpu6500Series_t(i2cInterface_t &_i2c) : mpuSeries_t(_i2c) {}

    public:
        int SelfTest() override;

        /**
         * @brief   返回加速度自检出厂数据的寄存器的寄存器地址
         * @return {uint8_t}  : 
         */
        virtual uint8_t RegSelfTestXAccel() = 0;

        /**
         * @brief   返回陀螺仪自检出厂数据的寄存器的寄存器地址
         * @return {uint8_t}  : 
         */
        virtual uint8_t RegSelfTestXGyro() = 0;

        constexpr static const int DEF_ST_PRECISION = 1000;
        constexpr static const int DEF_GYRO_CT_SHIFT_DELTA = 500;
        constexpr static const int DEF_ACCEL_ST_SHIFT_DELTA = 500;
        /* Gyro Offset Max Value (dps) */
        constexpr static const int DEF_GYRO_OFFSET_MAX = 20;
        /* Gyro Self Test Absolute Limits ST_AL (dps) */
        constexpr static const int DEF_GYRO_ST_AL = 60;
        /* Accel Self Test Absolute Limits ST_AL (mg) */
        constexpr static const int DEF_ACCEL_ST_AL_MIN = 225;
        constexpr static const int DEF_ACCEL_ST_AL_MAX = 675;

        constexpr static const uint16_t sSelfTestEquation[256] = {
                2620, 2646, 2672, 2699, 2726, 2753, 2781, 2808,
                2837, 2865, 2894, 2923, 2952, 2981, 3011, 3041,
                3072, 3102, 3133, 3165, 3196, 3228, 3261, 3293,
                3326, 3359, 3393, 3427, 3461, 3496, 3531, 3566,
                3602, 3638, 3674, 3711, 3748, 3786, 3823, 3862,
                3900, 3939, 3979, 4019, 4059, 4099, 4140, 4182,
                4224, 4266, 4308, 4352, 4395, 4439, 4483, 4528,
                4574, 4619, 4665, 4712, 4759, 4807, 4855, 4903,
                4953, 5002, 5052, 5103, 5154, 5205, 5257, 5310,
                5363, 5417, 5471, 5525, 5581, 5636, 5693, 5750,
                5807, 5865, 5924, 5983, 6043, 6104, 6165, 6226,
                6289, 6351, 6415, 6479, 6544, 6609, 6675, 6742,
                6810, 6878, 6946, 7016, 7086, 7157, 7229, 7301,
                7374, 7448, 7522, 7597, 7673, 7750, 7828, 7906,
                7985, 8065, 8145, 8227, 8309, 8392, 8476, 8561,
                8647, 8733, 8820, 8909, 8998, 9088, 9178, 9270,
                9363, 9457, 9551, 9647, 9743, 9841, 9939, 10038,
                10139, 10240, 10343, 10446, 10550, 10656, 10763, 10870,
                10979, 11089, 11200, 11312, 11425, 11539, 11654, 11771,
                11889, 12008, 12128, 12249, 12371, 12495, 12620, 12746,
                12874, 13002, 13132, 13264, 13396, 13530, 13666, 13802,
                13940, 14080, 14221, 14363, 14506, 14652, 14798, 14946,
                15096, 15247, 15399, 15553, 15709, 15866, 16024, 16184,
                16346, 16510, 16675, 16842, 17010, 17180, 17352, 17526,
                17701, 17878, 18057, 18237, 18420, 18604, 18790, 18978,
                19167, 19359, 19553, 19748, 19946, 20145, 20347, 20550,
                20756, 20963, 21173, 21385, 21598, 21814, 22033, 22253,
                22475, 22700, 22927, 23156, 23388, 23622, 23858, 24097,
                24338, 24581, 24827, 25075, 25326, 25579, 25835, 26093,
                26354, 26618, 26884, 27153, 27424, 27699, 27976, 28255,
                28538, 28823, 29112, 29403, 29697, 29994, 30294, 30597,
                30903, 31212, 31524, 31839, 32157, 32479, 32804
        };
    };

    class icm20602_t : public mpu6500Series_t {
    public:
        virtual ~icm20602_t() {}

        /**
         * icm20602_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        icm20602_t(i2cInterface_t &_i2c) : mpu6500Series_t(_i2c) {}

        int SoftReset(void) override;
        using mpuSeries_t::Converter;
        int Converter(float *temp) override;
        std::string Report() override;

        uint8_t WhoAmI() override { return 0x12; }

        uint8_t RegSelfTestXAccel() override { return (uint8_t) icm20602_RegMap::SELF_TEST_X_ACCEL; }

        uint8_t RegSelfTestXGyro() override { return (uint8_t) icm20602_RegMap::SELF_TEST_X_GYRO; }
    };

    class mpu9250_t : public mpu6500Series_t {
    public:
        virtual ~mpu9250_t() {}

        /**
         * mpu9250_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        mpu9250_t(i2cInterface_t &_i2c);
        int Init(config_t _cfg = config_t()) override;
        int Converter(float *acc_x, float *acc_y, float *acc_z,
                      float *gyro_x, float *gyro_y, float *gyro_z) override;
        int Converter(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z,
                      int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) override;
        int Converter(float *mag_x, float *mag_y, float *mag_z) override;
        int Converter(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) override;
        int Converter(float *temp) override;
        int ReadSensorBlocking() override;
        int ReadSensorNonBlocking() override;
        std::string Report() override;
        int SoftReset(void) override;

        uint8_t WhoAmI() override { return 0x71; }

        uint8_t RegSelfTestXAccel() override { return (uint8_t) mpu9250_RegMap::SELF_TEST_X_ACCEL; }

        uint8_t RegSelfTestXGyro() override { return (uint8_t) mpu9250_RegMap::SELF_TEST_X_GYRO; }

    public:
        /**
         * @brief   使用mpu9250的片上iic主机控制器读写挂载在mpu9250 iic bus上的iic从机
         * @param  {uint8_t} addr :  iic从机地址
         * @param  {uint8_t} reg  :  从机寄存器地址
         * @param  {uint8_t*} val :  缓存地址
         * @param  {unsigned} int :  数据长度
         * @return {int}          :  错误码
         */
        int SubI2cRead(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len = 1);

        /**
         * @brief   使用mpu9250的片上iic主机控制器读写挂载在mpu9250 iic bus上的iic从机
         * @param  {uint8_t} addr       : iic从机地址
         * @param  {uint8_t} reg        : 从机寄存器地址
         * @param  {const uint8_t*} val : 缓存地址
         * @param  {unsigned} int       : 数据长度
         * @return {int}                : 错误码
         */
        int SubI2cWrite(uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len = 1);
    public:
        constexpr static const int MPU9250_I2C_SLV4_EN = 0x80;
        constexpr static const int MPU9250_I2C_SLV4_DONE = 0x40;
        constexpr static const int MPU9250_I2C_SLV4_NACK = 0x10;
        constexpr static const int MPU9250_AK8963_I2C_ADDR = 0x0C;
        constexpr static const int MPU9250_AK8963_POWER_DOWN = 0x10;
        constexpr static const int MPU9250_AK8963_FUSE_ROM_ACCESS = 0x1F;
        constexpr static const int MPU9250_AK8963_SINGLE_MEASUREMENT = 0x11;
        constexpr static const int MPU9250_AK8963_CONTINUOUS_MEASUREMENT = 0x16; //MODE 2
        constexpr static const int MPU9250_AK8963_DATA_READY = (0x01);
        constexpr static const int MPU9250_AK8963_DATA_OVERRUN = (0x02);
        //constexpr static const int MPU9250_AK8963_OVERFLOW = (0x80);
        constexpr static const int MPU9250_AK8963_OVERFLOW = (0x08);
        constexpr static const int MPU9250_AK8963_DATA_ERROR = (0x40);
        constexpr static const int MPU9250_AK8963_CNTL2_SRST = 0x01;

    private:
        uint8_t buf[22];
        uint8_t ak8963DeviceId;
        uint8_t ak8963Information;
        constexpr static float magUnit = 0.15f;;//固定量程4900uT 0.15µT/LSB
        float ak8963Asa[3];
    };

    class imuPtr_t : public std::shared_ptr<imu_t> {
    public:
        /**
         * @brief   在堆上创建并接管mpu6050,mpu9250,icm20602对象
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         * @return {int}                   : 错误码
         */
        int Load(i2cInterface_t &_i2c);
    };
}
#endif //UTILITIES_DRV_IMU_INVENSENSE_HPP
