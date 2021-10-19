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
#ifndef UTILITIES_LIB_ADRCCTRL_H
#define UTILITIES_LIB_ADRCCTRL_H


#include <inc_stdlib.h>
#include "hitsic_common.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct
{
	float v1;// v的滤波结果，其对时间的二阶导数被限制在-r,r之间
	float v2;//	v1的导数
	float h;//采样时间or计算周期
	float r;//参，将v1的对时间的二阶导数限制在-r,r之间
}adrc_td_t;

typedef struct
{
	//括号内容以速度环举例
	float h;//采样时间or计算周期（控制环周期）
	float v;//输入的目标（设定速度）
	float v1;
	float v2;
	float r;//参，将v1的二阶导数限制在-r,r之间
	float e;
	float e1;
	float e2;
	float z1;
	float z2;
	float z3;
	float u0;
	float u;//输出的控制量(pwm)
	float u_max;//输出量限幅
	float u_min;//输出量限幅
	float y;//输入的测量量(编码器返回的速度)
	float b;//参，补偿系数
	float b01;
	float b02;
	float b03;
	float w0;//参，观测器带宽
	float kp;//参
	float kd;//参
}adrc_t;

//初始化，传入参数仅进行拷贝
void ADRCCTRL_TDInit(adrc_td_t* td_t, float h);

//更新一次TD
void ADRCCTRL_TD(adrc_td_t* td, float v);

//初始化，传入参数仅进行拷贝
void ADRCCTRL_Init(adrc_t* p,float h);

//更新一次adrc控制
void ADRCCTRL_Update(adrc_t* p);

//更新一次adrc控制，只不过是非线性d(根据误差微分大小动态调整kd,d_line是调整的分界线，具体请检查代码)
void ADRCCTRL_UpdateFal(adrc_t* p, float d_line);







#ifdef __cplusplus
}
#endif



#endif // ! UTILITIES_LIB_ADRCCTRL_H


