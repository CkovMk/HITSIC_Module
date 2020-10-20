# DRV_DMADVP

[TOC]

## 简介

数字视频接口DMA接收器（DMADVP）是使用外部中断和DMA设备接收摄像头图像数据的组件。本组件适用于任何使用8-bit数字视频接口（DVP，Digital Video Port）的摄像头。

> DVP（Digital Video Port） 是传统的sensor输出接口，采用并行输出方式，d数据位宽有8bit、10bit、12bit、16bit，是CMOS电平信号（重点是非差分信号），PCLK最大速率为96MHz，接口如下图：
>
> ![img](drv_edmadvp.assets/20190317220219153.png)
>
> **PCLK**：pixel clock ,像素时钟，每个时钟对应一个像素数据；
> **HSYNC**：horizonal synchronization，行同步信号
> **VSYNC**：vertical synchronization，帧同步信号；
> **DATA**：像素数据，视频数据，具体位宽要看ISP是否支持；
> **XCLK**：或者MCLK，ISP芯片输出给驱动sensor的时钟；
> **SCL**，**SDA**：IIC用来读写sensor的寄存器，配置sensor。
>
> 以上内容摘自 https://blog.csdn.net/hua371242480/article/details/88629023 。如有侵权请联系我们删除。



## 版本说明

### v0.1-beta.0

by CkovMk @hitsic 2020.10.20

初始版本。

**改动说明**

- 初始版本

**开发计划**

- 暂无

**已知问题**

- 暂无





## API文档

详见头文件内的doxygen风格注释



## 设计文档

DMADVP组件在设计上模仿了I.MX RTyyyy系列单片机SDK中CSI外设的Transactional API。为了尽可能贴近CSI的使用体验，降低切换单片机的难度，我把DMADVP写成了虚拟外设的形式，代码如下：

```c++
struct DMADVP_Type
{
    INTC_Type* vsnc_intc;
    uint32_t vsnc_pin;
    INTC_Type* pclk_intc;
    uint32_t pclk_pin;

    uint32_t dmaDataAddress;
    uint32_t dmaChannel;
    uint32_t dmauxChannel;
    uint32_t dmamuxRequestSrc;
    IRQn_Type dmaIrqn;
    uint32_t dmaIrqPrio;

    uint32_t imgSize;

    port_interrupt_t vsncInterruptCfg;
    port_interrupt_t pclkInterruptCfg;
};

#define DMADVP_CNT (1U)

#define DMADVP0_DAT0_GPIO          RTEPIN_DIGITAL_CAM_D_0_GPIO
#define DMADVP0_DAT0_PORT          RTEPIN_DIGITAL_CAM_D_0_PORT
#define DMADVP0_DAT0_PIN           RTEPIN_DIGITAL_CAM_D_0_PIN
#define DMADVP0_DMA_INST           (DMA0)
#define DMADVP0_DMAMUX_INST        (DMAMUX0)
#define DMADVP0_DATA_DMA_ADDR      (((uint32_t)DMADVP0_DAT0_GPIO)+0x10+DMADVP0_DAT0_PIN/8)
extern DMADVP_Type __DMADVP0;
#define DMADVP0 (&__DMADVP0)
```

您可以像使用单片机原生的GPIO外设一样使用本模组。但是请时刻记得，该模块是由GPIO外部中断、DMA触发器（DMAMUX）和DMA设备共同组合成的软件模拟虚拟设备，在配置这些设备的中断优先级时务必仔细处理。



## 应用指南





## 移植指南



