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
/**
 * @file 	:	lib_pidctrl.h
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1-beta.0
 *
 * @date 	:	v0.1-beta.0 2019.10.18
 *
 * @brief   :   PID控制算法
 */

#ifndef UTILITIES_LIB_PIDCTRL_H
#define UTILITIES_LIB_PIDCTRL_H

#include <inc_stdlib.h>
#include "hitsic_common.h"

/*!
 * @addtogroup pidctrl
 * @{
 */

/** @brief : 软件版本 */
#define LIB_PIDCTRL_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))

//#define assert(...)

struct pidCtrl_t
{
    float kp, ki, kd;
    float errCurr, errIntg, errDiff, errPrev;
};

struct pid2Ctrl_t
{
    pidCtrl_t outer, inner;
};

/**
 * @brief 设置PID结构体参数
 *
 * @param _pid  要设置的PID参数结构体指针
 * @param _kp  比例系数
 * @param _ki  积分系数
 * @param _kd  微分系数
 */
void PIDCTRL_Setup(pidCtrl_t *_pid, float _kp, float _ki, float _kd);

/**
 * @brief 构造并返回一个PID结构体指针
 *
 * @param _kp  比例系数
 * @param _ki  积分系数
 * @param _kd  微分系数
 * @return pidCtrl_t*  构造完成的PID参数结构体指针
 */
pidCtrl_t* PIDCTRL_Construct(float _kp, float _ki, float _kd);

/**
 * @brief 更新PID偏差，已知偏差项
 *
 * @param _pid  PID参数结构体指针
 * @param _err  偏差
 */
void PIDCTRL_ErrUpdate(pidCtrl_t *_pid, float _err);

/**
 * @brief 更新PID偏差，已知微分项
 *
 * @param _pid  PID参数结构体指针
 * @param _diff  偏差的微分项
 */
void PIDCTRL_ErrUpdateByDiff(pidCtrl_t *_pid, float _diff);

/**
 * @brief 更新PID偏差，已知偏差项和微分项
 *
 * @param _pid  PID参数结构体指针
 * @param _err  偏差
 * @param _diff  偏差的微分项
 */
void PIDCTRL_ErrUpdateByErrAndDiff(pidCtrl_t *_pid, float _err, float _diff);

/**
 * @brief 计算比例输出
 *
 * @param _pid  PID参数结构体指针
 * @return float 比例结果输出
 */
float PIDCTRL_CalcPGain(pidCtrl_t *_pid);

/**
 * @brief 计算积分输出
 *
 * @param _pid  PID参数结构体指针
 * @return float 积分结果输出
 */
float PIDCTRL_CalcIGain(pidCtrl_t *_pid);

/**
 * @brief 计算微分输出
 *
 * @param _pid  PID参数结构体指针
 * @return float 微分结果输出
 */
float PIDCTRL_CalcDGain(pidCtrl_t *_pid);

/**
 * @brief 计算PID总输出
 *
 * @param _pid  PID参数结构体指针
 * @return float PID结果输出
 */
float PIDCTRL_CalcPIDGain(pidCtrl_t *_pid);

/**
 * @brief 更新偏差，并计算PID输出。已知偏差项
 *
 * @param _pid  PID参数结构体指针
 * @param _err  偏差
 * @return float PID结果输出
 */
float PIDCTRL_UpdateAndCalcPID(pidCtrl_t *_pid, float _err);

/**
 * @brief 更新偏差，并计算PID输出。已知微分项
 *
 * @param _pid  PID参数结构体指针
 * @param _diff  偏差的微分项
 * @return float PID结果输出
 */
float PIDCTRL_UpdateByDiffAndCalcPID(pidCtrl_t *_pid, float _diff);

/**
 * @brief 设置串级PID结构体参数。
 *
 * @param _pid  要设置的串级PID参数结构体指针
 * @param _okp  外环比例系数
 * @param _oki  外环积分系数
 * @param _okd  外环微分系数
 * @param _ikp  内环比例系数
 * @param _iki  内环积分系数
 * @param _ikd  内环微分系数
 */
void PID2CTRL_Setup(pid2Ctrl_t *_pid, float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd);

/**
 * @brief 构造并返回一个串级PID结构体指针
 *
 * @param _okp  外环比例系数
 * @param _oki  外环积分系数
 * @param _okd  外环微分系数
 * @param _ikp  内环比例系数
 * @param _iki  内环积分系数
 * @param _ikd  内环微分系数
 * @return pid2Ctrl_t*  串级PID参数结构体指针
 */
pid2Ctrl_t* PID2CTRL_Construct(float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd);

/**
 * @brief 更新偏差，并计算串级PID输出。已知偏差项
 *
 * @param _pid  串级PID参数结构体指针
 * @param _err  偏差输入
 * @return float 串级PID结果输出
 */
float PID2CTRL_UpdateAndCalcPID(pid2Ctrl_t *_pid, float _err);

/**
 * @brief 更新偏差，并计算串级PID输出。已知微分项
 *
 * @param _pid  串级PID参数结构体指针
 * @param _diff  偏差微分输入
 * @return float 串级PID结果输出
 */
float PID2CTRL_UpdateByDiffeAndCalcPID(pid2Ctrl_t *_pid, float _diff);

/* @} */

#endif // ! UTILITIES_LIB_PIDCTRL_H
