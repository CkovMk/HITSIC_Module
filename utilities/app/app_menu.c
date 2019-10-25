#include "appui.h"

//test...
int a = 100;
float b = 25.12;
ui_menuList_t t_list;
void test_app(key_btOp_t *op)
{
	switch(*op)
	{
	case key_btOp_t::LDN:
		a=555;
		break;
	default:
		a=10;
		break;
	}
}

//extern float appfilter_compTgRev;
//extern volatile float ctrl_dir_kp,ctrl_dir_ki,ctrl_dir_kd;
//extern  float ctrl_tcspd_kp,ctrl_tcspd_ki,ctrl_tcspd_kd;
//extern  float ctrl_ang_kp,ctrl_ang_ki,ctrl_ang_kd;
//extern float emag_ad[4];


//----------ui---------

ui_mode_t ui_mode;
uint32_t ui_sysTimer_irqPos;
bool ui_updateFlag;
uint8_t ui_img[CAM_IMG_R/2][CAM_IMG_C/2];

void ui_init()
{
	ui_mode = ui_mode_t::menu_ctns_mode;
	ui_sysTimer_irqPos = sysirq_pitInt_setup(50, (sysirqHandler_t)ui_sysTimer_irqHandler);
	ui_updateFlag = true;
	//ui_menu init
	ui_menu_currList = &ui_menu_root;
	ui_menu_currItem = NULL;

	ui_menuInit();

	 
}

void ui_print_oled(void)
{
	switch (ui_mode)
	{
	case ui_mode_t::off_mode:
		break;
	case ui_mode_t::menu_idle_mode:
	case ui_mode_t::menu_ctns_mode:
		//oled_fill(0);
		if (ui_menu_currItem != NULL)
		{
			
			ui_menu_currItem->print_oled();
		}
		else
		{
			ui_menu_currList->print_oled();
		}
		break;
	case ui_mode_t::img_mode:
        img_getHalfResolution((uint8_t*)(*camera_imgData).img, (uint8_t*)ui_img, CAM_IMG_R, CAM_IMG_C);
        oled_printImg(CAM_IMG_R/2, CAM_IMG_C/2, (uint8_t*)ui_img, 200);
//		oled_fill(0);
		break;
	}
	
}

void ui_key_op(key_btOp_t *_op)
{
	if (key_btOp == key_btOp_t::NOP) { return; }
	switch (ui_mode)
	{
	case ui_mode_t::off_mode:
		break;
	case ui_mode_t::menu_idle_mode:
	case ui_mode_t::menu_ctns_mode:
		if (ui_menu_currItem != NULL)
		{
			ui_menu_currItem->key_op(_op);
		}
		else
		{
			ui_menu_currList->key_op(_op);
		}
		break;
	case ui_mode_t::img_mode:

		break;
	}
	*_op = key_btOp_t::NOP;
}

void ui_update(void)
{
	if(drvkey_swKey[UI_MODE_SW0].read() == KEY_SW_ONN_LOGIC)
	{
		if(drvkey_swKey[UI_MODE_SW1].read() == KEY_SW_ONN_LOGIC)
		{
			ui_mode = ui_mode_t::menu_ctns_mode;
		}
		else
		{
			ui_mode = ui_mode_t::menu_idle_mode;
			//ui_print_oled();
		}
	}
	else
	{
		if(drvkey_swKey[UI_MODE_SW1].read() == KEY_SW_ONN_LOGIC)
		{
			ui_mode = ui_mode_t::img_mode;
		}
		else
		{
			ui_mode = ui_mode_t::off_mode;
			oled_fill(0);
		}
	}
  
	
	switch (ui_mode)
	{
	case ui_mode_t::off_mode:
		break;
	case ui_mode_t::menu_idle_mode:
		if (key_btOp == key_btOp_t::NOP) { break; }
	case ui_mode_t::menu_ctns_mode:
		ui_key_op(&key_btOp);
		ui_print_oled();
		break;
	case ui_mode_t::img_mode:
        ui_print_oled();
		break;
	}
}

void ui_sysTimer_irqHandler(void)
{
	ui_updateFlag = true;
}

//----------ui_menu---------

ui_menuList_t* ui_menu_currList;
ui_menuItem_t* ui_menu_currItem;
ui_menuList_t 	ui_menu_root = ui_menuList_t("RootMenu", &ui_menu_root);

ui_menuList_t	emag_list = ui_menuList_t("Emag", &ui_menu_root),\
				mode_list = ui_menuList_t("modeCtrl",&ui_menu_root),\
				tcmode_list = ui_menuList_t("TCmode", &ui_menu_root),\
				urmode_list = ui_menuList_t("URmode", &ui_menu_root);

void ui_menuInit(void)
{	
	ui_menu_root.insert(ui_menuItem_t("readData", (void*)ui_menuReadSetContent, ui_menuItem_type_t::procType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
	ui_menu_root.insert(ui_menuItem_t("saveData", (void*)ui_menuGetSaveContent, ui_menuItem_type_t::procType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
	ui_menu_root.insert(ui_menuItem_t("emag", &emag_list, ui_menuItem_type_t::menuType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		emag_list.insert(ui_menuItem_t("-EXL  ", (void*)& emag_ad[MAG_EXL], ui_menuItem_type_t::varfType, 0));
		emag_list.insert(ui_menuItem_t("-EXR  ", (void*)& emag_ad[MAG_EXR], ui_menuItem_type_t::varfType, 0));
    	emag_list.insert(ui_menuItem_t("-EYR  ", (void*)&emag_ad[MAG_EYR], ui_menuItem_type_t::varfType, 0));
		emag_list.insert(ui_menuItem_t("-EYL  ", (void*)& emag_ad[MAG_EYL], ui_menuItem_type_t::varfType, 0));
    	emag_list.insert(ui_menuItem_t("-DM   ", (void*)&emag_dm, ui_menuItem_type_t::varfType, 0));
    	emag_list.insert(ui_menuItem_t("-DMdot   ", (void*)&emag_dm_dot, ui_menuItem_type_t::varfType, 0));
    	emag_list.insert(ui_menuItem_t("m-drEr", (void*)&ctrl_dir_magErr, ui_menuItem_type_t::varfType, 0));
    	emag_list.insert(ui_menuItem_t("tcs-H     ", (void*)&tcs.hsv.h, ui_menuItem_type_t::varfType, 0));
    	emag_list.insert(ui_menuItem_t("tsc-S     ", (void*)&tcs.hsv.s, ui_menuItem_type_t::varfType, 0));
    	emag_list.insert(ui_menuItem_t("tsc-V     ", (void*)&tcs.hsv.v, ui_menuItem_type_t::varfType, 0));
		emag_list.insert(ui_menuItem_t("Range  ", (void*)&tof0.range_mm, ui_menuItem_type_t::variType, 0));
		emag_list.insert(ui_menuItem_t("ax_dot", (void*)& appfilter_ax_dot, ui_menuItem_type_t::varfType, 0));
		emag_list.insert(ui_menuItem_t("gz_dot", (void*)& appfilter_gz_dot, ui_menuItem_type_t::varfType, 0));
		emag_list.insert(ui_menuItem_t("journey", (void*)& ctrl_journey, ui_menuItem_type_t::varfType, 0));
		emag_list.insert(ui_menuItem_t("yaw   ", (void*)& appfilter_yaw, ui_menuItem_type_t::varfType, 0));
		emag_list.insert(ui_menuItem_t("height", (void*)& appfilter_height, ui_menuItem_type_t::varfType, 0));
		emag_list.insert(ui_menuItem_t("bluesl", (void*)& blue_s_low, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		emag_list.insert(ui_menuItem_t("bluehl", (void*)& blue_h_low, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		emag_list.insert(ui_menuItem_t("bluehh", (void*)& blue_h_high, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		
	ui_menu_root.insert(ui_menuItem_t("filt_Tg", &appfilter_compTgRev, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
	ui_menu_root.insert(ui_menuItem_t("sumRate", (void*)&ctrl_dir_magErr_mixRate, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
	//ui_menu_root.insert(ui_menuItem_t("Dir_err", (void*)&ctrl_dir_err, ui_menuItem_type_t::varfType, 0));
    //ui_menu_root.insert(ui_menuItem_t("Dir_p", (void*)&ctrl_dir_kp, ui_menuItem_type_t::varfType, false));
    //ui_menu_root.insert(ui_menuItem_t("Dir_i", (void*)&ctrl_dir_ki, ui_menuItem_type_t::varfType, false));
    //ui_menu_root.insert(ui_menuItem_t("Dir_d", (void*)&ctrl_dir_kd, ui_menuItem_type_t::varfType, false));

	ui_menu_root.insert(ui_menuItem_t("Modes", &mode_list, ui_menuItem_type_t::menuType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		mode_list.insert(ui_menuItem_t("Motor", (void*)&task_motorPwmUpdate_ENflag, ui_menuItem_type_t::variType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		mode_list.insert(ui_menuItem_t("Servo", (void*)&task_servoPwmUpdate_ENflag, ui_menuItem_type_t::variType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		mode_list.insert(ui_menuItem_t("urMode", (void*)task_set_ur_mode, ui_menuItem_type_t::procType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		mode_list.insert(ui_menuItem_t("tcMode", (void*)task_set_tc_mode, ui_menuItem_type_t::procType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		mode_list.insert(ui_menuItem_t("PWMtest", (void*)task_set_pwmTest, ui_menuItem_type_t::procType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		mode_list.insert(ui_menuItem_t("motorL", (void*)&ctrl_motorL, ui_menuItem_type_t::variType, 0));
		mode_list.insert(ui_menuItem_t("motorR", (void*)&ctrl_motorR, ui_menuItem_type_t::variType, 0));
		mode_list.insert(ui_menuItem_t("servoF", (void*)&ctrl_servoF, ui_menuItem_type_t::variType, 0));
		mode_list.insert(ui_menuItem_t("servoB", (void*)&ctrl_servoB, ui_menuItem_type_t::variType, 0));

	ui_menu_root.insert(ui_menuItem_t("URset", &urmode_list, ui_menuItem_type_t::menuType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("urMode", (void*)task_set_ur_mode, ui_menuItem_type_t::procType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("AngCur", (void*)&appfilter_ang, ui_menuItem_type_t::varfType, 0));
		urmode_list.insert(ui_menuItem_t("AngSet", (void*)&ctrl_ur_angSet, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("AngErr", (void*)&ctrl_ur_ang_err, ui_menuItem_type_t::varfType, 0));
		urmode_list.insert(ui_menuItem_t("AngGain", (void*)&ctrl_ur_ang_pidGain, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("Ang_p", (void*)&ctrl_ur_ang_kp, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
    	urmode_list.insert(ui_menuItem_t("Ang_i", (void*)&ctrl_ur_ang_ki, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
    	urmode_list.insert(ui_menuItem_t("Ang_d", (void*)&ctrl_ur_ang_kd, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("SpdCur", (void*)&ctrl_spdAvg, ui_menuItem_type_t::varfType, 0));
		urmode_list.insert(ui_menuItem_t("SpdSet", (void*)&ctrl_ur_spdSet, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("SpdErr", (void*)&ctrl_ur_spd_err, ui_menuItem_type_t::varfType, 0));
		urmode_list.insert(ui_menuItem_t("SpdGain", (void*)&ctrl_ur_spd_pidGain, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
    	urmode_list.insert(ui_menuItem_t("Spd_p", (void*)&ctrl_ur_spd_kp, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
    	urmode_list.insert(ui_menuItem_t("Spd_i", (void*)&ctrl_ur_spd_ki, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
    	urmode_list.insert(ui_menuItem_t("Spd_d", (void*)&ctrl_ur_spd_kd, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("DirErr", (void*)& ctrl_dir_err, ui_menuItem_type_t::varfType, 0));
		urmode_list.insert(ui_menuItem_t("Dir_p", (void*)& ctrl_ur_dir_kp, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("Dir_i", (void*)& ctrl_ur_dir_ki, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("Dir_d", (void*)& ctrl_ur_dir_kd, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		urmode_list.insert(ui_menuItem_t("DirGain", (void*)& ctrl_dir_pidGain, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));

	ui_menu_root.insert(ui_menuItem_t("TCset", &tcmode_list, ui_menuItem_type_t::menuType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("tcMode", (void*)task_set_tc_mode, ui_menuItem_type_t::procType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("SpdCur", (void*)& ctrl_spdAvg, ui_menuItem_type_t::variType, 0));
		tcmode_list.insert(ui_menuItem_t("SpdSet", (void*)& ctrl_tc_spdSet, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("Spd_p", (void*)& ctrl_tc_spd_kp, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("Spd_i", (void*)& ctrl_tc_spd_ki, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("Spd_d", (void*)& ctrl_tc_spd_kd, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("DirErr", (void*)& ctrl_dir_err, ui_menuItem_type_t::varfType, 0));
		tcmode_list.insert(ui_menuItem_t("Dir_p", (void*)& ctrl_tc_dir_kp, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("Dir_i", (void*)& ctrl_tc_dir_ki, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("Dir_d", (void*)& ctrl_tc_dir_kd, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
		tcmode_list.insert(ui_menuItem_t("DirGain", (void*)& ctrl_dir_pidGain, ui_menuItem_type_t::varfType, (uint32_t)ui_menuItem_propety_t::menuModify | (uint32_t)ui_menuItem_propety_t::enableflashRW));
}

void ui_menuGetContent(void)
{
	deque<ui_menuList_t *> que;
	que.push_back(&ui_menu_root);	//push root menu in queue
	int32_t currentItemNum = 0;
	for(uint8_t list=0; list<que.size(); list++)
	{
		for (vector<ui_menuItem_t>::iterator it = que[list]->menu.begin(); it != que[list]->menu.end(); it++)
		{
			switch((*it).type)
			{
			case ui_menuItem_type_t::nullType:
				break;
			case ui_menuItem_type_t::variType:
				((ui_menuItem_variType_t*)it->ptr)->getContent();
				break;
			case ui_menuItem_type_t::varfType:
				((ui_menuItem_varfType_t*)it->ptr)->getContent();
				break;
			case ui_menuItem_type_t::procType:
				break;
			case ui_menuItem_type_t::menuType:
				bool list_push = true;
				for (deque<ui_menuList_t*>::iterator it2 = que.begin(); it2 != que.end(); it2++)
				{
					if((*it2) == (*(ui_menuItem_menuType_t *)(*it).ptr).ptr){list_push = false; break;}
				}
				if(list_push) {que.push_back((*(ui_menuItem_menuType_t *)(*it).ptr).ptr);}
				break;
			}
			currentItemNum++;
		}
	}
}
void ui_menuSetContent(void)
{
	deque<ui_menuList_t *> que;
	que.push_back(&ui_menu_root);	//push root menu in queue
	int32_t currentItemNum = 0;
	for(uint8_t list=0; list<que.size(); list++)
	{
		for (vector<ui_menuItem_t>::iterator it = que[list]->menu.begin(); it != que[list]->menu.end(); it++)
		{
		  	if(it->readOnly){continue;}
			switch((*it).type)
			{
			case ui_menuItem_type_t::nullType:
				break;
			case ui_menuItem_type_t::variType:
				it->setContent();
				break;
			case ui_menuItem_type_t::varfType:
				it->setContent();
				break;
			case ui_menuItem_type_t::procType:
				break;
			case ui_menuItem_type_t::menuType:
				bool list_push = true;
				for (deque<ui_menuList_t*>::iterator it2 = que.begin(); it2 != que.end(); it2++)
				{
					if((*it2) == (*(ui_menuItem_menuType_t *)(*it).ptr).ptr){list_push = false; break;}
				}
				if(list_push) {que.push_back((*(ui_menuItem_menuType_t *)(*it).ptr).ptr);}
				break;
			}
			currentItemNum++;
		}
	}
}
void ui_menuReadContent_old(void)
{
	uint16_t flashBlockIdx = 0,flashSectorIdx = 0;
	uint32_t flashBuf[2] = {0,0};
	
	//start reading
	deque<ui_menuList_t *> que;
	que.push_back(&ui_menu_root);	//push root menu in queue
	uint32_t currentItemNum = 0;
	for(uint8_t list=0; list<que.size(); list++)
	{
		for (vector<ui_menuItem_t>::iterator it = que[list]->menu.begin(); it != que[list]->menu.end(); it++)
		{
			//calculate blockIdx and sectorIdx. caution: this"flashBlockIdx" is the physical address.
			flashBlockIdx = currentItemNum / UI_MENU_FLASH_ITEM_PER_BLOCK+ UI_MENU_FLASH_BLOCK_OFFSET;
			flashSectorIdx = (currentItemNum % UI_MENU_FLASH_ITEM_PER_BLOCK) * 16;
			flashBuf[0] = (uint32_t)flash_rx64(flashBlockIdx, flashSectorIdx);
			flashBuf[1] = (uint32_t)flash_rx64(flashBlockIdx, flashSectorIdx + 8);
			//check hash
			char comBuf[128];
			sprintf(comBuf,"item%3.3d Rx=%2.2d,%4.4d: 0x%8.8x,0x%8.8x\r\n",currentItemNum,flashBlockIdx,flashSectorIdx,flashBuf[0],flashBuf[1]);
			
			currentItemNum++;
			if(flashBuf[0] != it->hash()){continue;}
			if(it->readOnly){continue;}
			switch((*it).type)
			{
			case ui_menuItem_type_t::nullType:
				break;
			case ui_menuItem_type_t::variType:
				(((ui_menuItem_variType_t*)it->ptr)->v) = (int16_t)(flashBuf[1]>>16);
				(((ui_menuItem_variType_t*)it->ptr)->e) = (int16_t)flashBuf[1];
				//sprintf(comBuf + strlen(comBuf),"vari %4.4x,%4.4x\r\n",(int16_t)(flashBuf[1]>>16),(int16_t)flashBuf[1]);
				break;
			case ui_menuItem_type_t::varfType:
				(((ui_menuItem_varfType_t*)it->ptr)->v) = (int16_t)(flashBuf[1]>>16);
				(((ui_menuItem_varfType_t*)it->ptr)->e) = (int16_t)flashBuf[1];
				//sprintf(comBuf + strlen(comBuf),"varf %4.4x,%4.4x\r\n",(int16_t)(flashBuf[1]>>16),(int16_t)flashBuf[1]);
				break;
			case ui_menuItem_type_t::procType:
				break;
			case ui_menuItem_type_t::menuType:
				bool list_push = true;
				for (deque<ui_menuList_t*>::iterator it2 = que.begin(); it2 != que.end(); it2++)
				{
					if((*it2) == (*(ui_menuItem_menuType_t *)(*it).ptr).ptr){list_push = false; break;}
				}
				if(list_push) {que.push_back((*(ui_menuItem_menuType_t *)(*it).ptr).ptr);}
				break;
			}
			com_tx_str(comBuf);
		}
	}
}
void ui_menuReadContent(void)
{
	uint16_t flashBlockIdx = UI_MENU_FLASH_BLOCK_OFFSET,flashSectorIdx = 0;
	uint32_t flashBuf[2] = {0,0};
	map<uint32_t,uint32_t> fdata;
	deque<ui_menuList_t *> que;
	que.push_back(&ui_menu_root);	//push root menu in queue
	uint32_t currentItemNum = 0;
	int32_t menuItemFound = 0, menuItemNotFound = 0;
	char strBuf[128];
	sprintf(strBuf,"\r\n>UI_MENU: Begin reading flash content...\r\n");
	com_tx_str(strBuf);
	while(1)
	{
		flashBlockIdx = currentItemNum / UI_MENU_FLASH_ITEM_PER_BLOCK + UI_MENU_FLASH_BLOCK_OFFSET;
		flashSectorIdx = (currentItemNum % UI_MENU_FLASH_ITEM_PER_BLOCK) * 16;
		flashBuf[0] = (uint32_t)flash_rx64(flashBlockIdx, flashSectorIdx);
		flashBuf[1] = (uint32_t)flash_rx64(flashBlockIdx, flashSectorIdx + 8);
		sprintf(strBuf,"flash read at B%2.2d,S%4.4d: 0x%8.8x,0x%8.8x\r\n",flashBlockIdx, flashSectorIdx, flashBuf[0], flashBuf[1]);
		com_tx_str(strBuf);
		++currentItemNum;
		if(flashBuf[0] == UI_MENU_FLASH_EOF_MSDW && flashBuf[1] == UI_MENU_FLASH_EOF_LSDW)//EOF
		{
			sprintf(strBuf,"EOF Reached at block %2.2d, sector %4.4d.\r\n",flashBlockIdx, flashSectorIdx);
			com_tx_str(strBuf);
			break;
		}
		if(fdata.find(flashBuf[0]) == fdata.end())
		{ 
			fdata[flashBuf[0]] = flashBuf[1]; 
		}
		else
		{
			sprintf(strBuf,"Menu Error! Duplicate hash found at block %2.2d, sector %3.3d.\r\n",flashBlockIdx, flashSectorIdx);
			com_tx_str(strBuf);
			//while(1);	//error! duplicate hash found.
		}
	}

	//start searching
	sprintf(strBuf,"\r\n>UI_MENU: Reading flash content End. Begin seeking for content.\r\n");
	com_tx_str(strBuf);

	for(uint8_t list=0; list<que.size(); list++)
	{
		for (vector<ui_menuItem_t>::iterator it = que[list]->menu.begin(); it != que[list]->menu.end(); ++it)
		{
			if(it->type == ui_menuItem_type_t::menuType)
			{
					bool list_push = true;
					for (deque<ui_menuList_t*>::iterator it2 = que.begin(); it2 != que.end(); it2++)
					{
						if((*it2) == (*(ui_menuItem_menuType_t *)(*it).ptr).ptr){list_push = false; break;}
					}
					if(list_push) {que.push_back((*(ui_menuItem_menuType_t *)(*it).ptr).ptr);}
					continue;
			}

				if(((uint32_t)ui_menuItem_propety_t::enableflashRW) != ((it->porpetyFlag) & ((uint32_t)ui_menuItem_propety_t::enableflashRW))){ continue; }
			
				auto search = fdata.find(it->hash());
				if(search == fdata.end())
				{
					++menuItemNotFound;
					sprintf(strBuf,"menuItem \" %16.16s \" not found.\r\n", it->name);
					com_tx_str(strBuf);
					continue;
				}	//not found.
				else
				{
					sprintf(strBuf,"menuItem \" %16.16s \" found. H: 0x%8.8x,V: 0x%8.8x", it->name,search->first,search->second);
					//com_tx_str(strBuf);
				}

				switch(it->type)
				{
				case ui_menuItem_type_t::variType:
					(((ui_menuItem_variType_t*)it->ptr)->v) = (int16_t)((search->second)>>16);
					(((ui_menuItem_variType_t*)it->ptr)->e) = (int16_t)(search->second);
					sprintf(strBuf + strlen(strBuf),"  value= %+4.4de%+1.1d\r\n", (((ui_menuItem_variType_t*)it->ptr)->v),(((ui_menuItem_variType_t*)it->ptr)->e));
					break;
				case ui_menuItem_type_t::varfType:
					(((ui_menuItem_varfType_t*)it->ptr)->v) = (int16_t)((search->second)>>16);
					(((ui_menuItem_varfType_t*)it->ptr)->e) = (int16_t)(search->second);
					sprintf(strBuf + strlen(strBuf),"  value= %+4.4de%+1.1d\r\n", (((ui_menuItem_varfType_t*)it->ptr)->v),(((ui_menuItem_varfType_t*)it->ptr)->e));
					break;
				default:
					sprintf(strBuf + strlen(strBuf),"\r\n");
					break;
				}
				com_tx_str(strBuf);
		}
	}
	sprintf(strBuf,">UI_MENU: Reading flash content End. Total %d menuItem(s) not found.\r\n\r\n",menuItemNotFound);
	com_tx_str(strBuf);
}
void ui_menuSaveContent(void)
{
	uint16_t flashBlockIdx = 0,flashSectorIdx = 0;
	uint32_t flashBuf[2] = {0,0};
	deque<ui_menuList_t*> que(0);
	que.push_back(&ui_menu_root);	//push root menu in queue
	uint32_t currentItemNum = 0;
	char comBuf[128] = "";
	
	sprintf(comBuf,"\r\n>UI_MENU: Begin saving flash content...\r\n");
	com_tx_str(comBuf);
	//erase reserved blocks, caution: "flashBlockIdx" is not the physical address in this scope!
	for(flashBlockIdx = 0;flashBlockIdx<UI_MENU_FLASH_RESERVE_BLOCK;flashBlockIdx++)
	{
		flash_erase_sector(flashBlockIdx + UI_MENU_FLASH_BLOCK_OFFSET);
	}
	
	//start writing

	for(uint8_t list=0; list<que.size(); list++)
	{
		for (vector<ui_menuItem_t>::iterator it = que[list]->menu.begin(); it != que[list]->menu.end(); it++)
		{
			//if(~(it->porpetyFlag & ((uint32_t)ui_menuItem_propety_t::enableflashRW))){continue;}
			//calculate blockIdx and sectorIdx. caution: this"flashBlockIdx" is the physical address.
			flashBlockIdx = currentItemNum / UI_MENU_FLASH_ITEM_PER_BLOCK + UI_MENU_FLASH_BLOCK_OFFSET;
			flashSectorIdx = (currentItemNum % UI_MENU_FLASH_ITEM_PER_BLOCK) * 16;
			it->flashBlockIdx = flashBlockIdx;
			it->flashSectorIdx = flashSectorIdx;
			flashBuf[0] = flashBuf[1] = 0;
			flashBuf[0] = (*it).hash();
			
			switch((*it).type)
			{
			case ui_menuItem_type_t::nullType:
				break;
			case ui_menuItem_type_t::variType:
				flashBuf[1] |= ((uint16_t)((ui_menuItem_variType_t*)it->ptr)->v);
				flashBuf[1] = flashBuf[1] << 16;
				flashBuf[1] |= ((uint16_t)((ui_menuItem_variType_t*)it->ptr)->e);
				//sprintf(comBuf + strlen(comBuf),"vari %8.8x,%8.8x\r\n",(uint16_t)(((ui_menuItem_variType_t*)it->ptr)->v),(uint16_t)(((ui_menuItem_variType_t*)it->ptr)->e));
				break;
			case ui_menuItem_type_t::varfType:
				flashBuf[1] |= ((uint16_t)((ui_menuItem_varfType_t*)it->ptr)->v);
				flashBuf[1] = flashBuf[1] << 16;
				flashBuf[1] |= ((uint16_t)((ui_menuItem_varfType_t*)it->ptr)->e);
				//sprintf(comBuf + strlen(comBuf),"varf %8.8x,%8.8x\r\n",(uint16_t)(((ui_menuItem_varfType_t*)it->ptr)->v),(uint16_t)(((ui_menuItem_varfType_t*)it->ptr)->e));
				break;
			case ui_menuItem_type_t::procType:
				break;
			case ui_menuItem_type_t::menuType:
				bool list_push = true;
				for (deque<ui_menuList_t*>::iterator it2 = que.begin(); it2 != que.end(); it2++)
				{
					if((*it2) == (*(ui_menuItem_menuType_t *)(*it).ptr).ptr){list_push = false; break;}
				}
				if(list_push) {que.push_back((*(ui_menuItem_menuType_t *)(*it).ptr).ptr);}
				break;
			}
			
			sprintf(comBuf,"Write item%3.3d name:%16.16s B:%2.2d,S:%4.4d: 0x%8.8x,0x%8.8x\r\n",currentItemNum,it->name,flashBlockIdx,flashSectorIdx,flashBuf[0],flashBuf[1]);
			//sprintf(comBuf + strlen(comBuf),"hash=0x%8.8x,fhash=0x%8.8x\r\n\r\n",((*it).hash()),(flashBuf[0]));
			com_tx_str(comBuf);
			flash_tx64(flashBlockIdx, flashSectorIdx, flashBuf[0]);
			flash_tx64(flashBlockIdx, flashSectorIdx + 8, flashBuf[1]);
			currentItemNum++;
		}
	}
	//TODO:print EOF in the next sector.
	//++currentItemNum;
	flashBlockIdx = currentItemNum / UI_MENU_FLASH_ITEM_PER_BLOCK + UI_MENU_FLASH_BLOCK_OFFSET;
	flashSectorIdx = (currentItemNum % UI_MENU_FLASH_ITEM_PER_BLOCK) * 16;
	flash_tx64(flashBlockIdx, flashSectorIdx, UI_MENU_FLASH_EOF_MSDW);
	flash_tx64(flashBlockIdx, flashSectorIdx + 8, UI_MENU_FLASH_EOF_LSDW);
	sprintf(comBuf,"EOF set at block %d,sector %d\r\n",flashBlockIdx, flashSectorIdx);
	com_tx_str(comBuf);
	sprintf(comBuf,">UI_MENU: Save flash content End.\r\n\r\n");
	com_tx_str(comBuf);
}

void ui_menuGetSaveContent(void)
{
	//DisableInterrupts;
	ui_menuGetContent();
	ui_menuSaveContent();
	//EnableInterrupts;
}
void ui_menuReadSetContent(void)
{
	//DisableInterrupts;
	ui_menuReadContent();
	ui_menuSetContent();
	//EnableInterrupts;
}

ui_menuItem_t::ui_menuItem_t(void)
{
	//uid = cnt++;
	ptr = NULL;
	lid = 0;
	type = ui_menuItem_type_t::nullType;
	//name = "\0";
	strcpy(name, "\0");
}

ui_menuItem_t::ui_menuItem_t(const char* uname, void * uptr, ui_menuItem_type_t utype, uint32_t propety)
{
	assert(uptr);
	//uid = cnt++;
	//cur = 0;
	porpetyFlag = propety;
	ptr = uptr;
	//lid = uid;
	type = utype;
	switch (type)
	{
	case ui_menuItem_type_t::variType:
		ptr = malloc(sizeof(ui_menuItem_variType_t));
		(*(ui_menuItem_variType_t*)ptr) = ui_menuItem_variType_t((int32_t*)uptr);
		break;
	case ui_menuItem_type_t::varfType:
		ptr = malloc(sizeof(ui_menuItem_varfType_t));
		(*(ui_menuItem_varfType_t*)ptr) = ui_menuItem_varfType_t((float*)uptr);
		break;
	case ui_menuItem_type_t::procType:
		ptr = malloc(sizeof(ui_menuItem_procType_t));
		(*(ui_menuItem_procType_t*)ptr) = ui_menuItem_procType_t((ui_menuItem_procHandler_t)uptr);
		break;
	case ui_menuItem_type_t::menuType:
		ptr = malloc(sizeof(ui_menuItem_menuType_t));
		(*(ui_menuItem_menuType_t*)ptr) = ui_menuItem_menuType_t((ui_menuList_t*)uptr);
		break;
	}
	//name = uname;
	strcpy(name, uname);
	getContent();
}

ui_menuItem_t::~ui_menuItem_t(void)
{
	//free(ptr);
	//cnt--;
}

uint32_t ui_menuItem_t::hash(void)
{
	uint32_t ret = 0;
	for(uint32_t i=0;i<nameLen;i++)
	{
		ret += (i+1)*((uint32_t)name[i]);
	}
	ret |= ((uint32_t)lid << 16);
	ret |= (((uint32_t)type)<<24);
	return ret;
}


//var operation
void ui_menuItem_t::getContent(void)
{
	//if (ptr == NULL || type == ui_menuItem_type_t::nullType) { return; }
	switch (type)
	{
	case ui_menuItem_type_t::variType:
		(*(ui_menuItem_variType_t*)ptr).getContent();
		break;
	case ui_menuItem_type_t::varfType:
		(*(ui_menuItem_varfType_t*)ptr).getContent();
		break;
	}
}
void ui_menuItem_t::setContent(void) {
	//if (ptr == NULL || type == ui_menuItem_type_t::nullType) { return; }
	switch (type)
	{
	case ui_menuItem_type_t::variType:
		(*(ui_menuItem_variType_t*)ptr).setContent();
		break;
	case ui_menuItem_type_t::varfType:
		(*(ui_menuItem_varfType_t*)ptr).setContent();
		break;
	}
}
//sdcard operation
inline void ui_menuItem_t::readContent(void)
{
	//read var from sepecified sdcard file HERE. 
}
inline void ui_menuItem_t::saveContent(void)
{
	//write var to sepecified sdcard file HERE. 
}

void ui_menuItem_t::print_oled(void)
{
	//print menuItem page itself
	char strbuf[20];
	
	switch (type)
	{
	case ui_menuItem_type_t::variType:
		sprintf(strbuf, "vari-%s",name);
		oled_print_font_6x8(0,0,(uint8_t*)strbuf);
		(*(ui_menuItem_variType_t*)ptr).print_oled();
		break;
	case ui_menuItem_type_t::varfType:
		sprintf(strbuf, "varf-%s",name);
		oled_print_font_6x8(0,0,(uint8_t*)strbuf);
		(*(ui_menuItem_varfType_t*)ptr).print_oled();
		break;
	case ui_menuItem_type_t::procType:
		sprintf(strbuf, "proc-%s",name);
		oled_print_font_6x8(0,0,(uint8_t*)strbuf);
		(*(ui_menuItem_procType_t*)ptr).print_oled();
		break;
	case ui_menuItem_type_t::menuType:
		//(*(ui_menuItem_menuType_t*)ptr).print_oled();
		break;
	}
}

void ui_menuItem_t::key_op(key_btOp_t *_op)
{
	switch (*_op)
	{
	default:
		switch (type)
		{
		case ui_menuItem_type_t::variType:
			(*(ui_menuItem_variType_t*)ptr).key_op(_op);
			break;
		case ui_menuItem_type_t::varfType:
			(*(ui_menuItem_varfType_t*)ptr).key_op(_op);
			break;
		case ui_menuItem_type_t::procType:
			(*(ui_menuItem_procType_t*)ptr).key_op(_op);
			break;
		case ui_menuItem_type_t::menuType:
			//ui_menu_currList = (*(ui_menuItem_menuType_t*)ptr).ptr;
			//ui_menu_currList = NULL;
			break;
		}
	}
}








//*************************
ui_menuItem_variType_t::ui_menuItem_variType_t(int32_t* _ptr)
{
	assert(_ptr);
	ptr = _ptr;
	cur = 0;
	//getContent();
}
//var operation
void ui_menuItem_variType_t::writeContent(int16_t _v, int16_t _e)
{
	v = _v; e = _e;
	setContent();
}
void ui_menuItem_variType_t::writeContent(int32_t _var)
{
	*ptr = _var;
	getContent();
}
//menu operation
void ui_menuItem_variType_t::getContent(void)
{
	v = *ptr; e = 0;
	while (-10 < e&&e < 10) {
		if (v > 9999) { v *= 0.1; e++; continue; }
		if (v < -9999) { v *= 0.1; e++; continue; }
		break;
	}
}
void ui_menuItem_variType_t::setContent(void)
{
	int32_t V = v, E = e;
	while (1) {
		if (E > 0) { V *= 10; E--; continue; }
		if (E < 0) { V *= 0.1; E++; continue; }
		break;
	}
	*ptr = V;
}
//sdcard operation
void ui_menuItem_variType_t::readContent(void)
{

}
void ui_menuItem_variType_t::saveContent(void)
{

}
//api
void ui_menuItem_variType_t::print_oled(void)
{
	char strbuf[20];
	sprintf(strbuf, "%+4.4de%+1.1d",v,e);
	oled_print_font_8x16(28,3,(uint8_t*)strbuf);
	if(cur == -1)
	{
		oled_print_font_6x8(84,5,(uint8_t*)"^");
	}
	else
	{
		oled_print_font_6x8(60-cur*8,5,(uint8_t*)"^");
	}
}
void ui_menuItem_variType_t::key_op(key_btOp_t *_op)
{
	if (*_op == key_btOp_t::LOK)
	{
		setContent();
		ui_menu_currItem = NULL;
		drvkey_btKey[KEY_BTOK].ms_cnt -= BT_TIME_REPT;
	}
	else if (*_op == key_btOp_t::SOK)
	{
		getContent();
		ui_menu_currItem = NULL;
		drvkey_btKey[KEY_BTOK].ms_cnt -= BT_TIME_REPT;
	}
	else if (*_op == key_btOp_t::SUP)
	{
		switch (cur)
		{
		case -1:
			e += 1; break;
		case 0:
			v += 1; break;
		case 1:
			v += 10; break;
		case 2:
			v += 100; break;
		case 3:
			v += 1000; break;
		}
	}
	else if (*_op == key_btOp_t::SDN)
	{
		switch (cur)
		{
		case -1:
			e -= 1; break; 
		case 0:
			v -= 1; break;
		case 1:
			v -= 10; break;
		case 2:
			v -= 100; break;
		case 3:
			v -= 1000; break;
		}
	}
	else if (*_op == key_btOp_t::LUP)
	{
		switch (cur)
		{
		case -1:
			e += 2; break;
		case 0:
			v += 2; break;
		case 1:
			v += 20; break;
		case 2:
			v += 200; break;
		case 3:
			v += 2000; break;
		}
	}
	else if (*_op == key_btOp_t::LDN)
	{
		switch (cur)
		{
		case -1:
			if (e > 1) { e -= 2; break; }
		case 0:
			v -= 2; break;
		case 1:
			v -= 20; break;
		case 2:
			v -= 200; break;
		case 3:
			v -= 2000; break;
		}
	}
	else if (*_op == key_btOp_t::SLF || *_op == key_btOp_t::LLF)
	{
		cur++;
		if (cur > 3) { cur = 3; }
	}
	else if (*_op == key_btOp_t::SRT || *_op == key_btOp_t::LRT)
	{
		cur--;
		if (cur < -1) { cur = -1; }
	}
	//v & e ranging
	while (v > 9999 || v < -9999 || e < 0)
	{
		v *= 0.1;
		e++;
	}
	while (((v > 0 && v < 1000) || (v < 0 && v > -1000)) && e > 0)
	{
		v *= 10;
		e--;
	}
}







//*********************************
ui_menuItem_varfType_t::ui_menuItem_varfType_t(float* _ptr)
{
	assert(_ptr);
	ptr = _ptr;
	cur = 0;
	//getContent();
}
//var operation
void ui_menuItem_varfType_t::writeContent(int16_t _v, int16_t _e)
{
	v = _v; e = _e;
	setContent();
}
void ui_menuItem_varfType_t::writeContent(float _var)
{
	*ptr = _var;
	getContent();
}
//menu operation
void ui_menuItem_varfType_t::getContent(void)
{
	float vv = *ptr; e = 0;
	while (-10 < e&&e < 10) {
		if (vv > 0.0 && vv < 999.0) { vv *= 10.0; e--; continue; }
		if (vv > 9999.0) { vv /= 10.0; e++; continue; }
		if (vv < 0.0 && vv > -999.0) { vv *= 10.0; e--; continue; }
		if (vv < -9999.0) { vv /= 10.0; e++; continue; }
		break;
	}
	v = (int32_t)vv;
}
void ui_menuItem_varfType_t::setContent(void)
{
	float V = v;
	int32_t E = e;
	while (1) {
		if (E > 0) { V *= 10.0; E--; continue; }
		if (E < 0) { V /= 10.0; E++; continue; }
		break;
	}
	*ptr = V;
}
//sdcard operation
void ui_menuItem_varfType_t::readContent(void)
{

}
void ui_menuItem_varfType_t::saveContent(void)
{

}
//api
void ui_menuItem_varfType_t::print_oled(void)
{
	char strbuf[20];
	sprintf(strbuf, "%+4.4de%+1.1d",v,e);
	oled_print_font_8x16(28,3,(uint8_t*)strbuf);
	oled_print_font_6x8(36,5,(uint8_t*)"       ");
	if(cur == -1)
	{
		oled_print_font_6x8(84,5,(uint8_t*)"^");
	}
	else
	{
		oled_print_font_6x8(60-cur*8,5,(uint8_t*)"^");
	}
}
void ui_menuItem_varfType_t::key_op(key_btOp_t *_op)
{
	if (*_op == key_btOp_t::LOK)
	{
		setContent();
		ui_menu_currItem = NULL;
		drvkey_btKey[KEY_BTOK].ms_cnt -= BT_TIME_REPT;
	}
	else if (*_op == key_btOp_t::SOK)
	{
		getContent();
		ui_menu_currItem = NULL;
		drvkey_btKey[KEY_BTOK].ms_cnt -= BT_TIME_REPT;
	}
	else if (*_op == key_btOp_t::SUP)
	{
		switch (cur)
		{
		case -1:
			e += 1; break;
		case 0:
			v += 1; break;
		case 1:
			v += 10; break;
		case 2:
			v += 100; break;
		case 3:
			v += 1000; break;
		}
	}
	else if (*_op == key_btOp_t::SDN)
	{
		switch (cur)
		{
		case -1:
			e -= 1; break;
		case 0:
			v -= 1; break;
		case 1:
			v -= 10; break;
		case 2:
			v -= 100; break;
		case 3:
			v -= 1000; break;
		}
	}
	else if (*_op == key_btOp_t::LUP)
	{
		switch (cur)
		{
		case -1:
			e += 2; break;
		case 0:
			v += 2; break;
		case 1:
			v += 20; break;
		case 2:
			v += 200; break;
		case 3:
			v += 2000; break;
		}
	}
	else if (*_op == key_btOp_t::LDN)
	{
		switch (cur)
		{
		case -1:
			if (e > 1) { e -= 2; break; }
		case 0:
			v -= 2; break;
		case 1:
			v -= 20; break;
		case 2:
			v -= 200; break;
		case 3:
			v -= 2000; break;
		}
	}
	else if (*_op == key_btOp_t::SLF || *_op == key_btOp_t::LLF)
	{
		cur++;
		if (cur > 3) { cur = 3; }
	}
	else if (*_op == key_btOp_t::SRT || *_op == key_btOp_t::LRT)
	{
		cur--;
		if (cur < -1) { cur = -1; }
	}
	//v & e ranging
	while (v > 9999 || v < -9999)
	{
		v /= 10;
		e++;
	}
	while ((v > 0 && v < 1000) || (v < 0 && v > -1000))
	{
		v *= 10;
		e--;
	}
}








//****************************
ui_menuItem_procType_t::ui_menuItem_procType_t(ui_menuItem_procHandler_t _ptr)
{
	assert(_ptr);
	ptr = _ptr;
	handlerReturn = 0;
	returnEnable = false;
}
void ui_menuItem_procType_t::print_oled(void)
{
	oled_print_font_6x8(10,2,(uint8_t*)"LOK to run>");
	oled_print_font_6x8(10,3,(uint8_t*)"SOK to exit");
}
void ui_menuItem_procType_t::key_op(key_btOp_t *_op)
{
	if (*_op == key_btOp_t::SOK)
	{
		ui_menu_currItem = NULL;
		//drvkey_btKey[KEY_BTOK].ms_cnt -= BT_TIME_REPT;
		return;
	}
	if(returnEnable)
	{	
		handlerReturn = (*ptr)(_op);
	}
	else
	{
		(*ptr)(_op);
	}
	if (*_op == key_btOp_t::LOK)
	{
		ui_menu_currItem = NULL;
		drvkey_btKey[KEY_BTOK].ms_cnt -= BT_TIME_REPT;
	}
}










//**********************************
ui_menuItem_menuType_t::ui_menuItem_menuType_t(ui_menuList_t* _ptr)
{
	assert(_ptr);
	ptr = _ptr;
	//cur = 0;
	//getContent();
}









//****************************************
ui_menuList_t::ui_menuList_t(void)
{
	//uid = cnt++;
	//menu = (ui_menuItem_t*)malloc(size * sizeof(ui_menuItem_t));
	disp_p = slct_p = 0;
	//name = "no_name";
	strcpy(name, "noname");
}
ui_menuList_t::ui_menuList_t(const char* _name, ui_menuList_t* _prev)
{
	//uid = cnt++;
	//menu = (ui_menuItem_t*)malloc(size * sizeof(ui_menuItem_t));
	insert(ui_menuItem_t("<back>...   ", _prev, ui_menuItem_type_t::menuType, false));
	disp_p = slct_p = 0;
	//name = _name;	//this will cause problem !
	strcpy(name,_name);
}

void ui_menuList_t::insert(ui_menuItem_t item)
{
  	//uint32_t sz = menu.size();
	item.lid = menu.size();
	menu.push_back(item);
}

void ui_menuList_t::print_oled(void)
{
	//***print menuList itself***
	char strbuf[24];
	//TO DO ---print tilte in the first row---
	sprintf(strbuf, "#%s#          ",name);
	oled_print_font_6x8(0, 0, (uint8_t*)strbuf);
	uint32_t disp_num = UI_MENU_MAX_ROW-1;
	if(disp_num > menu.size()){disp_num = menu.size();}
        uint32_t i = 0;
	for (i = 0; i < disp_num; i++)
	{
		switch (menu[disp_p + i].type)
		{
		case ui_menuItem_type_t::variType:
			//sprintf(strbuf,"vari test");
			(*((ui_menuItem_variType_t*)menu[disp_p + i].ptr)).getContent();
			sprintf(strbuf, " %02di %6.6s %+4.4de%+1.1d", menu[disp_p + i].lid, menu[disp_p + i].name, (*((ui_menuItem_variType_t*)menu[disp_p + i].ptr)).v, (*((ui_menuItem_variType_t*)menu[disp_p + i].ptr)).e);
			break;
		case ui_menuItem_type_t::varfType:
			//sprintf(strbuf,"varf test");
			(*((ui_menuItem_varfType_t*)menu[disp_p + i].ptr)).getContent();
			sprintf(strbuf, " %02df %6.6s %+4.4de%+1.1d", menu[disp_p + i].lid, menu[disp_p + i].name, (*((ui_menuItem_varfType_t*)menu[disp_p + i].ptr)).v, (*((ui_menuItem_varfType_t*)menu[disp_p + i].ptr)).e);
			break;
		case ui_menuItem_type_t::procType:
			sprintf(strbuf, " %02dp %8.8s <-->", menu[disp_p + i].lid, menu[disp_p + i].name);
			break;
		case ui_menuItem_type_t::menuType:
			//sprintf(strbuf,"menu test");
			sprintf(strbuf, " %02dm %8.8s ....", menu[disp_p + i].lid, menu[disp_p + i].name);
			break;
		default:
			sprintf(strbuf, " error");
			break;
		}
		//TO DO ---oled print menuItmes Here---
		//oled_print_font_6x8(0, i+1,"                      ");
		oled_print_font_6x8(0, i+1, (uint8_t*)strbuf);
	}
	//TO DO ---then print slct_p cursor---
	oled_print_font_6x8(1, slct_p-disp_p+1, (uint8_t*)">");
}
void ui_menuList_t::key_op(key_btOp_t *_op)
{

	if (*_op == key_btOp_t::SOK)			//select item
	{
		if(menu[slct_p].type == ui_menuItem_type_t::menuType)
		{
			ui_menu_currList = (*(ui_menuItem_menuType_t*)menu[slct_p].ptr).ptr;
		}
		else
		{
			ui_menu_currItem = &menu[slct_p];
			oled_fill(0);
		}
	}
	else if (*_op == key_btOp_t::LOK)		//exit
	{
		//slct_p = disp_p = 0;
		ui_menu_currList = (*(ui_menuItem_menuType_t*)(menu[0].ptr)).ptr;
		drvkey_btKey[KEY_BTOK].ms_cnt -= BT_TIME_REPT;
	}
	else if (*_op == key_btOp_t::SLF || *_op == key_btOp_t::LLF)
	{
		switch (menu[slct_p].type)
		{
		case ui_menuItem_type_t::variType:
			(*(ui_menuItem_variType_t *)menu[slct_p].ptr).v -= 100;
			(*(ui_menuItem_variType_t *)menu[slct_p].ptr).setContent();
			(*(ui_menuItem_variType_t *)menu[slct_p].ptr).getContent();
			break;
		case ui_menuItem_type_t::varfType:
			(*(ui_menuItem_varfType_t *)menu[slct_p].ptr).v -= 100;
			(*(ui_menuItem_varfType_t *)menu[slct_p].ptr).setContent();
			(*(ui_menuItem_varfType_t *)menu[slct_p].ptr).getContent();
			break;
		}
	}
	else if (*_op == key_btOp_t::SRT || *_op == key_btOp_t::LRT)
	{
		switch (menu[slct_p].type)
		{
		case ui_menuItem_type_t::variType:
			(*(ui_menuItem_variType_t *)menu[slct_p].ptr).v += 100;
			(*(ui_menuItem_variType_t *)menu[slct_p].ptr).setContent();
			(*(ui_menuItem_variType_t *)menu[slct_p].ptr).getContent();
			break;
		case ui_menuItem_type_t::varfType:
			(*(ui_menuItem_varfType_t *)menu[slct_p].ptr).v += 100;
			(*(ui_menuItem_varfType_t *)menu[slct_p].ptr).setContent();
			(*(ui_menuItem_varfType_t *)menu[slct_p].ptr).getContent();
			break;
		}
	}
	else if (*_op == key_btOp_t::SUP || *_op == key_btOp_t::LUP)	//roll up
	{
		if(slct_p > 0)
		{
			slct_p--;
			if (disp_p > slct_p) { disp_p = slct_p; }
		}
	}
	else if (*_op == key_btOp_t::SDN || *_op == key_btOp_t::LDN)	//roll dn
	{
		if(slct_p < menu.size() - 1)
		{
		slct_p++;
		if (disp_p + UI_MENU_MAX_ROW - 2 < slct_p ) { disp_p ++; }//this number may be incorrect.
		//while(disp_p <= slct_p - UI_MENU_MAX_ROW){disp_p++;}
		}
	}
	
}
