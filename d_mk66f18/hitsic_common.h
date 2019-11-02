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

#ifndef _HITSIC_COMMON_H_
#define _HITSIC_COMMON_H_

#include "hitsic_def.h"
#include "hitsic_hal.h"




/**
 * @brief : 软件版本产生方式定义
 */
#define HITSIC_MAKE_VERSION(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))




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