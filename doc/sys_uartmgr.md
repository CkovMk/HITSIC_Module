# SYS_UARTMGR

[TOC]

## 简介

串行接口管理器（UARTMGR，Universal Asynchronous Receiver/Transmitter Manager），是管理系统串口的系统组件，为其他上层应用提供完整的串口收发支持。目前支持3套完整的API：阻塞API（Polling）、中断API（Interrupt）、数传协议（Transfer Protocal）。另有类printf函数可以使用。

**该组件可能即将废弃。**



## 版本说明

### v0.1.1

by CkovMk @hitsic 2020.08.08

**改动说明**

- 完善”多件模式”。
- 新增“Transfer Protocal”传输协议和非阻塞API。

**开发计划**

- 完善传输协议（Transfer Protocal），对接RMCALL。

**已知问题**

- 暂无



### v0.1-beta.0

by CkovMk @hitsic 2019.11.24

**改动说明**

- 首个版本。

**开发计划**

- 增加传输协议（Transfer Protocal）。
- 增加非阻塞API。

**已知问题**

- 暂无



## API文档

- **（多件模式）获取对象引用**

  `static uartMgr_t& uartMgr_t::getInst(UART_Type* instNum);`

  向该API传入与UARTMGR绑定的UART设备地址，将返回对对应`uartMgr_t`对象的引用。**注意：如果传入非法地址，将抛出`std::invalid_argument`异常。**

  UARTMGR采用了改进的“单件模式”——多件模式，您无法在运行时创建`uartMgr_t`对象，而只能获取已有对象的引用。对于一个给定的BSP，串口的数量是一定的。显然，允许用户在运行时随意创建对象没有任何意义。

- **阻塞同步API**

	`status_t uartMgr_t::txPoll(uint8_t* data, uint32_t size);`
	阻塞发送数据。数据发送完成后返回。用法与FSL库函数相同。

	`status_t uartMgr_t::rxPoll(uint8_t* data, uint32_t size);`
	阻塞接收数据。如果没有接收到`size`个字节的数据，该函数将永不退出。用法与FSL库函数相同。
	
- **阻塞同步格式化输出**

	`int32_t uartMgr_t::uprintf(const char* _fmt, ...);`
	阻塞发送格式化字符串。

- **非阻塞异步API**

	`status_t uartMgr_t::txIntr(uint8_t* data, uint32_t size, uart_transfer_callback_t callback, void* userData);`
	启动异步发送任务。用法与FSL库函数相同。

	`static void uartMgr_t::defaultCallback(UART_Type* base, uart_handle_t* handle, status_t status, void* userData);`
	异步API的默认回调函数，维护了UARTMGR内部的状态机。如要实现其他功能，您也可以自己实现回调函数，但是必须在您的回调函数中调用本函数，以保证UARTMGR内部的状态变量得到维护。

	`void uartMgr_t::txIntrAbort(void);`
	停止正在进行的异步发送任务。

	`status_t uartMgr_t::rxIntr(uint8_t* data, uint32_t size, uart_transfer_callback_t callback, void* userData);`
	启动异步接受任务。用法与FSL库函数相同。

	`void uartMgr_t::rxIntrAbort(void);`
	停止正在进行的异步接受任务。

- **简单传输协议（Transfer Protocal）API**

	`status_t uartMgr_t::tpModeSetup(uint8_t* dataBuf, tpDataHandler_t handler);`
	初始化传输协议并设置数据处理回调函数。**注意：传输协议不能与其他API混用。**

	> `typedef void (*tpDataHandler_t)(uint8_t* dataBuf, int32_t size);`：
	>
> 当数据传输完成后，将调用注册的回调函数处理数据。该回调函数运行于串口中断服务函数的中断优先级。

	`void uartMgr_t::tp_defaultcallback(UART_Type* base, uart_handle_t* handle, status_t status, void* userData);`
	传输协议的串口中断默认回调函数。无需用户调用。
	
	`void uartMgr_t::tp_txIntr(uint8_t* data, uint32_t size);`
	通过传输协议发送数据。





## 设计文档

### 传输协议





## 应用指南





## 移植指南



