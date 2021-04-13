 /**
 * Copyright 2018 - 2021 HITSIC
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
 * @file 	:	drv_dmadvp.hpp
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1-beta.1
 *
 * @date 	:	v0.1-beta.0 2020.10.20
 * @date 	:	v0.1-beta.1 2020.10.30
 * @date    :   v0.1.2      2020.11.09
 * @date    :   v0.2.0      2021.04.07
 *
 * @brief    :   数字视频接口DMA接收器
 */
#ifndef UTILITIES_DRV_DMADVP_HPP_
#define UTILITIES_DRV_DMADVP_HPP_
#include "hitsic_common.h"

#if defined(HITSIC_USE_DMADVP) && (HITSIC_USE_DMADVP > 0)
#include <drv_dmadvp_port.h>
#include <sys_extint.h>
#include <m-deque.h>

/** @brief : 软件版本 */
#define DRV_DMADVP_VERSION (HITSIC_MAKE_VERSION(0U, 2U, 0U))

/*!
 * @addtogroup dmadvp
 * @{
 */

/*! @brief Error codes for the DMADVP driver. */
enum
{
    kStatus_DMADVP_NoEmptyBuffer = MAKE_STATUS(kStatusGroup_DMADVP, 0), /*!< No empty frame buffer in queue to load to CSI. */
    kStatus_DMADVP_NoFullBuffer  = MAKE_STATUS(kStatusGroup_DMADVP, 1), /*!< No full frame buffer in queue to read out. */
    kStatus_DMADVP_QueueFull = MAKE_STATUS(kStatusGroup_DMADVP, 2), /*!< Queue is full, no room to save new empty buffer. */
    kStatus_DMADVP_FrameDone = MAKE_STATUS(kStatusGroup_DMADVP, 3), /*!< New frame received and saved to queue. */
};

/*! @brief DMADVP signal polarity. */
enum dmadvp_polarity_flags
{
    DMADVP_HsyncActiveLow         = 0U,                        /*!< HSYNC is active low. No Use HERE*/
    DMADVP_HsyncActiveHigh        = 1U,                        /*!< HSYNC is active high. No Use HERE*/
    DMADVP_DataLatchOnRisingEdge  = 1U << 1,                   /*!< Pixel data latched at rising edge of pixel clock. */
    DMADVP_DataLatchOnFallingEdge = 0U,                        /*!< Pixel data latched at falling edge of pixel clock. */
    DMADVP_VsyncActiveHigh        = 0U,                        /*!< VSYNC is active high. */
    DMADVP_VsyncActiveLow         = 1U << 2,                   /*!< VSYNC is active low. */
};

/*! @brief DMADVP配置结构体 */
struct dmadvp_config_t
{
    uint16_t width;           /*!< 图像宽度 */
    uint16_t height;          /*!< 图像高度  */
    uint8_t bytesPerPixel;    /*!< 每像素的字节数。对于黑白摄像头，该值为1；对于RGB565，该值为2；以此类推。 */
    uint8_t polarityFlags;    /*!< 摄像头触发信号的极性标志位。 */
};

struct dmadvp_handle_t;

typedef struct _dmadvp_buffer
{
    uint8_t* buffer[DMADVP_DRIVER_QUEUE_SIZE + 1U];
    int16_t bufferHead, bufferTail;
}dmadvp_bufferQue_t;


/*! @brief DMADVP句柄 */
struct dmadvp_handle_t
{
    DMADVP_Type* base;              /*!< DMADVP虚拟设备地址 */
    edma_handle_t dmaHandle;        /*!< DMA传输句柄 */
    edma_transfer_config_t xferCfg; /*!< DMA传输配置 */
    volatile bool transferStarted;  /*!< 传输中标志位，true：正在进行传输 */
    dmadvp_bufferQue_t emptyBuffer, fullBuffer;
};

/**
 * @brief DMADVP虚拟设备初始化。
 * 
 * @param base DMADVP虚拟设备地址。
 * @param config DMADVP配置结构体。可由摄像头配置组件产生。
 * @retval kStatus_Success 初始化成功。
 */
status_t DMADVP_Init(DMADVP_Type *base, const dmadvp_config_t *config);

/**
 * @brief DMADVP虚拟设备取消初始化。
 * 
 * @param base DMADVP虚拟设备地址。
 */
//void DMADVP_Deinit(DMADVP_Type *base);

/**
 * @brief 初始化DMADVP传输句柄。一般只需在初始化时调用一次。
 * 
 * @param handle DMADVP传输句柄。
 * @param base DMADVP虚拟设备地址。
 * @param callback 要使用的DMA回调函数。
 */
void DMADVP_TransferCreateHandle(dmadvp_handle_t *handle, DMADVP_Type *base, edma_callback callback);

/**
 * @brief 将缓存区提交至DMADVP句柄。
 * 
 * @param base DMADVP虚拟设备地址。
 * @param handle DMADVP传输句柄。
 * @param destAddr 要提交的缓存区指针。
 * @retval kStatus_Success 提交成功。
 */
status_t DMADVP_TransferSubmitEmptyBuffer(DMADVP_Type *base, dmadvp_handle_t *handle, uint8_t *buffer);

/**
 * @brief 获取传输完成的缓存区。
 * 
 * @param base DMADVP虚拟设备地址。
 * @param handle DMADVP传输句柄。
 * @param buffer 用于接收缓存区的指针。
 * @retval kStatus_Success 成功获取到了传输完成的缓存区。
 * @retval kStatus_DMADVP_NoFullBuffer 没有可供获取的传输完成的缓存区。
 */
status_t DMADVP_TransferGetFullBuffer(DMADVP_Type *base, dmadvp_handle_t *handle, uint8_t **buffer);

/**
 * @brief 启动当前传输。
 * 
 * 从空缓存队列中取出一个空缓存并启动下一帧图像的传输。
 * 在启动第一帧传输时，本函数将使能VSNC中断进行帧同步。
 * 在一帧传输结束时，如果立即启动下一帧传输，将不再进行帧同步。
 *
 * @param base DMADVP虚拟设备地址。
 * @param handle DMADVP传输句柄。
 * @retval kStatus_Success 成功启动传输。
 * @retval kStatus_EDMA_Busy 启动失败，传输进行中。
 * @retval kStatus_DMADVP_NoEmptyBuffer 启动失败，没有空缓存可供使用。
 */
status_t DMADVP_TransferStart(DMADVP_Type *base, dmadvp_handle_t *handle);

/**
 * @brief 停止当前传输。
 * 
 * @param base DMADVP虚拟设备地址
 * @param handle DMADVP传输句柄
 * 
 * @note 如果您在传输中途停止传输，正在传输的buffer会留在handle->xferCfg.destAddr中。
 *              您需要手动将其提交到空缓存队列。
 * @note 在一帧传输结束时，如果想要终止传输，必须调用此函数。
 */
void DMADVP_TransferStop(DMADVP_Type *base, dmadvp_handle_t *handle);

/**
 * @brief 外部中断服务函数
 * 
 * @param userData 要响应的DMADVP传输句柄
 */
void DMADVP_VsncExtIntHandler(void *userData);

/**
 * @brief DMA回调预处理函数。必须在DMA回调函数中最先调用。
 * 
 * @param handle DMADVP句柄指针，应由回调函数中的userData经类型转换得到。
 * @param transferDone 标志位，标志是否传输完成。
 */
void DMADVP_EdmaCallbackService(dmadvp_handle_t *handle, bool transferDone);

/* @} */

#endif // ! HITSIC_USE_DMADVP

#endif // ! UTILITIES_DRV_DMADVP_HPP_
