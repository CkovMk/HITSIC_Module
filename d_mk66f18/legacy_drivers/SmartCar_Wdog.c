/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		看门狗
 * @company	        哈工大智能车创新俱乐部
 * @author     		田竺轩573763581		吴文华 qq1280390945  
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
		看门狗初始化以后，在定时中断内按照设定的时间定时喂狗，用于检测单片机卡死（单片机卡死会自动复位）
		
		哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_Wdog.h"

#ifdef __cplusplus
extern "C" {
#endif

static void WaitWctClose()
{
    /* Accessing register by bus clock    通过总线时钟访问寄存器*/
    for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
    {
        (void)WDOG->RSTCNT;
    }
}
/**********************************************************************************************************************
*  @brief      看门狗初始化
*  @param      ms 		喂狗间隔
*  @return     void
*  @since      v1.0
*  Sample usage:         Wdog_Init(100);
**********************************************************************************************************************/
void Wdog_Init(uint32_t ms)
{
    wdog_config_t config;            //描述WDOG配置结构。
    /*If not wdog reset   如果不是WDOG重置*/
    if (!(RCM_GetPreviousResetSources(RCM) & kRCM_SourceWdog))
    {
        WDOG_ClearResetCount(WDOG);  //此函数清除wdog重置计数值。
    }
    WDOG_GetDefaultConfig(&config);  //初始化WDOG配置结构
    config.timeoutValue = ms;
    WDOG_Init(WDOG, &config);
    WaitWctClose();
}

/**********************************************************************************************************************
*  @brief      喂狗函数
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:         Wdog_Feed();
**********************************************************************************************************************/
void Wdog_Feed(void)
{
  WDOG_Refresh(WDOG);
}


#ifdef __cplusplus
}
#endif

