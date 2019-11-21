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

#include "drv_imu_invensense.h"

#if defined(HITSIC_USE_DRV_IMU) && (HITSIC_USE_DRV_IMU > 0)

#ifdef __cplusplus
extern "C" {
#endif



	void DRVIMU_INV_GetDefaultConfig(drvimu_inv_device_t* s)
	{
		memset(s, 0, sizeof(drvimu_inv_device_t));
		//默认滤波
		DRVIMU_INV_SetLPF(s, MPU_ABW_218, MPU_GBW_176);

		//默认量程
		DRVIMU_INV_SetFS(s, MPU_FS_4G, MPU_FS_2000dps);

		//默认不改变坐标轴
		drvimu_axis_t acc, gyro;
		memset(&acc, 0, sizeof(drvimu_axis_t));
		memset(&gyro, 0, sizeof(drvimu_axis_t));
		acc.x2x = 1; gyro.x2x = 1;
		acc.y2y = 1; gyro.y2y = 1;
		acc.z2z = 1; gyro.z2z = 1;
		DRVIMU_INV_SetAxis(s, &acc, &gyro);
        
        s->serif.spi_xfer = NULL;
		s->serif.i2c_rx = DRVIMU_icm_example_i2c_rx;
		s->serif.i2c_tx = DRVIMU_icm_example_i2c_tx;
	}

	void DRVIMU_INV_SetSerif(drvimu_inv_device_t* s, drvimu_inv_serif_t* serif_)
	{
		memcpy(&(s->serif), serif_, sizeof(drvimu_inv_serif_t));
	}

	void DRVIMU_INV_SetLPF(drvimu_inv_device_t* s, enum drvimu_inv_accel_bw accbw, enum  drvimu_inv_gyro_bw gyrobw)
	{
		s->states.accel_bw = accbw;
		s->states.gyro_bw = gyrobw;
	}

	void DRVIMU_INV_SetFS(drvimu_inv_device_t* s, enum drvimu_inv_accel_fs accfs, enum  drvimu_inv_gyro_fs gyrofs)
	{
		s->states.accel_fullscale = accfs;
		s->states.gyro_fullscale = gyrofs;
		switch (s->states.accel_fullscale)
		{
		case (uint8_t)MPU_FS_2G:
			s->states.acc_inv = 2.0 * 9.8 / 32768.0;
			break;
		case (uint8_t)MPU_FS_4G:
			s->states.acc_inv = 4.0 * 9.8 / 32768.0;
			break;
		case (uint8_t)MPU_FS_8G:
			s->states.acc_inv = 8.0 * 9.8 / 32768.0;
			break;
		case (uint8_t)MPU_FS_16G:
			s->states.acc_inv = 16.0 * 9.8 / 32768.0;
			break;
		default:
			s->states.acc_inv = 16.0 * 9.8 / 32768.0;
			break;
		}
		switch (s->states.gyro_fullscale)
		{
		case (uint8_t)MPU_FS_250dps:
			s->states.gyro_inv = /*0.0174532925199432957692369076848f * */(250.0 / 32768.0);
			break;
		case (uint8_t)MPU_FS_500dps:
			s->states.gyro_inv =/* 0.0174532925199432957692369076848f * */(500.0 / 32768.0);
			break;
		case (uint8_t)MPU_FS_1000dps:
			s->states.gyro_inv =/* 0.0174532925199432957692369076848f * */(1000.0 / 32768.0);
			break;
		case (uint8_t)MPU_FS_2000dps:
			s->states.gyro_inv =/* 0.0174532925199432957692369076848f * */(2000.0 / 32768.0);
			break;
		default:
			s->states.gyro_inv =/* 0.0174532925199432957692369076848f * */(2000.0 / 32768.0);
			break;
		}
	}
	int DRVIMU_INV_Init(drvimu_inv_device_t* s)
	{
		uint8_t whoami = 0xff;
		int times = 100;
		s->serif.i2c_slave_addr = IMU_MPU9250_I2C_ADDR;
		//s->name = nickname;
		while (1)
		{
			DRVIMU_INV_ReadReg(s, ICM20602_WHO_AM_I, &whoami, 1);
			if (whoami == INV_ICM20602)
			{
				s->states.mpu = INV_ICM20602;
				inv_icm20602_message(inv_icm20602_information, "%s:select ICM20602", s->name);
				break;
			}
			else if (whoami == INV_MPU9250)
			{
				s->states.mpu = INV_MPU9250;
				inv_icm20602_message(inv_icm20602_information, "%s:select MPU9250", s->name);
				break;
			}
			else if (whoami == INV_MPU6050)
			{
				s->states.mpu = INV_MPU6050;
				inv_icm20602_message(inv_icm20602_information, "%s:select MPU6050", s->name);
				break;
			}
			else
			{
				s->serif.i2c_slave_addr = s->serif.i2c_slave_addr ^ 0x01;//异或运算，翻转最低位，因为地址只有0x68和0x69
				DRVIMU_INV_Delay_ms(10);
				inv_icm20602_message(inv_icm20602_warning, "%s:select warning,%d times remaining", s->name, times);
				inv_icm20602_message(inv_icm20602_warning, "%s:whoami = %#X", s->name, whoami);
				--times;
				if (times <= 0)
				{
					inv_icm20602_message(inv_icm20602_error, "%s:select error,%d times remaining", s->name, times);
					inv_icm20602_message(inv_icm20602_error, "%s:whoami = %#X", s->name, whoami);
					return -1;
				}
			}
		}
		uint8_t val = 0x80;
		switch (s->states.mpu)//每种imu都一样的配置过程
		{
		default:
			val = 0x80;
			DRVIMU_INV_WriteReg(s, ICM20602_PWR_MGMT_1, &val, 1);//复位设备
			do
			{//等待复位成功
				DRVIMU_INV_Delay_ms(10);
				DRVIMU_INV_ReadReg(s, ICM20602_PWR_MGMT_1, &val, 1);
				inv_icm20602_message(inv_icm20602_warning, "%s:wait...PWR_MGMT_1 = %#X", s->name, val);
			} while (!(0x41 == val || 0x01 == val || 0x40 == val));
			val = 0x00; DRVIMU_INV_Delay_ms(10); DRVIMU_INV_WriteReg(s, ICM20602_PWR_MGMT_1, &val, 1);                          //使用内部时钟
			val = 0x00; DRVIMU_INV_Delay_ms(10); DRVIMU_INV_WriteReg(s, ICM20602_PWR_MGMT_2, &val, 1);                          //开启陀螺仪和加速度计
			val = s->states.gyro_bw; DRVIMU_INV_Delay_ms(10); DRVIMU_INV_WriteReg(s, ICM20602_CONFIG, &val, 1);	             //配置陀螺仪数字滤波器
			val = s->states.sample_div; DRVIMU_INV_Delay_ms(10); DRVIMU_INV_WriteReg(s, ICM20602_SMPLRT_DIV, &val, 1);			  //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
			val = s->states.gyro_fullscale << 3; DRVIMU_INV_Delay_ms(10); DRVIMU_INV_WriteReg(s, ICM20602_GYRO_CONFIG, &val, 1);  //设置陀螺仪量程
			val = s->states.accel_fullscale << 3; DRVIMU_INV_Delay_ms(10); DRVIMU_INV_WriteReg(s, ICM20602_ACCEL_CONFIG, &val, 1);//设置加速度计量程
			val = s->states.accel_bw; DRVIMU_INV_Delay_ms(10); DRVIMU_INV_WriteReg(s, ICM20602_ACCEL_CONFIG_2, &val, 1);        //配置加速度计数字滤波器
		}
		DRVIMU_INV_Delay_ms(10);
		s->if_init = 1;
		return 0;
	}

	void DRVIMU_INV_GyroOffset(drvimu_inv_device_t* s)
	{
		uint32_t times = 1000;
		float buffgxo = 0;
		float buffgyo = 0;
		float buffgzo = 0;
		s->rawdata.gxo = 0;
		s->rawdata.gyo = 0;
		s->rawdata.gzo = 0;
		while (times--)
		{
			DRVIMU_INV_SensorReadBlocking(s);
			DRVIMU_INV_Delay_ms(5);
			buffgxo += s->rawdata.gx;
			buffgyo += s->rawdata.gy;
			buffgzo += s->rawdata.gz;
		}
		s->rawdata.gxo = buffgxo * 0.001f;
		s->rawdata.gyo = buffgyo * 0.001f;
		s->rawdata.gzo = buffgzo * 0.001f;
	}

	void DRVIMU_INV_AcceOffset(drvimu_inv_device_t* s, float axo_, float ayo_, float azo_)
	{
		s->rawdata.axo = axo_;
		s->rawdata.ayo = ayo_;
		s->rawdata.azo = azo_;
	}


	//加速度校准算法需要使用到矩阵计算库，并且在工程设置printf允许打印浮点数
	void DRVIMU_INV_AcceCube(drvimu_inv_device_t* s)
	{
		//PRINTF("start to imu cube\r\n");
		//char ch;
		//float A_val[14][6];
		//float B_val[14][1];
		//float aax, aay, aaz;
		//s->rawdata.axo = 0;
		//s->rawdata.ayo = 0;
		//s->rawdata.azo = 0;
		//int times = 1000;
		//for (int i = 0; i < 14; i++)
		//{
		//	B_val[i][0] = 9.8f * 9.8f;
		//	do
		//	{
		//		PRINTF("input q to the %dtimes cube\r\n", i);
		//		ch = GETCHAR();
		//	} while (ch != 'q');
		//	aax = 0;
		//	aay = 0;
		//	aaz = 0;
		//	for (int jj = 0; jj < 1000; jj++)
		//	{
		//		DRVIMU_INV_SensorReadBlocking(s);
		//		DRVIMU_INV_Delay_ms(1);
		//		aax += s->rawdata.ax;
		//		aay += s->rawdata.ay;
		//		aaz += s->rawdata.az;
		//	}
		//	aax *= 0.001;
		//	aay *= 0.001;
		//	aaz *= 0.001;
		//	A_val[i][0] = aay * aay;
		//	A_val[i][1] = aaz * aaz;
		//	A_val[i][2] = aax;
		//	A_val[i][3] = aay;
		//	A_val[i][4] = aaz;
		//	A_val[i][5] = 1.0f;
		//	B_val[i][0] = -aax * aax;
		//}
		//Mat A, A_T, X, B, aa, bb, cc;
		//MatDump(MatCreate(&A, 14, 6));
		//MatDump(MatCreate(&A_T, 6, 14));
		//MatDump(MatCreate(&B, 14, 1));
		//MatDump(MatCreate(&aa, 6, 6));//A_T*A
		//MatDump(MatCreate(&cc, 6, 6));//(A_T*A)^-1
		//MatDump(MatCreate(&bb, 6, 1));//A_T*B
		//MatDump(MatCreate(&X, 6, 1));
		//MatSetVal(&A, (float*)A_val);
		//MatSetVal(&B, (float*)B_val);
		//MatTrans(&A, &A_T);
		//MatMul(&A_T, &A, &aa);
		//MatMul(&A_T, &B, &bb);
		//MatInv(&aa, &cc);
		//MatMul(&cc, &bb, &X);
		//PRINTF("data of cube:\r\n");
		//char bsihbuff[100];
		//float x0 = MatGetElementVal(&X, 0, 0);
		//float x1 = MatGetElementVal(&X, 1, 0);
		//float x2 = MatGetElementVal(&X, 2, 0);
		//float x3 = MatGetElementVal(&X, 3, 0);
		//float x4 = MatGetElementVal(&X, 4, 0);
		//float x5 = MatGetElementVal(&X, 5, 0);
		//sprintf(bsihbuff, "X0:  %f  \r\n", x0);	PRINTF(bsihbuff);
		//sprintf(bsihbuff, "X1:  %f  \r\n", x1);	PRINTF(bsihbuff);
		//sprintf(bsihbuff, "X2:  %f  \r\n", x2);	PRINTF(bsihbuff);
		//sprintf(bsihbuff, "X3:  %f  \r\n", x3);	PRINTF(bsihbuff);
		//sprintf(bsihbuff, "X4:  %f  \r\n", x4);	PRINTF(bsihbuff);
		//sprintf(bsihbuff, "X5:  %f  \r\n", x5);	PRINTF(bsihbuff);
		//MatDelete(&A);
		//MatDelete(&A_T);
		//MatDelete(&B);
		//MatDelete(&aa);
		//MatDelete(&cc);
		//MatDelete(&bb);
		//MatDelete(&X);
		//s->rawdata.axo = x2 / -2.0f;
		//s->rawdata.ayo = x3 / (-2.0f * x0);
		//s->rawdata.azo = x4 / (-2.0f * x1);
		//float xrxr = 9.8f * 9.8f / (s->rawdata.axo * s->rawdata.axo + s->rawdata.ayo * s->rawdata.ayo * x0 + s->rawdata.azo * s->rawdata.azo * x1 - x5);
		//float axr = sqrt(xrxr);
		//float ayr = sqrt(xrxr * x0);
		//float azr = sqrt(xrxr * x1);
		//sprintf(bsihbuff, "cube.axo:  %f  \r\n", s->rawdata.axo); PRINTF(bsihbuff);
		//sprintf(bsihbuff, "cube.ayo:  %f  \r\n", s->rawdata.ayo); PRINTF(bsihbuff);
		//sprintf(bsihbuff, "cube.azo:  %f  \r\n", s->rawdata.azo); PRINTF(bsihbuff);
		//sprintf(bsihbuff, "cube.axr:  %f  \r\n", axr); PRINTF(bsihbuff);
		//sprintf(bsihbuff, "cube.ayr:  %f  \r\n", ayr); PRINTF(bsihbuff);
		//sprintf(bsihbuff, "cube.azr:  %f  \r\n", azr); PRINTF(bsihbuff);
		//PRINTF("the end:\r\n");
		//while (1)
		//{
		//}
	}

	void DRVIMU_INV_ReadReg(drvimu_inv_device_t* s, uint8_t reg, uint8_t* data, uint32_t len)
	{
		int result = 1;
		if (s->serif.spi_xfer != NULL)
		{
			uint8_t *buf = (uint8_t*)malloc((len + 1) * sizeof(uint8_t));
			if(buf == NULL){ result = -1; return; }
			memset(buf, 0, len + 1);
			buf[0] = reg | 0x80;
			result = s->serif.spi_xfer(buf, buf, len + 1);
			memcpy(data, buf + 1, len);
		}
		else
		{
			result = s->serif.i2c_rx(s->serif.i2c_slave_addr, reg, data, len);
		}
		if (result != 0)
		{
			inv_icm20602_message(inv_icm20602_warning, "%s:Icm Read error:code=%d", s->name, result);
		}
	}

	void DRVIMU_INV_WriteReg(drvimu_inv_device_t* s, uint8_t reg, uint8_t* data, uint32_t len)
	{
		int result = 1;
		if (s->serif.spi_xfer != NULL)
		{
			uint8_t *buf = (uint8_t*)malloc((len + 1) * sizeof(uint8_t));
			if(buf == NULL){ result = -1; return; }
			buf[0] = reg | 0x80;
			memcpy(buf + 1, data, len);
			result = s->serif.spi_xfer(buf, buf, len + 1);
			
		}
		else
		{
			result = s->serif.i2c_tx(s->serif.i2c_slave_addr, reg, data, len);
		}
		if (result != 0)
		{
			inv_icm20602_message(inv_icm20602_warning, "%s:Icm Write error:code=%d", s->name, result);
		}
	}

	void DRVIMU_INV_SensorReadBlocking(drvimu_inv_device_t* s)
	{
		DRVIMU_INV_ReadReg(s, ICM20602_ACCEL_XOUT_H, s->rawdata.buff, 14);
		s->rawdata.ax = s->states.acc_inv * ((int16_t)(s->rawdata.buff[0] << 8) | s->rawdata.buff[1]) - s->rawdata.axo;
		s->rawdata.ay = s->states.acc_inv * ((int16_t)(s->rawdata.buff[2] << 8) | s->rawdata.buff[3]) - s->rawdata.ayo;
		s->rawdata.az = s->states.acc_inv * ((int16_t)(s->rawdata.buff[4] << 8) | s->rawdata.buff[5]) - s->rawdata.azo;
		s->temp = 0.02f * (25.0f + 0.003059975520f * ((int16_t)(s->rawdata.buff[6] << 8) | s->rawdata.buff[7])) + 0.98f * s->temp;
		s->rawdata.gx = s->states.gyro_inv * ((int16_t)(s->rawdata.buff[8] << 8) | s->rawdata.buff[9]) - s->rawdata.gxo;
		s->rawdata.gy = s->states.gyro_inv * ((int16_t)(s->rawdata.buff[10] << 8) | s->rawdata.buff[11]) - s->rawdata.gyo;
		s->rawdata.gz = s->states.gyro_inv * ((int16_t)(s->rawdata.buff[12] << 8) | s->rawdata.buff[13]) - s->rawdata.gzo;
		s->rawdata.neg_ax = -s->rawdata.ax;
		s->rawdata.neg_ay = -s->rawdata.ay;
		s->rawdata.neg_az = -s->rawdata.az;
		s->rawdata.neg_gx = -s->rawdata.gx;
		s->rawdata.neg_gy = -s->rawdata.gy;
		s->rawdata.neg_gz = -s->rawdata.gz;
		s->acc.x = *s->p_acc.x;
		s->acc.y = *s->p_acc.y;
		s->acc.z = *s->p_acc.z;
		s->gyro.x = *s->p_gyro.x;
		s->gyro.y = *s->p_gyro.y;
		s->gyro.z = *s->p_gyro.z;
	}

	void DRVIMU_INV_SetAxis(drvimu_inv_device_t* s, drvimu_axis_t* fa, drvimu_axis_t* fg)
	{
		//acc_x
		if (fa->x2x)
		{
			if (fa->x2x > 0)
			{
				s->p_acc.x = &s->rawdata.ax;
			}
			else
			{
				s->p_acc.x = &s->rawdata.neg_ax;
			}
		}
		else if (fa->x2y)
		{
			if (fa->x2y > 0)
			{
				s->p_acc.x = &s->rawdata.ay;
			}
			else
			{
				s->p_acc.x = &s->rawdata.neg_ay;
			}
		}
		else if (fa->x2z)
		{
			if (fa->x2z > 0)
			{
				s->p_acc.x = &s->rawdata.az;
			}
			else
			{
				s->p_acc.x = &s->rawdata.neg_az;
			}
		}

		//acc_y
		if (fa->y2x)
		{
			if (fa->y2x > 0)
			{
				s->p_acc.y = &s->rawdata.ax;
			}
			else
			{
				s->p_acc.y = &s->rawdata.neg_ax;
			}
		}
		else if (fa->y2y)
		{
			if (fa->y2y > 0)
			{
				s->p_acc.y = &s->rawdata.ay;
			}
			else
			{
				s->p_acc.y = &s->rawdata.neg_ay;
			}
		}
		else if (fa->y2z)
		{
			if (fa->y2z > 0)
			{
				s->p_acc.y = &s->rawdata.az;
			}
			else
			{
				s->p_acc.y = &s->rawdata.neg_az;
			}
		}

		//acc_z
		if (fa->z2x)
		{
			if (fa->z2x > 0)
			{
				s->p_acc.z = &s->rawdata.ax;
			}
			else
			{
				s->p_acc.z = &s->rawdata.neg_ax;
			}
		}
		else if (fa->z2y)
		{
			if (fa->z2y > 0)
			{
				s->p_acc.z = &s->rawdata.ay;
			}
			else
			{
				s->p_acc.z = &s->rawdata.neg_ay;
			}
		}
		else if (fa->z2z)
		{
			if (fa->z2z > 0)
			{
				s->p_acc.z = &s->rawdata.az;
			}
			else
			{
				s->p_acc.z = &s->rawdata.neg_az;
			}
		}

		//以下都是gyro部分
			//gyro_x
		if (fg->x2x)
		{
			if (fg->x2x > 0)
			{
				s->p_gyro.x = &s->rawdata.gx;
			}
			else
			{
				s->p_gyro.x = &s->rawdata.neg_gx;
			}
		}
		else if (fg->x2y)
		{
			if (fg->x2y > 0)
			{
				s->p_gyro.x = &s->rawdata.gy;
			}
			else
			{
				s->p_gyro.x = &s->rawdata.neg_gy;
			}
		}
		else if (fg->x2z)
		{
			if (fg->x2z > 0)
			{
				s->p_gyro.x = &s->rawdata.gz;
			}
			else
			{
				s->p_gyro.x = &s->rawdata.neg_gz;
			}
		}

		//gyro_y
		if (fg->y2x)
		{
			if (fg->y2x > 0)
			{
				s->p_gyro.y = &s->rawdata.gx;
			}
			else
			{
				s->p_gyro.y = &s->rawdata.neg_gx;
			}
		}
		else if (fg->y2y)
		{
			if (fg->y2y > 0)
			{
				s->p_gyro.y = &s->rawdata.gy;
			}
			else
			{
				s->p_gyro.y = &s->rawdata.neg_gy;
			}
		}
		else if (fg->y2z)
		{
			if (fg->y2z > 0)
			{
				s->p_gyro.y = &s->rawdata.gz;
			}
			else
			{
				s->p_gyro.y = &s->rawdata.neg_gz;
			}
		}

		//gyro_z
		if (fg->z2x)
		{
			if (fg->z2x > 0)
			{
				s->p_gyro.z = &s->rawdata.gx;
			}
			else
			{
				s->p_gyro.z = &s->rawdata.neg_gx;
			}
		}
		else if (fg->z2y)
		{
			if (fg->z2y > 0)
			{
				s->p_gyro.z = &s->rawdata.gy;
			}
			else
			{
				s->p_gyro.z = &s->rawdata.neg_gy;
			}
		}
		else if (fg->z2z)
		{
			if (fg->z2z > 0)
			{
				s->p_gyro.z = &s->rawdata.gz;
			}
			else
			{
				s->p_gyro.z = &s->rawdata.neg_gz;
			}
		}
	}

	void DRVIMU_INV_Delay_ms(uint32_t ms)
	{
		while (--ms)
		{
			uint32_t x = 2501;
			while (--x)
			{

			}
		}
	}

	void inv_icm20602_message(enum inv_icm20602_message_type type, const char* Format, ...)
	{
		char buf[128];
		int len;
		va_list args;
		va_start(args, Format);
		len = vsnprintf(buf, 128, Format, args);
		if (len > sizeof(buf))
		{
			inv_icm20602_message(inv_icm20602_error, "消息");
			inv_icm20602_message(inv_icm20602_error, "越界\r\n");
		}
		//len = vsprintf_s(buf,sizeof(buf), Format, args);
		switch (type)
		{
		case inv_icm20602_information:
			PRINTF(buf); PRINTF("\r\n");
			break;
		case inv_icm20602_warning:
			PRINTF(buf); PRINTF("\r\n");
			break;
		case inv_icm20602_error:
			PRINTF(buf); PRINTF("\r\n");
			break;
		default:
			break;
		}
		va_end(args);
	}




	void DRVIMU_INV_Demo(void)
	{
		PRINTF("icm20602 deemo \r\n");
		drvimu_inv_device_t icm0;
		DRVIMU_INV_GetDefaultConfig(&icm0);//必须调用这个函数进行默认配置
        I2C_SimpleInit(I2C1, 400000);

		//icm0初始化为硬件i2c驱动
		icm0.serif.spi_xfer = NULL;
		icm0.serif.i2c_rx = DRVIMU_icm_example_i2c_rx;
		icm0.serif.i2c_tx = DRVIMU_icm_example_i2c_tx;
		PRINTF("icm0 init \r\n");
		DRVIMU_INV_Init(&icm0);

		//视成功初始化与否校准陀螺仪
		if (icm0.if_init) { PRINTF("%s init complete!\r\n", icm0.name); DRVIMU_INV_GyroOffset(&icm0); }
		else { PRINTF("%s init fail!\r\n", icm0.name); }
		while (1)
		{
			DRVIMU_INV_Delay_ms(5);
			if (icm0.if_init) {
				DRVIMU_INV_SensorReadBlocking(&icm0);
				PRINTF("%s:\r\n", icm0.name);
				PRINTF("ax=%f\r\n", icm0.acc.x);
				PRINTF("ay=%f\r\n", icm0.acc.y);
				PRINTF("az=%f\r\n", icm0.acc.z);
				PRINTF("gx=%f\r\n", icm0.gyro.x);
				PRINTF("gy=%f\r\n", icm0.gyro.y);
				PRINTF("gz=%f\r\n", icm0.gyro.z);
				PRINTF("T =%f\r\n", icm0.temp);
				PRINTF("\r\n");
			}
			
		}
	}


#ifdef __cplusplus
}
#endif

#endif // HITSIC_USE_DRV_IMU
