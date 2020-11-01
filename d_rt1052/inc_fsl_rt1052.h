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

#ifndef _INC_FSL_MK66F18_H_
#define _INC_FSL_MK66F18_H_

#ifndef CPU_MIMXRT1052DVL6B
#error CPU incorrect !
#endif // ! CPU_MIMXRT1052DVL6B


#include "MIMXRT1052.h"

#include "arm_math.h"
#include "arm_nnfunctions.h"

//fsl sdk
#include "fsl_adc_etc.h"
#include "fsl_adc.h"
#include "fsl_aipstz.h"
#include "fsl_aoi.h"
#include "fsl_bee.h"
#include "fsl_cache.h"
#include "fsl_clock.h"
#include "fsl_cmp.h"
#include "fsl_common.h"
#include "fsl_csi.h"
#include "fsl_dcdc.h"
#include "fsl_dcp.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_elcdif.h"
#include "fsl_enc.h"
#include "fsl_enet.h"
#include "fsl_ewm.h"
#include "fsl_flexcan.h"
#include "fsl_flexio_i2c_master.h"
#include "fsl_flexio_i2s_edma.h"
#include "fsl_flexio_i2s.h"
#include "fsl_flexio_spi_edma.h"
#include "fsl_flexio_spi.h"
#include "fsl_flexio_uart_edma.h"
#include "fsl_flexio_uart.h"
#include "fsl_flexio.h"
#include "fsl_flexram.h"
#include "fsl_flexspi.h"
#include "fsl_gpc.h"
#include "fsl_gpio.h"
#include "fsl_gpt.h"
#include "fsl_iomuxc.h"
#include "fsl_kpp.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_edma.h"
#include "fsl_lpspi.h"
#include "fsl_lpspi_edma.h"
#include "fsl_lpuart.h"
#include "fsl_lpuart_edma.h"
#include "fsl_pit.h"
#include "fsl_pmu.h"	//???
#include "fsl_pwm.h"
#include "fsl_pxp.h"
#include "fsl_qtmr.h"
#include "fsl_rtwdog.h"	//???
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_semc.h"
#include "fsl_snvs_hp.h"	//???
#include "fsl_snvs_lp.h"	//???
#include "fsl_spdif_edma.h"
#include "fsl_spdif.h"
#include "fsl_src.h"
#include "fsl_tempmon.h"	//???
#include "fsl_trng.h"
//#include "fsl_tsc.h"		//this file has BUG. include this file will lead to an error reports a missing "}" at the end of "main()".
#include "fsl_usdhc.h"
#include "fsl_wdog.h"
#include "fsl_xbara.h"
#include "fsl_xbarb.h"


//utilities
//#include "fsl_debug_console.h"


//BSP RTE

#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "peripherals.h"


//CMB
//#include "cm_backtrace.h"

/* SRE 压摆率选择 */
#define SRE_0_SLOW_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(0)
#define SRE_1_FAST_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(1)

/* 驱动能力配置，配置阻值的大小 */
#define DSE_0_OUTPUT_DRIVER_DISABLED        IOMUXC_SW_PAD_CTL_PAD_DSE(0)
/* R0 260 Ohm @ 3.3V, 150Ohm@1.8V, 240 Ohm for DDR */
#define DSE_1_R0_1                          IOMUXC_SW_PAD_CTL_PAD_DSE(1)
/* R0/2 */
#define DSE_2_R0_2                          IOMUXC_SW_PAD_CTL_PAD_DSE(2)
/* R0/3 */
#define DSE_3_R0_3                          IOMUXC_SW_PAD_CTL_PAD_DSE(3)
/* R0/4 */
#define DSE_4_R0_4                          IOMUXC_SW_PAD_CTL_PAD_DSE(4)
/* R0/5 */
#define DSE_5_R0_5                          IOMUXC_SW_PAD_CTL_PAD_DSE(5)
/* R0/6 */
#define DSE_6_R0_6                          IOMUXC_SW_PAD_CTL_PAD_DSE(6)
/* R0/7 */
#define DSE_7_R0_7                          IOMUXC_SW_PAD_CTL_PAD_DSE(7)

/* SPEED 带宽配置 */
#define SPEED_0_LOW_50MHz                   IOMUXC_SW_PAD_CTL_PAD_SPEED(0)
#define SPEED_1_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(1)
#define SPEED_2_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(2)
#define SPEED_3_MAX_200MHz                  IOMUXC_SW_PAD_CTL_PAD_SPEED(3)

/* ODE 是否使用开漏模式 */
#define ODE_0_OPEN_DRAIN_DISABLED           IOMUXC_SW_PAD_CTL_PAD_ODE(0)
#define ODE_1_OPEN_DRAIN_ENABLED            IOMUXC_SW_PAD_CTL_PAD_ODE(1)

/* PKE 是否使能保持器或上下拉功能 */
#define PKE_0_PULL_KEEPER_DISABLED          IOMUXC_SW_PAD_CTL_PAD_PKE(0)
#define PKE_1_PULL_KEEPER_ENABLED           IOMUXC_SW_PAD_CTL_PAD_PKE(1)

/* PUE 选择使用保持器还是上下拉 */
#define PUE_0_KEEPER_SELECTED               IOMUXC_SW_PAD_CTL_PAD_PUE(0)
#define PUE_1_PULL_SELECTED                 IOMUXC_SW_PAD_CTL_PAD_PUE(1)

/* PUS 上下拉配置 */
#define PUS_0_100K_OHM_PULL_DOWN            IOMUXC_SW_PAD_CTL_PAD_PUS(0)
#define PUS_1_47K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(1)
#define PUS_2_100K_OHM_PULL_UP              IOMUXC_SW_PAD_CTL_PAD_PUS(2)
#define PUS_3_22K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(3)

/* HYS 滞后功能 */
#define HYS_0_HYSTERESIS_DISABLED           IOMUXC_SW_PAD_CTL_PAD_HYS(0)
#define HYS_1_HYSTERESIS_ENABLED            IOMUXC_SW_PAD_CTL_PAD_HYS(1)






#endif // ! _INC_FSL_MK66F18_H_
