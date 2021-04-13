#include"drv_imu_invensense.h"

#if (defined(INV_USE_HITSIC_SYSLOG) && (INV_USE_HITSIC_SYSLOG > 0))

#define SYSLOG_LVL (INVIMU_LOG_LVL)
#define SYSLOG_TAG "INVIMU"
#include<inc_syslog.h>

#else // INV_USE_HITSIC_SYSLOG

#define SYSLOG_A(...) INV_PRINTF("\r\n");
#define SYSLOG_E(...) INV_PRINTF("\r\n");
#define SYSLOG_W(...) INV_PRINTF("\r\n");
#define SYSLOG_I(...) INV_PRINTF("\r\n");
#define SYSLOG_D(...) INV_PRINTF("\r\n");
#define SYSLOG_V(...) INV_PRINTF("\r\n");

#endif // ! INV_USE_HITSIC_SYSLOG
int INV_IMU_Example(int argn, char **argv)
{
    inv_i2c_t i2c0 =
    {
        .masterTransferBlocking = IMU_INV_I2C_TransferBlocking,
        .masterTransferNonBlocking = NULL,
    };
    //i2c0 = ....
    //or
    //i2c0 spi0;
    //spi0 = .....
    inv_imu_handle_t imu0 = IMU_AutoConstructI2C(i2c0, IMU_SlaveAddressAutoDetect);
    //or inv_imu_handle imu0 = IMU_AutoConstructSPI(spi0);
    if (imu0 != NULL)
    {
        SYSLOG_I("当前使用的传感器为 %s", IMU_Report(imu0));
        if (0 == IMU_Init(imu0, IMU_ConfigDefault()))
        {
            SYSLOG_I("ST = %d,0为自检通过,自检时保持静止", IMU_SelfTest(imu0));
            // Delay(10)//delay 10ms
            int loop = 10;
            while (loop--)
            {
                IMU_ReadSensorBlocking(imu0);
                float temp;
                float data[9];
                float *buf = data;
                IMU_Convert(imu0, data);
                IMU_ConvertTemp(imu0, &temp);
                SYSLOG_I("temp = %f", temp);
                SYSLOG_I("accel(xyz) = %.3f %.3f %.3f\t gyro(xyz) = %.3f %.3f %.3f\t mag(xyz) = %.3f %.3f %.3f \r\n",
                       *buf++, *buf++, *buf++, *buf++, *buf++, *buf++, *buf++, *buf++, *buf++);
            }
        }
        else
        {
            SYSLOG_I("初始化失败");
        }
    }
    else
    {
        SYSLOG_I("imu0 == NULL，没接或者iic/spi读写出错");
    }
    if (imu0 != NULL)
    {
        SYSLOG_I("释放资源");
        IMU_Destruct(imu0);
    }
    return 0;
}

#include"drv_imu_inv_mpu6050.h"
int INV_MPU6050_Example(int argn, char **argv)
{
    inv_i2c_t i2c0;
    //i2c0 = ....
    //or
    //i2c0 spi0;
    //spi0 = .....
    inv_mpu6050_handle_t imu0 = MPU6050_ConstructI2C(i2c0, IMU_SlaveAddressAutoDetect);
    //or inv_mpu9250_handle imu0 = MPU9250_ConstructSPI(spi0);
    if (imu0 != NULL)
    {
        SYSLOG_I("当前使用的传感器为 %s", MPU6050_Report(imu0));
        if (0 == MPU6050_Init(imu0, IMU_ConfigDefault()))
        {
            SYSLOG_I("ST = %d,0为自检通过,自检时保持静止", MPU6050_SelfTest(imu0));
            // Delay(10)//delay 10ms
            int loop = 10;
            while (loop--) {
                MPU6050_ReadSensorBlocking(imu0);
                float temp;
                float data[9];
                float *buf = data;
                MPU6050_Convert(imu0, data);
                MPU6050_ConvertTemp(imu0, &temp);
                SYSLOG_I("temp = %f", temp);
                SYSLOG_I("accel(xyz) = %.3f %.3f %.3f\t gyro(xyz) = %.3f %.3f %.3f\t mag(xyz) = %.3f %.3f %.3f",
                       *buf++, *buf++, *buf++, *buf++, *buf++, *buf++, *buf++, *buf++, *buf++);
            }
        }
        else
        {
            SYSLOG_I("初始化失败");
        }
    }
    else
    {
        SYSLOG_I("imu0 == NULL，没接或者iic/spi读写出错");
    }
    if (imu0 != NULL)
    {
        SYSLOG_I("释放资源");
        MPU6050_Destruct(imu0);
    }
    return 0;
}

#undef SYSLOG_LVL
#undef SYSLOG_TAG
#undef SYSLOG_A
#undef SYSLOG_E
#undef SYSLOG_W
#undef SYSLOG_I
#undef SYSLOG_D
#undef SYSLOG_V
