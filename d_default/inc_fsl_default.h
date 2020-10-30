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

#ifndef _INC_FSL_DEFAULT_H_
#define _INC_FSL_DEFAULT_H_

#if (!defined(CPU_MKV10Z128VLC7))
#error CPU incorrect !
#endif // ! CPU_MKV10Z128VLC7


#include "MKV10Z1287.h"


//#include "arm_math.h"
//#include "arm_nnfunctions.h"

//NXP fsl Library

#include "fsl_adc16.h"
#include "fsl_clock.h"
#include "fsl_cmp.h"
#include "fsl_common.h"
#include "fsl_crc.h"
#include "fsl_dac.h"
#include "fsl_dmamux.h"
#include "fsl_dspi.h"
#include "fsl_dspi_edma.h"
#include "fsl_edma.h"
#include "fsl_ewm.h"
#include "fsl_flash.h"
#include "fsl_ftfx_adapter.h"
#include "fsl_ftfx_cache.h"
#include "fsl_ftfx_controller.h"
#include "fsl_ftfx_features.h"
#include "fsl_ftfx_flash.h"
#include "fsl_ftfx_flexnvm.h"
#include "fsl_ftfx_utilities.h"
#include "fsl_ftm.h"
#include "fsl_gpio.h"
#include "fsl_i2c.h"
#include "fsl_i2c_edma.h"
#include "fsl_llwu.h"
#include "fsl_lptmr.h"
#include "fsl_pdb.h"
#include "fsl_pmc.h"
#include "fsl_port.h"
#include "fsl_rcm.h"
#include "fsl_sim.h"
#include "fsl_smc.h"
#include "fsl_uart.h"
#include "fsl_uart_edma.h"
#include "fsl_wdog.h"
#include "fsl_debug_console.h"


//BSP RTE

#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "peripherals.h"


//CMB
//#include "cm_backtrace.h"




#endif // ! _INC_FSL_DEFAULT_H_
