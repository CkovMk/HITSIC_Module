#ifndef UTILITIES_TEST_DRV_IMU_INVENSENSE_TEST_HPP
#define UTILITIES_TEST_DRV_IMU_INVENSENSE_TEST_HPP

#include "drv_imu_invensense.hpp"


//remote_i2c iic("/dev/i2c-1");
inv::i2cInterface_t my_i2c(nullptr, IMU_INV_I2cRxBlocking, IMU_INV_I2cTxBlocking);

float acc[3] = {0, 0, 0};
float gyro[3] = {0, 0, 0};
float mag[3] = {0, 0, 0};
float temp = 0;



int example1(void) {
    inv::imuPtr_t my_imu;
    if (0 == my_imu.Load(my_i2c)) {
        if (my_imu->Init() == 0) {
            //自检时保持静止，否则会直接失败
            if (my_imu->SelfTest() == 0) {
                SDK_DelayAtLeastUs(10*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));//等待10ms
                my_imu->ReadSensorBlocking();
                my_imu->Convert(acc, acc + 1, acc + 2, gyro, gyro + 1, gyro + 2);
//                static_cast<inv::mpuSeries_t*>(my_imu.get())->Convert(&temp);
                my_imu->Convert(mag, mag + 1, mag + 2);
                PRINTF("%s\r\n", my_imu->Report().c_str());
                PRINTF("accel \t%.3f \t%.3f \t%.3f m/s^2\r\n", acc[0], acc[1], acc[2]);
                PRINTF("gyro \t%.3f \t%.3f \t%.3f dps \r\n", gyro[0], gyro[1], gyro[2]);
                PRINTF("mag \t%.1f \t%.1f \t%.1f uT \r\n", mag[0], mag[1], mag[2]);
//                PRINTF("temp \t%.3f C \r\n", temp);
            } else {
                PRINTF("自检未通过\r\n");
            }
        } else {
            PRINTF("初始化未通过\r\n");
        }
    } else {
        PRINTF("没有imu\r\n");
    }
    return 0;
}

int example2(void) {
    //inv::mpu6050_t my_imu(my_i2c);
    inv::icm20602_t my_imu(my_i2c);
    if (true == my_imu.Detect()) {
        if (my_imu.Init() == 0) {
            //自检时保持静止，否则会直接失败
            if (my_imu.SelfTest() == 0) {
                SDK_DelayAtLeastUs(10*1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));//等待10ms
                my_imu.ReadSensorBlocking();
                my_imu.Convert(acc, acc + 1, acc + 2, gyro, gyro + 1, gyro + 2);
                my_imu.Convert(&temp);
                my_imu.Convert(mag, mag + 1, mag + 2);
                PRINTF("%s\r\n", my_imu.Report().c_str());
                PRINTF("accel \t%.3f \t%.3f \t%.3f m/s^2\r\n", acc[0], acc[1], acc[2]);
                PRINTF("gyro \t%.3f \t%.3f \t%.3f dps \r\n", gyro[0], gyro[1], gyro[2]);
                PRINTF("mag \t%.1f \t%.1f \t%.1f uT \r\n", mag[0], mag[1], mag[2]);
                PRINTF("temp \t%.3f C \r\n", temp);
            } else {
                PRINTF("自检未通过\r\n");
            }
        } else {
            PRINTF("初始化未通过\r\n");
        }
    } else {
        PRINTF("没有mpu6050\r\n");
    }
    return 0;
}




#endif // !UTILITIES_TEST_DRV_IMU_INVENSENSE_TEST_HPP
