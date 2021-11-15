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

#ifndef D_MK24F12_TEXTMENU_PORT_H
#define D_MK24F12_TEXTMENU_PORT_H

#include <inc_stdlib.h>

#define kStatusGroup_MENU (205U)

/**
 * @name 调试输出
 * @ {
 */

#define TEXTMENU_USE_SYSLOG (1U)

#if defined(TEXTMENU_USE_SYSLOG) && (TEXTMENU_USE_SYSLOG > 0)

/*! 核心逻辑 LOG级别定义 */
#define TEXTMENU_MAIN_LOG_LVL    (3U)

/*! 数据存储 LOG级别定义 */
#define TEXTMENU_KVDB_LOG_LVL    (3U)

/*! 按键处理 LOG级别定义 */
#define TEXTMENU_BUTN_LOG_LVL    (3U)

/*! 菜单项目 LOG级别定义 */
#define TEXTMENU_ITEM_LOG_LVL    (3U)

/*! 菜单列表 LOG级别定义 */
#define TEXTMENU_LIST_LOG_LVL    (3U)

/*! 菜单迭代 LOG级别定义 */
#define TEXTMENU_ITER_LOG_LVL    (2U)

#else

#error Log output without SYSLOG is not supported yet!
 // TODO
#endif // ! TEXTMENU_USE_SYSLOG

/* @ } */

/** @brief : 根菜单最大容量 */
#define TEXTMENU_ROOT_SIZE 35 //maximum size of Root Menu

/**
 * @name 事件任务接口
 * @brief : 菜单消息调用事件处理线程。
 *
 * 对于裸机，可以使用一个空闲的中断向量来模拟线程，但需要注意优先级不可过高。
 * 对于RTOS，可以直接使用一个线程。
 * @ {
 */
#include <MK24F12.h>
#include "fsl_common.h"
#define TEXTMENU_SERVICE_IRQHandler (Reserved71_IRQHandler)  ///< 要使用的中断服务函数
#define TEXTMENU_SERVICE_IRQn (Reserved71_IRQn)              ///< 要使用的中断号
#define TEXTMENU_SERVICE_IRQPrio (12u)                       ///< 中断优先级，需要设置一个较低的值，以免打断重要任务。

#define TEXTMENU_SERVICE_SEM_GIVE() NVIC_SetPendingIRQ(TEXTMENU_SERVICE_IRQn)
#define TEXTMENU_SERVICE_SEM_TAKE() NVIC_ClearPendingIRQ(TEXTMENU_SERVICE_IRQn)
/* @ } */



/**
 * @name 屏幕打印接口
 * @ {
 */


/**
 * @brief 字符缓存大小（行、列）
 * 
 * 行、列均为实际能够显示的行数和列数，不需要考虑C语言的'\0'。
 */
#define TEXTMENU_DISPLAY_STRBUF_ROW (16u)
#define TEXTMENU_DISPLAY_STRBUF_COL (21u)

/**
 * @brief 启用色盘
 */
#define TEXTMENU_USE_PALETTE (0U)

#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)

#define TEXTMENU_DISPLAY_PAL_IDX_NORMAL_F (1U) ///< 前景色-正常
#define TEXTMENU_DISPLAY_PAL_IDX_NORMAL_B (0U) ///< 背景色-正常
#define TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F (1U) ///< 前景色-高亮
#define TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B (0U) ///< 背景色-高亮
#define TEXTMENU_DISPLAY_PAL_IDX_TITBAR_F (1U) ///< 前景色-标题
#define TEXTMENU_DISPLAY_PAL_IDX_TITBAR_B (0U) ///< 背景色-标题

#endif // ! TEXTMENU_USE_PALETTE


/* @ } */

/**
 *  @name 键值数据库
 *  @ {
 */
/*! @brief 是否启用非易失性键值数据库存储支持。 */
#define TEXTMENU_USE_KVDB (0U)

// #if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

/**
 * @brief : 局部存储 Region Storage
 */
#define TEXTMENU_NVM_REGION_CNT (3) 					///< 局部存储区的数量

// #endif // ! TEXTMENU_USE_KVDB

/* @ } */


#endif // ! D_MK24F12_TEXTMENU_PORT_H

