/**
 * Copyright 2018 - 2020 HITSIC
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
/**
 * @file 	:	"drv_button.h"; "drv_button.c"
 * @author  :	CkovMk/马奇科(qq:905497173)
 * @version :	v0.1.1          2020.08.02
 * @date 	:	v0.1.0-beta.1   2019.10.28
 * @date    :   v0.1.1          2019.10.29
 * 
 * @note    :   依赖库：sys_extint.h、sys_pitmgr.h
                依赖库必须先初始化。
 */

#ifndef UTILITIES_DRV_BUTTON_HPP
#define UTILITIES_DRV_BUTTON_HPP
#include "inc_stdlib.hpp"
#include "hitsic_common.h"

#if defined(HITSIC_USE_DRV_BUTTON) && (HITSIC_USE_DRV_BUTTON > 0)
#include "sys_extint.hpp"
#include "sys_pitmgr.hpp"
#include "drv_button_port.h"

/*!
 * @addtogroup button
 * @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/** @brief : 软件版本 */
#define DRV_BUTTON_VERSION (HITSIC_MAKE_VERSION(0u, 1u, 1u))

    typedef enum 
    {
        BUTTON_STAT_NONE = 0, ///< button no operation
        BUTTON_SHRT_PRES = 1, ///< button short press
        BUTTON_SHRT_CLER = 2, ///< service responded short press
        BUTTON_LONG_PRES = 3, ///< button long press
        BUTTON_LONG_CLER = 4, ///< service responded long press
        BUTTON_LRPT_PRES = 5, ///< button long_repeat press
        BUTTON_LRPT_CLER = 6, ///< service responded long_repeat press
    }button_status_t;

    typedef void(*button_handler_t)(void *_inst);   // (button_t *inst)

    typedef struct 
    {
        GPIO_Type *gpio;
        //PORT_Type* port;
        uint32_t pin;
        extInt_interruptMode_t intCfg;
        uint64_t msCnt;
        button_status_t status;
        button_handler_t handler;
    }button_t;


    void BUTTON_Setup(button_t *_inst, GPIO_Type *_gpio, uint32_t _pin);

    button_t *BUTTON_Construct(GPIO_Type *_gpio, uint32_t _pin);

    void BUTTON_InstallHandler(button_t *_inst, button_handler_t _handler);

    void BUTTON_UninstallHandler(button_t *_inst);

    void BUTTON_SetInterrupt(button_t *_inst, port_interrupt_t _int);

    uint32_t BUTTON_ReadPin(button_t *_inst);

    void BUTTON_ExtIsr(button_t *_inst);

    void BUTTON_PitIsr(button_t *_inst);




#ifdef __cplusplus
}
#endif

/* @} */

#endif // ! HITSIC_USE_DRV_BUTTON

#endif // ! UTILITIES_DRV_BUTTON_HPP
