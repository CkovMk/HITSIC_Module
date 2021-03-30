#ifndef UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
#define UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
#include "hitsic_common.h"
#include "drv_cam_zf9v034.h"


#if (defined(HITSIC_USE_DMADVP) && (HITSIC_USE_DMADVP > 0))
#include "drv_dmadvp.h"

extern pitmgr_t pitmgr_main;
#define CAM_ZF9V034_TEST_TIMER_MS     (pitmgr_main.tickCounter)

inline void CAM_ZF9V034_UnitTestDmaCallback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    dmadvp_handle_t *dmadvpHandle = (dmadvp_handle_t*)userData;
    status_t result = 0;

    DMADVP_EdmaCallbackService(dmadvpHandle, transferDone);

    result = DMADVP_TransferStart(dmadvpHandle->base, dmadvpHandle);
    //PRINTF("new full buffer: 0x%-8.8x = 0x%-8.8x\n", handle->fullBuffer.front(), handle->xferCfg.destAddr);
    if(kStatus_Success != result)
    {
        DMADVP_TransferStop(dmadvpHandle->base, dmadvpHandle);
        PRINTF("[E]DMADVP.TEST: transfer stop! insufficent buffer\n");
    }
}

inline void CAM_ZF9V034_UnitTest(void)
{
    PRINTF("---------- ZF9V034 Unit Test ----------\n\n");

    cam_zf9v034_configPacket_t cameraCfg;
    CAM_ZF9V034_GetDefaultConfig(&cameraCfg);
    //PRINTF("[D] ZF9V034: Writing config data.\n");
    CAM_ZF9V034_CfgWrite(&cameraCfg);
    //CAM_ZF9V034_CfgRead(&cameraCfg);

    dmadvp_config_t dmadvpCfg;
    //PRINTF("[D] ZF9V034: Generating DMADVP config data.\n");
    CAM_ZF9V034_GetReceiverConfig(&dmadvpCfg, &cameraCfg);

    //PRINTF("[D] DMADVP: Initlize DMADVP0.\n");
    DMADVP_Init(DMADVP0, &dmadvpCfg);

    dmadvp_handle_t dmadvpHandle;
    //PRINTF("[D] DMADVP: Creating handle.\n");
    DMADVP_TransferCreateHandle(&dmadvpHandle, DMADVP0, CAM_ZF9V034_UnitTestDmaCallback);

    PRINTF("[D]DMADVP.TEST: Allocating image buffer.\n");
    uint8_t *imageBuffer0 = (uint8_t*)malloc(DMADVP0->imgSize);
    uint8_t *imageBuffer1 = (uint8_t*)malloc(DMADVP0->imgSize);
    uint8_t *fullBuffer = NULL;
    //FIXME: do cache operation HERE!
    disp_ssd1306_fb_t * testDispBuffer = (disp_ssd1306_fb_t*)malloc(sizeof(disp_ssd1306_fb_t));

    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, imageBuffer0);
    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, imageBuffer1);
    DMADVP_TransferStart(DMADVP0, &dmadvpHandle);
    PRINTF("[D]DMADVP.TEST: Begin time: %d ms.\n", CAM_ZF9V034_TEST_TIMER_MS);
    for(int time = 0; time < 512; ++time){
    PRINTF("[D]DMADVP.TEST: Transfer %4.4d.\n", time);
    while(kStatus_Success != DMADVP_TransferGetFullBuffer(DMADVP0, &dmadvpHandle, &fullBuffer));

    //memset((void*)dispBuffer, 0x00, 8 * 128);
    DISP_SSD1306_FB_Clear(testDispBuffer);
    const uint8_t imageTH = 100;
    for(int i = 0; i < cameraCfg.imageRow; i += 2)
    {
        int16_t imageRow = i >> 1;
        int16_t dispRow = (imageRow / 8) + 1, dispShift = (imageRow % 8);
        for(int j = 0; j < cameraCfg.imageCol; j += 2)
        {
            int16_t dispCol = j >> 1;
            if(fullBuffer[i * cameraCfg.imageCol + j] > imageTH)
            {
                DISP_SSD1306_FB_SetPixelColor(testDispBuffer, dispCol, imageRow, 1);
            }
        }
    }
    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, fullBuffer);
    DISP_SSD1306_BufferUpload((uint8_t*)testDispBuffer);
    }
    DMADVP_TransferStop(DMADVP0, &dmadvpHandle);
    PRINTF("[D]DMADVP.TEST: End time: %lld ms.\n", CAM_ZF9V034_TEST_TIMER_MS);



    PRINTF("[D]DMADVP.TEST: Transfer Stop. Cleaning up.\n");
    free(imageBuffer0);
    free(imageBuffer1);
    free(testDispBuffer);
    PRINTF("---------- ZF9V034 Test Done ----------\n\n");
}

#endif // ! HITSIC_USE_DMADVP

#endif // ! UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
