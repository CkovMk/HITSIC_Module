#ifndef UTILITIES_TEST_DRV_IMU_INVENSENSE_TEST_HPP
#define UTILITIES_TEST_DRV_IMU_INVENSENSE_TEST_HPP

#include "drv_imu_invensense.hpp"

namespace inv
{
//remote_i2c iic("/dev/i2c-1");
inv::i2cInterface_t my_i2c(nullptr, IMU_INV_I2cRxBlocking, IMU_INV_I2cTxBlocking);

/**
 * @brief A unit test on IMU. Using auto detection mode.
 * @param void
 * @return 0
 */
inline int32_t IMU_UnitTest_AutoDetect(void)
{
    float acc[3] =
    { 0, 0, 0 };
    float gyro[3] =
    { 0, 0, 0 };
    float mag[3] =
    { 0, 0, 0 };
    float temp = 0;
    inv::imuPtr_t my_imu;

    PRINTF("IMU UnitTest Start\r\n");
    PRINTF("Mode: Auto Detect\r\n");

    if (0 != my_imu.Load(my_i2c))
    {
        PRINTF("IMU Detection Failure\r\n");
        return 0;
    }

    if (0 != my_imu->Init())
    {
        PRINTF("IMU Initialization Failure\r\n");
        return 0;
    }
    //自检时保持静止，否则会直接失败
    if (0 != my_imu->SelfTest())
    {
        PRINTF("IMU Self Test Failure\r\n");
        return 0;
    }

    //等待10ms
    IMU_DelayMs(10 * 1000);

    my_imu->ReadSensorBlocking();
    my_imu->Convert(acc, acc + 1, acc + 2, gyro, gyro + 1, gyro + 2);
//     static_cast<inv::mpuSeries_t*>(my_imu.get())->Convert(&temp);
    my_imu->Convert(mag, mag + 1, mag + 2);
    PRINTF("%s\r\n", my_imu->Report().c_str());
    PRINTF("accel \t%.3f \t%.3f \t%.3f m/s^2\r\n", acc[0], acc[1], acc[2]);
    PRINTF("gyro \t%.3f \t%.3f \t%.3f dps \r\n", gyro[0], gyro[1], gyro[2]);
    PRINTF("mag \t%.1f \t%.1f \t%.1f uT \r\n", mag[0], mag[1], mag[2]);
//    PRINTF("temp \t%.3f C \r\n", temp);

    return 0;
}

/**
 * @brief A unit test on IMU. For MPU6050 Only.
 * @param void
 * @return 0
 */
inline int32_t IMU_UnitTest_MPU6050(void)
{
    float acc[3] =
    { 0, 0, 0 };
    float gyro[3] =
    { 0, 0, 0 };
    float mag[3] =
    { 0, 0, 0 };
    float temp = 0;
    inv::mpu6050_t my_imu(my_i2c);

    PRINTF("IMU UnitTest Start\r\n");
    PRINTF("Mode: MPU6050 Only\r\n");

    if (true != my_imu.Detect())
    {
        PRINTF("IMU Detection Failure\r\n");
        return 0;
    }

    if (0 != my_imu.Init())
    {
        PRINTF("IMU Initialization Failure\r\n");
        return 0;
    }

    //自检时保持静止，否则会直接失败
    if (0 != my_imu.SelfTest())
    {
        PRINTF("IMU Self Test Failure\r\n");
        return 0;
    }

    //等待10ms
    IMU_DelayMs(10 * 1000);

    my_imu.ReadSensorBlocking();
    my_imu.Convert(acc, acc + 1, acc + 2, gyro, gyro + 1, gyro + 2);
    my_imu.Convert(&temp);
    my_imu.Convert(mag, mag + 1, mag + 2);
    PRINTF("%s\r\n", my_imu.Report().c_str());
    PRINTF("accel \t%.3f \t%.3f \t%.3f m/s^2\r\n", acc[0], acc[1], acc[2]);
    PRINTF("gyro \t%.3f \t%.3f \t%.3f dps \r\n", gyro[0], gyro[1], gyro[2]);
    PRINTF("mag \t%.1f \t%.1f \t%.1f uT \r\n", mag[0], mag[1], mag[2]);
    PRINTF("temp \t%.3f C \r\n", temp);

    return 0;
}

inv::imuPtr_t imu_unitTest_imu;

float imu_unitTest_acc[3] =
{ 0, 0, 0 };
float imu_unitTest_gyro[3] =
{ 0, 0, 0 };
float imu_unitTest_mag[3] =
{ 0, 0, 0 };
float imu_unitTest_temp = 0;

inline void IMU_UnitTest_AutoRefreshPitTask(void *userData)
{
    imu_unitTest_imu->ReadSensorBlocking();
    imu_unitTest_imu->Convert(imu_unitTest_acc, imu_unitTest_acc + 1, imu_unitTest_acc + 2, imu_unitTest_gyro, imu_unitTest_gyro + 1, imu_unitTest_gyro + 2);
    //imu_unitTest_imu->Convert(&imu_unitTest_temp);
    imu_unitTest_imu->Convert(imu_unitTest_mag, imu_unitTest_mag + 1, imu_unitTest_mag + 2);
}

#if (defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0))
inline void IMU_UnitTest_AutoRefreshAddMenu(const menu_list_t *menu)
{
    static menu_list_t *imuinvList = MENU_ListConstruct("IMU(INV)", 11, menu);
    assert(imuinvList);
    MENU_ListInsert(menu, MENU_ItemConstruct(menuType, imuinvList, "IMU(INV)", 0, 0));
    {
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_acc[0], "accl.x", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_acc[1], "accl.y", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_acc[2], "accl.z", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_gyro[0], "gyro.x", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_gyro[1], "gryo.y", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_gyro[2], "gyro.z", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_mag[0], "mag .x", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_mag[1], "mag .y", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_mag[2], "mag .z", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
        MENU_ListInsert(imuinvList,
                MENU_ItemConstruct(varfType, &imu_unitTest_temp, "temp  ", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));

    }
}
#endif // ! HITSIC_USE_APP_MENU

inline void IMU_UnitTest_AutoRefresh(void)
{
    uint32_t result = 0U;
    result |= imu_unitTest_imu.Load(my_i2c);
    if (result != 0U)
    {
        PRINTF("未检测到IMU\n");
        return;
    }
    result |= imu_unitTest_imu->Init();
    if (result != 0U)
    {
        PRINTF("初始化失败\n");
        return;
    }
    result |= imu_unitTest_imu->SelfTest();
    if (result != 0U)
    {
        PRINTF("自检失败，IMU精度可能降低\n");
        return;
    }
    pitMgr_t::insert(5U, 1U, inv::IMU_UnitTest_AutoRefreshPitTask, pitMgr_t::enable);
}

}

#endif // !UTILITIES_TEST_DRV_IMU_INVENSENSE_TEST_HPP
