#ifndef UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
#define UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
#include "hitsic_common.h"
#include "drv_cam_zf9v034.hpp"


#if (defined(HITSIC_USE_DMADVP) && (HITSIC_USE_DMADVP > 0))
#include "drv_dmadvp.hpp"

inline void CAM_ZF9V034_UnitTestDmaCallback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    DMADVP_EdmaCallbackService((dmadvp_handle_t*)userData, transferDone);
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

    static uint8_t* imageBuffer;
    PRINTF("[D] DMADVP: Allocating image buffer.\n");
    imageBuffer = new uint8_t[DMADVP0->imgSize];
    assert(imageBuffer);

    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, imageBuffer);
    PRINTF("[D] DMADVP: Transfer Start.\n");
    DMADVP_TransferStart(DMADVP0, &dmadvpHandle);

    while(dmadvpHandle.transferStarted == true);

    PRINTF("[D] DMADVP: Transfer Stop. Cleaning up.\n");
    delete[] imageBuffer;
    PRINTF("\n---------- ZF9V034 Test Done ----------\n\n");
}

#endif // ! HITSIC_USE_DMADVP

#endif // ! UTILITIES_TEST_DRV_CAM_ZF9V034_TEST_HPP
