# DRV_CAM_ZF9V034

[TOC]

## 简介

总钻风摄像头驱动（CAM_ZF9V034）是用于配置逐飞（SeekFree）公司生产的MT9V034“总钻风”摄像头模块的模组。该摄像头通过波特率为9600的串口与主控单片机通信，其电路板上包含一个8051单片机用于接收主控单片机发送的串口配置，并转换为SCCB配置指令写入到摄像头。该摄像头具备掉电保存配置的能力，无需每次使用前都进行配置。

本驱动库是在逐飞提供的示例库的基础上修改而来，基本保留了其串口配置部分的原貌。摄像头数据接收部分则是使用DMADVP库（用于Kinetis系列单片机）或fsl_csi库（用于I.MX yyyy系列单片机）接收。本库具备直接生成DMADVP库配置数据的功能。



## 版本说明

### v0.1-beta.0

by CkovMk @hitsic 2020.10.20

初始版本

**改动说明**

- 初始版本

**开发计划**

- 暂无

**已知问题**

- 暂无



## API文档

详见头文件内的doxygen风格注释。



## 设计文档

（暂无）



## 应用指南

- 调用`void CAM_ZF9V034_GetDefaultConfig(cam_zf9v034_configPacket_t *config);`获取默认配置，或自行编写配置数据。
- 调用`void CAM_ZF9V034_CfgWrite(const cam_zf9v034_configPacket_t *config);`写入配置。
- 调用`void CAM_ZF9V034_CfgRead(cam_zf9v034_configPacket_t *config);`读出配置，并验证其是否正确。
- 调用`void CAM_ZF9V034_GetReceiverConfig(dmadvp_config_t* config, const cam_zf9v034_configPacket_t *camConfig);`生成对应接收器的配置数据，使用此数据初始化接受器并接收图像数据。





## 移植指南

### 数字接口

```c++
/**
 * @brief 阻塞同步串口接收函数。
 * 
 * @param data 数据指针
 * @param length 数据长度
 * @return status_t 返回状态，成功时返回kStatus_Success。
 */
inline status_t CAM_ZF9V034_UartRxBlocking(uint8_t *data, uint32_t length);

/**
 * @brief 阻塞同步串口发送函数。
 * 
 * @param data 数据指针
 * @param length 数据长度
 * @return status_t 返回状态，成功时返回kStatus_Success。
 */
inline status_t CAM_ZF9V034_UartTxBlocking(const uint8_t *data, uint32_t length);

/**
 * @brief 毫秒级延迟函数
 * 
 * @param ms 要延迟的毫秒数
 */
inline void CAM_ZF9V034_Delay_ms(uint32_t ms);
```



### 配置生成器

```c++
#ifndef ZF9V034_USE_DMADVP
#define ZF9V034_USE_DMADVP (1U)
#endif // ! ZF9V034_USE_DMADVP

#ifndef ZF9V034_USE_RTCSI
#define ZF9V034_USE_RTCSI (0U)
#endif // ! ZF9V034_USE_RTCSI
```





