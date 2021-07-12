/**
 * Copyright 2018 - 2020 HITSIC
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

#include <app_menu.h>

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

 /*!
  * @addtogroup menu
  * @{
  */

#define SYSLOG_TAG  ("MENU.MAIN")
#define SYSLOG_LVL  (HITSIC_MENU_MAIN_LOG_LVL)
#include <inc_syslog.h>

/**
 * 菜单定义
 */

//menu_t menuInst;
menu_list_t *menu_currList;
menu_itemIfce_t *menu_currItem;
menu_list_t *menu_menuRoot;
menu_list_t *menu_manageList;
int32_t menu_currRegionNum[3] = { 0, 0, HITSIC_MENU_NVM_REGION_CNT - 1 };
int32_t menu_statusFlag;
uint32_t menu_nvm_statusFlagAddr;

int32_t menu_nvmCopySrc[3] = { 0, 0, HITSIC_MENU_NVM_REGION_CNT - 1 };
int32_t menu_nvmCopyDst[3] = { 0, 0, HITSIC_MENU_NVM_REGION_CNT - 1 };

char menu_dispStrBuf[MENU_DISP_STRBUF_ROW][MENU_DISP_STRBUF_COL];

menu_keyOp_t menu_keyOpBuff;

pitmgr_handle_t menu_pitHandle =
{
    .tickInterval = 250UL,
    .tickOffset = 7UL,
    .handler = MENU_PitIsr,
    .pptFlag = pitmgr_pptEnable,
    .userData = NULL,
};


volatile uint32_t menu_suspendCnt = 0U;

const char menu_itemNameStr_RegnSel[] = {'R','e','g','n','S','e','l','(','0','-',('0' + HITSIC_MENU_NVM_REGION_CNT - 1),')','\0'};

/**
 * ********** 菜单顶层操作接口 **********
 */

void MENU_Init(void)
{
    SYSLOG_I("Init Begin: v%d.%d.%d", HITSIC_VERSION_MAJOR(APP_MENU_VERSION), HITSIC_VERSION_MINOR(APP_MENU_VERSION), HITSIC_VERSION_PATCH(APP_MENU_VERSION));
#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)
    SYSLOG_D("Using KVDB");
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
		MENU_ListInsert(menu_manageList, p = MENU_ItemConstruct(variType, menu_currRegionNum, menu_itemNameStr_RegnSel/*"RegnSel(0-N)"*/, 0, menuItem_data_global | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_dataExt_HasMinMax));
		//p->nameStr[10] = '0' + HITSIC_MENU_NVM_REGION_CNT - 1;
		MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmSave_Boxed, "Save Data", 0, menuItem_proc_runOnce));
		MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmRead_Boxed, "Load Data", 0, menuItem_proc_runOnce));
		MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmSaveRegionConfig_Boxed, "RegnSave", 0, menuItem_proc_runOnce));
		MENU_ListInsert(menu_manageList, MENU_ItemConstruct(nullType, NULL, "COPY", 0, 0));
		MENU_ListInsert(menu_manageList, p = MENU_ItemConstruct(variType, menu_nvmCopySrc, "CopySrc(0-N)", 0, menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_dataExt_HasMinMax));
		p->nameStr[10] = '0' + HITSIC_MENU_NVM_REGION_CNT - 1;
		MENU_ListInsert(menu_manageList, p = MENU_ItemConstruct(variType, menu_nvmCopyDst, "CopyDst(0-N)", 0, menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_dataExt_HasMinMax));
		p->nameStr[10] = '0' + HITSIC_MENU_NVM_REGION_CNT - 1;
		MENU_ListInsert(menu_manageList, MENU_ItemConstruct(procType, (void *)MENU_Data_NvmCopy_Boxed, "CopyData(S>D)", 0, menuItem_proc_runOnce));
		MENU_ListInsert(menu_manageList, MENU_ItemConstruct(nullType, NULL, "INFO", 0, 0));
		MENU_ListInsert(menu_manageList, MENU_ItemConstruct(variType, &menu_nvm_eraseCnt, "EraseCnt", 3, menuItem_data_global | menuItem_data_ROFlag));
#endif // ! HITSIC_MENU_USE_NVM
	}
	MENU_DataSetUp();

#if defined(HITSIC_MENU_USE_BUTTON) && (HITSIC_MENU_USE_BUTTON > 0)
	SYSLOG_D("Using BUTTON");
	MENU_ButtonSetup();
#endif // ! HITSIC_MENU_USE_BUTTON
	NVIC_SetPriority(HITSIC_MENU_SERVICE_IRQn, HITSIC_MENU_SERVICE_IRQPrio);
	NVIC_EnableIRQ(HITSIC_MENU_SERVICE_IRQn);
	//menu_pitHandle = pitMgr_t::insert(250U, 7U, MENU_PitIsr, pitMgr_t::enable);
	extern pitmgr_t pitmgr_main;
	bool b = PITMGR_HandleInsert(&pitmgr_main, &menu_pitHandle);
	assert(kStatus_Success == b);
	menu_suspendCnt = 0U;
	SYSLOG_I("Init Complete");
}

__WEAK void MENU_DataSetUp(void)
{
    SYSLOG_W("Using default menu structure (__WEAK). If not intended, define your own menu FIRST.");
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
    /** 清空缓存，准备打印 */
    HITSIC_MENU_DISPLAY_BUFFER_CLEAR();
	memset(menu_dispStrBuf, ' ', MENU_DISP_STRBUF_ROW * MENU_DISP_STRBUF_COL);
	/** 根据责任链打印缓存 */
	if (menu_currItem == NULL)
	{
		MENU_ListPrintDisp(menu_currList);
	}
	else
	{
		MENU_ItemPrintDisp(menu_currItem);
	}
	/** 处理字符缓存超控标志位 */
	if(menu_statusFlag & menu_message_strBufOverride)
	{
	    MENU_StatusFlagClr(menu_message_strBufOverride);
	}
	else
	{
	    for (uint8_t i = 0; i < MENU_DISP_STRBUF_ROW; ++i)
	    {
	        menu_dispStrBuf[i][MENU_DISP_STRBUF_COL - 1] = '\0';
	        HITSIC_MENU_DISPLAY_PRINT(1, i, menu_dispStrBuf[i]);
	    }
	}
	HITSIC_MENU_DISPLAY_BUFFER_UPDATE();
	MENU_StatusFlagClr(menu_message_printDisp);
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
		SYSLOG_W("KeyOp remained unclear. OP = %d", *_op);
	}
	MENU_StatusFlagClr(menu_message_buttonOp);
	MENU_StatusFlagSet(menu_message_printDisp);
}

menu_list_t *MENU_DirGetList(const char *str)
{
    assert(str);
    /** 识别路径首字符 */
    if(str[0] != '/')
    {
        return nullptr;
    }
    /** 字符串长度保护 */
    uint32_t str_length = strlen(str);
    if(str_length > 256)
    {
        return nullptr;
    }
    /** 环境准备 */
    char *str_copy = new char[str_length + 1U];
    // 退出时析构，自动释放内存
    std::unique_ptr<char, void(*)(char*)>
            bufferGuard(str_copy, [](char* p){ delete[] p; });
    strncpy(str_copy, str, str_length + 1U);
    char *pch = strtok(str_copy, "/");
    menu_list_t *currDirList = menu_menuRoot;
    /** 启动识别 */
    while(pch != nullptr)
    {
        bool isFound = false;
        for(uint32_t i = 0; i < currDirList->listNum; ++i)
        {
            menu_itemIfce_t *it = currDirList->menu[i];
            if(it->type == menuType && 0 == strcmp(it->nameStr, pch))
            {
                isFound = true;
                currDirList = it->handle.p_menuType->data;
                break;
            }
        }
        if(false == isFound)
        {
            return nullptr;
        }
        pch = strtok(nullptr, "/");
    }
    return currDirList;
}

menu_itemIfce_t *MENU_DirGetItem(const menu_list_t *dir, const char *str)
{
    assert(dir);
    assert(str);
    for(uint32_t i = 0; i < dir->listNum; ++i)
    {
        menu_itemIfce_t *it = dir->menu[i];
        if(0 == strcmp(it->nameStr, str))
        {
            return it;
        }
    }
    return nullptr;
}

#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)

void MENU_Data_NvmSave(int32_t _region)
{
	if(_region < 0 || (uint32_t)_region >= HITSIC_MENU_NVM_REGION_CNT) { return; }
    ++menu_nvm_eraseCnt;
	SYSLOG_I("Data Save Begin");
	SYSLOG_I("Global Data");
	menu_iterator_t *iter = MENU_IteratorConstruct();
	do{
	    menu_nvmData_t dataBuf;
	    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
	    if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoSave))
	    {
	        MENU_ItemGetData(thisItem, &dataBuf);
	        //SYSLOG_D("Get Data.  menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
	        uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
	        if (!MENU_NvmCacheable(realAddr))
	        {
	            MENU_NvmUpdateCache();
	            assert(MENU_NvmCacheable(realAddr));
	        }
	        MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
	    }
	}while(kStatus_Success == MENU_IteratorIncrease(iter));
	SYSLOG_I("Global Data End");
	if (menu_currRegionNum < 0 || menu_currRegionNum >= HITSIC_MENU_NVM_REGION_CNT)
	{
	    SYSLOG_W("RegionNum illegal! Aborting.");
	    MENU_IteratorDestruct(iter);
		return;
	}
	SYSLOG_I("Nvm Region %d Data", menu_currRegionNum);
	MENU_IteratorSetup(iter);
	do{
	    menu_nvmData_t dataBuf;
	    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
	    if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoSave))
	    {
	        MENU_ItemGetData(thisItem, &dataBuf);
	        //SYSLOG_D("Get Data.  menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
	        uint32_t realAddr = menu_nvm_rgAddrOffset[_region] + thisItem->saveAddr * sizeof(menu_nvmData_t);
	        if (!MENU_NvmCacheable(realAddr))
	        {
	            MENU_NvmUpdateCache();
	            assert(MENU_NvmCacheable(realAddr));
	        }
	        MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
	    }
	}while(kStatus_Success == MENU_IteratorIncrease(iter));
	MENU_NvmUpdateCache();
	SYSLOG_I("Region %d Data End.", menu_currRegionNum);
	MENU_IteratorDestruct(iter);
	SYSLOG_I("Save Complete");
}

void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op)
{
	MENU_Data_NvmSave(menu_currRegionNum);
	*_op = 0;
}

void MENU_Data_NvmRead(int32_t _region)
{
    static_assert(sizeof(menu_nvmData_t) == 32, "sizeof menu_nvmData_t error !");
	if(_region < 0 || (uint32_t)_region >= HITSIC_MENU_NVM_REGION_CNT) { return; }
	SYSLOG_I("Read Begin");
	SYSLOG_I("Global Data");
	menu_iterator_t *iter = MENU_IteratorConstruct();
	do{
	    menu_nvmData_t dataBuf;
	    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
	    if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoLoad))
	    {
	        uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
	        MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
	        SYSLOG_D("Get Flash. menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
	        MENU_ItemSetData(thisItem, &dataBuf);
	        //SYSLOG_D("Set Data.  menu: %-16.16s addr: %-4.4d .", thisItem->nameStr, thisItem->saveAddr);
	    }
	}while(kStatus_Success == MENU_IteratorIncrease(iter));
	SYSLOG_I("Global Data End.");
	if (menu_currRegionNum < 0 || menu_currRegionNum >= HITSIC_MENU_NVM_REGION_CNT)
	{
	    SYSLOG_W("RegionNum illegal! Aborting");
	    MENU_IteratorDestruct(iter);
		return;
	}
	SYSLOG_I("Region %d Data.", menu_currRegionNum);
	MENU_IteratorSetup(iter);
	do{
	    menu_nvmData_t dataBuf;
	    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
	    if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoLoad))
	    {
	        uint32_t realAddr = menu_nvm_rgAddrOffset[_region] + thisItem->saveAddr * sizeof(menu_nvmData_t);
	        MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
	        SYSLOG_D("Get Flash. menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
	        MENU_ItemSetData(thisItem, &dataBuf);
	        //SYSLOG_D("Set Data.  menu: %-16.16s addr: %-4.4d .", thisItem->nameStr, thisItem->saveAddr);
	    }
	}while(kStatus_Success == MENU_IteratorIncrease(iter));
	SYSLOG_I("Region %d Data End", menu_currRegionNum);
	MENU_IteratorDestruct(iter);
	SYSLOG_I("Read complete");
}

void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op)
{
	MENU_Data_NvmRead(menu_currRegionNum);
	*_op = 0;
}

void MENU_Data_NvmSaveRegionConfig(void)
{
	SYSLOG_I("Saving region config ...");
	menu_nvmData_t dataBuf;
	//const char itemNameStr[] = {'R','e','g','n','S','e','l','(','0','-',('0' + HITSIC_MENU_NVM_REGION_CNT - 1),')','\0'};
	menu_itemIfce_t *thisItem = MENU_DirGetItem(MENU_DirGetList("/MenuManager"), menu_itemNameStr_RegnSel);
	MENU_ItemGetData(thisItem, &dataBuf);
	uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
	if (!MENU_NvmCacheable(realAddr))
	{
		MENU_NvmUpdateCache();
		assert(MENU_NvmCacheable(realAddr));
	}
	MENU_NvmWriteCache(realAddr, (void *)&dataBuf, sizeof(menu_nvmData_t));
	MENU_NvmUpdateCache();
	SYSLOG_I("Save region config complete");
}
void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op)
{
	MENU_Data_NvmSaveRegionConfig();
	*_op = 0;
}

void MENU_Data_NvmReadRegionConfig(void)
{
    SYSLOG_I("Reading region config ...");
	menu_nvmData_t dataBuf;
	//const char itemNameStr[] = {'R','e','g','n','S','e','l','(','0','-',('0' + HITSIC_MENU_NVM_REGION_CNT - 1),')','\0'};
	menu_itemIfce_t *thisItem = MENU_DirGetItem(MENU_DirGetList("/MenuManager"), menu_itemNameStr_RegnSel);
	uint32_t realAddr = menu_nvm_glAddrOffset + thisItem->saveAddr * sizeof(menu_nvmData_t);
	MENU_NvmRead(realAddr, &dataBuf, sizeof(menu_nvmData_t));
	MENU_ItemSetData(thisItem, &dataBuf);
	SYSLOG_I("Read region config complete");
}

void MENU_Data_NvmReadRegionConfig_Boxed(menu_keyOp_t *const _op)
{
	MENU_Data_NvmReadRegionConfig();
	*_op = 0;
}

void MENU_Data_NvmCopy(int32_t _srcRegion, int32_t _dstRegion)
{
	if ((_srcRegion == _dstRegion) || (_srcRegion < 0 || (uint32_t)_srcRegion >= HITSIC_MENU_NVM_REGION_CNT) || (_dstRegion < 0 || (uint32_t)_dstRegion >= HITSIC_MENU_NVM_REGION_CNT))
	{
		return;
	}
	++menu_nvm_eraseCnt;
	if (menu_nvm_cache != NULL)
	{
		MENU_NvmUpdateCache();
	}
	for (uint32_t i = 0; i < HITSIC_MENU_NVM_REGION_SECT_SIZE; ++i)
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

// int32_t MENU_GetNvmStatus(void)
// {
// 	MENU_NvmRead(menu_nvm_statusFlagAddr, (void *)&menu_statusFlag, sizeof(int32_t));
// 	return menu_statusFlag;
// }

// void MENU_SetNvmStatus(int32_t _status)
// {
// 	if (!MENU_NvmCacheable(menu_nvm_statusFlagAddr))
// 	{
// 		MENU_NvmUpdateCache();
// 	}
// 	MENU_NvmWriteCache(menu_nvm_statusFlagAddr, (void *)&menu_statusFlag, sizeof(int32_t));
// }

#endif // ! HITSIC_MENU_USE_NVM

void MENU_PitIsr(void* userData)
{
	MENU_StatusFlagSet(menu_message_printDisp);
	HITSIC_MENU_SERVICE_SEM_GIVE();
}

void MENU_EventService(void)
{
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

void MENU_Suspend(void)
{
	//assert(menu_pitHandle);
	if (0U == menu_suspendCnt)
	{
		menu_pitHandle.pptFlag &= ~pitmgr_pptEnable;
		NVIC_DisableIRQ(HITSIC_MENU_SERVICE_IRQn);
		SYSLOG_I("Suspended.");
	}
	++menu_suspendCnt;
	SYSLOG_D("SuspendCnt = %d", menu_suspendCnt);
}

void MENU_Resume(void)
{
	//assert(menu_pitHandle);
	menu_suspendCnt = menu_suspendCnt == 0 ? menu_suspendCnt : menu_suspendCnt - 1;
	SYSLOG_D("SuspendCnt = %d", menu_suspendCnt);
	if (0U == menu_suspendCnt)
	{
		menu_statusFlag = 0;
		menu_pitHandle.pptFlag |= pitmgr_pptEnable;
		NVIC_EnableIRQ(HITSIC_MENU_SERVICE_IRQn);
		SYSLOG_I("Resumed.");
	}
}

//#undef SYSLOG_TAG
//#undef SYSLOG_LVL

/* @} */

#endif // ! HITSIC_USE_APP_MENU
