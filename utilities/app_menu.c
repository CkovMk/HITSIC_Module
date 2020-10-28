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
 * @file 	:	app_menu.c
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1.1
 * 
 * @date 	:	v0.1-beta.0 2019.10.28
 * @date 		v0.1.1		2019.11.02
 * 
 * @note    :   依赖库：drv_ftfx_flash、drv_button、drv_disp_ssd1306
 依赖库必须先初始化。
 * @note 	:	预发布版本，代码不完整，仅供学习。
 */

#include "app_menu.h"

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)



	/**
	 * 菜单定义
	 */

	//menu_t menuInst;
	volatile menu_list_t *menu_currList;
	volatile menu_itemIfce_t *menu_currItem;
	menu_list_t *menu_menuRoot;
	menu_list_t *menu_manageList;
	volatile int32_t menu_currRegionNumAdj[3] = { 0, 0, HITSIC_MENU_NVM_REGION_CNT - 1 };
	volatile int32_t &menu_currRegionNum = menu_currRegionNumAdj[0];
	int32_t menu_statusFlag;
	uint32_t menu_nvm_statusFlagAddr;

	volatile int32_t menu_nvmCopySrcAdj[3] = { 0, 0, HITSIC_MENU_NVM_REGION_CNT - 1 };
	volatile int32_t menu_nvmCopyDstAdj[3] = { 0, 0, HITSIC_MENU_NVM_REGION_CNT - 1 };
	volatile int32_t &menu_nvmCopySrc = menu_nvmCopySrcAdj[0], &menu_nvmCopyDst = menu_nvmCopyDstAdj[0];

	char menu_dispStrBuf[MENU_DISP_STRBUF_ROW][MENU_DISP_STRBUF_COL];

	menu_keyOp_t menu_keyOpBuff;

	pitMgr_t* menu_pitHandle = NULL;
	volatile int32_t menu_suspendCnt = 0U;

	/**
	 * ********** 菜单顶层操作接口 **********
	 */

	void MENU_Init(void)
	{
#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)
		/**
		 * @brief : 全局存储 Global Storage
		 */
		menu_nvm_glAddrOffset = HITSIC_MENU_NVM_GLOBAL_SECT_OFFSET * HITSIC_MENU_NVM_SECTOR_SIZE;/// 全局存储区地址偏移
		/**
		 * @brief : 局部存储 Region Storage
		 */
		/// 三个局部存储区的扇区偏移
		{
			menu_nvm_rgSectOffset[0] = HITSIC_MENU_NVM_GLOBAL_SECT_OFFSET + HITSIC_MENU_NVM_GLOBAL_SECT_SIZE + 0u * HITSIC_MENU_NVM_REGION_SECT_SIZE;
			menu_nvm_rgSectOffset[1] = HITSIC_MENU_NVM_GLOBAL_SECT_OFFSET + HITSIC_MENU_NVM_GLOBAL_SECT_SIZE + 1u * HITSIC_MENU_NVM_REGION_SECT_SIZE;
			menu_nvm_rgSectOffset[2] = HITSIC_MENU_NVM_GLOBAL_SECT_OFFSET + HITSIC_MENU_NVM_GLOBAL_SECT_SIZE + 2u * HITSIC_MENU_NVM_REGION_SECT_SIZE;
		}
		/// 三个局部存储区的地址偏移
		{
			menu_nvm_rgAddrOffset[0] = menu_nvm_rgSectOffset[0] * HITSIC_MENU_NVM_SECTOR_SIZE;
			menu_nvm_rgAddrOffset[1] = menu_nvm_rgSectOffset[1] * HITSIC_MENU_NVM_SECTOR_SIZE;
			menu_nvm_rgAddrOffset[2] = menu_nvm_rgSectOffset[2] * HITSIC_MENU_NVM_SECTOR_SIZE;
		}
		/**
		 * @brief : 菜单存储占用的总扇区数
		 */
		menu_nvm_totalSectCnt = HITSIC_MENU_NVM_GLOBAL_SECT_SIZE + HITSIC_MENU_NVM_REGION_CNT * HITSIC_MENU_NVM_REGION_SECT_SIZE;
		/**
		 * @brief : 每个菜单项保存时占用的字节数
		 */
		menu_nvm_dataSize = 32u;

		menu_nvm_statusFlagAddr = menu_nvm_glAddrOffset + 0u;

#endif // ! HITSIC_MENU_USE_NVM

		menu_menuRoot = MENU_ListConstruct("MenuRoot", HITSIC_MENU_ROOT_SIZE, (menu_list_t *)1);
		assert(menu_menuRoot);
		MENU_itemDestruct(menu_menuRoot->menu[0]);
		--menu_menuRoot->listNum;
		//menu_menuRoot->menu[0]->handle.p_menuType->data = menu_menuRoot;
		menu_currList = menu_menuRoot;
		menu_currItem = NULL;
		menu_statusFlag = 0;

		menu_manageList = MENU_ListConstruct("MenuManager", 21, menu_menuRoot);
		assert(menu_manageList);
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(menuType, menu_manageList, "MenuManager", 0, 0));
		{
#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)
		    menu_itemIfce_t *p = NULL;
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(nullType, NULL, "SAVE", 0, 0));
			MENU_ListInsert(menu_manageList, p = MENU_ItemConstruct(variType, &menu_currRegionNum, "RegnSel(0-N)", 0, menuItem_data_global | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_dataExt_HasMinMax));
			p->nameStr[10] = '0' + HITSIC_MENU_NVM_REGION_CNT - 1;
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmSave_Boxed, "Save Data", 0, menuItem_proc_runOnce));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmRead_Boxed, "Load Data", 0, menuItem_proc_runOnce));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmSaveRegionConfig_Boxed, "RegnSave", 0, menuItem_proc_runOnce));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(nullType, NULL, "COPY", 0, 0));
			MENU_ListInsert(menu_manageList, p = MENU_ItemConstruct(variType, &menu_nvmCopySrc, "CopySrc(0-N)", 1, menuItem_data_global | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_dataExt_HasMinMax));
			p->nameStr[10] = '0' + HITSIC_MENU_NVM_REGION_CNT - 1;
			MENU_ListInsert(menu_manageList, p = MENU_ItemConstruct(variType, &menu_nvmCopyDst, "CopyDst(0-N)", 2, menuItem_data_global | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_dataExt_HasMinMax));
			p->nameStr[10] = '0' + HITSIC_MENU_NVM_REGION_CNT - 1;
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmCopy_Boxed, "CopyData(S>D)", 0, menuItem_proc_runOnce));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(nullType, NULL, "INFO", 0, 0));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(variType, &menu_nvm_eraseCnt, "EraseCnt", 3, menuItem_data_global |menuItem_data_ROFlag));

#endif // ! HITSIC_MENU_USE_NVM
		}
		MENU_DataSetUp();

		//read...

#if defined(HITSIC_MENU_USE_BUTTON) && (HITSIC_MENU_USE_BUTTON > 0)
		MENU_ButtonSetup();
#endif // ! HITSIC_MENU_USE_BUTTON

		NVIC_SetPriority(HITSIC_MENU_SERVICE_IRQn, HITSIC_MENU_SERVICE_IRQPrio);
		NVIC_EnableIRQ(HITSIC_MENU_SERVICE_IRQn);
		menu_pitHandle = pitMgr_t::insert(250U, 7U, MENU_PitIsr, pitMgr_t::enable);
		assert(menu_pitHandle);
		menu_suspendCnt = 0U;
	}

	__WEAK void MENU_DataSetUp(void)
	{
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(nullType, NULL, "DATA", 0, 0));

		static int32_t region_i = 4096, global_i = 1024, readonly_i = 1998;
		static float region_f = 32.768, global_f = 3.14, longname_f = 12.14;
		static int32_t forceSciData = 202000;
		static menu_list_t *testList;
		testList = MENU_ListConstruct("TestList", 50, menu_menuRoot);
		assert(testList);
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(variType, &readonly_i, "readonly", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(menuType, testList, "TestList", 0, 0));
		{
			MENU_ListInsert(testList, MENU_ItemConstruct(variType, &global_i, "global_i", 10, menuItem_data_global));
			MENU_ListInsert(testList, MENU_ItemConstruct(varfType, &global_f, "global_f", 11, menuItem_data_global));
			MENU_ListInsert(testList, MENU_ItemConstruct(variType, &region_i, "region_i", 1, menuItem_data_region));
			MENU_ListInsert(testList, MENU_ItemConstruct(varfType, &region_f, "region_f", 2, menuItem_data_region));
		}
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(varfType, &longname_f, "C.M.'s Birthday", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_disp_noPreview));
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(variType, &forceSciData, "forceSci", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_disp_forceSci));
	}

	void MENU_PrintDisp(void)
	{
		//update disp strbuf
		memset(menu_dispStrBuf, ' ', MENU_DISP_STRBUF_ROW * MENU_DISP_STRBUF_COL);
		if (menu_currItem == NULL)
		{
			MENU_ListPrintDisp(menu_currList);
		}
		else
		{
			MENU_ItemPrintDisp(menu_currItem);
		}
		//update display
		for (uint8_t i = 0; i < MENU_DISP_STRBUF_ROW; ++i)
		{
			menu_dispStrBuf[i][MENU_DISP_STRBUF_COL - 1] = '\0';
			HITSIC_MENU_DISPLAY_PRINT(1, i, menu_dispStrBuf[i]);
		}
		menu_statusFlag &= ~menu_message_printDisp;
	}

	void MENU_KeyOp(menu_keyOp_t *const _op)
	{
		if (menu_currItem == NULL)
		{
			MENU_ListKeyOp(menu_currList, _op);
		}
		else
		{
			MENU_ItemKeyOp(menu_currItem, _op);
		}
		if (*_op != 0)
		{
			MENU_LOG_W("KeyOp remained unclear. OP = %d", *_op);
		}
		menu_statusFlag &= ~menu_message_buttonOp;
		menu_statusFlag |= menu_message_printDisp;
	}

#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)

	void MENU_Data_NvmSave(int32_t _region)
	{
	    ++menu_nvm_eraseCnt;
		if(_region < 0 || _region >= HITSIC_MENU_NVM_REGION_CNT) { return; }
		MENU_NVM_LOG_I("Data Save Begin.");
		menu_list_t **listQue = (menu_list_t**)calloc(menu_listCnt, sizeof(menu_list_t *));
		listQue[0] = menu_manageList;
		listQue[1] = menu_menuRoot;
		MENU_NVM_LOG_D("Global Data.");
		MENU_NVM_LOG_D("Add list [%s].", menu_manageList->nameStr);
		MENU_NVM_LOG_D("Add list [%s].", menu_menuRoot->nameStr);
		for (int listNum = 0; listNum < menu_listCnt; ++listNum)
		{
		    MENU_NVM_LOG_D("In List [%s]:", listQue[listNum]->nameStr);
			assert(listQue[listNum]);
			static_assert(sizeof(menu_nvmData_t) == 32, "sizeof menu_nvmData_t error !");
			for (int i = 0; i < listQue[listNum]->listNum; ++i)
			{
				menu_nvmData_t dataBuf;
				menu_itemIfce_t *thisItem = listQue[listNum]->menu[i];
				if (thisItem->type == menuType)
				{
					for (int j = 0; j < menu_listCnt; ++j)
					{
						if (listQue[j] == thisItem->handle.p_menuType->data)
						{
							break;
						}
						else if (listQue[j] == NULL)
						{
							listQue[j] = thisItem->handle.p_menuType->data;
							MENU_NVM_LOG_D("Add list [%s].", listQue[j]->nameStr);
							break;
						}
					}
					continue;
				}

				if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoSave))
				{
					MENU_ItemGetData(thisItem, &dataBuf);
					MENU_NVM_LOG_D("Get Data.  menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
					uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
					if (!MENU_NvmCacheable(realAddr))
					{
						MENU_NvmUpdateCache();
						assert(MENU_NvmCacheable(realAddr));
					}
					MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
				}
			}
		}
		MENU_NVM_LOG_D("Global Data End.");
		if (menu_currRegionNum < 0 || menu_currRegionNum > 2)
		{
			return;
		}
		MENU_NVM_LOG_D("Nvm Region%d Data.", menu_currRegionNum);
		for (int listNum = 0; listNum < menu_listCnt; ++listNum)
		{
		    MENU_NVM_LOG_D("In List [%s].", listQue[listNum]->nameStr);
			for (int i = 0; i < listQue[listNum]->listNum; ++i)
			{
				menu_nvmData_t dataBuf;
				menu_itemIfce_t *thisItem = listQue[listNum]->menu[i];
				if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoSave))
				{
					MENU_ItemGetData(thisItem, &dataBuf);
					MENU_NVM_LOG_D("Get Data.  menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
					uint32_t realAddr = menu_nvm_rgAddrOffset[_region] + thisItem->saveAddr * sizeof(menu_nvmData_t);
					if (!MENU_NvmCacheable(realAddr))
					{
						MENU_NvmUpdateCache();
						assert(MENU_NvmCacheable(realAddr));
					}
					MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
				}
			}
		}
		MENU_NVM_LOG_D("Region%d Data End.", menu_currRegionNum);
		MENU_NvmUpdateCache();
		MENU_NVM_LOG_I("Save Complete.\n\n");
	}

	void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op)
	{
		MENU_Data_NvmSave(menu_currRegionNum);
		*_op = 0;
	}

	void MENU_Data_NvmRead(int32_t _region)
	{
		if(_region < 0 || _region >= HITSIC_MENU_NVM_REGION_CNT) { return; }
		MENU_NVM_LOG_I("Read Begin.");
		menu_list_t **listQue = (menu_list_t**)calloc(menu_listCnt, sizeof(menu_list_t *));
		listQue[0] = menu_manageList;
		listQue[1] = menu_menuRoot;
		MENU_NVM_LOG_D("Global Data.");
		MENU_NVM_LOG_D("Add list [%s].", menu_manageList->nameStr);
		MENU_NVM_LOG_D("Add list [%s].", menu_menuRoot->nameStr);
		for (int listNum = 0; listNum < menu_listCnt; ++listNum)
		{
		    MENU_NVM_LOG_D("In List [%s]:", listQue[listNum]->nameStr);
			assert(listQue[listNum]);
			static_assert(sizeof(menu_nvmData_t) == 32, "sizeof menu_nvmData_t error !");
			for (int i = 0; i < listQue[listNum]->listNum; ++i)
			{
				menu_nvmData_t dataBuf;
				menu_itemIfce_t *thisItem = listQue[listNum]->menu[i];
				if (thisItem->type == menuType)
				{
					for (int j = 0; j < menu_listCnt; ++j)
					{
						if (listQue[j] == thisItem->handle.p_menuType->data)
						{
							break;
						}
						else if (listQue[j] == NULL)
						{
							listQue[j] = thisItem->handle.p_menuType->data;
							MENU_NVM_LOG_D("Add list [%s].", listQue[j]->nameStr);
							break;
						}
					}
					continue;
				}

				if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoLoad))
				{
					uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
					MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
					MENU_NVM_LOG_D("Get Flash. menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
					MENU_ItemSetData(thisItem, &dataBuf);
					MENU_NVM_LOG_D("Set Data.  menu: %-16.16s addr: %-4.4d .", thisItem->nameStr, thisItem->saveAddr);
				}
			}
		}
		if (menu_currRegionNum < 0 || menu_currRegionNum > 2)
		{
			return;
		}
		MENU_NVM_LOG_D("Region%d Data.", menu_currRegionNum);
		for (int listNum = 0; listNum < menu_listCnt; ++listNum)
		{
			for (int i = 0; i < listQue[listNum]->listNum; ++i)
			{
				menu_nvmData_t dataBuf;
				menu_itemIfce_t *thisItem = listQue[listNum]->menu[i];
				if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoLoad))
				{
					uint32_t realAddr = menu_nvm_rgAddrOffset[_region] + thisItem->saveAddr * sizeof(menu_nvmData_t);
					MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
					MENU_NVM_LOG_D("Get Flash. menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
					MENU_ItemSetData(thisItem, &dataBuf);
					MENU_NVM_LOG_D("Set Data.  menu: %-16.16s addr: %-4.4d .", thisItem->nameStr, thisItem->saveAddr);
				}
			}
		}
		MENU_NVM_LOG_D("Region%d Data End.", menu_currRegionNum);
		MENU_NVM_LOG_I("Read Complete.\n\n");
	}

	void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op)
	{
		MENU_Data_NvmRead(menu_currRegionNum);
		*_op = 0;
	}

	void MENU_Data_NvmSaveRegionConfig(void)
	{
		menu_nvmData_t dataBuf;
		menu_itemIfce_t *thisItem = menu_manageList->menu[2];
		MENU_ItemGetData(thisItem, &dataBuf);
		uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
		if (!MENU_NvmCacheable(realAddr))
		{
			MENU_NvmUpdateCache();
			assert(MENU_NvmCacheable(realAddr));
		}
		MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
		MENU_NvmUpdateCache();
	}
	void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op)
	{
		MENU_Data_NvmSaveRegionConfig();
		*_op = 0;
	}

	void MENU_Data_NvmReadRegionConfig(void)
	{
		menu_nvmData_t dataBuf;
		menu_itemIfce_t *thisItem = menu_manageList->menu[2];
		uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
		MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
		MENU_ItemSetData(thisItem, &dataBuf);
	}

	void MENU_Data_NvmReadRegionConfig_Boxed(menu_keyOp_t *const _op)
	{
		MENU_Data_NvmReadRegionConfig();
		*_op = 0;
	}

	void MENU_Data_NvmCopy(int32_t _srcRegion, int32_t _dstRegion)
	{
		if ((_srcRegion == _dstRegion) || (_srcRegion < 0 || _srcRegion >= HITSIC_MENU_NVM_REGION_CNT) || (_dstRegion < 0 || _dstRegion >= HITSIC_MENU_NVM_REGION_CNT))
		{
			return;
		}
		++menu_nvm_eraseCnt;
		if (menu_nvm_cache != NULL)
		{
			MENU_NvmUpdateCache();
		}
		for (int i = 0; i < HITSIC_MENU_NVM_REGION_SECT_SIZE; ++i)
		{
			MENU_NvmCacheSector(menu_nvm_rgSectOffset[_srcRegion] + i);
			menu_nvm_cachedSector = menu_nvm_rgSectOffset[_dstRegion] + i;
			MENU_NvmUpdateCache();
		}
	}

	void MENU_Data_NvmCopy_Boxed(menu_keyOp_t *const _op)
	{
		MENU_Data_NvmCopy(menu_nvmCopySrc, menu_nvmCopyDst);
		*_op = 0;
	}

	int32_t MENU_GetNvmStatus(void)
	{
		MENU_NvmRead(menu_nvm_statusFlagAddr, (void *)&menu_statusFlag, sizeof(int32_t));
		return menu_statusFlag;
	}

	void MENU_SetNvmStatus(int32_t _status)
	{
		if (!MENU_NvmCacheable(menu_nvm_statusFlagAddr))
		{
			MENU_NvmUpdateCache();
		}
		MENU_NvmWriteCache(menu_nvm_statusFlagAddr, (void *)&menu_statusFlag, sizeof(int32_t));
	}

#endif // ! HITSIC_MENU_USE_NVM

	void MENU_PitIsr(void)
	{
		menu_statusFlag |= menu_message_printDisp;
		NVIC_SetPendingIRQ(HITSIC_MENU_SERVICE_IRQn);
	}

#ifdef __cplusplus
extern "C"
{
#endif

	void HITSIC_MENU_SERVICE_IRQHandler(void)
	{
		NVIC_ClearPendingIRQ(HITSIC_MENU_SERVICE_IRQn);
		if (menu_statusFlag & menu_message_buttonOp)
		{
			//HITSIC_MENU_PRINTF("Verbose: MENU: Key %d pressed.\n", menu_keyOpBuff);
			MENU_KeyOp(&menu_keyOpBuff);
		}
		if (menu_statusFlag & menu_message_printDisp)
		{
			MENU_PrintDisp();
		}
	}

#ifdef __cplusplus
}
#endif

	void MENU_Suspend(void)
	{
		assert(menu_pitHandle);
		if (0U == menu_suspendCnt)
		{
			menu_pitHandle->setEnable(false);
			NVIC_DisableIRQ(HITSIC_MENU_SERVICE_IRQn);
			MENU_LOG_I("Suspended.");
		}
		++menu_suspendCnt;
		MENU_LOG_D("SuspendCnt = %d", menu_suspendCnt);
	}

	void MENU_Resume(void)
	{
		assert(menu_pitHandle);
		--menu_suspendCnt;
		MENU_LOG_D("SuspendCnt = %d", menu_suspendCnt);
		if (0U <= menu_suspendCnt)
		{
			menu_suspendCnt = 0;
			menu_statusFlag = 0;
			menu_pitHandle->setEnable(true);
			NVIC_EnableIRQ(HITSIC_MENU_SERVICE_IRQn);
			MENU_LOG_I("Resumed.");
		}
	}



#endif // ! HITSIC_USE_APP_MENU
