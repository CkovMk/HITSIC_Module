#ifndef UTILITIES_APP_MENU_BUTTON_HPP
#define UTILITIES_APP_MENU_BUTTON_HPP
#include <hitsic_common.h>

#if defined(HITSIC_MENU_USE_BUTTON) && (HITSIC_MENU_USE_BUTTON > 0)
#include <app_menu_def.h>

/*!
 * @addtogroup menu_button
 * @{
 */

/**
 * @brief 按键处理部分 函数调用宏定义
 */
#define MENU_BUTTON_MAKE_EXT_ISR(instNum) (MENU_ButtonExtIsr##instNum)
#define MENU_BUTTON_CALL_EXT_ISR(instNum) BUTTON_ExtIsr(&menu_button[instNum])
#define MENU_BUTTON_DEF_EXT_ISR(instNum)                                        \
    void MENU_BUTTON_MAKE_EXT_ISR(instNum)(void *userData)                      \
    {                                                                           \
        MENU_BUTTON_CALL_EXT_ISR(instNum);                                      \
    }                                                                           \
    extint_handle_t menu_buttonExtintHandle##instNum =                          \
    {                                                                           \
        .index = 0U,                                                            \
        .handler = MENU_BUTTON_MAKE_EXT_ISR(instNum),                           \
        .userData = NULL,                                                       \
    };
#define MENU_BUTTON_MAKEE_PIT_ISR(instNum) (MENU_ButtonPitIsr##instNum)
#define MENU_BUTTON_CALL_PIT_ISR(instNum) BUTTON_PitIsr(&menu_button[instNum]);

// test macro
// MENU_BUTTON_DEF_EXT_ISR(1)

/**
 * ********** 按键操作定义 **********
 */

/**
 * @brief : 按键数量定义。
 */
#define MENU_BUTTON_COUNT (5U)

// test marco
// MENU_BUTTON_MAKE_OP(5wayStick_ok,shrt)

/***********************************
 ************ 按键操作接口 **********
 ***********************************/

/**
 * @brief : 按键初始化函数，无需外部调用。
 */
void MENU_ButtonSetup(void);

/**
 * @brief : 按键定时中断服务函数，无需外部调用。
 */
void MENU_ButtonPitIsr(void *userData);

/**
 * @brief : 按键触发回调函数，无需外部调用。
 */
void MENU_ButtonCallback(button_t *_inst);

/* @} */

#endif // ! HITSIC_MENU_USE_BUTTON

#endif // ! UTILITIES_APP_MENU_BUTTON_HPP
