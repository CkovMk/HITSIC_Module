#ifndef UTILITIES_TEST_APP_MENU_TEST_HPP
#define UTILITIES_TEST_APP_MENU_TEST_HPP
#include "hitsic_common.h"
#include "app_menu.h"

void MENU_SuspendAndResumeTest(void)
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

#endif // ! UTILITIES_TEST_APP_MENU_TEST_HPP
