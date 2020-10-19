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
 * @file 	:	drv_edmadvp.hpp
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1-beta.0
 *
 * @date 	:	v0.1-beta.0 2020.10.13
 *
 * @brief    :   DVP摄像头数据EDMA接收器
 */
#ifndef UTILITIES_DRV_EDMADVP_HPP_
#define UTILITIES_DRV_EDMADVP_HPP_
#include "inc_stdlib.h"
#include "hitsic_common.h"

#if defined(HITSIC_USE_EDMADVP) && (HITSIC_USE_EDMADVP > 0)
#inclide "drv_edmadvp_port.hpp"

/*! @brief Error codes for the EDMADVP driver. */
enum
{
    kStatus_EDMADVP_NoEmptyBuffer = MAKE_STATUS(kStatusGroup_EDMADVP, 0), /*!< No empty frame buffer in queue to load to CSI. */
    kStatus_EDMADVP_NoFullBuffer  = MAKE_STATUS(kStatusGroup_EDMADVP, 1), /*!< No full frame buffer in queue to read out. */
    kStatus_EDMADVP_QueueFull = MAKE_STATUS(kStatusGroup_EDMADVP, 2), /*!< Queue is full, no room to save new empty buffer. */
    kStatus_EDMADVP_FrameDone = MAKE_STATUS(kStatusGroup_EDMADVP, 3), /*!< New frame received and saved to queue. */
};

/*! @brief CSI signal polarity. */
enum _edmadvp_polarity_flags    //TODO: fix this
{
    EDMADVP_HsyncActiveLow         = 0U,                        /*!< HSYNC is active low. */
    EDMADVP_HsyncActiveHigh        = 1U,                        /*!< HSYNC is active high. */
    EDMADVP_DataLatchOnRisingEdge  = 1U << 1,                   /*!< Pixel data latched at rising edge of pixel clock. */
    EDMADVP_DataLatchOnFallingEdge = 0U,                        /*!< Pixel data latched at falling edge of pixel clock. */
    EDMADVP_VsyncActiveHigh        = 0U,                        /*!< VSYNC is active high. */
    EDMADVP_VsyncActiveLow         = 1U << 2,                   /*!< VSYNC is active low. */
};

struct edmadvp_config_t
{
    uint16_t width;           /*!< Pixels of the input frame. */
    uint16_t height;          /*!< Lines of the input frame.  */
    uint8_t bytesPerPixel;    /*!< Bytes per pixel            */
    uint8_t polarityFlags;   //当前未使用。/*!< Timing signal polarity flags, OR'ed value of @ref _edmadvp_polarity_flags. */
};

struct edmadvp_handle_t;

struct edmadvp_handle_t
{
    EDMADVP_Type* base;
    edma_handle_t dmaHandle;
    edma_transfer_config_t xferCfg;
    volatile bool transferStarted;
}

status_t EDMADVP_Init(EDMADVP_Type *base, const edmadvp_config_t *config);

void EDMADVP_Deinit(EDMADVP_Type *base);

// void EDMADVP_EnableInterrupts(EDMADVP_Type *base, uint32_t mask);

// void EDMADVP_DisableInterrupts(EDMADVP_Type *base, uint32_t mask);

status_t EDMADVP_TransferCreateHandle(EDMADVP_Type *base, edmadvp_handle_t *handle, edma_callback callback, void *userData);

status_t EDMADVP_TransferStart(EDMADVP_Type *base, edmadvp_handle_t *handle);

status_t EDMADVP_TransferStop(EDMADVP_Type *base, edmadvp_handle_t *handle);

void EDMADVP_VsncExtIntHandler(edmadvp_handle_t* handle);



#endif // ! HITSIC_USE_EDMADVP

#endif // ! UTILITIES_DRV_EDMADVP_HPP_