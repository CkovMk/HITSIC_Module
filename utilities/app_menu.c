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

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * 菜单定义
	 */

	//menu_t menuInst;
	menu_list_t *menu_currList;
	menu_itemIfce_t *menu_currItem;
	menu_list_t *menu_menuRoot;
	menu_list_t *menu_manageList;
	int32_t menu_currRegionNum;
	int32_t menu_statusFlag;
	uint32_t menu_nvm_statusFlagAddr;

	int32_t menu_nvmCopySrc = 0, menu_nvmCopyDst = 0;

	char menu_dispStrBuf[MENU_DISP_STRBUF_ROW][MENU_DISP_STRBUF_COL];

	menu_keyOp_t menu_keyOpBuff;

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
		menu_menuRoot->menu[0]->handle.p_menuType->data = menu_menuRoot;
		menu_currList = menu_menuRoot;
		menu_currItem = NULL;
		menu_currRegionNum = 0;
		menu_statusFlag = 0;

#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)
		menu_manageList = MENU_ListConstruct("Manager", 21, menu_menuRoot);
		assert(menu_manageList);
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(menuType, menu_manageList, "MenuManager", 0, 0));
		{
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(nullType, NULL, "", 0, 0));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(variType, &menu_currRegionNum, "RegnSel(0-2)", 0, menuItem_data_global | menuItem_data_NoSave));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmSave_Boxed, "Save Data", 0, menuItem_proc_runOnce));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmRead_Boxed, "Load Data", 0, menuItem_proc_runOnce));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmSaveRegionConfig_Boxed, "RegnSave", 0, menuItem_proc_runOnce));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(nullType, NULL, "", 0, 0));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(variType, &menu_nvmCopySrc, "CopySrc(0-2)", 1, menuItem_data_global | menuItem_data_NoSave));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(variType, &menu_nvmCopyDst, "CopyDst(0-2)", 2, menuItem_data_global | menuItem_data_NoSave));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmCopy_Boxed, "CopyData(S>D)", 0, menuItem_proc_runOnce));
			MENU_ListInsert(menu_manageList, MENU_ItemConstruct(nullType, NULL, "", 0, 0));
		}
#endif // ! HITSIC_MENU_USE_NVM
		MENU_DataSetUp();

		//read...

#if defined(HITSIC_MENU_USE_BUTTON) && (HITSIC_MENU_USE_BUTTON > 0)
		MENU_ButtonSetup();
#endif // ! HITSIC_MENU_USE_BUTTON

		NVIC_SetPriority(HITSIC_MENU_SERVICE_IRQn, HITSIC_MENU_SERVICE_IRQPrio);
		NVIC_EnableIRQ(HITSIC_MENU_SERVICE_IRQn);
		pitMgr_t* ptr = pitMgr_t::insert(250U, 7U, MENU_PitIsr, pitMgr_t::enable);
		assert(ptr);
	}

	__WEAK void MENU_DataSetUp(void)
	{
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(nullType, NULL, "", 0, 0));

		static int32_t T_int = 253000;
		static float T_flt = 32.768;
		static menu_list_t *testList;
		testList = MENU_ListConstruct("TestList", 50, menu_menuRoot);
		assert(testList);
		MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(menuType, testList, "TestList", 0, 0));
		{
			MENU_ListInsert(testList, MENU_ItemConstruct(variType, &T_int, "T_int", 1, menuItem_data_region));
			MENU_ListInsert(testList, MENU_ItemConstruct(varfType, &T_flt, "T_float", 2, menuItem_data_region));
		}
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
			HITSIC_MENU_PRINTF("Warning: MENU: KeyOp remained unclear. OP = %d\n", *_op);
		}
		menu_statusFlag &= ~menu_message_buttonOp;
		menu_statusFlag |= menu_message_printDisp;
	}

#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)

	void MENU_Data_NvmSave(int32_t _region)
	{
		if(_region < 0 || _region >= HITSIC_MENU_NVM_REGION_CNT) { return; }
		HITSIC_MENU_PRINTF("\nVerbose: MENU: Nvm Save Begin.\n");
		menu_list_t **listQue = (menu_list_t**)calloc(menu_listCnt, sizeof(menu_list_t *));
		listQue[0] = menu_manageList;
		listQue[1] = menu_menuRoot;
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Global Data.\n");
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Add list [%s].\n", menu_manageList->nameStr);
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Add list [%s].\n", menu_menuRoot->nameStr);
		for (int listNum = 0; listNum < menu_listCnt; ++listNum)
		{
			HITSIC_MENU_PRINTF("Verbose: MENU: Nvm In List [%s]:\n", listQue[listNum]->nameStr);
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
							HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Add list [%s].\n", listQue[j]->nameStr);
							break;
						}
					}
					continue;
				}

				if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoSave) && !(thisItem->pptFlag & menuItem_data_ROFlag))
				{
					MENU_ItemGetData(thisItem, &dataBuf);
					HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Get Data.  menu: %-16.16s addr: %4.4d data: %8.8x .\n", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
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
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Global Data End.\n");
		if (menu_currRegionNum < 0 || menu_currRegionNum > 2)
		{
			return;
		}
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Region%d Data.\n", menu_currRegionNum);
		for (int listNum = 0; listNum < menu_listCnt; ++listNum)
		{
			HITSIC_MENU_PRINTF("Verbose: MENU: Nvm In List [%s].\n", listQue[listNum]->nameStr);
			for (int i = 0; i < listQue[listNum]->listNum; ++i)
			{
				menu_nvmData_t dataBuf;
				menu_itemIfce_t *thisItem = listQue[listNum]->menu[i];
				if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoSave) && !(thisItem->pptFlag & menuItem_data_ROFlag))
				{
					MENU_ItemGetData(thisItem, &dataBuf);
					HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Get Data.  menu: %-16.16s addr: %4.4d data: 0x%8.8x .\n", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
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
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Region%d Data End.\n", menu_currRegionNum);
		MENU_NvmUpdateCache();
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Save Complete.\n\n");
	}

	void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op)
	{
		MENU_Data_NvmSave(menu_currRegionNum);
		*_op = 0;
	}

	void MENU_Data_NvmRead(int32_t _region)
	{
		if(_region < 0 || _region >= HITSIC_MENU_NVM_REGION_CNT) { return; }
		HITSIC_MENU_PRINTF("\nVerbose: MENU: Nvm Read Begin.\n");
		menu_list_t **listQue = (menu_list_t**)calloc(menu_listCnt, sizeof(menu_list_t *));
		listQue[0] = menu_manageList;
		listQue[1] = menu_menuRoot;
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Global Data.\n");
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Add list [%s].\n", menu_manageList->nameStr);
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Add list [%s].\n", menu_menuRoot->nameStr);
		for (int listNum = 0; listNum < menu_listCnt; ++listNum)
		{
			HITSIC_MENU_PRINTF("Verbose: MENU: Nvm In List [%s].\n", listQue[listNum]->nameStr);
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
							HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Add list [%s].\n", listQue[j]->nameStr);
							break;
						}
					}
					continue;
				}

				if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoSave) && !(thisItem->pptFlag & menuItem_data_ROFlag))
				{
					uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
					MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
					HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Get Flash. menu: %-16.16s addr: %4.4d data: %8.8x .\n", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
					MENU_ItemSetData(thisItem, &dataBuf);
					HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Set Data.  menu: %-16.16s addr: %4.4d .\n", thisItem->nameStr, thisItem->saveAddr);
				}
			}
		}
		if (menu_currRegionNum < 0 || menu_currRegionNum > 2)
		{
			return;
		}
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Region%d Data.\n", menu_currRegionNum);
		for (int listNum = 0; listNum < menu_listCnt; ++listNum)
		{
			for (int i = 0; i < listQue[listNum]->listNum; ++i)
			{
				menu_nvmData_t dataBuf;
				menu_itemIfce_t *thisItem = listQue[listNum]->menu[i];
				if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoSave) && !(thisItem->pptFlag & menuItem_data_ROFlag))
				{
					uint32_t realAddr = menu_nvm_rgAddrOffset[_region] + thisItem->saveAddr * sizeof(menu_nvmData_t);
					MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
					HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Get Flash. menu: %-16.16s addr: %4.4d data: 0x%8.8x .\n", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
					MENU_ItemSetData(thisItem, &dataBuf);
					HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Set Data.  menu: %-16.16s addr: %4.4d .\n", thisItem->nameStr, thisItem->saveAddr);
				}
			}
		}
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Region%d Data End.\n", menu_currRegionNum);
		HITSIC_MENU_PRINTF("Verbose: MENU: Nvm Read Complete.\n\n");
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

#endif // ! HITSIC_USE_APP_MENU
