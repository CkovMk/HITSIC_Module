#include "drv_imu_port.h"

#ifdef __cplusplus
extern "C"{
#endif



#if defined(HITSIC_IMU_SPI) && (HITSIC_IMU_SPI > 0)
    status_t DRVIMU_icm_example_spi_xfer(uint8_t* txbuf, uint8_t* rxbuf, uint32_t len)
	{
		return  kStatus_Fail;
	}
#elif defined (HITSIC_IMU_I2C) && (HITSIC_IMU_I2C > 0)
    status_t DRVIMU_icm_example_i2c_rx(uint8_t slave_addr, uint8_t reg, uint8_t* buf, uint32_t len)
	{
		return kStatus_Fail;
	}
	
	status_t DRVIMU_icm_example_i2c_tx(uint8_t slave_addr, uint8_t reg, uint8_t* buf, uint32_t len)
	{
		return kStatus_Fail;
	}
	
#endif




#ifdef __cplusplus
}
#endif

