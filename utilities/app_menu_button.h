#ifndef _APP_MENU_BUTTON_H_
#define _APP_MENU_BUTTON_H_

#include "app_menu_def.h"

/**
 * @brief 按键处理部分 函数调用宏定义
 */
#define MENU_BUTTON_MAKE_EXT_ISR(instNum) (MENU_ButtonExtIsr##instNum)
#define MENU_BUTTON_CALL_EXT_ISR(instNum) BUTTON_ExtIsr(&menu_button[instNum])
#define MENU_BUTTON_DEF_EXT_ISR(instNum) \
    void MENU_BUTTON_MAKE_EXT_ISR(instNum)(void) { MENU_BUTTON_CALL_EXT_ISR(instNum); }
#define MENU_BUTTON_MAKEE_PIT_ISR(instNum) (MENU_ButtonPitIsr##instNum)
#define MENU_BUTTON_CALL_PIT_ISR(instNum) BUTTON_PitIsr(&menu_button[instNum]);

//test macro
//MENU_BUTTON_DEF_EXT_ISR(1)

#ifdef __cplusplus
extern "C"
{
#endif

    /**
	 * ********** 按键操作定义 **********
	 */


/**
 * @brief : 按键数量定义。
 */
#define MENU_BUTTON_COUNT 5

/**
 * @brief : 按键操作生成宏
 */
#define MENU_BUTTON_MAKE_OP(code, type) (menuOpCode_##code|menuOpType_##type)

    //test marco
    //MENU_BUTTON_MAKE_OP(ok,shrt)



    extern pitmgr_handle_t *menu_butonPitMgrHandle;

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
    void MENU_ButtonPitIsr(void);

    /**
	 * @brief : 按键触发回调函数，无需外部调用。
	 */
    void MENU_ButtonCallback(button_t *_inst);

#ifdef __cplusplus
}
#endif

#endif // ! _APP_MENU_BUTTON_H_
