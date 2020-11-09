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
/**
 * @file 	:	app_menu.h
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1.4
 * 
 * @date 	:	v0.1-beta.0 2019.10.28
 * @date 	:	v0.1.1		2019.11.02
 * @date    :   v0.1.2      2019.11.22
 * @date    :   v0.1.3      2020.07.31
 * @date    :   v0.1.4      2020.10.09
 * @date    :   v0.1.5      2020.10.30
 * @date    :   v0.2.0      2020.11.09
 * 
 */

#pragma once
#ifndef _APP_MENU_H_
#define _APP_MENU_H_

#include "app_menu_button.hpp"
#include "app_menu_def.hpp"
#include "app_menu_nvm.hpp"
#include "app_menu_type.hpp"

 /*!
  * @addtogroup menu
  * @{
  */

/** @brief : 软件版本 */
#define APP_MENU_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 4U))

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

/**************************************
 ************ 菜单顶层操作接口 **********
 **************************************/

/**
 * @brief : 菜单全局初始化。
 * 该函数会初始化必要的变量，创建根菜单和其他最基本的菜单列表和菜单项。
 *
 * @param  {void} undefined :
 */
void MENU_Init(void);

/**
 * @brief : 初始化各菜单列表和菜单项。
 * 注意：该函数在外部定义，不需初始化最基本的菜单列表和菜单项。该函数禁止用户手动调用。
 *
 * @param  {void} undefined :
 */
void MENU_DataSetUp(void);

/**
 * @brief : 打印屏幕。
 * 该函数是整个菜单的屏幕打印入口，将自动根据菜单的状态打印屏幕。
 *
 * @param  {void} undefined :
 */
void MENU_PrintDisp(void);

/**
 * @brief : 响应按键操作。
 * 该函数是整个菜单的按键响应入口，将自动根据菜单的状态调用按键响应函数。
 * 该函数不会更新屏幕显示。
 *
 * @param  {void} undefined :
 */
void MENU_KeyOp(menu_keyOp_t *const _op);

#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)

/**
 * @brief : 保存整个菜单到NVM。
 *
 * @param  {int32_t} _region :  所选择的局部存储区。
 */
void MENU_Data_NvmSave(int32_t _region);

/**
 * @brief : 保存整个菜单到NVM。
 * 该函数将使用全局变量 menu_currRegionNum 中保存的局部存储区号。
 * 
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 从NVM读取整个菜单。
 *
 * @param  {int32_t} _region : 所选择的局部存储区。
 */
void MENU_Data_NvmRead(int32_t _region);

/**
 * @brief : 从NVM读取整个菜单。
 * 该函数将使用全局变量 menu_currRegionNum 中保存的局部存储区号。
 *
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 保存当前局部存储区号到NVM。
 * 该数值设置为不自动保存。
 *
 */
void MENU_Data_NvmSaveRegionConfig(void);

/**
 * @brief : 保存当前局部存储区号到NVM。
 * 该数值设置为不自动保存。
 *
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 从NVM中读取当前局部存储区号。
 * 该数值设置为不自动保存。
 *
 */
void MENU_Data_NvmReadRegionConfig(void);

/**
 * @brief : 从NVM中读取当前局部存储区号。
 * 该数值设置为不自动保存。
 * 
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmReadRegionConfig_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 将一个局部存储区的数据拷贝到另一个局部存储区。
 *
 * @param  {int32_t} _srcRegion : 源存储序号。
 * @param  {int32_t} _dstRegion : 目的存储区序号。
 */
void MENU_Data_NvmCopy(int32_t _srcRegion, int32_t _dstRegion);

/**
 * @brief : 将一个局部存储区的数据拷贝到另一个局部存储区。
 * 该函数将使用全局变量 menu_nvmCopySrc 和 menu_nvmCopyDst 中存储的值。
 * 
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmCopy_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 读取NVM状态标志。
 *
 * @return {int32_t}        : 返回读取到的状态标志。
 */
int32_t MENU_GetNvmStatus(void);

/**
 * @brief : 设置NVM状态标志。
 *
 * @param  {int32_t} _status : 要设置的状态标志。
 */
void MENU_SetNvmStatus(int32_t _status);

#endif // ! HITSIC_MENU_USE_NVM

/**
 * @brief : 定时中断管理器句柄。
 *
 */
void MENU_PitIsr(void* userData);

/**
 * @brief 挂起菜单。这将暂时禁用菜单的事件处理，包括按键输入和屏幕打印。
 *          注意：调用多次MENU_Suspend()将需要调用相同次数的MENU_Resume()才能将菜单恢复至运行状态。
 */
void MENU_Suspend(void);

/**
 * @brief 恢复菜单。这将启用菜单的事件处理，包括按键输入和屏幕打印。
 *          注意：调用多次MENU_Suspend()将需要调用相同次数的MENU_Resume()才能将菜单恢复至运行状态。
 */
void MENU_Resume(void);



#endif // ! HITSIC_USE_APP_MENU

/*! @} */

#endif // ! _APP_MENU_H_
