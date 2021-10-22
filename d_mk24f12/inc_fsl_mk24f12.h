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

#ifndef INC_FSL_MK24F12_H
#define INC_FSL_MK24F12_H

#include "MK24F12.h"

#include "arm_math.h"

//NXP fsl Library

#include "fsl_adc16.h"
#include "fsl_clock.h"
#include "fsl_cmp.h"
#include "fsl_cmt.h"
#include "fsl_common.h"
#include "fsl_crc.h"
#include "fsl_dac.h"
#include "fsl_dmamux.h"
#include "fsl_dspi.h"
#include "fsl_dspi_edma.h"
#include "fsl_edma.h"
#include "fsl_ewm.h"
#include "fsl_flash.h"
#include "fsl_flexbus.h"
#include "fsl_flexcan.h"
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
#include "fsl_pit.h"
#include "fsl_pmc.h"
#include "fsl_port.h"
#include "fsl_rcm.h"
#include "fsl_rnga.h"
#include "fsl_rtc.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_sim.h"
#include "fsl_smc.h"
#include "fsl_sysmpu.h"
#include "fsl_uart.h"
#include "fsl_uart_edma.h"
#include "fsl_vref.h"
#include "fsl_wdog.h"


//BSP RTE

#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "peripherals.h"


//CMB
//#include "cm_backtrace.h"




#endif // ! INC_FSL_MK24F12_H
