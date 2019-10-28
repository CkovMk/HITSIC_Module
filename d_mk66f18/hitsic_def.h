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


#ifndef _HITSIC_DEF_H_
#define _HITSIC_DEF_H_
#include "hitsic_hal.h"


/**
 * @brief : 软件版本产生方式定义
 */
#define HITSIC_MAKE_VERSION(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))









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

















#define HITSIC_USE_APP_MENU          (1U)


#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

    /**
    * @brief : 菜单调试输出开关
    * 编译选项为release时无效。
    */
    #define HITSIC_MENU_DEBUG_ENABLE 1u

    /**
    * @brief : 菜单调试输出语句定义。
    * 编译选项为release时始终为空。
    */
    #if defined(DEBUG) && defined(MENU_DEBUG_ENABLE)
    #define HITSIC_MENU_DEBUG_PRINTF(...) (printf(__VA_ARGS__))
    #else
    #define HITSIC_MENU_DEBUG_PRINTF(...)
    #endif // ! DEBUG



    /** @brief : 根菜单最大容量 */
    #define HITSIC_MENU_ROOT_SIZE 35 //maxium size of Root Menu

    /** @brief : 菜单使用的五向按键初始化。每组数据前两个是GPIO和Pin，其余数据为0。 */
    #define HITSIC_MENU_BUTTON_5DIR_BSP_INIT \
	{                                    \
		{                                \
			GPIOA,                       \
			15,                          \
			0,                           \
			0,                           \
			0,                           \
		},                               \
			{                            \
				GPIOA,                   \
				15,                      \
				0,                       \
				0,                       \
				0,                       \
			},                           \
			{                            \
				GPIOA,                   \
				15,                      \
				0,                       \
				0,                       \
				0,                       \
			},                           \
			{                            \
				GPIOA,                   \
				15,                      \
				0,                       \
				0,                       \
				0,                       \
			},                           \
			{                            \
				GPIOA,                   \
				15,                      \
				0,                       \
				0,                       \
				0,                       \
			},                           \
	}
    /**
    * @brief : 菜单消息调用所使用的中断信息。
    * 可以使用任何当前工程中未使用的中断。注意中断号和中断服务函数
    * 必须对应。优先级不可过高。
    */
    #define HITSIC_MENU_SERVICE_IRQHandler (Reserved71_IRQHandler)
    #define HITSIC_MENU_SERVICE_IRQn (Reserved71_IRQn)
    #define HITSIC_MENU_SERVICE_IRQPrio (10u)

    /**
    * @brief 屏幕打印接口
    */
    #define HITSIC_MENU_DISPLAY_PRINT(row, col, str) (OLED_PrintStr_F6x8(row, col, (uint8_t *)str))
    #define HITSIC_MENU_DISPLAY_PRINTF(row, col, fmt, ...) (OLED_Printf(row, col, fmt, __VA_ARGS__))
    // #define HITSIC_MENU_DISPLAY_PRINTF(row,col,fmt,...)		\
    // 	{													\
    // 		char* buf = malloc(24*sizeof(char));			\
    // 		vsnprintf(buf,24,fmt,__VA_ARGS__);				\
    // 		HITSIC_MENU_DISPLAY_PRINT(row,col,buf);			\
    // 		free(buf);										\
    // 	}

#endif // ! HITSIC_USE_APP_MENU





    
    





#endif // ! _HITSIC_DEF_H_



