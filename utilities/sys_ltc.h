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
#ifndef UTILITIES_SYS_LTC_H
#define UTILITIES_SYS_LTC_H
#include <hitsic_common.h>

#if (defined(HITSIC_USE_LTC) && (HITSIC_USE_LTC != 0U))
#include <sys_ltc_port.h>


/** @brief : 软件版本 */
#define SYS_LTC_VERSION (HITSIC_MAKE_VERSION(1U, 0U, 0U))

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
