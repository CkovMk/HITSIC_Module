#ifndef _HITSIC_COMMON_H_
#define _HITSIC_COMMON_H_

#include "hitsic_def.h"
#include "hitsic_hal.h"




#ifdef __cplusplus
extern "C"{
#endif
    


#if defined(HITSIC_USE_SYS_PITMGR) && (HITSIC_USE_SYS_PITMGR > 0)


#endif // HITSIC_USE_SYS_PITMGR





#if defined(HITSIC_USE_SYS_EXTMGR) && (HITSIC_USE_SYS_EXTMGR > 0)


#endif // HITSIC_USE_SYS_EXTMGR






#define HITSIC_USE_SYS_UARTMGR      (1U)
#if defined(HITSIC_USE_SYS_UARTMGR) && (HITSIC_USE_SYS_UARTMGR > 0)

#endif // HITSIC_USE_SYS_UARTMGR







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

#endif // ! _HITSIC_COMMON_H_