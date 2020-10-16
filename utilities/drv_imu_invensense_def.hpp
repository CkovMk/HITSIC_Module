/**
 * @brief 陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 * @author xiao qq1761690868
 * @doc drv_imu_invensense.md
 * @version v1.0
 * @date 2020-10-16
 */

#ifndef REMOTEIIC_DRV_IMU_INVENSENSE_DEF_HPP
#define REMOTEIIC_DRV_IMU_INVENSENSE_DEF_HPP
#include <cstdint>
namespace inv {
	enum class icm20602_RegMap :uint8_t
	{
		XG_OFFS_TC_H = 0x4,			// READ/ WRITE 
		XG_OFFS_TC_L = 0x5,			// READ/ WRITE 
		YG_OFFS_TC_H = 0x7,			// READ/ WRITE 
		YG_OFFS_TC_L = 0x8,			// READ/ WRITE 
		ZG_OFFS_TC_H = 0x0A,		// READ/ WRITE 
		ZG_OFFS_TC_L = 0x0B,		// READ/ WRITE 
		SELF_TEST_X_ACCEL = 0x0D,	// READ/ WRITE 
		SELF_TEST_Y_ACCEL = 0x0E,	// READ/ WRITE 
		SELF_TEST_Z_ACCEL = 0x0F,	// READ/ WRITE 
		XG_OFFS_USRH = 0x13,		// READ/ WRITE 
		XG_OFFS_USRL = 0x14,		// READ/ WRITE 
		YG_OFFS_USRH = 0x15,		// READ/ WRITE 
		YG_OFFS_USRL = 0x16,		// READ/ WRITE 
		ZG_OFFS_USRH = 0x17,		// READ/ WRITE 
		ZG_OFFS_USRL = 0x18,		// READ/ WRITE 
		SMPLRT_DIV = 0x19,			// READ/ WRITE 
		CONFIG = 0x1A,				// READ/ WRITE default value:0x80
		GYRO_CONFIG = 0x1B,			// READ/ WRITE 
		ACCEL_CONFIG = 0x1C,		// READ/ WRITE 
		ACCEL_CONFIG2 = 0x1D,		// READ/ WRITE 
		LP_MODE_CFG = 0x1E,			// READ/ WRITE 
		ACCEL_WOM_X_THR = 0x20,		// READ/ WRITE 
		ACCEL_WOM_Y_THR = 0x21,		// READ/ WRITE 
		ACCEL_WOM_Z_THR = 0x22,		// READ/ WRITE 
		FIFO_EN = 0x23,				// READ/ WRITE 
		FSYNC_INT = 0x36,			// READ to CLEAR
		INT_PIN_CFG = 0x37,			// READ/ WRITE 
		INT_ENABLE = 0x38,			// READ/ WRITE 
		FIFO_WM_INT_STATUS = 0x39,	// READ to CLEAR
		INT_STATUS = 0x3A,			// READ to CLEAR
		ACCEL_XOUT_H = 0x3B,		// READ 
		ACCEL_XOUT_L = 0x3C,		// READ 
		ACCEL_YOUT_H = 0x3D,		// READ 
		ACCEL_YOUT_L = 0x3E,		// READ 
		ACCEL_ZOUT_H = 0x3F,		// READ 
		ACCEL_ZOUT_L = 0x40,		// READ 
		TEMP_OUT_H = 0x41,			// READ 
		TEMP_OUT_L = 0x42,			// READ 
		GYRO_XOUT_H = 0x43,			// READ 
		GYRO_XOUT_L = 0x44,			// READ 
		GYRO_YOUT_H = 0x45,			// READ 
		GYRO_YOUT_L = 0x46,			// READ 
		GYRO_ZOUT_H = 0x47,			// READ 
		GYRO_ZOUT_L = 0x48,			// READ 
		SELF_TEST_X_GYRO = 0x50,	// READ/ WRITE 
		SELF_TEST_Y_GYRO = 0x51,	// READ/ WRITE 
		SELF_TEST_Z_GYRO = 0x52,	// READ/ WRITE 
		FIFO_WM_TH1 = 0x60,			// READ/ WRITE 
		FIFO_WM_TH2 = 0x61,			// READ/ WRITE 
		SIGNAL_PATH_RESET = 0x68,	// READ/ WRITE 
		ACCEL_INTEL_CTRL = 0x69,	// READ/ WRITE 
		USER_CTRL = 0x6A,			// READ/ WRITE 
		PWR_MGMT_1 = 0x6B,			// READ/ WRITE default value:0x41
		PWR_MGMT_2 = 0x6C,			// READ/ WRITE 
		I2C_IF = 0x70,				// READ/ WRITE 
		FIFO_COUNTH = 0x72,			// READ 
		FIFO_COUNTL = 0x73,			// READ 
		FIFO_R_W = 0x74,			// READ/ WRITE 
		WHO_AM_I = 0x75,			// READ default value:0x12
		XA_OFFSET_H = 0x77,			// READ/ WRITE 
		XA_OFFSET_L = 0x78,			// READ/ WRITE 
		YA_OFFSET_H = 0x7A,			// READ/ WRITE 
		YA_OFFSET_L = 0x7B,			// READ/ WRITE 
		ZA_OFFSET_H = 0x7D,			// READ/ WRITE 
		ZA_OFFSET_L = 0x7E,			// READ/ WRITE 
	};


	enum class mpu6050_RegMap :uint8_t
	{
		SELF_TEST_X = 0x0D,			 //R/W 
		SELF_TEST_Y = 0x0E,			 //R/W 
		SELF_TEST_Z = 0x0F,			 //R/W 
		SELF_TEST_A = 0x10,			 //R/W 
		SMPLRT_DIV = 0x19,			 //R/W 
		CONFIG = 0x1A,				 //R/W 
		GYRO_CONFIG = 0x1B,			 //R/W 
		ACCEL_CONFIG = 0x1C,		 //R/W 
		FIFO_EN = 0x23,				 //R/W 
		I2C_MST_CTRL = 0x24,		 //R/W 
		I2C_SLV0_ADDR = 0x25,		 //R/W 
		I2C_SLV0_REG = 0x26,		 //R/W 
		I2C_SLV0_CTRL = 0x27,		 //R/W 
		I2C_SLV1_ADDR = 0x28,		 //R/W 
		I2C_SLV1_REG = 0x29,		 //R/W 
		I2C_SLV1_CTRL = 0x2A,		 //R/W 
		I2C_SLV2_ADDR = 0x2B,		 //R/W 
		I2C_SLV2_REG = 0x2C,		 //R/W 
		I2C_SLV2_CTRL = 0x2D,		 //R/W 
		I2C_SLV3_ADDR = 0x2E,		 //R/W 
		I2C_SLV3_REG = 0x2F,		 //R/W 
		I2C_SLV3_CTRL = 0x30,		 //R/W 
		I2C_SLV4_ADDR = 0x31,		 //R/W 
		I2C_SLV4_REG = 0x32,		 //R/W 
		I2C_SLV4_DO = 0x33,			 //R/W 
		I2C_SLV4_CTRL = 0x34,		 //R/W 
		I2C_SLV4_DI = 0x35,			 //R 
		I2C_MST_STATUS = 0x36,		 //R 
		INT_PIN_CFG = 0x37,			 //R/W 
		INT_ENABLE = 0x38,			 //R/W 
		INT_STATUS = 0x3A,			 //R 
		ACCEL_XOUT_H = 0x3B,		 //R 
		ACCEL_XOUT_L = 0x3C,		 //R 
		ACCEL_YOUT_H = 0x3D,		 //R 
		ACCEL_YOUT_L = 0x3E,		 //R 
		ACCEL_ZOUT_H = 0x3F,		 //R 
		ACCEL_ZOUT_L = 0x40,		 //R 
		TEMP_OUT_H = 0x41,			 //R 
		TEMP_OUT_L = 0x42,			 //R 
		GYRO_XOUT_H = 0x43,			 //R 
		GYRO_XOUT_L = 0x44,			 //R 
		GYRO_YOUT_H = 0x45,			 //R 
		GYRO_YOUT_L = 0x46,			 //R 
		GYRO_ZOUT_H = 0x47,			 //R 
		GYRO_ZOUT_L = 0x48,			 //R 
		EXT_SENS_DATA_00 = 0x49,	 //R 
		EXT_SENS_DATA_01 = 0x4A,	 //R 
		EXT_SENS_DATA_02 = 0x4B,	 //R 
		EXT_SENS_DATA_03 = 0x4C,	 //R 
		EXT_SENS_DATA_04 = 0x4D,	 //R 
		EXT_SENS_DATA_05 = 0x4E,	 //R 
		EXT_SENS_DATA_06 = 0x4F,	 //R 
		EXT_SENS_DATA_07 = 0x50,	 //R 
		EXT_SENS_DATA_08 = 0x51,	 //R 
		EXT_SENS_DATA_09 = 0x52,	 //R 
		EXT_SENS_DATA_10 = 0x53,	 //R 
		EXT_SENS_DATA_11 = 0x54,	 //R 
		EXT_SENS_DATA_12 = 0x55,	 //R 
		EXT_SENS_DATA_13 = 0x56,	 //R 
		EXT_SENS_DATA_14 = 0x57,	 //R 
		EXT_SENS_DATA_15 = 0x58,	 //R 
		EXT_SENS_DATA_16 = 0x59,	 //R 
		EXT_SENS_DATA_17 = 0x5A,	 //R 
		EXT_SENS_DATA_18 = 0x5B,	 //R 
		EXT_SENS_DATA_19 = 0x5C,	 //R 
		EXT_SENS_DATA_20 = 0x5D,	 //R 
		EXT_SENS_DATA_21 = 0x5E,	 //R 
		EXT_SENS_DATA_22 = 0x5F,	 //R 
		EXT_SENS_DATA_23 = 0x60,	 //R 
		I2C_SLV0_DO = 0x63,			 //R/W 
		I2C_SLV1_DO = 0x64,			 //R/W 
		I2C_SLV2_DO = 0x65,			 //R/W 
		I2C_SLV3_DO = 0x66,			 //R/W 
		I2C_MST_DELAY_CTRL = 0x67,	 //R/W 
		SIGNAL_PATH_RESET = 0x68,	 //R/W 
		USER_CTRL = 0x6A,			 //R/W 
		PWR_MGMT_1 = 0x6B,			 //R/W 
		PWR_MGMT_2 = 0x6C,			 //R/W 
		FIFO_COUNTH = 0x72,			 //R/W 
		FIFO_COUNTL = 0x73,			 //R/W 
		FIFO_R_W = 0x74,			 //R/W 
		WHO_AM_I = 0x75,			 //R 
	};

	enum class mpu9250_RegMap :uint8_t
	{
		SELF_TEST_X_GYRO = 0x0,//R/W 
		SELF_TEST_Y_GYRO = 0x1,//R/W 
		SELF_TEST_Z_GYRO = 0x2,//R/W 
		SELF_TEST_X_ACCEL = 0x0D,//R/W 
		SELF_TEST_Y_ACCEL = 0x0E,//R/W 
		SELF_TEST_Z_ACCEL = 0x0F,//R/W 
		XG_OFFSET_H = 0x13,//R/W 
		XG_OFFSET_L = 0x14,//R/W 
		YG_OFFSET_H = 0x15,//R/W 
		YG_OFFSET_L = 0x16,//R/W 
		ZG_OFFSET_H = 0x17,//R/W 
		ZG_OFFSET_L = 0x18,//R/W 
		SMPLRT_DIV = 0x19,//R/W 
		CONFIG = 0x1A,//R/W 
		GYRO_CONFIG = 0x1B,//R/W 
		ACCEL_CONFIG = 0x1C,//R/W 
		ACCEL_CONFIG2 = 0x1D,//R/W 
		LP_ACCEL_ODR = 0x1E,//R/W 
		WOM_THR = 0x1F,//R/W 
		FIFO_EN = 0x23,//R/W 
		I2C_MST_CTRL = 0x24,//R/W 
		I2C_SLV0_ADDR = 0x25,//R/W 
		I2C_SLV0_REG = 0x26,//R/W 
		I2C_SLV0_CTRL = 0x27,//R/W 
		I2C_SLV1_ADDR = 0x28,//R/W 
		I2C_SLV1_REG = 0x29,//R/W 
		I2C_SLV1_CTRL = 0x2A,//R/W 
		I2C_SLV2_ADDR = 0x2B,//R/W 
		I2C_SLV2_REG = 0x2C,//R/W 
		I2C_SLV2_CTRL = 0x2D,//R/W 
		I2C_SLV3_ADDR = 0x2E,//R/W 
		I2C_SLV3_REG = 0x2F,//R/W 
		I2C_SLV3_CTRL = 0x30,//R/W 
		I2C_SLV4_ADDR = 0x31,//R/W 
		I2C_SLV4_REG = 0x32,//R/W 
		I2C_SLV4_DO = 0x33,//R/W 
		I2C_SLV4_CTRL = 0x34,//R/W 
		I2C_SLV4_DI = 0x35,//R 
		I2C_MST_STATUS = 0x36,//R 
		INT_PIN_CFG = 0x37,//R/W 
		INT_ENABLE = 0x38,//R/W 
		INT_STATUS = 0x3A,//R 
		ACCEL_XOUT_H = 0x3B,//R 
		ACCEL_XOUT_L = 0x3C,//R 
		ACCEL_YOUT_H = 0x3D,//R 
		ACCEL_YOUT_L = 0x3E,//R 
		ACCEL_ZOUT_H = 0x3F,//R 
		ACCEL_ZOUT_L = 0x40,//R 
		TEMP_OUT_H = 0x41,//R 
		TEMP_OUT_L = 0x42,//R 
		GYRO_XOUT_H = 0x43,//R 
		GYRO_XOUT_L = 0x44,//R 
		GYRO_YOUT_H = 0x45,//R 
		GYRO_YOUT_L = 0x46,//R 
		GYRO_ZOUT_H = 0x47,//R 
		GYRO_ZOUT_L = 0x48,//R 
		EXT_SENS_DATA_00 = 0x49,//R 
		EXT_SENS_DATA_01 = 0x4A,//R 
		EXT_SENS_DATA_02 = 0x4B,//R 
		EXT_SENS_DATA_03 = 0x4C,//R 
		EXT_SENS_DATA_04 = 0x4D,//R 
		EXT_SENS_DATA_05 = 0x4E,//R 
		EXT_SENS_DATA_06 = 0x4F,//R 
		EXT_SENS_DATA_07 = 0x50,//R 
		EXT_SENS_DATA_08 = 0x51,//R 
		EXT_SENS_DATA_09 = 0x52,//R 
		EXT_SENS_DATA_10 = 0x53,//R 
		EXT_SENS_DATA_11 = 0x54,//R 
		EXT_SENS_DATA_12 = 0x55,//R 
		EXT_SENS_DATA_13 = 0x56,//R 
		EXT_SENS_DATA_14 = 0x57,//R 
		EXT_SENS_DATA_15 = 0x58,//R 
		EXT_SENS_DATA_16 = 0x59,//R 
		EXT_SENS_DATA_17 = 0x5A,//R 
		EXT_SENS_DATA_18 = 0x5B,//R 
		EXT_SENS_DATA_19 = 0x5C,//R 
		EXT_SENS_DATA_20 = 0x5D,//R 
		EXT_SENS_DATA_21 = 0x5E,//R 
		EXT_SENS_DATA_22 = 0x5F,//R 
		EXT_SENS_DATA_23 = 0x60,//R 
		I2C_SLV0_DO = 0x63,//R/W 
		I2C_SLV1_DO = 0x64,//R/W 
		I2C_SLV2_DO = 0x65,//R/W 
		I2C_SLV3_DO = 0x66,//R/W 
		I2C_MST_DELAY_CTRL = 0x67,//R/W 
		SIGNAL_PATH_RESET = 0x68,//R/W 
		MOT_DETECT_CTRL = 0x69,//R/W 
		USER_CTRL = 0x6A,//R/W 
		PWR_MGMT_1 = 0x6B,//R/W 
		PWR_MGMT_2 = 0x6C,//R/W 
		FIFO_COUNTH = 0x72,//R/W 
		FIFO_COUNTL = 0x73,//R/W 
		FIFO_R_W = 0x74,//R/W 
		WHO_AM_I = 0x75,//R 
		XA_OFFSET_H = 0x77,//R/W 
		XA_OFFSET_L = 0x78,//R/W 
		YA_OFFSET_H = 0x7A,//R/W 
		YA_OFFSET_L = 0x7B,//R/W 
		ZA_OFFSET_H = 0x7D,//R/W 
		ZA_OFFSET_L = 0x7E,//R/W 
	};

    enum class ak8963_RegMap : uint8_t {
        //Magnetometer register maps
        WIA = 0x00,
        INFO = 0x01,
        ST1 = 0x02,
        XOUT_L = 0x03,
        XOUT_H = 0x04,
        YOUT_L = 0x05,
        YOUT_H = 0x06,
        ZOUT_L = 0x07,
        ZOUT_H = 0x08,
        ST2 = 0x09,
        CNTL = 0x0A,
        CNTL2 = 0x0B,
        RSV = 0x0B, //DO NOT ACCESS <MPU9250_AK8963_CNTL2>
        ASTC = 0x0C,
        TS1 = 0x0D, //DO NOT ACCESS
        TS2 = 0x0E, //DO NOT ACCESS
        I2CDIS = 0x0F,
        ASAX = 0x10,
        ASAY = 0x11,
        ASAZ = 0x12,
    };



}
#endif //REMOTEIIC_DRV_IMU_INVENSENSE_DEF_HPP