/**
 * Copyright 2018 - 2021 HITSIC
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
#ifndef D_STM32F4XX_HITSIC_COMMON_H
#define D_STM32F4XX_HITSIC_COMMON_H

#include "inc_hal_stm32f4xx.h"
#include "assert.h"
#include "stdint.h"

/**
 * @brief : 软件版本产生方式定义
 */
#define HITSIC_MAKE_VERSION(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))
#define HITSIC_VERSION_MAJOR(x) (x >> 16U)
#define HITSIC_VERSION_MINOR(x) ((x >> 8U) & 0xffU)
#define HITSIC_VERSION_PATCH(x) (x & 0xffU)


/** DRI */

// #ifndef HITSIC_USE_FTFX_FLASH
// #define HITSIC_USE_FTFX_FLASH (0U)
// #endif // ! HITSIC_USE_FTFX_FLASH

// #ifndef HITSIC_USE_DISP_SSD1306
// #define HITSIC_USE_DISP_SSD1306 (0U)
// #endif // ! HITSIC_USE_DISP_SSD1306

// #ifndef HITSIC_USE_DISP_SSD1327
// #define HITSIC_USE_DISP_SSD1327 (0U)
// #endif // ! HITSIC_USE_DISP_SSD1327

// #ifndef HITSIC_USE_DISP_ST7789
// #define HITSIC_USE_DISP_ST7789 (0U)
// #endif // ! HITSIC_USE_DISP_ST7789

// #ifndef HITSIC_USE_DRV_BUTTON
// #define HITSIC_USE_DRV_BUTTON (0U)
// #endif // ! HITSIC_USE_DRV_BUTTON

// #ifndef HITSIC_USE_DMADVP
// #define HITSIC_USE_DMADVP (0U)
// #endif // ! HITSIC_USE_DMADVP

// #ifndef HITSIC_USE_CAM_ZF9V034
// #define HITSIC_USE_CAM_ZF9V034 (0U)
// #endif // ! HITSIC_USE_CAM_ZF9V034

/** SYS */

// #ifndef HITSIC_USE_LTC
// #define HITSIC_USE_LTC       (0U)
// #endif // ! HITSIC_USE_LTC

#ifndef HITSIC_USE_PITMGR
#define HITSIC_USE_PITMGR 		(1U)
#endif // ! HITSIC_USE_PITMGR

// #ifndef HITSIC_USE_EXTINT
// #define HITSIC_USE_EXTINT 		(0U)
// #endif // ! HITSIC_USE_EXTINT

#ifndef HITSIC_USE_RMCALL
#define HITSIC_USE_RMCALL 		(1U)
#endif // ! HITSIC_USE_RMCALL

/** APP */

// #ifndef HITSIC_USE_APP_SVBMP
// #define HITSIC_USE_APP_SVBMP (1U)
// #endif // ! HITSIC_USE_APP_SVBMP





// typedef I2C_Type HAL_I2C_Type;
// typedef SPI_Type HAL_SPI_Type;
// typedef UART_Type HAL_UART_Type;

/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))

/*! @brief Construct the version number for drivers. */
#define MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))

/*! @brief Status group numbers. */
enum _status_groups
{
    kStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
    kStatusGroup_FLASH = 1,                   /*!< Group number for FLASH status codes. */
    kStatusGroup_LPSPI = 4,                   /*!< Group number for LPSPI status codes. */
    kStatusGroup_FLEXIO_SPI = 5,              /*!< Group number for FLEXIO SPI status codes. */
    kStatusGroup_DSPI = 6,                    /*!< Group number for DSPI status codes. */
    kStatusGroup_FLEXIO_UART = 7,             /*!< Group number for FLEXIO UART status codes. */
    kStatusGroup_FLEXIO_I2C = 8,              /*!< Group number for FLEXIO I2C status codes. */
    kStatusGroup_LPI2C = 9,                   /*!< Group number for LPI2C status codes. */
    kStatusGroup_UART = 10,                   /*!< Group number for UART status codes. */
    kStatusGroup_I2C = 11,                    /*!< Group number for UART status codes. */
    kStatusGroup_LPSCI = 12,                  /*!< Group number for LPSCI status codes. */
    kStatusGroup_LPUART = 13,                 /*!< Group number for LPUART status codes. */
    kStatusGroup_SPI = 14,                    /*!< Group number for SPI status code.*/
    kStatusGroup_XRDC = 15,                   /*!< Group number for XRDC status code.*/
    kStatusGroup_SEMA42 = 16,                 /*!< Group number for SEMA42 status code.*/
    kStatusGroup_SDHC = 17,                   /*!< Group number for SDHC status code */
    kStatusGroup_SDMMC = 18,                  /*!< Group number for SDMMC status code */
    kStatusGroup_SAI = 19,                    /*!< Group number for SAI status code */
    kStatusGroup_MCG = 20,                    /*!< Group number for MCG status codes. */
    kStatusGroup_SCG = 21,                    /*!< Group number for SCG status codes. */
    kStatusGroup_SDSPI = 22,                  /*!< Group number for SDSPI status codes. */
    kStatusGroup_FLEXIO_I2S = 23,             /*!< Group number for FLEXIO I2S status codes */
    kStatusGroup_FLEXIO_MCULCD = 24,          /*!< Group number for FLEXIO LCD status codes */
    kStatusGroup_FLASHIAP = 25,               /*!< Group number for FLASHIAP status codes */
    kStatusGroup_FLEXCOMM_I2C = 26,           /*!< Group number for FLEXCOMM I2C status codes */
    kStatusGroup_I2S = 27,                    /*!< Group number for I2S status codes */
    kStatusGroup_IUART = 28,                  /*!< Group number for IUART status codes */
    kStatusGroup_CSI = 29,                    /*!< Group number for CSI status codes */
    kStatusGroup_MIPI_DSI = 30,               /*!< Group number for MIPI DSI status codes */
    kStatusGroup_SDRAMC = 35,                 /*!< Group number for SDRAMC status codes. */
    kStatusGroup_POWER = 39,                  /*!< Group number for POWER status codes. */
    kStatusGroup_ENET = 40,                   /*!< Group number for ENET status codes. */
    kStatusGroup_PHY = 41,                    /*!< Group number for PHY status codes. */
    kStatusGroup_TRGMUX = 42,                 /*!< Group number for TRGMUX status codes. */
    kStatusGroup_SMARTCARD = 43,              /*!< Group number for SMARTCARD status codes. */
    kStatusGroup_LMEM = 44,                   /*!< Group number for LMEM status codes. */
    kStatusGroup_QSPI = 45,                   /*!< Group number for QSPI status codes. */
    kStatusGroup_DMA = 50,                    /*!< Group number for DMA status codes. */
    kStatusGroup_EDMA = 51,                   /*!< Group number for EDMA status codes. */
    kStatusGroup_DMAMGR = 52,                 /*!< Group number for DMAMGR status codes. */
    kStatusGroup_FLEXCAN = 53,                /*!< Group number for FlexCAN status codes. */
    kStatusGroup_LTC = 54,                    /*!< Group number for LTC status codes. */
    kStatusGroup_FLEXIO_CAMERA = 55,          /*!< Group number for FLEXIO CAMERA status codes. */
    kStatusGroup_LPC_SPI = 56,                /*!< Group number for LPC_SPI status codes. */
    kStatusGroup_LPC_USART = 57,              /*!< Group number for LPC_USART status codes. */
    kStatusGroup_DMIC = 58,                   /*!< Group number for DMIC status codes. */
    kStatusGroup_SDIF = 59,                   /*!< Group number for SDIF status codes.*/
    kStatusGroup_SPIFI = 60,                  /*!< Group number for SPIFI status codes. */
    kStatusGroup_OTP = 61,                    /*!< Group number for OTP status codes. */
    kStatusGroup_MCAN = 62,                   /*!< Group number for MCAN status codes. */
    kStatusGroup_CAAM = 63,                   /*!< Group number for CAAM status codes. */
    kStatusGroup_ECSPI = 64,                  /*!< Group number for ECSPI status codes. */
    kStatusGroup_USDHC = 65,                  /*!< Group number for USDHC status codes.*/
    kStatusGroup_LPC_I2C = 66,                /*!< Group number for LPC_I2C status codes.*/
    kStatusGroup_DCP = 67,                    /*!< Group number for DCP status codes.*/
    kStatusGroup_MSCAN = 68,                  /*!< Group number for MSCAN status codes.*/
    kStatusGroup_ESAI = 69,                   /*!< Group number for ESAI status codes. */
    kStatusGroup_FLEXSPI = 70,                /*!< Group number for FLEXSPI status codes. */
    kStatusGroup_MMDC = 71,                   /*!< Group number for MMDC status codes. */
    kStatusGroup_PDM = 72,                    /*!< Group number for MIC status codes. */
    kStatusGroup_SDMA = 73,                   /*!< Group number for SDMA status codes. */
    kStatusGroup_ICS = 74,                    /*!< Group number for ICS status codes. */
    kStatusGroup_SPDIF = 75,                  /*!< Group number for SPDIF status codes. */
    kStatusGroup_LPC_MINISPI = 76,            /*!< Group number for LPC_MINISPI status codes. */
    kStatusGroup_HASHCRYPT = 77,              /*!< Group number for Hashcrypt status codes */
    kStatusGroup_LPC_SPI_SSP = 78,            /*!< Group number for LPC_SPI_SSP status codes. */
    kStatusGroup_I3C = 79,                    /*!< Group number for I3C status codes */
    kStatusGroup_LPC_I2C_1 = 97,              /*!< Group number for LPC_I2C_1 status codes. */
    kStatusGroup_NOTIFIER = 98,               /*!< Group number for NOTIFIER status codes. */
    kStatusGroup_DebugConsole = 99,           /*!< Group number for debug console status codes. */
    kStatusGroup_SEMC = 100,                  /*!< Group number for SEMC status codes. */
    kStatusGroup_ApplicationRangeStart = 101, /*!< Starting number for application groups. */
    kStatusGroup_IAP = 102,                   /*!< Group number for IAP status codes */
    kStatusGroup_SFA = 103,                   /*!< Group number for SFA status codes*/
    kStatusGroup_SPC = 104,                   /*!< Group number for SPC status codes. */
    kStatusGroup_PUF = 105,                   /*!< Group number for PUF status codes. */
    kStatusGroup_TOUCH_PANEL = 106,           /*!< Group number for touch panel status codes */

    kStatusGroup_HAL_GPIO = 121,              /*!< Group number for HAL GPIO status codes. */
    kStatusGroup_HAL_UART = 122,              /*!< Group number for HAL UART status codes. */
    kStatusGroup_HAL_TIMER = 123,             /*!< Group number for HAL TIMER status codes. */
    kStatusGroup_HAL_SPI = 124,               /*!< Group number for HAL SPI status codes. */
    kStatusGroup_HAL_I2C = 125,               /*!< Group number for HAL I2C status codes. */
    kStatusGroup_HAL_FLASH = 126,             /*!< Group number for HAL FLASH status codes. */
    kStatusGroup_HAL_PWM = 127,               /*!< Group number for HAL PWM status codes. */
    kStatusGroup_HAL_RNG = 128,               /*!< Group number for HAL RNG status codes. */
    kStatusGroup_TIMERMANAGER = 135,          /*!< Group number for TiMER MANAGER status codes. */
    kStatusGroup_SERIALMANAGER = 136,         /*!< Group number for SERIAL MANAGER status codes. */
    kStatusGroup_LED = 137,                   /*!< Group number for LED status codes. */
    kStatusGroup_BUTTON = 138,                /*!< Group number for BUTTON status codes. */
    kStatusGroup_EXTERN_EEPROM = 139,         /*!< Group number for EXTERN EEPROM status codes. */
    kStatusGroup_SHELL = 140,                 /*!< Group number for SHELL status codes. */
    kStatusGroup_MEM_MANAGER = 141,           /*!< Group number for MEM MANAGER status codes. */
    kStatusGroup_LIST = 142,                  /*!< Group number for List status codes. */
    kStatusGroup_OSA = 143,                   /*!< Group number for OSA status codes. */
    kStatusGroup_COMMON_TASK = 144,           /*!< Group number for Common task status codes. */
    kStatusGroup_MSG = 145,                   /*!< Group number for messaging status codes. */
    kStatusGroup_SDK_OCOTP = 146,             /*!< Group number for OCOTP status codes. */
    kStatusGroup_SDK_FLEXSPINOR = 147,        /*!< Group number for FLEXSPINOR status codes.*/
    kStatusGroup_CODEC = 148,                 /*!< Group number for codec status codes. */
    kStatusGroup_ASRC = 149,                  /*!< Group number for codec status ASRC. */
    kStatusGroup_OTFAD = 150,                 /*!< Group number for codec status codes. */
    kStatusGroup_SDIOSLV = 151,               /*!< Group number for SDIOSLV status codes. */
    kStatusGroup_MECC = 152,                  /*!< Group number for MECC status codes. */
    kStatusGroup_ENET_QOS = 153,              /*!< Group number for ENET_QOS status codes. */
    kStatusGroup_LOG = 154,                   /*!< Group number for LOG status codes. */
};

/*! \public
 * @brief Generic status return codes.
 */
enum
{
    kStatus_Success = MAKE_STATUS(kStatusGroup_Generic, 0),  /*!< Generic status for Success. */
    kStatus_Fail = MAKE_STATUS(kStatusGroup_Generic, 1),      /*!< Generic status for Fail. */
    kStatus_ReadOnly = MAKE_STATUS(kStatusGroup_Generic, 2),    /*!< Generic status for read only failure. */
    kStatus_OutOfRange = MAKE_STATUS(kStatusGroup_Generic, 3),   /*!< Generic status for out of range access. */
    kStatus_InvalidArgument = MAKE_STATUS(kStatusGroup_Generic, 4),   /*!< Generic status for invalid argument check. */
    kStatus_Timeout = MAKE_STATUS(kStatusGroup_Generic, 5),   /*!< Generic status for timeout. */
    kStatus_NoTransferInProgress = MAKE_STATUS(kStatusGroup_Generic, 6),   /*!< Generic status for no transfer in progress. */
};

/*! @brief Type used for all status and error return values. */
typedef int32_t status_t;

/*! @name Timer utilities */
/* @{ */
/*! Macro to convert a microsecond period to raw count value */
#define USEC_TO_COUNT(us, clockFreqInHz) (uint64_t)(((uint64_t)(us) * (clockFreqInHz)) / 1000000U)
/*! Macro to convert a raw count value to microsecond */
#define COUNT_TO_USEC(count, clockFreqInHz) (uint64_t)((uint64_t)(count) * 1000000U / (clockFreqInHz))

/*! Macro to convert a millisecond period to raw count value */
#define MSEC_TO_COUNT(ms, clockFreqInHz) (uint64_t)((uint64_t)(ms) * (clockFreqInHz) / 1000U)
/*! Macro to convert a raw count value to millisecond */
#define COUNT_TO_MSEC(count, clockFreqInHz) (uint64_t)((uint64_t)(count) * 1000U / (clockFreqInHz))
/* @} */


//#define assert(test) ((test) ? (void) 0 : abort())

#ifdef __cplusplus
extern "C"{
#endif

extern uint32_t hal_criticalCnt;
extern uint32_t hal_regPrimask;

static  inline void HAL_EnterCritical(void)
{
    if(0u == hal_criticalCnt++)
    {
        hal_regPrimask = __get_PRIMASK();
        __disable_irq();
    }
}

static inline void HAL_ExitCritical(void)
{
    assert(hal_criticalCnt);
    if(--hal_criticalCnt == 0u)
    {
        __set_PRIMASK(hal_regPrimask);
    }
}

    
#ifdef __cplusplus
}
#endif

#endif // ! D_STM32F4XX_HITSIC_COMMON_H
