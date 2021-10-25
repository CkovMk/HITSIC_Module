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
 * @file    :   "drv_button.h"; "drv_button.c"
 * @author  :   CkovMk/马奇科(qq:905497173)
 * @version :   v0.1.0          2020.10.22
 * @date    :   v0.1.0          2019.10.22
 *
 * @note    :   依赖库：sys_extint.h、sys_pitmgr.h
                依赖库必须先初始化。
 */
#ifndef UTILITIES_DRV_CAM_ZF9V034_HPP
#define UTILITIES_DRV_CAM_ZF9V034_HPP

#include "hitsic_common.h"

#if (defined(HITSIC_USE_CAM_ZF9V034) && (HITSIC_USE_CAM_ZF9V034 > 0))
#include  <drv_cam_zf9v034_port.h>

/*!
 * @addtogroup cam_zf9v034
 * @{
 */

/** @brief : 软件版本 */
#define DRV_CAM_ZF9V034_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))

/** @brief : 摄像头命令枚举 */
enum class cam_zf9v034_cmd_t : uint16_t
{
    INIT = 0,               ///> 摄像头初始化命令
    AUTO_EXP,               ///> 自动曝光命令
    EXP_TIME,               ///> 曝光时间命令
    FPS,                    ///> 摄像头帧率命令
    SET_COL,                ///> 图像列命令
    SET_ROW,                ///> 图像行命令
    LR_OFFSET,              ///> 图像左右偏移命令
    UD_OFFSET,              ///> 图像上下偏移命令
    GAIN,                   ///> 图像偏移命令
    CONFIG_FINISH,          ///> 非命令位，主要用来占位计数

    COLOR_GET_WHO_AM_I = 0xEF,
    SET_EXP_TIME = 0XF0,    ///> 单独设置曝光时间命令
    GET_STATUS,             ///> 获取摄像头配置命令
    GET_VERSION,            ///> 固件版本号命令

    SET_ADDR = 0XFE,        ///> 寄存器地址命令
    SET_DATA                ///> 寄存器数据命令
};

/** @brief : 摄像头命令传输结构体 */
__PACKED
struct _cam_zf9v034_configPacket
{
    uint16_t autoExpSetCmd, autoExpSet;                 ///> 自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
                                                        ///> 一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    uint16_t autoExpTimeCmd, autoExpTime;               ///> 曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    uint16_t FrameRateCmd, FrameRate;                   ///> 图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    uint16_t imageColCmd, imageCol;                     ///> 图像列数量        范围1-752     K60采集不允许超过188
    uint16_t imageRowCmd, imageRow;                     ///> 图像行数量        范围1-480
    uint16_t HorizontalOffsetCmd, HorizontalOffset;     ///> 图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    uint16_t VerticalOffsetCmd, VerticalOffset;         ///> 图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    uint16_t imageGainCmd, imageGain;                   ///> 图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度
    uint16_t initCmd, dummyData0;                       ///> 摄像头开始初始化
};

typedef struct _cam_zf9v034_configPacket cam_zf9v034_configPacket_t;

/**
 * @brief 获取摄像头的默认配置。
 * 
 * @param config 要获取的配置信息的地址
 */
void CAM_ZF9V034_GetDefaultConfig(cam_zf9v034_configPacket_t *config);

/**
 * @brief 配置摄像头内部配置信息
 * 
 * @param config 要发送的配置信息的地址
 */
void CAM_ZF9V034_CfgWrite(const cam_zf9v034_configPacket_t *config);

/**
 * @brief 获取摄像头内部配置信息
 * 
 * @param config 要接收的配置信息的地址
 */
void CAM_ZF9V034_CfgRead(cam_zf9v034_configPacket_t *config);

/**
 * @brief 获取摄像头固件版本
 * 
 * @return uint16_t 接收到的版本数据 //FIXME: 接受失败怎么办？
 */
uint16_t CAM_ZF9V034_GetVersion(void);

/**
 * @brief 单独设置摄像头曝光时间
 * 
 * @param light 设置曝光时间越大图像越亮，摄像头收到后会根据分辨率及FPS计算最大曝光时间。
 *                  如果设置的数据过大，那么摄像头将会设置这个最大值
 * @return uint16_t 当前曝光值，用于确认是否正确写入
 * 
 * @note 该数据不会保存到摄像头的EEPROM，下次上电此配置将会丢失，需要重新配置。
 */
uint16_t CAM_ZF9V034_SetExposeTime(uint16_t light);


// Receiver Config
#if (defined(ZF9V034_USE_DMADVP) && (ZF9V034_USE_DMADVP > 0U))
#include <drv_dmadvp.h>
typedef dmadvp_config_t receiver_config_type;

/**
 * @brief 获取摄像头接受器的配置信息，用于DMADVP库。
 * 
 * @param config 要写入的DMADVP配置结构体
 * @param camConfig 要读取的ZF9V034摄像头配置结构体
 */
void CAM_ZF9V034_GetReceiverConfig(receiver_config_type *config, const cam_zf9v034_configPacket_t *camConfig);

#elif(defined(ZF9V034_USE_RTCSI) && (ZF9V034_USE_RTCSI > 0U))
#include "drv_rtdvp.hpp"
typedef csi_config_t receiver_config_type;
/**
 * @brief 获取摄像头接受器的配置信息，用于RTCSI库。
 * 
 * @param config 要写入的DCSIP配置结构体
 * @param camConfig 要读取的ZF9V034摄像头配置结构体
 */
void CAM_ZF9V034_GetReceiverConfig(receiver_config_type *config, const cam_zf9v034_configPacket_t *camConfig); //FIXME

#endif // ! Receiver Config

/* @} */

#endif // ! HITSIC_USE_CAM_ZF9V034

#endif // ! UTILITIES_DRV_CAM_ZF9V034_HPP
