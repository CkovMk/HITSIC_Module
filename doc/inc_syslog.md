# INC_SYSLOG

[TOC]

## 简介

系统日志（SYSLOG）是一个简易日志库模组。本模组的主要功能有：

- 用于打印LOG的统一接口，阻塞输出。
- 可为每个文件设置独立的日志Tag。
- 可为每个文件设置静态日志级别控制。



## 版本说明

### v0.1.0

by CkovMk @hitsic 2020.11.24

首个版本。实现了LOG输出、LOG TAG设置、静态LOG级别控制。

**改动说明**

- 首个版本。

**开发计划**

- 开发环形缓存/非阻塞输出模式。
- Log存储、回放模式。
- 使用`ANSI escape code`实现彩色显示。

**已知问题**

- 暂无。



## API文档

**`SYSLOG_X(...)`：类printf函数。使用时，会自动在前面添加LOG级别、LOG_TAG信息，自动在尾部添加换行。包含以下六种级别：**

- `SYSLOG_A`：0.断言（Assert）级别。发生严重错误，导致程序必须立即终止时使用。无法被屏蔽 。

- `SYSLOG_E`：1.错误（Error）级别。发生意料之内的异常，但无法被处理时使用。

- `SYSLOG_W`：2.警告（Warning）级别。发生意料之内的异常，且可以被处理时使用。

- `SYSLOG_I`：3.信息（Infomation）级别。用于指示程序运行的进度或状态。不能在对实时性有要求的上下文中使用。一般情况下使用该级别日志输出。

- `SYSLOG_D`：4.调试（Debug）级别。用于输出进一步的调试信息。可以在对实时性有要求的上下文中使用，会影响程序的实时性。

- `SYSLOG_V`：5.冗长（Verbose）级别。最详细的日志信息。一般仅在开发时启用。



## 应用指南

#### SYSLOG基本用法

SYSLOG仅支持在源文件（\*.c、\*.cpp）中使用。

首先定义宏`SYSLOG_TAG`和`SYSLOG_LVL`。`SYSLOG_TAG`是表示LOG信息来源的字符串，将自动打印在LOG级别之后、具体信息之前。`SYSLOG_LVL`则控制**该文件中**LOG信息的最低级别。例如，`#define SYSLOG_LVL (3U)`将启用`SYSLOG_I`及以上LOG级别的输出，而禁用`SYSLOG_D`和`SYSLOG_V`。

然后包含`inc_syslog.hpp`。在这个特殊的应用场景下，可以在源文件内直接包含其他头文件。

完成以上工作后，就可以在本源文件内使用`SYSKOG_X(...)`API进行LOG输出了。



#### SYSLOG进阶用法：在一个源文件中使用多个LOG_TAG

如果想在一个源文件内使用几个不同的LOG_TAG和LOG_LVL，该如何实现呢？

该源文件中的第一个LOG_TAG和LOG_LVL与基础用法没有任何区别。

要定义第二个LOG_TAG和LOG_LVL时：使用`#undef SYSLOG_TAG`和`#undef SYSLOG_LVL`取消定义这两个宏的值，然后重新定义`SYSLOG_TAG`和`SYSLOG_LVL`。再次包含`inc_syslog.hpp`即可。

> 实现原理：`inc_syslog.hpp`中已经处理过这种情况。当在一个文件内多次包含该头文件时，其会先`#undef`所有的`SYSLOG_X(...)`宏定义，再根据最近一次定义的`SYSLOG_TAG`和`SYSLOG_LVL`重新定义各个`SYSLOG_X(...)`宏。



## 设计文档

暂无



## 移植指南

定义调试宏。如果没有`#define SYSLOG_DEBUG`，SYSLOG将不会打印任何信息。我们这里把`SYSLOG_DEBUG`连接到编译器`DEBUG`，这样就可以在编译release时自动禁用LOG输出。

```c++
#ifdef DEBUG
#define SYSLOG_DEBUG
#endif
```



定义打印函数。

```c++
#define HITSIC_LOG_PRINTF(...)  (printf(__VA_ARGS__))
```



定义换行符。

```c++
#define HITSIC_LOG_LINEFEED ("\r\n")
```





