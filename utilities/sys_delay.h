#pragma once
#ifndef _SYS_DELAY_H
#define _SYS_DELAY_H

#include "board.h"
#include "fsl_ftm.h"
#include "fsl_pit.h"
#include "clock_config.h"
#include "pin_mux.h"

#if defined(HITSIC_USE_SYS_DELAY) && (HITSIC_USE_SYS_DELAY>0)

#ifdef __cplusplus
extern "C" {
#endif
#if defined(HITSIC_SYS_DELAY_FREE) && (HITSIC_SYS_DELAY_FREE > 0)
/**
 * @brief	Systick延时函数
 *
 * @param  time : 需要延时的时钟周期数 
 * Sample usage : SysTick_Delay(1000);  延时1000个内核时钟周期 
 */
void SysTick_Delay(uint32_t time);
/**
 * @brief	毫秒级Systick延时函数
 *
 * @param  time : 需要延时的时间
 * Sample usage : SysTick_Delay_Ms(1000);  延时1000毫秒
 */
void SysTick_Delay_Ms(uint32_t ms);
/**
 * @brief	Systick定时器
 *
 * @param  time : 定时时间
 * Sample usage : SysTick_Timing(1000);  定时1000个内核时钟周期
 */
void SysTick_Timing(uint32_t time);
/**
 * @brief	获取当前Systick Timer值
 * @return  当前Systick Timer值
 *
 * Sample usage : uint32_t SysTick_time = SysTick_Getval(); 获取当前Systick Timer值  
 */
uin32_t SysTick_Getval(void);
#endif


/**
 * @brief	PIT时钟初始化
 * @return
 *
 * Sample usage :
 */
void Sys_Pit_My_Init();

/**
 * @brief	设置PIT中断时间
 * @return
 *
 * Sample usage :
 */
void Sys_Pit_Set_Period(PIT_Type* base, pit_chnl_t channel, uint32_t count);

/**
 * @brief	开始计时
 * @return
 *
 * Sample usage :
 */
void Sys_Pit_Start_Timer(PIT_Type *base, pit_chnl_t channel);

/**
 * @brief	暂停计时
 * @return
 *
 * Sample usage :
 */
void Sys_Pit_Stop_Timer(PIT_Type* base, pit_chnl_t channel);

/**
 * @brief	获得标志位
 * @return
 *
 * Sample usage :
 */
void Sys_Pit_Flag_Get(PIT_Type* base, pit_chnl_t channel);
#endif


