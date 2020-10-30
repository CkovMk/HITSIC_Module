#ifndef UTILITIES_TEST_SYS_FATFS_TEST_HPP
#define UTILITIES_TEST_SYS_FATFS_TEST_HPP
#include "hitsic_common.h"
#include "inc_stdlib.hpp"

extern sd_card_t g_sd;

inline void FATFS_BasicTest(void)
{
    PRINTF("\n---------- FATFS Unit Test ----------\n\n");
    static FIL g_fileObject; /* File object */
    static char g_bufferWrite[] = "HITSIC FATFS\n哈工大智能车俱乐部\n";
    int32_t bytesWritten = 0;

    SYSMPU_Enable(SYSMPU, false);
    status_t result = 0;
    HAL_EnterCritical();
    BOARD_SD_Config(&g_sd, NULL, BOARD_SDMMC_SD_HOST_IRQ_PRIORITY, NULL);
    HAL_ExitCritical();
    while(1){
    result = SD_HostInit(&g_sd);
    if (result)
    {
        PRINTF("\r\nHost Init failed.\r\n");
        break;
    }
    else
    {
        PRINTF("Host Init Success.\r\n");
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
        PRINTF("\r\nMount SDCard failed.\r\n");
        break;
    }
    else
    {
        PRINTF("Mount SDCard Success.\r\n");
        PRINTF("SDCard Size = %ld MB.\r\n", (g_sd.blockCount) >> 11);
    }
    result = f_chdrive("2:");
    if (result)
    {
        PRINTF("\r\nChange drive failed.\r\n");
        break;
    }
    else
    {
        PRINTF("Change drive Success.\r\n");
    }
    //result = f_mkfs("2:", 0, NULL, sizeof work);
    result = f_mkdir(_T("/dir_1"));
    if (result)
    {
        if (result == FR_EXIST)
        {
            PRINTF("Directory exists.\r\n");
        }
        else
        {
            PRINTF("Make directory failed.\r\n");
            break;
        }
    }
    else
    {
        PRINTF("Make directory Success.\r\n");
    }
    result = f_open(&g_fileObject, "f_1.txt",
            (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (result)
    {
        if (result == FR_EXIST)
        {
            PRINTF("File exists.\r\n");
        }
        else
        {
            PRINTF("Open file failed.\r\n");
            break;
        }
    }
    else
    {
        PRINTF("Open file Success.\r\n");
    }

    result = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite),
            (UINT*)&bytesWritten);
    if (result)
    {
        PRINTF("\r\nWrite file failed.\r\n");
        break;
    }
    else
    {
        PRINTF("Write file Success.\r\n");
    }
    result = f_close(&g_fileObject);
    if (result)
    {
        PRINTF("\r\nClose file failed.\r\n");
        break;
    }
    else
    {
        PRINTF("Close file Success.\r\n");
    }
    result = f_mount(0, "2:", 0);
    if (result)
    {
        PRINTF("\r\nUnmount SDCard failed.\r\n");
        break;
    }
    else
    {
        PRINTF("Unmount SDCard Success.\r\n");
    }
    break;
    }
    PRINTF("\n---------- FATFS Test Done ----------\n\n");
}

#endif // ! UTILITIES_TEST_SYS_FATFS_TEST_HPP
