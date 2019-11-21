/**
 * Copyright 2018 - 2019 HITSIC
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
 *
 * @file       		drv_imu_invensense.h
 * @brief     		icm20602、mpu9250以及mpu6050驱动
 * @company	        哈工大智能车创新俱乐部
 * @author     		肖日涛 qq1761690868
 * @version    		v1.2
 * @Software 		IAR 7.7+ MCUXpressoIDE
 * @Target core		K66
 * @date       		ver1.0 2019年6月5日 已测试 兼容9250,6050,spi及iic
 * @date       		ver1.1 2019年6月7日 兼容icm的iic驱动
 * @date       		ver1.2 2019年9月24日 添加Icm20602Message.h"
 *
 * @note：icm20602、mpu9250以及mpu6050驱动，使用spi或者i2c或者i2c_soft都能用
 * @note：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
 * @SampleUsage	见void DRVIMU_INV_Demo();
 */

#pragma once
#ifndef DRV_IMU_INV_H
#define DRV_IMU_INV_H
#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"
#include "rte_i2c.h"
#include "rte_softi2c.h"
#include "drv_imu_port.h"

#if defined(HITSIC_USE_DRV_IMU) && (HITSIC_USE_DRV_IMU > 0)


/*
* ________________________________________________________________________________________________________
* Copyright ?2014-2015 InvenSense Inc. Portions Copyright ?2014-2015 Movea. All rights reserved.
* This software, related documentation and any modifications thereto (collectively 揝oftware? is subject
* to InvenSense and its licensors' intellectual property rights under U.S. and international copyright and
* other intellectual property rights laws.
* InvenSense and its licensors retain all intellectual property and proprietary rights in and to the Software
* and any use, reproduction, disclosure or distribution of the Software without an express license
* agreement from InvenSense is strictly prohibited.
* ________________________________________________________________________________________________________
*/


#define		IMU_MPU9250_I2C_ADDR		0x68 //规则类似
#define		IMU_MPU6050_I2C_ADDR		0x68 //规则类似
#define     ICM20602_DEV_ADDR           0x69 //SA0接地：0x68   SA0上拉：0x69  模块默认上拉

#define     ICM20602_SPI_W              0x00
#define     ICM20602_SPI_R              0x80

#define     ICM20602_XG_OFFS_TC_H       0x04
#define     ICM20602_XG_OFFS_TC_L       0x05
#define     ICM20602_YG_OFFS_TC_H       0x07
#define     ICM20602_YG_OFFS_TC_L       0x08
#define     ICM20602_ZG_OFFS_TC_H       0x0A
#define     ICM20602_ZG_OFFS_TC_L       0x0B
#define     ICM20602_SELF_TEST_X_ACCEL  0x0D
#define     ICM20602_SELF_TEST_Y_ACCEL  0x0E
#define     ICM20602_SELF_TEST_Z_ACCEL  0x0F
#define     ICM20602_XG_OFFS_USRH       0x13
#define     ICM20602_XG_OFFS_USRL       0x14
#define     ICM20602_YG_OFFS_USRH       0x15
#define     ICM20602_YG_OFFS_USRL       0x16
#define     ICM20602_ZG_OFFS_USRH       0x17
#define     ICM20602_ZG_OFFS_USRL       0x18
#define     ICM20602_SMPLRT_DIV         0x19
#define     ICM20602_CONFIG             0x1A
#define     ICM20602_GYRO_CONFIG        0x1B
#define     ICM20602_ACCEL_CONFIG       0x1C
#define     ICM20602_ACCEL_CONFIG_2     0x1D
#define     ICM20602_LP_MODE_CFG        0x1E
#define     ICM20602_ACCEL_WOM_X_THR    0x20
#define     ICM20602_ACCEL_WOM_Y_THR    0x21
#define     ICM20602_ACCEL_WOM_Z_THR    0x22
#define     ICM20602_FIFO_EN            0x23
#define     ICM20602_FSYNC_INT          0x36
#define     ICM20602_INT_PIN_CFG        0x37
#define     ICM20602_INT_ENABLE         0x38
#define     ICM20602_FIFO_WM_INT_STATUS 0x39 
#define     ICM20602_INT_STATUS         0x3A
#define     ICM20602_ACCEL_XOUT_H       0x3B
#define     ICM20602_ACCEL_XOUT_L       0x3C
#define     ICM20602_ACCEL_YOUT_H       0x3D
#define     ICM20602_ACCEL_YOUT_L       0x3E
#define     ICM20602_ACCEL_ZOUT_H       0x3F
#define     ICM20602_ACCEL_ZOUT_L       0x40
#define     ICM20602_TEMP_OUT_H         0x41
#define     ICM20602_TEMP_OUT_L         0x42
#define     ICM20602_GYRO_XOUT_H        0x43
#define     ICM20602_GYRO_XOUT_L        0x44
#define     ICM20602_GYRO_YOUT_H        0x45
#define     ICM20602_GYRO_YOUT_L        0x46
#define     ICM20602_GYRO_ZOUT_H        0x47
#define     ICM20602_GYRO_ZOUT_L        0x48
#define     ICM20602_SELF_TEST_X_GYRO   0x50
#define     ICM20602_SELF_TEST_Y_GYRO   0x51
#define     ICM20602_SELF_TEST_Z_GYRO   0x52
#define     ICM20602_FIFO_WM_TH1        0x60
#define     ICM20602_FIFO_WM_TH2        0x61
#define     ICM20602_SIGNAL_PATH_RESET  0x68
#define     ICM20602_ACCEL_INTEL_CTRL   0x69
#define     ICM20602_USER_CTRL          0x6A
#define     ICM20602_PWR_MGMT_1         0x6B
#define     ICM20602_PWR_MGMT_2         0x6C
#define     ICM20602_I2C_IF             0x70
#define     ICM20602_FIFO_COUNTH        0x72
#define     ICM20602_FIFO_COUNTL        0x73
#define     ICM20602_FIFO_R_W           0x74
#define     ICM20602_WHO_AM_I           0x75
#define     ICM20602_XA_OFFSET_H        0x77
#define     ICM20602_XA_OFFSET_L        0x78
#define     ICM20602_YA_OFFSET_H        0x7A
#define     ICM20602_YA_OFFSET_L        0x7B
#define     ICM20602_ZA_OFFSET_H        0x7D
#define     ICM20602_ZA_OFFSET_L        0x7E

enum drvimu_inv_whoami_t {
	INV_ICM20602 = 0x12,
	INV_MPU9250 = 0x71,
	INV_MPU6050 = 0x68,
};

enum drvimu_inv_accel_fs {	// In the ACCEL_CONFIG (0x1C) register,[4,3] the full scale select  bits are :
	MPU_FS_2G = 0,	// 00 = 2G
	MPU_FS_4G,		// 01 = 4
	MPU_FS_8G,		// 10 = 8
	MPU_FS_16G,		// 11 = 16
	NUM_MPU_AFS
};

enum drvimu_inv_accel_ois_fs {	// In the ACCEL_CONFIG (0x1C) register, the accel ois full scale select bits are :
	MPU_OIS_FS_2G = 0,	// 00 = 2G
	MPU_OIS_FS_4G,		// 01 = 4
	MPU_OIS_FS_8G,		// 10 = 8
	MPU_OIS_FS_1G,		// 11 = 1
	NUM_MPU_OIS_AFS
};

enum drvimu_inv_accel_bw {		// In the ACCEL_CONFIG2 (0x1D) register, [2,0]the BW setting bits are :
	MPU_ABW_218 = 1,	// 001 = 218 Hz
	MPU_ABW_99,			// 010 = 99 Hz
	MPU_ABW_45,			// 011 = 45 Hz
	MPU_ABW_21,			// 100 = 21 Hz
	MPU_ABW_10,			// 101 = 10 Hz
	MPU_ABW_5,			// 110 = 5 Hz
	MPU_ABW_420,		// 111 = 420 Hz
	NUM_MPU_ABW
};

enum drvimu_inv_gyro_fs {		// In the GYRO_CONFIG register, the [4,3]fS_SEL bits are :
	MPU_FS_250dps = 0,	// 00 = 250
	MPU_FS_500dps,		// 01 = 500
	MPU_FS_1000dps,		// 10 = 1000
	MPU_FS_2000dps,		// 11 = 2000
	NUM_MPU_GFS
};

enum drvimu_inv_gyro_bw {   // In the CONFIG register, the  [2,0]BW setting bits are :
	MPU_GBW_250 = 0,  //000 = 250hz
	MPU_GBW_176 = 1, // 001 = 176 Hz
	MPU_GBW_92,		 // 010 = 92 Hz
	MPU_GBW_41,		 // 011 = 41 Hz
	MPU_GBW_20,		 // 100 = 20 Hz
	MPU_GBW_10,		 // 101 = 10 Hz
	MPU_GBW_5,		 // 110 = 5 Hz
	NUM_MPU_GBW
};


#ifndef INV_MIN
#define INV_MIN(x,y) (((x)<(y))?(x):(y))
#endif

#ifndef INV_MAX
#define INV_MAX(x,y) (((x)>(y))?(x):(y))
#endif

#ifndef INV_ABS
#define INV_ABS(x) (((x) < 0) ? -(x) : (x))
#endif



#ifdef __cplusplus
extern "C"
{
#endif
#define DEG2RAD (0.0174532925199432957692369076848f)
#define RAD2DEG (57.295779513082320876798154814105f)
	typedef struct inv_icm20602_serif { //传输对象
		//uint8_t is_spi; //非0时启用spi，否则启用iic

		//void* i2c_base; //i2c外设地址
		uint8_t i2c_slave_addr;
		int32_t (*i2c_rx)(uint8_t slave_addr, uint8_t reg, uint8_t* buf, uint32_t len);
		int32_t (*i2c_tx)(uint8_t slave_addr, uint8_t reg, uint8_t* buf, uint32_t len);

		//void* spi_base; //spi外设地址
		//uint32_t spi_pcs;
		int32_t(*spi_xfer)(uint8_t* txbuf, uint8_t* rxbuf, uint32_t len);
		//uint8_t spi_transferTxBuf[15];
		//uint8_t spi_transferRxBuf[15];
	} drvimu_inv_serif_t;

	typedef struct AxisTransformation { //转换表的意思是 例如t2g,意味着新的t轴对应这原来坐标系的g轴，最终的是新坐标系，新在前，旧在后
		int8_t x2x;
		int8_t x2y;
		int8_t x2z;
		int8_t y2x;
		int8_t y2y;
		int8_t y2z;
		int8_t z2x;
		int8_t z2y;
		int8_t z2z;
	} drvimu_axis_t;

	typedef struct inv_icm20602 {//icm对象
		drvimu_inv_serif_t serif;
		struct inv_icm20602_states {//icm配置
			uint8_t mpu;
			uint8_t gyro_fullscale;
			uint8_t accel_fullscale;
			float gyro_inv;
			float acc_inv;
			uint8_t gyro_bw;
			uint8_t accel_bw;
			uint8_t sample_div;
		} states;
		struct inv_icm20602_data {//传感器数据acc, gyro,由驱动更新；acc_dot, gyro_dot供用户使用
			volatile float x;
			volatile float y;
			volatile float z;
		} acc, gyro, acc_dot, gyro_dot;
		volatile float temp;//传感器数据temp，温度
		struct inv_icm20602_pointer_2_data {//实现转换坐标轴的指针
			float* x;
			float* y;
			float* z;
		} p_acc, p_gyro;
		struct inv_icm20602_rawdata {//原始数据
			uint8_t buff[14];
			float ax;
			float ay;
			float az;
			float gx;
			float gy;
			float gz;
			float neg_ax;
			float neg_ay;
			float neg_az;
			float neg_gx;
			float neg_gy;
			float neg_gz;
			float axo;
			float ayo;
			float azo;
			float gxo;
			float gyo;
			float gzo;
		} rawdata;
		const char* name;
		uint8_t if_init;//如果成功初始化了为1，没有则为0
	} drvimu_inv_device_t;

	/**
	 * @brief	设置为默认配置
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 */
	void DRVIMU_INV_GetDefaultConfig(drvimu_inv_device_t* s);

	/**
	 * @brief	设置传输结构体
	 *
	 * @param  {drvimu_inv_device_t*} s            : icm对象句柄
	 * @param  {drvimu_inv_serif_t*} serif_ : 传输对象句柄
	 */
	void DRVIMU_INV_SetSerif(drvimu_inv_device_t* s, drvimu_inv_serif_t* serif_);

	/**
	 * @brief 	设置数字滤波器的截止频率
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 * @param  {enum} drvimu_inv_accel_bw : 截止频率
	 * @param  {enum} drvimu_inv_gyro_bw  : 截止频率
	 */
	void DRVIMU_INV_SetLPF(drvimu_inv_device_t* s, enum drvimu_inv_accel_bw accbw, enum drvimu_inv_gyro_bw gyrobw);

	/**
	 * @brief 	设置量程
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 * @param  {enum} drvimu_inv_accel_fs : 量程
	 * @param  {enum} drvimu_inv_gyro_fs  : 量程
	 */
	void DRVIMU_INV_SetFS(drvimu_inv_device_t* s, enum drvimu_inv_accel_fs accfs, enum drvimu_inv_gyro_fs gyrofs);

	/**
	 * @brief 	设置输出的坐标轴
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 * @param  {drvimu_axis_t*} fa        : 描述输出加速度转换坐标轴的一个规则
	 * @param  {drvimu_axis_t*} fg        : 描述输出加速度转换坐标轴的一个规则
	 */
	void DRVIMU_INV_SetAxis(drvimu_inv_device_t* s, drvimu_axis_t* fa, drvimu_axis_t* fg);

	/**
	 * @brief 	初始化，除此之外的设置配置都是在本地进行，只有这一步会把配置写入icm20602
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 * @param  {char*} nickname    : 起个别名吧
	 * @return {int}               : 错误代码
	 */
	int DRVIMU_INV_Init(drvimu_inv_device_t* s);

	/**
	 * @brief	polling方式读取传感器，读取完自动转换
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 */
	void DRVIMU_INV_SensorReadBlocking(drvimu_inv_device_t* s);

	/**
	 * @brief 	主动校准陀螺仪，使用时要保证传感器静止
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 */
	void DRVIMU_INV_GyroOffset(drvimu_inv_device_t* s);

	/**
	 * @brief 	被动校准加速度计，需要传入加速度的偏移值
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 * @param  {float} axo_        : 偏移值，可能量程不同会不一样
	 * @param  {float} ayo_        :
	 * @param  {float} azo_        :
	 */
	void DRVIMU_INV_AcceOffset(drvimu_inv_device_t* s, float axo_, float ayo_, float azo_);

	/**
	 * @brief 	自动计算被动校准加速度计需要的加速度的偏移值
	 * 		算法需要使用矩阵库，因为底层不含矩阵库于是注释掉了本函数，
	 * 		有需要联系作者
	 *
	 * @param  {drvimu_inv_device_t*} s : icm对象句柄
	 */
	void DRVIMU_INV_AcceCube(drvimu_inv_device_t* s);

	/**
	 * @brief 	内部使用
	 *
	 * @param  {drvimu_inv_device_t*} s :
	 * @param  {uint8_t} reg       :
	 * @param  {uint8_t*} data     :
	 * @param  {uint32_t} len      :
	 */
	void DRVIMU_INV_ReadReg(drvimu_inv_device_t* s, uint8_t reg, uint8_t* data, uint32_t len);

	/**
	 * @brief 	内部使用
	 *
	 * @param  {drvimu_inv_device_t*} s :
	 * @param  {uint8_t} reg       :
	 * @param  {uint8_t*} data     :
	 * @param  {uint32_t} len      :
	 */
	void DRVIMU_INV_WriteReg(drvimu_inv_device_t* s, uint8_t reg, uint8_t* data, uint32_t len);

	/**
	 * @brief	简单的cpu延时，内部使用
	 *
	 * @param  {uint32_t} ms :
	 */
	void DRVIMU_INV_Delay_ms(uint32_t ms);


	enum inv_icm20602_message_type
	{
		inv_icm20602_information = 0,
		inv_icm20602_warning,
		inv_icm20602_error,
	};
	/**
	 * @brief	驱动内部使用的消息打印函数，使用驱动前先修改本函数
	 *
	 * @param  {enum} inv_icm20602_message_type : 如其名
	 * @param  {char*} Format                   :
	 * @param  {...} undefined                  :
	 */
	void inv_icm20602_message(enum inv_icm20602_message_type type, const char* Format, ...);

	/* -------------------------------------------------------------------------- */
	/*                                    示例                                    */
	/* -------------------------------------------------------------------------- */

	void DRVIMU_INV_Demo(void);
#ifdef __cplusplus
}
#endif

#endif // HITSIC_USE_DRV_IMU

#endif // ! DRV_IMU_INV_H
	
