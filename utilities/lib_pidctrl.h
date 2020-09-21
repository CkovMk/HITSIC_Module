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
/**
 * @file 	:	lib_pidctrl.h
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1-beta.0
 *
 * @date 	:	v0.1-beta.0 2019.10.18 马奇科
 *
 * @brief   :   PID控制算法
 */

#ifndef _LIB_PIDCTRL_H_
#define _LIB_PIDCTRL_H_

#include "inc_stdlib.h"
#include "hitsic_common.h"

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


#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief 设置PID结构体参数
	 * 
	 * @param _pid  
	 * @param _kp  
	 * @param _ki  
	 * @param _kd  
	 */
	void PIDCTRL_Setup(pidCtrl_t *_pid, float _kp, float _ki, float _kd);

	/**
	 * @brief 构造并返回一个PID结构体指针
	 * 
	 * @param _kp  
	 * @param _ki  
	 * @param _kd  
	 * @return pidCtrl_t*
	 */
	pidCtrl_t *PIDCTRL_Construct(float _kp, float _ki, float _kd);

	/**
	 * @brief 更新PID偏差，已知偏差项
	 * 
	 * @param _pid  
	 * @param _err  
	 */
	void PIDCTRL_ErrUpdate(pidCtrl_t *_pid, float _err);

	/**
	 * @brief 更新PID偏差，已知微分项
	 * 
	 * @param _pid  
	 * @param _diff  
	 */
	void PIDCTRL_ErrUpdateByDiff(pidCtrl_t *_pid, float _diff);

	/**
	 * @brief 更新PID偏差，已知偏差项和微分项
	 * 
	 * @param _pid  
	 * @param _diff  
	 */
	void PIDCTRL_ErrUpdateByErrAndDiff(pidCtrl_t *_pid, float _err, float _diff);

	/**
	 * @brief 计算比例输出
	 * 
	 * @param _pid  
	 * @return float 
	 */
	float PIDCTRL_CalcPGain(pidCtrl_t *_pid);

	/**
	 * @brief 计算积分输出
	 * 
	 * @param _pid  
	 * @return float 
	 */
	float PIDCTRL_CalcIGain(pidCtrl_t *_pid);

	/**
	 * @brief 计算微分输出
	 * 
	 * @param _pid  
	 * @return float 
	 */
	float PIDCTRL_CalcDGain(pidCtrl_t *_pid);

	/**
	 * @brief 计算PID总输出
	 * 
	 * @param _pid  
	 * @return float 
	 */
	float PIDCTRL_CalcPIDGain(pidCtrl_t *_pid);

	/**
	 * @brief 更新偏差，并计算PID输出。已知偏差项
	 * 
	 * @param _pid  
	 * @param _err  
	 * @return float 
	 */
	float PIDCTRL_UpdateAndCalcPID(pidCtrl_t *_pid, float _err);

	/**
	 * @brief 更新偏差，并计算PID输出。已知微分项
	 * 
	 * @param _pid  
	 * @param _diff  
	 * @return float 
	 */
	float PIDCTRL_UpdateByDiffAndCalcPID(pidCtrl_t *_pid, float _diff);

	/**
	 * @brief 设置串级PID结构体参数。
	 * 
	 * @param _pid  
	 * @param _okp  
	 * @param _oki  
	 * @param _okd  
	 * @param _ikp  
	 * @param _iki  
	 * @param _ikd  
	 */
	void PID2CTRL_Setup(pid2Ctrl_t *_pid, float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd);

	/**
	 * @brief 构造并返回一个串级PID结构体指针
	 * 
	 * @param _okp  
	 * @param _oki  
	 * @param _okd  
	 * @param _ikp  
	 * @param _iki  
	 * @param _ikd  
	 * @return pid2Ctrl_t*
	 */
	pid2Ctrl_t *PID2CTRL_Construct(float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd);
	
	/**
	 * @brief 更新偏差，并计算串级PID输出。已知偏差项
	 * 
	 * @param _pid  
	 * @param _err  
	 * @return float 
	 */
	float PID2CTRL_UpdateAndCalcPID(pid2Ctrl_t *_pid, float _err);

	/**
	 * @brief 更新偏差，并计算串级PID输出。已知微分项
	 * 
	 * @param _pid  
	 * @param _diff  
	 * @return float 
	 */
	float PID2CTRL_UpdateByDiffeAndCalcPID(pid2Ctrl_t *_pid, float _diff);

#ifdef __cplusplus
}
#endif

#endif // ! _LIB_PIDCTRL_H_
