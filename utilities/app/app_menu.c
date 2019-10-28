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
 * @author  :	Chekhov Mark(qq:905497173)
 * @version :	v0.1.0-beta.1
 * @date 	:	v0.1.0-beta.1 2019.10.28
 * 
 * @note 	:	预发布版本，代码不完整，仅供学习。
 */

#include "app_menu.h"

#ifdef __cplusplus
extern "C"
{
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
		//send out signal...
		//TODO: fix this. set key_op flag HERE !
		NVIC_SetPendingIRQ(HITSIC_MENU_SERVICE_IRQn);
	}

	/**
	 * @brief : 菜单项和菜单列表的计数器。
	 */
	uint32_t menu_itemCnt = 0;
	uint32_t menu_listCnt = 0;

	/**
	 * @brief : 占位类型菜单项的操作句柄及操作函数。
	 */
	void MENU_itemConstruct_nullType(menu_item_nullHandle_t *_item, void *_data)
	{
		_item = malloc(sizeof(menu_item_nullHandle_t));
		_item->data = _data;
	}
	void MENU_ItemGetData_nullType(menu_item_nullHandle_t *_item, void *_data)
	{
		//blank
	}
	void MENU_ItemSetData_nullType(menu_item_nullHandle_t *_item, void *_data)
	{
		//blank
	}
	//used when in menuList
	void MENU_ItemPrintSlot_nullType(menu_item_nullHandle_t *_item, uint32_t _slotNum)
	{
		//TODO: print blank HERE !.
	}
	void MENU_ItemDirectKeyOp_nullType(menu_item_nullHandle_t *_item, menu_keyOp_t *_op)
	{
		*_op = 0;
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_nullType(menu_item_nullHandle_t *_item)
	{
		assert(0); //should never end up here.
	}
	void MENU_ItemKeyOp_nullType(menu_item_nullHandle_t *_item, menu_keyOp_t *_op)
	{
		*_op = 0;
	}

	/**
	 * @brief : 整数类型菜单项的操作句柄及操作函数。
	 */
	void MENU_itemConstruct_variType(menu_item_variHandle_t *_item, void *_data)
	{
		_item = malloc(sizeof(menu_item_nullHandle_t));
		_item->data = _data;
	}
	void MENU_ItemGetData_variType(menu_item_variHandle_t *_item, void *_data)
	{
		*(int32_t *)_data = *(_item->data);
	}
	void MENU_ItemSetData_variType(menu_item_variHandle_t *_item, void *_data)
	{
		*(_item->data) = *(int32_t *)_data;
	}
	//used when in menuList
	void MENU_ItemPrintSlot_variType(menu_item_variHandle_t *_item, uint32_t _slotNum)
	{
		//TODO:fix this.
	}
	void MENU_ItemDirectKeyOp_variType(menu_item_variHandle_t *_item, menu_keyOp_t *_op)
	{
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_variType(menu_item_variHandle_t *_item)
	{
	}
	void MENU_ItemKeyOp_variType(menu_item_variHandle_t *_item, menu_keyOp_t *_op)
	{
	}

	/**
	 * @brief : 浮点类型菜单项的操作句柄及操作函数。
	 */
	void MENU_itemConstruct_varfType(menu_item_varfHandle_t *_item, void *_data)
	{
		_item = malloc(sizeof(menu_item_nullHandle_t));
		_item->data = _data;
	}
	void MENU_ItemGetData_varfType(menu_item_varfHandle_t *_item, void *_data)
	{
	}
	void MENU_ItemSetData_varfType(menu_item_varfHandle_t *_item, void *_data)
	{
	}
	//used when in menuList
	void MENU_ItemPrintSlot_varfType(menu_item_varfHandle_t *_item, uint32_t _slotNum)
	{
	}
	void MENU_ItemDirectKeyOp_varfType(menu_item_varfHandle_t *_item, menu_keyOp_t *_op)
	{
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_varfType(menu_item_varfHandle_t *_item)
	{
	}
	void MENU_ItemKeyOp_varfType(menu_item_varfHandle_t *_item, menu_keyOp_t *_op)
	{
	}

	/**
	 * @brief : 布尔类型菜单项的操作句柄及操作函数。
	 */
	void MENU_itemConstruct_boolType(menu_item_boolHandle_t *_item, void *_data)
	{
		_item = malloc(sizeof(menu_item_nullHandle_t));
		_item->data = _data;
	}
	void MENU_ItemGetData_boolType(menu_item_boolHandle_t *_item, void *_data)
	{
	}
	void MENU_ItemSetData_boolType(menu_item_boolHandle_t *_item, void *_data)
	{
	}
	//used when in menuList
	void MENU_ItemPrintSlot_boolType(menu_item_boolHandle_t *_item, uint32_t _slotNum)
	{
	}
	void MENU_ItemDirectKeyOp_boolType(menu_item_boolHandle_t *_item, menu_keyOp_t *_op)
	{
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_boolType(menu_item_boolHandle_t *_item)
	{
	}
	void MENU_ItemKeyOp_boolType(menu_item_boolHandle_t *_item, menu_keyOp_t *_op)
	{
	}

	/**
	 * @brief : 函数类型菜单项的操作句柄及操作函数。
	 */
	void MENU_itemConstruct_procType(menu_item_procHandle_t *_item, void *_data)
	{
		_item = malloc(sizeof(menu_item_nullHandle_t));
		_item->data = (void (*)(void))_data;
	}
	void MENU_ItemGetData_procType(menu_item_procHandle_t *_item, void *_data)
	{
	}
	void MENU_ItemSetData_procType(menu_item_procHandle_t *_item, void *_data)
	{
	}
	//used when in menuList
	void MENU_ItemPrintSlot_procType(menu_item_procHandle_t *_item, uint32_t _slotNum)
	{
	}
	void MENU_ItemDirectKeyOp_procType(menu_item_procHandle_t *_item, menu_keyOp_t *_op)
	{
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_procType(menu_item_procHandle_t *_item)
	{
	}
	void MENU_ItemKeyOp_procType(menu_item_procHandle_t *_item, menu_keyOp_t *_op)
	{
	}

	/**
	 * @brief : 跳转类型菜单项的操作句柄及操作函数。
	 */
	void MENU_itemConstruct_menuType(menu_item_menuHandle_t *_item, void *_data)
	{
		_item = malloc(sizeof(menu_item_nullHandle_t));
		_item->data = _data;
	}
	void MENU_ItemGetData_menuType(menu_item_menuHandle_t *_item, void *_data)
	{
	}
	void MENU_ItemSetData_menuType(menu_item_menuHandle_t *_item, void *_data)
	{
	}
	//used when in menuList
	void MENU_ItemPrintSlot_menuType(menu_item_menuHandle_t *_item, uint32_t _slotNum)
	{
	}
	void MENU_ItemDirectKeyOp_menuType(menu_item_menuHandle_t *_item, menu_keyOp_t *_op)
	{
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_menuType(menu_item_menuHandle_t *_item)
	{
	}
	void MENU_ItemKeyOp_menuType(menu_item_menuHandle_t *_item, menu_keyOp_t *_op)
	{
	}

	/**
	 * ********** 菜单项操作接口 **********
	 */

	menu_itemIfce_t *MENU_itemConstruct(menu_itemType_t _type, void *_data, const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag)
	{

		menu_itemIfce_t *item;
		item = calloc(1, sizeof(menu_itemIfce_t));
		assert(item);
		item->type = _type;
		item->pptFlag = _pptFlag;
		item->unique_id = menu_itemCnt++;
		item->saveAddr = _saveAddr;
		strlcpy(item->nameStr, _nameStr, menu_nameStrSize);
		item->nameStr[menu_nameStrSize - 1] = '\0';

		MENU_ITEM_SWITCH_CASE(MENU_itemConstruct, item, _data);
		return item;
	}

	void MENU_itemDestruct(menu_itemIfce_t *_item)
	{
		free(&_item->handle);        //TODO: fix this!
		free(_item);
		_item = NULL;
		--menu_itemCnt;
	}

	void MENU_ItemGetData(menu_itemIfce_t *_item, void *_data)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemGetData, _item, _data);
	}

	void MENU_ItemSetData(menu_itemIfce_t *_item, void *_data)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemSetData, _item, _data);
	}

	void MENU_ItemPrintSlot(menu_itemIfce_t *_item, uint32_t _slotNum)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemPrintSlot, _item, _slotNum);
	}

	void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *_op)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemDirectKeyOp, _item, _op);
	}

	void MENU_ItemPrintDisp(menu_itemIfce_t *_item)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemPrintDisp, _item);
	}

	void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *_op)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, _item, _op);
	}

	/**
	 * ********** 菜单列表操作接口 **********
	 */

	menu_list_t *MENU_ListConstruct(const char *_nameStr, uint32_t _size, menu_list_t *_prev)
	{
		assert(_prev);
		menu_list_t *list;
		list = calloc(1, sizeof(menu_list_t));
		assert(list);
		list->listSize = _size;
		list->listNum = 0;
		list->menu = calloc(_size, sizeof(menu_itemIfce_t*));
		assert(list->menu);
		strlcpy(list->nameStr, _nameStr, menu_nameStrSize);
		list->prev = _prev;

		MENU_ListInsert(list, MENU_itemConstruct(menuType, (void *)_prev, "<< back", 0, 0));
		++menu_listCnt;
		return list;
	}

	void MENU_ListDestruct(menu_list_t *_list)
	{
		free(_list->menu);
		free(_list);
		--menu_listCnt;
	}

	status_t MENU_ListInsert(menu_list_t *_list, menu_itemIfce_t *_item)
	{
		assert(_list);
		assert(_item);

		if (_list->listNum == _list->listSize)
		{
			return kStatus_Fail;
		}
		_list->menu[_list->listNum] = _item;
		_item->list_id = _list->listNum++;
		//_item->myList = _list;

		return kStatus_Success;
	}

	void MENU_ListPrintDisp(menu_list_t *_list) //TODO: fix this.
	{
		//HITSIC_MENU_DISPLAY_PRINTF(0,0,)
	}

	void MENU_ListKeyOp(menu_list_t *_list, menu_keyOp_t *_op)
	{
		switch (*_op)
		{
		case MENU_BUTTON_MAKE_OP(ok, long):
			//return
			menu_currList = _list->prev;
			break;
		case MENU_BUTTON_MAKE_OP(up, shrt):
		case MENU_BUTTON_MAKE_OP(up, long):
			//menu up
			if (_list->slct_p > 0)
			{
				--_list->slct_p;
			}
			while (_list->disp_p > _list->slct_p)
			{
				--_list->disp_p;
			}
			break;
		case MENU_BUTTON_MAKE_OP(dn, shrt):
		case MENU_BUTTON_MAKE_OP(dn, long):
			//menu dn
			if (_list->slct_p < _list->listNum - 1)
			{
				++_list->slct_p;
			}
			while (_list->slct_p >= 6 && _list->disp_p < _list->slct_p - 6) //TODO: fix this. 6 is a magic number.
			{
				++_list->disp_p;
			}
			break;
		default:
			//direct op
			MENU_ItemKeyOp(_list->menu[_list->slct_p], _op);
			break;
		}
	}

	/**
	 * ********** NVM存储变量定义 **********
	 */

	//gl = global
	//rg = region
	//addr = address
	//sect = sector
	/**
	  * @brief : 每个扇区包含的字节数
	  */
#define menu_nvm_sectorSize flash_sectorSize;
	/**
	 * @brief : 全局存储 Global Storage
	 */
	uint32_t menu_nvm_glSectCnt = 2u;	/// 全局存储区占用的扇区数
	uint32_t menu_nvm_glSectOffset = 2u; /// 全局存储区扇区偏移
	uint32_t menu_nvm_glAddrOffset;		 /// 全局存储区地址偏移
	/**
	 * @brief : 局部存储 Region Storage
	 */

	uint32_t menu_nvm_rgSectCnt = 4u;				/// 每个局部存储区占用的扇区数
	uint32_t menu_nvm_rgSectOffset[menu_nvm_rgCnt]; /// 三个局部存储区的扇区偏移

	uint32_t menu_nvm_rgAddrOffset[menu_nvm_rgCnt]; /// 三个局部存储区的地址偏移

	/**
	 * @brief : 菜单存储占用的总扇区数
	 */
	uint32_t menu_nvm_totalSectCnt;
	/**
	 * @brief : 每个菜单项保存时占用的字节数
	 */
	uint32_t menu_nvm_dataSize = 32u;

	/**
	 * @brief : 菜单项写入缓存。
	 * 当改写第N个扇区时，menu_nvm_cachedSector = N, menu_nvm_cache分配4KB缓存
	 * 并读入第N扇区的所有内容。此时能且仅能修改第N扇区的内容。对第N扇区内容的修改
	 * 将缓存至上述内存。
	 */
	uint8_t *menu_nvm_cache = NULL;
	uint32_t menu_nvm_cachedSector = 0;

	/**
	 * ********** NVM存储操作接口 **********
	 */

	status_t MENU_NvmRead(uint32_t _addr, void *_buf, uint32_t _byteCnt)
	{
		return FLASH_AddressRead(_addr, _buf, _byteCnt);
	}

	bool MENU_NvmCacheable(uint32_t _addr)
	{
		if (_addr / flash_sectorSize == menu_nvm_cachedSector)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	status_t MENU_NvmCacheSector(uint32_t _sect)
	{
		if (menu_nvm_cache != NULL)
		{
			return kStatus_Fail;
		}
		menu_nvm_cache = malloc(flash_sectorSize);
		if (menu_nvm_cache == NULL)
		{
			return kStatus_Fail;
		}
		if (kStatus_FTFx_Success != FLASH_SectorRead(menu_nvm_cachedSector, (void *)menu_nvm_cache))
		{
			free(menu_nvm_cache);
			menu_nvm_cache = NULL;
			return kStatus_Fail;
		}
		return kStatus_Success;
	}

	status_t MENU_NvmWriteCache(uint32_t _addr, void *_buf, uint32_t _byteCnt)
	{
		if (menu_nvm_cache == NULL)
		{
			menu_nvm_cachedSector = _addr / flash_sectorSize;
			MENU_NvmCacheSector(menu_nvm_cachedSector);
		}
		mencpy(_buf, menu_nvm_cache + _addr % flash_sectorSize, _byteCnt);
		return kStatus_Success;
	}

	status_t MENU_NvmUpdateCache(void)
	{
		if (kStatus_FTFx_Success != FLASH_SectorWrite(menu_nvm_cachedSector, menu_nvm_cache))
		{
			return kStatus_Fail;
		}
		free(menu_nvm_cache);
		menu_nvm_cache = NULL;
		return kStatus_Success;
	}

	/**
	 * 菜单定义
	 */

	//menu_t menuInst;

	menu_list_t *menu_currList;
	menu_itemIfce_t *menu_currItem;
	menu_list_t *menu_menuRoot;
	int32_t menu_currRegionNum;
	int32_t menu_statusFlag;
	uint32_t menu_nvm_statusFlagAddr;

	/**
	 * ********** 菜单顶层操作接口 **********
	 */

	void MENU_Init(void)
	{

		/**
	 * @brief : 全局存储 Global Storage
	 */
		menu_nvm_glSectCnt = 2u;											 /// 全局存储区占用的扇区数
		menu_nvm_glSectOffset = 2u;											 /// 全局存储区扇区偏移
		menu_nvm_glAddrOffset = menu_nvm_glSectOffset * menu_nvm_sectorSize; /// 全局存储区地址偏移
		/**
	 * @brief : 局部存储 Region Storage
	 */
		//menu_nvm_rgCnt = 3u;	 /// 局部存储区的数量
		menu_nvm_rgSectCnt = 4u; /// 每个局部存储区占用的扇区数
								 /// 三个局部存储区的扇区偏移
		{
			menu_nvm_rgSectOffset[0] = menu_nvm_glSectOffset + menu_nvm_glSectCnt + 0u * menu_nvm_rgSectCnt;
			menu_nvm_rgSectOffset[1] = menu_nvm_glSectOffset + menu_nvm_glSectCnt + 1u * menu_nvm_rgSectCnt;
			menu_nvm_rgSectOffset[2] = menu_nvm_glSectOffset + menu_nvm_glSectCnt + 2u * menu_nvm_rgSectCnt;
		}
		/// 三个局部存储区的地址偏移
		{
			menu_nvm_rgAddrOffset[0] = menu_nvm_rgSectOffset[0] * menu_nvm_sectorSize;
			menu_nvm_rgAddrOffset[1] = menu_nvm_rgSectOffset[1] * menu_nvm_sectorSize;
			menu_nvm_rgAddrOffset[2] = menu_nvm_rgSectOffset[2] * menu_nvm_sectorSize;
		}
		/**
	 * @brief : 菜单存储占用的总扇区数
	 */
		menu_nvm_totalSectCnt = menu_nvm_glSectCnt + menu_nvm_rgCnt * menu_nvm_rgSectCnt;
		/**
	 * @brief : 每个菜单项保存时占用的字节数
	 */
		menu_nvm_dataSize = 32u;

		menu_nvm_statusFlagAddr = menu_nvm_glAddrOffset + 0u;

		menu_menuRoot = MENU_ListConstruct("MenuRoot", HITSIC_MENU_ROOT_SIZE, menu_menuRoot);
		assert(menu_menuRoot);
		menu_currList = menu_menuRoot;
		menu_currItem = NULL;
		menu_currRegionNum = 0;
		menu_statusFlag = 0;
		MENU_ListInsert(menu_menuRoot, MENU_itemConstruct(variType, &menu_currRegionNum, "RegionSEL", 0, menuItem_data_global | menuItem_data_prioRW));

		MENU_DataSetUp();

		//read...

		NVIC_SetPriority(HITSIC_MENU_SERVICE_IRQn, HITSIC_MENU_SERVICE_IRQPrio);
		NVIC_EnableIRQ(HITSIC_MENU_SERVICE_IRQn);
	}

	void MENU_DataSetUp(void)
	{
	}

	void MENU_PrintDisp(void)
	{
		if (menu_currItem == NULL)
		{
			MENU_ListPrintDisp(menu_currList);
		}
		else
		{
			MENU_ItemPrintDisp(menu_currItem);
		}
	}

	void MENU_KeyOp(menu_keyOp_t *_op)
	{
		if (menu_currItem == NULL)
		{
			MENU_ListKeyOp(menu_currList, _op);
		}
		else
		{
			MENU_ItemDirectKeyOp(menu_currItem, _op);
		}
		if (*_op != 0)
		{
			HITSIC_MENU_DEBUG_PRINTF("-Warning MENU: KeyOp remained uncclear. OP = %d", *_op);
		}
	}

	void MENU_Data_NvmSave(int32_t _region)
	{
	}

	void MENU_Data_NvmRead(int32_t _region)
	{
	}

	void MENU_Data_NvmCopy(int32_t _srcRegion, int32_t _dstRegion)
	{
		if (_srcRegion == _dstRegion)
		{
			return;
		}
		for (int i = 0; i < menu_nvm_rgSectCnt; ++i)
		{
			FLASH_SectorWrite(menu_nvm_rgSectOffset[_dstRegion] + i, (void *)FLASH_GetPhysicalAddress(menu_nvm_rgSectOffset[_srcRegion] + i));
		}
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

	void HITSIC_MENU_SERVICE_IRQHandler(void)
	{
		NVIC_ClearPendingIRQ(HITSIC_MENU_SERVICE_IRQn);
		//if()
	}

#ifdef __cplusplus
}
#endif