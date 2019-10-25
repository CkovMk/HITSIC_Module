#ifndef _HITSIC_DEF_H_
#define _HITSIC_DEF_H_
#include "hitsic_hal.h"

#ifndef CPU_MK66FX1M0VLQ18
#error CPU incorrect !
#endif // ! CPU_MK66FX1M0VLQ18


 
    
    

#define HITSIC_USE_SYS_PITMGR       (1U)

#if defined(HITSIC_USE_SYS_PITMGR) && (HITSIC_USE_SYS_PITMGR > 0)

#define HITSIC_PITMGR_INITLIZE       (0U)
#define HTISIC_PITMGR_USE_IRQHANDLER (1U)

#endif // HITSIC_USE_SYS_PITMGR





#define HITSIC_USE_SYS_EXTMGR       (1U)
#if defined(HITSIC_USE_SYS_EXTMGR) && (HITSIC_USE_SYS_EXTMGR > 0)

#define HITSIC_EXTMGR_INITLIZE       (0U)
#define HTISIC_EXTMGR_USE_IRQHANDLER (1U)

#endif // HITSIC_USE_SYS_EXTMGR






#define HITSIC_USE_SYS_UARTMGR      (1U)
#if defined(HITSIC_USE_SYS_UARTMGR) && (HITSIC_USE_SYS_UARTMGR > 0)

#endif // HITSIC_USE_SYS_UARTMGR







#define HITSIC_USE_DRV_IMU          (1U)

#if defined(HITSIC_USE_DRV_IMU) && (HITSIC_USE_DRV_IMU > 0)

#define HITSIC_IMU_SPI              (0U)

#define HITSIC_IMU_SPI_INST         (SPI1)
#define HITSIC_IMU_SPI_PCSn         (0)
#define HITSIC_IMU_SPI_CTARn        (0)

#define HITSIC_IMU_I2C              (1U)

#define HITSIC_IMU_I2C_INST         (I2C1)


#endif // HITSIC_USE_DRV_IMU






    
    





#endif // ! _HITSIC_DEF_H_



