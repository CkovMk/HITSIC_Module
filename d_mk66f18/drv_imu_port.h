#ifndef _DRV_IMU_PORT_H_
#define _DRV_IMU_PORT_H_

#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"

#ifdef __cplusplus
extern "C"{
#endif

#define HITSIC_USE_DRV_IMU (1U)

#if defined(HITSIC_USE_DRV_IMU) && (HITSIC_USE_DRV_IMU > 0)

#define HITSIC_IMU_SPI (0U)

#define HITSIC_IMU_SPI_INST (SPI1)
#define HITSIC_IMU_SPI_PCSn (0)
#define HITSIC_IMU_SPI_CTARn (0)

#define HITSIC_IMU_I2C (1U)

#define HITSIC_IMU_I2C_INST (I2C1)

#endif // HITSIC_USE_DRV_IMU


#if defined(HITSIC_USE_DRV_IMU) && (HITSIC_USE_DRV_IMU > 0)

    
#if defined(HITSIC_IMU_SPI) && (HITSIC_IMU_SPI > 0)
    status_t DRVIMU_icm_example_spi_xfer(uint8_t* txbuf, uint8_t* rxbuf, uint32_t len);
#elif defined (HITSIC_IMU_I2C) && (HITSIC_IMU_I2C > 0)
    status_t DRVIMU_icm_example_i2c_rx(uint8_t slave_addr, uint8_t reg, uint8_t* buf, uint32_t len);
	status_t DRVIMU_icm_example_i2c_tx(uint8_t slave_addr, uint8_t reg, uint8_t* buf, uint32_t len);
	
#endif

#endif // HITSIC_USE_DRV_IMU



#ifdef __cplusplus
}
#endif

#endif // ! _DRV_IMU_PORT_H_
