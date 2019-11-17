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

#ifndef CPU_MIMXRT1052DVL6B
#error CPU incorrect !
#endif // ! CPU_MIMXRT1052DVL6B

#define HITSIC_USE_SYS_PITMGR (1U)

#if defined(HITSIC_USE_SYS_PITMGR) && (HITSIC_USE_SYS_PITMGR > 0)

#define HITSIC_PITMGR_INITLIZE (0U)
#define HTISIC_PITMGR_USE_IRQHANDLER (1U)

#endif // HITSIC_USE_SYS_PITMGR

#define HITSIC_USE_SYS_EXTMGR (1U)

#if defined(HITSIC_USE_SYS_EXTMGR) && (HITSIC_USE_SYS_EXTMGR > 0)

#define HITSIC_EXTMGR_INITLIZE (0U)
#define HTISIC_EXTMGR_USE_IRQHANDLER (1U)

#endif // HITSIC_USE_SYS_EXTMGR

#define HITSIC_USE_SYS_UARTMGR (1U)
#if defined(HITSIC_USE_SYS_UARTMGR) && (HITSIC_USE_SYS_UARTMGR > 0)

#endif // HITSIC_USE_SYS_UARTMGR





#endif // ! _HITSIC_DEF_H_
