#include "hitsic_hal.h"


#ifdef __cplusplus
extern "C"{
#endif
status_t HAL_I2C_Mem_ReadBlocking(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize)
{
    static i2c_master_transfer_t tof_i2c_xfer;
	tof_i2c_xfer.slaveAddress = _addr;
	tof_i2c_xfer.direction = kI2C_Read;
	tof_i2c_xfer.subaddress = _reg;
	tof_i2c_xfer.subaddressSize = _regSize;
	tof_i2c_xfer.data = _data;
	tof_i2c_xfer.dataSize = _dataSize;
	tof_i2c_xfer.flags = kI2C_TransferDefaultFlag;
	return I2C_MasterTransferBlocking(_i2c, &tof_i2c_xfer);
}

status_t HAL_I2C_Mem_WriteBlocking(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize)
{
	static i2c_master_transfer_t tof_i2c_xfer;
	tof_i2c_xfer.slaveAddress = _addr;
	tof_i2c_xfer.direction = kI2C_Read;
	tof_i2c_xfer.subaddress = _reg;
	tof_i2c_xfer.subaddressSize = _regSize;
	tof_i2c_xfer.data = _data;
	tof_i2c_xfer.dataSize = _dataSize;
	tof_i2c_xfer.flags = kI2C_TransferDefaultFlag;
	I2C_MasterTransferBlocking(_i2c, &tof_i2c_xfer);
}



#ifdef __cplusplus
}
#endif