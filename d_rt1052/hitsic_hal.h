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

#ifndef _HITSIC_HAL_H_
#define _HITSIC_HAL_H_

#ifndef CPU_MK66FX1M0VLQ18
#error CPU incorrect !
#endif // ! CPU_MK66FX1M0VLQ18

#include "inc_fsl_rt1052.h"
#include <stdint.h>

typedef LPI2C_Type HAL_I2C_Type;
typedef LPSPI_Type HAL_SPI_Type;
typedef LPUART_Type HAL_UART_Type;

#ifdef __cplusplus
extern "C"{
#endif

status_t HAL_I2C_Mem_Read(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize);
status_t HAL_I2C_Mem_Write(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize);


#ifdef __cplusplus
}
#endif

#endif // ! _HITSIC_HAL_H_
