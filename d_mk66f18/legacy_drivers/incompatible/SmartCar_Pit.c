/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		PIT计时
 * @company	        哈工大智能车创新俱乐部
 * @author     		王学嘉 qq512924543  吴文华qq1280390945 肖日涛1761690868
 * @version    		v1.1
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 * @date       		v1.1 2019-10-10 fix bug
 *
 * @note：
        中断服务函数里一定要加上PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);清除中断标志位
    哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_Pit.h"

/**********************************************************************************************************************
*  @brief      pit初始化
*  @return     void
*  @since      v1.1
*  Sample usage:          Pit_Init();
**********************************************************************************************************************/
void Pit_Init(void)
{
	pit_config_t pit_config;
	pit_config.enableRunInDebug = 1;
	PIT_Init(PIT, &pit_config);
}

/**********************************************************************************************************************
*  @brief      pit中断初始化
*  @param      channel            pit通道
*  @param      count              pit计时初始化，单位ms
*  @return     void
*  @since      v1.1
*  Sample usage:          Pit_Interrupt_Init(kPIT_Chnl_0,500U);
**********************************************************************************************************************/
void Pit_Interrupt_Init(pit_chnl_t channel, uint32_t msec)
{
    uint32_t PIT_SOURCE_CLOCK = CLOCK_GetFreq(kCLOCK_BusClk);
    uint32_t my_count = MSEC_TO_COUNT(msec, PIT_SOURCE_CLOCK);
    PIT_SetTimerPeriod(PIT, channel, my_count);
	PIT_EnableInterrupts(PIT, channel, kPIT_TimerInterruptEnable);
    PIT_StartTimer(PIT,channel);
}

/**********************************************************************************************************************
*  @brief      pit更变定时
*  @param      channel            pit通道
*  @param      count              pit计时初始化，单位ms
*  @return     void
*  @since      v1.1
*  Sample usage:          Pit_ChangeTime(kPIT_Chnl_0,500U)
**********************************************************************************************************************/
void Pit_ChangeTime(pit_chnl_t channel, uint32_t msec)
{
    PIT_StopTimer(PIT, channel);//清空计时
    uint32_t PIT_SOURCE_CLOCK = CLOCK_GetFreq(kCLOCK_BusClk);
    uint32_t my_count = MSEC_TO_COUNT(msec, PIT_SOURCE_CLOCK);
    PIT_SetTimerPeriod(PIT, channel, my_count);
}

/**********************************************************************************************************************
*  @brief      pit开始计时
*  @param      channel      pit通道
*  @return     void
*  @since      v1.1
*  Sample usage:          Time_Start(kPIT_Chnl_0);
**********************************************************************************************************************/
void Time_Start(pit_chnl_t channel)
{
    PIT->CHANNEL[channel].LDVAL = ~0;//将计数调为最大
    PIT_StopTimer(PIT, channel);//清空计时
    PIT_StartTimer(PIT, channel);//开始计时
}


/**********************************************************************************************************************
*  @brief      pit获得计时时间
*  @param      channel      pit通道
*  @return     uint32_t       计时时间，单位ms
*  @since      v1.1
*  Sample usage:         Get_Time(kPIT_Chnl_0);
**********************************************************************************************************************/
uint32_t Get_Time(pit_chnl_t channel)
{
    uint32_t PIT_SOURCE_CLOCK = CLOCK_GetFreq(kCLOCK_BusClk);
    uint32_t mytime =((uint32_t)(~0) - PIT_GetCurrentTimerCount(PIT,channel));
	uint32_t get_yime = COUNT_TO_MSEC(mytime, PIT_SOURCE_CLOCK);
    return get_yime;
}


/**********************************************************************************************************************
*  @brief      pit获得计时时间
*  @param      channel      pit通道
*  @return     uint32_t       计时时间，单位us
*  @since      v1.1
*  Sample usage:         Get_Time_us(kPIT_Chnl_0);
**********************************************************************************************************************/

uint32_t Get_Time_us(pit_chnl_t channel)
{
	uint32_t PIT_SOURCE_CLOCK = CLOCK_GetFreq(kCLOCK_BusClk);
	uint32_t mytime = ((uint32_t)(~0) - PIT_GetCurrentTimerCount(PIT, channel));
	uint32_t get_yime = COUNT_TO_USEC(mytime, PIT_SOURCE_CLOCK);
	return get_yime;
}
