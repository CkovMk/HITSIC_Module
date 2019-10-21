#include"rte_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif


status_t I2C_SimpleInit(I2C_Type* base, uint32_t baudRate_Bps_)
{
	i2c_master_config_t masterConfig;
	/*
	* masterConfig->baudRate_Bps = 100000U;
	* masterConfig->enableStopHold = false;
	* masterConfig->glitchFilterWidth = 0U;
	* masterConfig->enableMaster = true;
	*/
	I2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Bps = baudRate_Bps_;
	I2C_MasterInit(base, &masterConfig, CLOCK_GetFreq(BUS_CLK));
	return 0;
}

status_t I2C_SimpleReadBlocking(I2C_Type* base, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size)
{
	i2c_master_transfer_t smartcar_i2c_transfer;
	smartcar_i2c_transfer.slaveAddress = SlaveAddress;
	smartcar_i2c_transfer.direction = kI2C_Read;
	smartcar_i2c_transfer.subaddress = (uint32_t)reg;
	smartcar_i2c_transfer.subaddressSize = 1;
	smartcar_i2c_transfer.data = data;
	smartcar_i2c_transfer.dataSize = size;
	smartcar_i2c_transfer.flags = kI2C_TransferDefaultFlag;
	return I2C_MasterTransferBlocking(base, &smartcar_i2c_transfer);
}

status_t I2C_SimpleWriteBlocking(I2C_Type* base, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size)
{
	i2c_master_transfer_t smartcar_i2c_transfer;
	smartcar_i2c_transfer.slaveAddress = SlaveAddress;
	smartcar_i2c_transfer.direction = kI2C_Write;
	smartcar_i2c_transfer.subaddress = (uint32_t)reg;
	smartcar_i2c_transfer.subaddressSize = 1;
	smartcar_i2c_transfer.data = data;
	smartcar_i2c_transfer.dataSize = size;
	smartcar_i2c_transfer.flags = kI2C_TransferDefaultFlag;
	return I2C_MasterTransferBlocking(base, &smartcar_i2c_transfer);
}


#ifdef __cplusplus
}
#endif
