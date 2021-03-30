#ifndef UTILITIES_TEST_SYS_FATFS_TEST_HPP
#define UTILITIES_TEST_SYS_FATFS_TEST_HPP
#include "hitsic_common.h"
#include <inc_stdlib.h>

extern sd_card_t g_sd;

inline void FATFS_BasicTest(void)
{
    PRINTF("---------- FATFS Unit Test ----------\n");
    static FIL g_fileObject; /* File object */
    static char g_bufferWrite[] = "HITSIC FATFS\n哈工大智能车俱乐部\n";
    int32_t bytesWritten = 0;

    //SYSMPU_Enable(SYSMPU, false);
    status_t result = 0;
    HAL_EnterCritical();
    BOARD_SD_Config(&g_sd, NULL, BOARD_SDMMC_SD_HOST_IRQ_PRIORITY, NULL);
    HAL_ExitCritical();
    while(1){
    result = SD_HostInit(&g_sd);
    if (result)
    {
        PRINTF("Host Init failed.\n");
        break;
    }
    else
    {
        PRINTF("Host Init Success.\n");
    }
//    result = SD_CardInit(&g_sd);
//    if (result)
//    {
//        PRINTF("\r\nCard Init failed.\r\n");
//        return -1;
//    }
//    else
//    {
//        PRINTF("Card Init Success.\r\n");
//
//    }
    result = f_mount(&fatfs, "2:", 1);
    if (result)
    {
        PRINTF("Mount SDCard failed.\n");
        break;
    }
    else
    {
        PRINTF("Mount SDCard Success.\n");
        PRINTF("SDCard Size = %ld MB.\n", (g_sd.blockCount) >> 11);
    }
//    std::unique_ptr<FATFS, void(*)(FATFS)>
//                fatfsGuard(&fatfs, [](FATFS* p){ f_mount(0, "2:", 0); });
    result = f_chdrive("2:");
    if (result)
    {
        PRINTF("Change drive failed.\n");
        break;
    }
    else
    {
        PRINTF("Change drive Success.\n");
    }
    //result = f_mkfs("2:", 0, NULL, sizeof work);
    result = f_mkdir(_T("/dir_1"));
    if (result)
    {
        if (result == FR_EXIST)
        {
            PRINTF("Directory exists.\n");
        }
        else
        {
            PRINTF("Make directory failed.\n");
            break;
        }
    }
    else
    {
        PRINTF("Make directory Success.\n");
    }
    result = f_open(&g_fileObject, "f_1.txt",
            (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (result)
    {
        if (result == FR_EXIST)
        {
            PRINTF("File exists.\n");
        }
        else
        {
            PRINTF("Open file failed.\n");
            break;
        }
    }
    else
    {
        PRINTF("Open file Success.\n");
    }
    std::unique_ptr<FIL, void(*)(FIL*)>
            fileGuard(&g_fileObject, [](FIL* p){ f_close(p); });

    result = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite),
            (UINT*)&bytesWritten);
    if (result)
    {
        PRINTF("Write file failed.\n");
        break;
    }
    else
    {
        PRINTF("Write file Success.\n");
    }
    result = f_close(&g_fileObject);
    if (result)
    {
        PRINTF("Close file failed.\n");
        break;
    }
    else
    {
        PRINTF("Close file Success.\n");
    }
    result = f_mount(0, "2:", 0);
    if (result)
    {
        PRINTF("Unmount SDCard failed.\n");
        break;
    }
    else
    {
        PRINTF("Unmount SDCard Success.\n");
    }
    break;
    }
    PRINTF("---------- FATFS Test Done ----------\n\n");
}

#endif // ! UTILITIES_TEST_SYS_FATFS_TEST_HPP
