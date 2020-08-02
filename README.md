# HITSIC_Module 组件库

[TOC]

## 1. 简介

HITSIC_Module是哈工大智能车创新俱乐部各MCU平台共用的程序模组件。目前仍在开发中，支持MK66、KV1x、KV58、I.MX RT1052四款NXP公司32位单片机。组件库内部分为四种组件：LIB、DRV、SYS和APP。LIB组件是一些与平台无关的算法，DRV组件是一些外设和模块的驱动程序，SYS主要用于管理一些系统资源，APP是面向特定问题的应用。大多数组件都由两部分组成：平台无关的公共代码和平台相关的移植代码。这样的结构使得即使脱离了组件库移植到其他平台，使用上也非常方便。



## 2. 文档



## 3. 附录

### 3.1. 如何添加此模组库到已有底层

gitmodule的使用方法
```
//添加submodule
git submodule add https://github.com/CkovMk/HITSIC_Module /HITSIC_Module
```

## 4. 许可

Apache 2.0许可，详细信息参见根目录下LICENSE文件。





