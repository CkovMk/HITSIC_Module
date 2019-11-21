#ifndef _APP_MENU_PORT_H_
#define _APP_MENU_PORT_H_

#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"

#ifdef __cplusplus
extern "C"{
#endif

#define HITSIC_USE_APP_MENU (1U)

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

/**
    * @brief : 菜单调试输出开关
    * 编译选项为release时无效。
    */
#define HITSIC_MENU_PRINT_ENABLE (1u)
#define HITSIC_MENU_PRINT_VERBOSE_ENABLE (1u)
#define HITSIC_MENU_PRINT_WARNING_ENABLE (1u)

/**
    * @brief : 菜单调试输出语句定义。
    * 编译选项为release时始终为空。
    */
#if defined(DEBUG) && defined(HITSIC_MENU_PRINT_ENABLE) && (HITSIC_MENU_PRINT_ENABLE != 0u)
#define HITSIC_MENU_PRINTF(...) (printf(__VA_ARGS__))
#else
#define HITSIC_MENU_PRINTF(...)
#endif // ! DEBUG

/** @brief : 根菜单最大容量 */
#define HITSIC_MENU_ROOT_SIZE 35 //maxium size of Root Menu

/** @brief : 菜单使用的五向按键初始化。每组数据前两个是GPIO和Pin，其余数据为0。 */
#define HITSIC_MENU_BUTTON_5DIR_BSP_INIT      \
    {                                         \
        {                                     \
            RTEPIN_BOARD_BUTTON_OK_GPIO,      \
            RTEPIN_BOARD_BUTTON_OK_PIN,       \
            kPORT_InterruptOrDMADisabled,     \
            0,                                \
            BUTTON_STAT_NONE,                 \
            NULL,                             \
        },                                    \
            {                                 \
                RTEPIN_BOARD_BUTTON_UP_GPIO,  \
                RTEPIN_BOARD_BUTTON_UP_PIN,   \
                kPORT_InterruptOrDMADisabled, \
                0,                            \
                BUTTON_STAT_NONE,             \
                NULL,                         \
            },                                \
            {                                 \
                RTEPIN_BOARD_BUTTON_DN_GPIO,  \
                RTEPIN_BOARD_BUTTON_DN_PIN,   \
                kPORT_InterruptOrDMADisabled, \
                0,                            \
                BUTTON_STAT_NONE,             \
                NULL,                         \
            },                                \
            {                                 \
                RTEPIN_BOARD_BUTTON_LF_GPIO,  \
                RTEPIN_BOARD_BUTTON_LF_PIN,   \
                kPORT_InterruptOrDMADisabled, \
                0,                            \
                BUTTON_STAT_NONE,             \
                NULL,                         \
            },                                \
            {                                 \
                RTEPIN_BOARD_BUTTON_RT_GPIO,  \
                RTEPIN_BOARD_BUTTON_RT_PIN,   \
                kPORT_InterruptOrDMADisabled, \
                0,                            \
                BUTTON_STAT_NONE,             \
                NULL,                         \
            },                                \
    }
/**
    * @brief : 菜单消息调用所使用的中断信息。
    * 可以使用任何当前工程中未使用的中断。注意中断号和中断服务函数
    * 必须对应。优先级不可过高。
    */
#define HITSIC_MENU_SERVICE_IRQHandler (Reserved71_IRQHandler)
#define HITSIC_MENU_SERVICE_IRQn (Reserved71_IRQn)
#define HITSIC_MENU_SERVICE_IRQPrio (10u)

/**
    * @brief 屏幕打印接口
    */
#define HITSIC_MENU_DISPLAY_PRINT(row, col, str) (OLED_PrintStr_F6x8(row, col, (uint8_t *)str))
//#define HITSIC_MENU_DISPLAY_PRINTF(row, col, fmt, ...) (OLED_Printf(row, col, fmt, __VA_ARGS__))
// #define HITSIC_MENU_DISPLAY_PRINTF(row,col,fmt,...)		\
    // 	{													\
    // 		char* buf = malloc(24*sizeof(char));			\
    // 		vsnprintf(buf,24,fmt,__VA_ARGS__);				\
    // 		HITSIC_MENU_DISPLAY_PRINT(row,col,buf);			\
    // 		free(buf);										\
    // 	}

#endif // ! HITSIC_USE_APP_MENU


#ifdef __cplusplus
}
#endif

#endif // ! _APP_MENU_PORT_H_

