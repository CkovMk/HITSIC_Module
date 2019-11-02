#include "app_menu_button.h"



#ifdef __cplusplus
extern "C"{
#endif 

	/**
	 * ********** 按键操作定义 **********
	 */

	menu_keyOp_t menu_keyOpBuff;

	button_t menu_button[MENU_BUTTON_COUNT] = HITSIC_MENU_BUTTON_5DIR_BSP_INIT;

	pitmgr_handle_t *menu_butonPitMgrHandle;

	/**
	 * ********** 按键操作接口 **********
	 */
	MENU_BUTTON_DEF_EXT_ISR(0)

	MENU_BUTTON_DEF_EXT_ISR(1)

	MENU_BUTTON_DEF_EXT_ISR(2)

	MENU_BUTTON_DEF_EXT_ISR(3)

	MENU_BUTTON_DEF_EXT_ISR(4)

	void MENU_ButtonSetup(void)
	{
		for (int i = 0; i < MENU_BUTTON_COUNT; ++i)
		{
			BUTTON_Setup(&menu_button[i], menu_button[i].gpio, menu_button[i].pin);
			BUTTON_InstallHandler(&menu_button[i], (void(*)(void*))MENU_ButtonCallback);
		}
		EXTINT_PortSetup(BUTTON_GetPortInst(menu_button[0].gpio), menu_button[0].pin, 0, MENU_BUTTON_MAKE_EXT_ISR(0));
		EXTINT_PortSetup(BUTTON_GetPortInst(menu_button[1].gpio), menu_button[1].pin, 0, MENU_BUTTON_MAKE_EXT_ISR(1));
		EXTINT_PortSetup(BUTTON_GetPortInst(menu_button[2].gpio), menu_button[2].pin, 0, MENU_BUTTON_MAKE_EXT_ISR(2));
		EXTINT_PortSetup(BUTTON_GetPortInst(menu_button[3].gpio), menu_button[3].pin, 0, MENU_BUTTON_MAKE_EXT_ISR(3));
		EXTINT_PortSetup(BUTTON_GetPortInst(menu_button[4].gpio), menu_button[4].pin, 0, MENU_BUTTON_MAKE_EXT_ISR(4));

		menu_butonPitMgrHandle = PITMGR_HandleInsert(BUTTON_PITMGR_TIME_MS, 0, MENU_ButtonPitIsr, pitmgr_pptEnable);
	}

	void MENU_ButtonPitIsr(void)
	{
		for (int i = 0; i < MENU_BUTTON_COUNT; ++i)
		{
			BUTTON_PitIsr(&menu_button[i]);
		}
	}

	void MENU_ButtonCallback(button_t *_inst)
	{

		for (int i = 0; i < MENU_BUTTON_COUNT; ++i)
		{
			if (_inst == &menu_button[i])
			{
				menu_keyOpBuff = i + 1;
			}
		}
		switch (_inst->status)
		{
		case BUTTON_SHRT_PRES:
			menu_keyOpBuff |= menuOpType_shrt;
			break;
		case BUTTON_LONG_PRES:
			menu_keyOpBuff |= menuOpType_long;
			break;
		case BUTTON_LRPT_PRES:
			menu_keyOpBuff |= menuOpType_lrpt;
			break;
		}
		
		menu_statusFlag |= menu_message_buttonOp;
		NVIC_SetPendingIRQ(HITSIC_MENU_SERVICE_IRQn);
	}
	




#ifdef __cplusplus
}
#endif 