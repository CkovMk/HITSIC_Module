#include <app_menu_button.h>

#if defined(HITSIC_MENU_USE_BUTTON) && (HITSIC_MENU_USE_BUTTON > 0)

/*!
 * @addtogroup menu_button
 * @{
 */

#define SYSLOG_TAG  ("MENU.BUTN")
#define SYSLOG_LVL  (HITSIC_MENU_BUTN_LOG_LVL)
#include <inc_syslog.h>

/**
 * ********** 按键操作定义 **********
 */

button_t menu_button[MENU_BUTTON_COUNT] = HITSIC_MENU_BUTTON_5DIR_BSP_INIT;

pitmgr_handle_t menu_butonPitMgrHandle =
{
        .tickInterval = BUTTON_PITMGR_TIME_MS,
        .tickOffset = 0UL,
        .handler = MENU_ButtonPitIsr,
        .pptFlag = pitmgr_pptEnable,
        .userData = NULL,
};

/**
 * ********** 按键操作接口 **********
 */
MENU_BUTTON_DEF_EXT_ISR(0)

MENU_BUTTON_DEF_EXT_ISR(1)

MENU_BUTTON_DEF_EXT_ISR(2)

MENU_BUTTON_DEF_EXT_ISR(3)

MENU_BUTTON_DEF_EXT_ISR(4)

void MENU_ButtonSetup(void) {
    for (int i = 0; i < MENU_BUTTON_COUNT; ++i) {
        BUTTON_Setup(&menu_button[i], menu_button[i].gpio, menu_button[i].pin);
        BUTTON_InstallHandler(&menu_button[i],
                              (void (*)(void *))MENU_ButtonCallback);
    }

    menu_buttonExtintHandle0.index = menu_button[0].pin;
    EXTINT_HandleInsert(EXTINT_GetInst(EXTINT_GetPortInst(menu_button[0].gpio)), &menu_buttonExtintHandle0);

    menu_buttonExtintHandle1.index = menu_button[1].pin;
    EXTINT_HandleInsert(EXTINT_GetInst(EXTINT_GetPortInst(menu_button[1].gpio)), &menu_buttonExtintHandle1);

    menu_buttonExtintHandle2.index = menu_button[2].pin;
    EXTINT_HandleInsert(EXTINT_GetInst(EXTINT_GetPortInst(menu_button[2].gpio)), &menu_buttonExtintHandle2);

    menu_buttonExtintHandle3.index = menu_button[3].pin;
    EXTINT_HandleInsert(EXTINT_GetInst(EXTINT_GetPortInst(menu_button[3].gpio)), &menu_buttonExtintHandle3);

    menu_buttonExtintHandle4.index = menu_button[4].pin;
    EXTINT_HandleInsert(EXTINT_GetInst(EXTINT_GetPortInst(menu_button[4].gpio)), &menu_buttonExtintHandle4);

//    EXTINT_PortSetup(EXTINT_GetPortInst(menu_button[0].gpio), menu_button[0].pin,
//                     0, MENU_BUTTON_MAKE_EXT_ISR(0));
//    EXTINT_PortSetup(EXTINT_GetPortInst(menu_button[1].gpio), menu_button[1].pin,
//                     0, MENU_BUTTON_MAKE_EXT_ISR(1));
//    EXTINT_PortSetup(EXTINT_GetPortInst(menu_button[2].gpio), menu_button[2].pin,
//                     0, MENU_BUTTON_MAKE_EXT_ISR(2));
//    EXTINT_PortSetup(EXTINT_GetPortInst(menu_button[3].gpio), menu_button[3].pin,
//                     0, MENU_BUTTON_MAKE_EXT_ISR(3));
//    EXTINT_PortSetup(EXTINT_GetPortInst(menu_button[4].gpio), menu_button[4].pin,
//                     0, MENU_BUTTON_MAKE_EXT_ISR(4));

    PITMGR_HandleInsert(&pitmgr_main, &menu_butonPitMgrHandle);
}

void MENU_ButtonPitIsr(void *userData) {
    for (int i = 0; i < MENU_BUTTON_COUNT; ++i) {
        BUTTON_PitIsr(&menu_button[i]);
    }
}

void MENU_ButtonCallback(button_t *_inst) {
    for (int i = 0; i < MENU_BUTTON_COUNT; ++i) {
        if (_inst == &menu_button[i]) {
            menu_keyOpBuff = i + 1;
        }
    }
    switch (_inst->status) {
    case BUTTON_SHRT_PRES:
        menu_keyOpBuff |= menuOpType_shrt;
        break;
    case BUTTON_LONG_PRES:
        menu_keyOpBuff |= menuOpType_long;
        break;
    case BUTTON_LRPT_PRES:
        menu_keyOpBuff |= menuOpType_lrpt;
        break;
    default:
        break;
    }

    menu_statusFlag |= menu_message_buttonOp;

    SYSLOG_V("Button message: %ld", menu_keyOpBuff);
    HITSIC_MENU_SERVICE_SEM_GIVE();
}

/* @} */

#endif // ! HITSIC_MENU_USE_BUTTON
