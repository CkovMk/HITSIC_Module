#ifndef UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
#define UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
#include "hitsic_common.h"
#include "drv_cam_zf9v034.hpp"


#if (defined(HITSIC_USE_DMADVP) && (HITSIC_USE_DMADVP > 0))
#include "drv_dmadvp.hpp"

inline void CAM_ZF9V034_UnitTestDmaCallback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    dmadvp_handle_t *dmadvpHandle = (dmadvp_handle_t*)userData;

    DMADVP_EdmaCallbackService(dmadvpHandle, transferDone);
    //PRINTF("new full buffer: 0x%-8.8x = 0x%-8.8x\n", handle->fullBuffer.front(), handle->xferCfg.destAddr);
    if(kStatus_Success != DMADVP_TransferStart(dmadvpHandle->base, dmadvpHandle))
    {
        DMADVP_TransferStop(dmadvpHandle->base, dmadvpHandle);
        PRINTF("transfer stop! insufficent buffer\n");
    }
}

inline void CAM_ZF9V034_UnitTest(void)
{
    PRINTF("\n---------- ZF9V034 Unit Test ----------\n\n");

    cam_zf9v034_configPacket_t cameraCfg;
    CAM_ZF9V034_GetDefaultConfig(&cameraCfg);
    PRINTF("[D] ZF9V034: Writing config data.\n");
    CAM_ZF9V034_CfgWrite(&cameraCfg);
    //CAM_ZF9V034_CfgRead(&cameraCfg);

    dmadvp_config_t dmadvpCfg;
    PRINTF("[D] ZF9V034: Generating DMADVP config data.\n");
    CAM_ZF9V034_GetReceiverConfig(&dmadvpCfg, &cameraCfg);

    PRINTF("[D] DMADVP: Initlize DMADVP0.\n");
    DMADVP_Init(DMADVP0, &dmadvpCfg);

    dmadvp_handle_t dmadvpHandle;
    PRINTF("[D] DMADVP: Creating handle.\n");
    DMADVP_TransferCreateHandle(&dmadvpHandle, DMADVP0, CAM_ZF9V034_UnitTestDmaCallback);

    PRINTF("[D] DMADVP: Allocating image buffer.\n");
    uint8_t *imageBuffer0 = new uint8_t[DMADVP0->imgSize];
    uint8_t *imageBuffer1 = new uint8_t[DMADVP0->imgSize];
    uint8_t *fullBuffer = NULL;
    //FIXME: do cache operation HERE!
    disp_ssd1306_frameBuffer_t* dispBuffer = new disp_ssd1306_frameBuffer_t;

    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, imageBuffer0);
    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, imageBuffer1);
    DMADVP_TransferStart(DMADVP0, &dmadvpHandle);
    PRINTF("[D] DMADVP: Begin time: %d ms.\n", pitMgr_t::timer_ms);
    for(int time = 0; time < 128; ++time){
    PRINTF("[D] DMADVP: Transfer %4.4d.\n", time);
    while(kStatus_Success != DMADVP_TransferGetFullBuffer(DMADVP0, &dmadvpHandle, &fullBuffer));

    //memset((void*)dispBuffer, 0x00, 8 * 128);
    dispBuffer->Clear();
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
                dispBuffer->SetPixelColor(dispCol, imageRow, 1);
            }
        }
    }
    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, fullBuffer);
    DISP_SSD1306_BufferUpload((uint8_t*)dispBuffer);
    }
    DMADVP_TransferStop(DMADVP0, &dmadvpHandle);
    PRINTF("[D] DMADVP: End time: %d ms.\n", pitMgr_t::timer_ms);



    PRINTF("[D] DMADVP: Transfer Stop. Cleaning up.\n");
    delete[] imageBuffer0;
    delete[] imageBuffer1;
    delete[] dispBuffer;
    PRINTF("\n---------- ZF9V034 Test Done ----------\n\n");
}

#endif // ! HITSIC_USE_DMADVP

#endif // ! UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
