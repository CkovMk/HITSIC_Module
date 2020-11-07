# DRV_FTFX_FLASH

[TOC]

## 简介

FTFX_FLASH组件是NXP单片机内部Flash的读写库，适用于具有FTFX控制器控制的内部Flash的Kinetis系列单片机。在本组件库内用于MK66单片机和KV58单片机片上Flash存储的读写。APP_MENU组件的MENU_NVM子组件依赖本组件提供片上Flash的数据存储能力。



## 版本说明

### v0.2.0

by CkovMk @hitsic 2019.10.08

**改动说明**

- 支持所有FTFX_FLASH单片机。
- 出于性能考虑，删除了`status_t FLASH_Ram2Flash(uint32_t dst, void\* src, uint32_t size);`和`status_t FLASH_Flash2Ram(void\* dst, uint32_t src, uint32_t size);`两个API。

**开发计划**

- 暂无

**已知问题**

- 在KV1x系列单片机上未经过测试。



### v0.1.1

by beforelight @ 2019.09.22



### v0.1-beta.0

by beforelight @ 2019.07.25



## API文档

详见`drv_ftfx_flash.h`内的doxygen风格注释。



## 设计文档

无



## 应用指南



## 移植指南

本组件不支持移植。

