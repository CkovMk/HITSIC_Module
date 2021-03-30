#ifndef UTILITIES_SYS_LTC_H
#define UTILITIES_SYS_LTC_H

#include <inc_stdlib.h>
#include "hitsic_common.h"

#if (defined(HITSIC_USE_LTC) && (HITSIC_USE_LTC != 0U))

/**
 * @brief : 获取LifeTimeCounter当前计时数。
 *
 *  PIT是减法计数器，这里获得的数值已经过转换，为单调递增。
 *
 * @return {uint64_t} : 返回LTC距离启动经过的时钟数
 */
uint64_t LTC_GetTime(void);

/**
 * @brief : 获取LifeTimeCounter当前计时微秒数。
 *
 *  PIT是减法计数器，这里获得的数值已经过转换，为单调递增。
 *
 * @return {uint64_t} : 返回LTC距离启动经过的微秒数
 */
uint64_t LTC_GetTime_us(void);

/**
 * @brief : 获取LifeTimeCounter当前计时毫秒数。
 *
 *  PIT是减法计数器，这里获得的数值已经过转换，为单调递增。
 *
 * @return {uint64_t} : 返回LTC距离启动经过的毫秒数
 */
uint64_t LTC_GetTime_ms(void);

/**
 * @brief : 利用LifeTimeCounter进行时钟级别的阻塞延时。
 *
 * @param : {uint64_t} _t : 要延迟的时钟数
 */
void LTC_Delay(uint64_t _t);

/**
 * @brief : 利用LifeTimeCounter进行微秒级别的阻塞延时。
 *
 * @param : {uint64_t} _t : 要延迟的微秒数
 */
void LTC_Delay_us(uint64_t _t);

/**
 * @brief : 利用LifeTimeCounter进行毫秒级别的阻塞延时。
 *
 * @param : {uint64_t} _t : 要延迟的毫秒数
 */
void LTC_Delay_ms(uint64_t _t);

#endif // ! HITSIC_USE_LTC

#endif // ! UTILITIES_SYS_LTC_H
