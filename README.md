# HITSIC_Module 组件库

[TOC]

## 1. 简介

HITSIC_Module是哈工大智能车创新俱乐部各MCU平台共用的程序模组件。目前仍在开发中，支持MK66、KV1x、KV58、I.MX RT1052四款NXP公司32位单片机。组件库内部分为四种组件：LIB、DRV、SYS和APP。LIB组件是一些与平台无关的算法，DRV组件是一些外设和模块的驱动程序，SYS主要用于管理一些系统资源，APP是面向特定问题的应用。大多数组件都由两部分组成：平台无关的公共代码和平台相关的移植代码。这样的结构使得即使脱离了组件库移植到其他平台，使用上也非常方便。



## 2. 文档



### 2.1. 内部组件文档

| 模块名                                  | 说明                                                      |
| --------------------------------------- | --------------------------------------------------------- |
| **INC / 包含**                          |                                                           |
| HITSIC_COMMON | 设备公共包含头文件。 |
| INC_STDLIB                              | 包含了大多数C/C++标准库、位操作宏定义，禁用了不安全函数。 |
| [INC_SYSLOG](doc/inc_syslog.md) | 简易日志库。 |
| **SYS / 系统**                          |                                                           |
| LTC | 生命周期定时器 |
| [EXTINT](doc/sys_extint.md)             | 外部中断管理器组件                                        |
| [PITMGR](doc/sys_pitmgr.md)             | 定时中断管理器组件                                        |
| [RMCALL](doc/sys_rmcall.md)         | 远程调用组件                                              |
| **DRV / 驱动**                          |                                                           |
| [FTFX_FLASH](doc/drv_ftfx_flash.md)     | Kinetis系列单片机内部Flash驱动组件                        |
| [BUTTON](doc/drv_button.md)             | 按钮事件识别组件                                          |
| DISP_FONT                               | 显示字体组件                                              |
| [DISP_SSD1306](doc/drv_disp_ssd1306.md) | OLED屏幕SSD1306驱动组件                                   |
| BETA：DISP_SSD1327 | OLED屏幕SSD1327驱动组件 |
| WIP：ST7789 |  |
| [IMU_INVENSENSE](doc/drv_imu_invensense.md) | Invensense公司IMU惯导模块统一驱动组件（Deprecated）       |
| WIP：TCS34725                           | I2C颜色传感器                                             |
| WIP：TFMINI                             | TFmini激光测距雷达                                        |
| WIP：TOF_VL53L0x                        | ST VL53L0x激光测距传感器                                  |
| WIP：TOF_VL53L1x                        | ST VL53L1x激光测距传感器                                  |
| [DMADVP](doc/drv_dmadvp.md)    | DVP摄像头DMA接收器模组，用于Kinetis系列单片机             |
| [CAM_ZF9V034](doc/drv_cam_zf9v034.md) | 逐飞总钻风（CMOS：MT9V034）摄像头配置模组                 |
| WIP：NRF24L01 |  |
| **LIB / 工具**                          |                                                           |
| LIST                                    | 轻量级链表组件                                            |
| PIDCTRL                                 | PID控制组件                                               |
| ADRCCTRL                                | ADRC控制组件                                              |
| **APP / 应用**                          |                                                           |
| [MENU](doc/app_menu.md)                 | 简单调参菜单组件                                          |
| SVBMP | 保存图像为BMP图像文件 |



### 2.2. 外部组件文档

| 组件名称                                               | 说明                                                         |
| ------------------------------------------------------ | ------------------------------------------------------------ |
| [CmBackTrace](https://github.com/armink/CmBacktrace)   | Cortex-M 系列 MCU 的错误代码自动追踪、定位，错误原因自动分析的开源库。 |
| [EasyFlash](https://github.com/armink/EasyFlash)       | 开源的轻量级嵌入式Flash存储器库                              |
| [m*lib](https://github.com/P-p-H-d/mlib)               | C语言STL容器库                                               |
| [drv_invimu](https://github.com/beforelight/c_inv_imu) | 社团学长维护的Invensense公司IMU统一驱动库                    |



## 3. 附录

### 3.1. 如何添加此组件库到已有底层

gitmodule的使用方法
```
//添加submodule
git submodule add https://github.com/CkovMk/HITSIC_Module /HITSIC_Module
```



### 3.2. 如何阅读组件文档

组件文档一般包含以下内容：

> 模块简介

简单介绍该组件的功能、使用限制等。选用模块时，应通读此部分。

> 版本说明

该组件历史版本的更新说明。

> API文档

该组件当前版本的API文档。大多数时候，阅读API文档可以对整个模组的使用有一个大概的掌握。

> 设计文档

该组件的一些核心设计原理。如果想要了解该组件的工作原理，或对该组件进行二次开发，则应该阅读该部分。

> 应用指南

补充说明该组件的使用方法。一般只有在一些复杂的组件中才会有这部分内容。

> 移植指南

如何移植该组件到其他MCU平台。





## 4. 许可

Apache 2.0许可，详细信息参见根目录下LICENSE文件。

本库内引用了[CMSIS_5](https://github.com/ARM-software/CMSIS_5)、[MCUXpresso SDK](https://mcuxpresso.nxp.com/en/welcome)、[CmBacktrace](https://github.com/armink/CmBacktrace)、[EasyFlash](https://github.com/armink/EasyFlash)、[EasyLogger](https://github.com/armink/EasyLogger)中的代码。





