#include "app_menu.h"



#ifndef __cplusplus
extern "C" {
#endif






	/**
	 * ********** 菜单项操作接口 **********
	 */

	menu_itemIfce_t* MENU_itemConstruct(menu_itemType_t _type, void* _data, const char* _nameStr, uint32_t _saveAddr, uint32_t _pptFlag)
	{

		menu_itemIfce_t* item;
		item = malloc(sizeof(menu_itemIfce_t));
		assert(ptr);
		item->type = _type;
		item->pptFlag = _pptFlag;
		item->unique_id = menu_itemCnt++;
		saveAddr = _saveAddr;
		strncpy(item->nameStr, _nameStr, menu_nameStrSize);
		nameStr[menu_nameStrSize - 1] = '\0';

		MENU_ITEM_SWITCH_CASE(MENU_itemConstruct, item, _data);

		return ptr;
	}

	void MENU_itemDestruct(menu_itemIfce_t* _item)
	{
	}

	void MENU_ItemGetData(menu_itemIfce_t* _item, void* _data)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemGetData, _item, _data);
	}

	void MENU_ItemSetData(menu_itemIfce_t* _item, void* _data)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemSetData, _item, _data);
	}

	void MENU_ItemPrintSlot(menu_itemIfce_t* _item, uint32_t _slotNum)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemPrintSlot, _item, _slotNum);
	}

	void MENU_ItemDirectKeyOp(menu_itemIfce_t* _item, menu_keyOp_t* _op)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemDirectKeyOp, _item, _op);
	}

	void MENU_ItemPrintDisp(menu_itemIfce_t* _item)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemPrintDisp, _item);
	}

	void MENU_ItemKeyOp(menu_itemIfce_t* _item, menu_keyOp_t* _op)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, _item, _op);
	}





	/**
	 * ********** 菜单列表操作接口 **********
	 */


	menu_list_t* MENU_ListConstruct(const char* _nameStr, uint32_t _listSize, menu_list_t* _prev)
	{
		assert(_prev);
		menu_list_t* list;
		list = malloc(sizeof(menu_list_t));
		assert(list);
		list->listSize = _listSize;
		list->listNum = 0;
		list->menu = malloc(_listSize * sizeof(menuItemIfce_t*));
		assert(list->menu);
		list->nameStr = _nameStr;
		list->prev = _prev;

		MENU_ListInsert(list, MENU_itemConstruct(menuType, (void*)_prev, "<< back", 0, 0));

		return list;
	}

	void MENU_ListDestruct(menu_list_t* _list)
	{
		free(_list->menu);
		free(_list);
	}

	status_t MENU_ListInsert(menu_list_t* _list, menu_itemIfce_t* _item)
	{
		assert(_list);
		assert(_item);
		
		if (listNum == listSize)
		{
			return kStatus_Fail;
		}
		menu[listNum] = _item;
		++listNum;

		return kStatus_Success;
	}

	void MENU_ListPrintDisp(menu_list_t* _list)
	{
	}

	void MENU_ListKeyOp(menu_list_t* _list, menu_keyOp_t* _op)
	{
	}









	/**
	 * ********** NVM存储操作接口 **********
	 */



	status_t MENU_NvmRead(uint32_t _addr, void* _buf, uint32_t _byteCnt)
	{
		
		return kStatuss_Fail;
	}

	bool MENU_NvmCacheable(uint32_t _addr)
	{
		return false;
	}

	status_t MENU_NvmWriteCache(uint32_t _addr, void* _buf, uint32_t _byteCnt)
	{
		return kStatuss_Fail;
	}

	status_t MENU_NvmUpdateCache(void)
	{
		return kStatuss_Fail;
	}

	status_t MENU_NvmSectorErase(uint32_t _sect)
	{

	}

	status_t MENU_NvmAddressRead(uint32_t _addr, void* _buf, uint32_t _byteCnt)
	{
		assert(dst);
		uint32_t destAdrss;
		if (kFTFx_SecurityStateNotSecure != securityStatus)
		{
			return securityStatus;
		}
		if (size > pflashSectorSize)
		{
			size = pflashSectorSize;
		}
		/*Calculate the destAdrss*/
#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
	/* Note: we should make sure that the sector shouldn't be swap indicator sector*/
		destAdrss = pflashBlockBase + (pflashTotalSize - ((sectors + 1) * pflashSectorSize * 2));
#else
		destAdrss = pflashBlockBase + (pflashTotalSize - ((sectors + 1) * pflashSectorSize));
#endif

		/* Post-preparation work about flash Cache/Prefetch/Speculation. */
		FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, false);

#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
		L1CACHE_InvalidateCodeCache();
#endif /* FSL_FEATURE_HAS_L1CACHE */

#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
		/* Clean the D-Cache before reading the flash data*/
		SCB_CleanInvalidateDCache();
#endif

		for (uint32_t i = 0; i < size; i++)
		{
			dst[i] = *(volatile uint8_t*)(destAdrss + i);
		}
		return kStatus_FTFx_Success;
	}

	status_t MENU_NvmAddressProgram(uint32_t _addr, void* _buf, uint32_t _byteCnt)
	{

	}






	/**
	 * ********** 菜单顶层操作接口 **********
	 */



	void MENU_Init(void)
	{
		menuRoot = MENU_ListConstruct("MenuRoot", HITSIC_MENU_ROOT_SIZE, menuRoot);
		assert(menuRoot);
		currList = menuRoot;
		currItem = NULL;
		currRegionNum = 0;
		statusFlag = 0;
	}


	void MENU_DataSetUp(void)
	{

	}


	void MENU_PrintDisp(void)
	{
		if (currItem == NULL)
		{
			MENU_ListPrintDisp(currList);
		}
		else
		{
			MENU_ItemPrintDisp(currItem);
		}
	}


	void MENU_KeyOp(menu_keyOp_t* _op)
	{
		if (currItem == NULL)
		{
			MENU_ListKeyOp(currList, _op);
		}
		else
		{
			MENU_ItemDirectKeyOp(currItem, _op);
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

	}


	int32_t MENU_GetNvmStatus(void)
	{
		return 0;
	}


	void MENU_SetNvmStatus(int32_t _status)
	{

	}








#ifndef __cplusplus
}
#endif