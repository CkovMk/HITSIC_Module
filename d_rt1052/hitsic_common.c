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

#include "hitsic_common.h"

#ifdef __cplusplus
extern "C"{
#endif

uint32_t hal_criticalCnt = 0;
uint32_t hal_regPrimask = 0;


status_t HAL_I2C_Mem_ReadBlocking(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize)
{
    static lpi2c_master_transfer_t tof_i2c_xfer;
	tof_i2c_xfer.slaveAddress = _addr;
	tof_i2c_xfer.direction = kLPI2C_Read;
	tof_i2c_xfer.subaddress = _reg;
	tof_i2c_xfer.subaddressSize = _regSize;
	tof_i2c_xfer.data = _data;
	tof_i2c_xfer.dataSize = _dataSize;
	tof_i2c_xfer.flags = kLPI2C_TransferDefaultFlag;
	return LPI2C_MasterTransferBlocking(_i2c, &tof_i2c_xfer);
}

status_t HAL_I2C_Mem_WriteBlocking(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize)
{
	static lpi2c_master_transfer_t tof_i2c_xfer;
	tof_i2c_xfer.slaveAddress = _addr;
	tof_i2c_xfer.direction = kLPI2C_Read;
	tof_i2c_xfer.subaddress = _reg;
	tof_i2c_xfer.subaddressSize = _regSize;
	tof_i2c_xfer.data = _data;
	tof_i2c_xfer.dataSize = _dataSize;
	tof_i2c_xfer.flags = kLPI2C_TransferDefaultFlag;
	return LPI2C_MasterTransferBlocking(_i2c, &tof_i2c_xfer);
}

    
    
#ifdef __cplusplus
}
#endif
