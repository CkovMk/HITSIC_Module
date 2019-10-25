#ifndef _HITSIC_HAL_H_
#define _HITSIC_HAL_H_

#ifndef CPU_MK66FX1M0VLQ18
#error CPU incorrect !
#endif // ! CPU_MK66FX1M0VLQ18

#include "inc_fsl_mk66f18.h"
#include <stdint.h>

typedef I2C_Type HAL_I2C_Type;
typedef SPI_Type HAL_SPI_Type;
typedef UART_Type HAL_UART_Type;

#ifdef __cplusplus
extern "C"{
#endif

status_t HAL_I2C_Mem_Read(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize);
status_t HAL_I2C_Mem_Write(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize);


#ifdef __cplusplus
}
#endif

#endif // ! _HITSIC_HAL_H_