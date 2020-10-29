#ifndef UTILITIES_TEST_APP_MENU_TEST_HPP
#define UTILITIES_TEST_APP_MENU_TEST_HPP
#include "hitsic_common.h"
#include "app_menu.h"

inline void MENU_SuspendAndResumeTest(void)
{
    for(int i = 0; i < 10; ++i)
    {
        SDK_DelayAtLeastUs(2 * 1000 * 1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));
        MENU_Suspend();
        DISP_SSD1306_Fill(0);
        SDK_DelayAtLeastUs(2 * 1000 * 1000,CLOCK_GetFreq(kCLOCK_CoreSysClk));
        MENU_Resume();
    }
}

inline void MENU_DataSetupTest(menu_list_t* menu)
{
        static int32_t region_i = 4096, global_i = 1024, readonly_i = 1998;
        static float region_f = 32.768, global_f = 3.14, longname_f = 12.14;
        static int32_t forceSciData = 202000;
        static menu_list_t *testList;
        testList = MENU_ListConstruct("TestList", 20, menu);
        assert(testList);
        MENU_ListInsert(menu, MENU_ItemConstruct(menuType, testList, "TestList", 0, 0));
        {
            MENU_ListInsert(testList, MENU_ItemConstruct(variType, &global_i, "global_i", 10, menuItem_data_global));
            MENU_ListInsert(testList, MENU_ItemConstruct(varfType, &global_f, "global_f", 11, menuItem_data_global));
            MENU_ListInsert(testList, MENU_ItemConstruct(variType, &region_i, "region_i", 1, menuItem_data_region));
            MENU_ListInsert(testList, MENU_ItemConstruct(varfType, &region_f, "region_f", 2, menuItem_data_region));

            MENU_ListInsert(testList, MENU_ItemConstruct(variType, &readonly_i, "readonly", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
            MENU_ListInsert(testList, MENU_ItemConstruct(varfType, &longname_f, "C.M.'s Birthday", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_disp_noPreview));
            MENU_ListInsert(testList, MENU_ItemConstruct(variType, &forceSciData, "forceSci", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_disp_forceSci));

        }
}

#endif // ! UTILITIES_TEST_APP_MENU_TEST_HPP
