# SYS_PITMGR

[TOC]

## 简介

定时中断管理器（PITMGR，Periodic Interrupt Timer Manager），是利用定时器计时产生的稳定中断服务，为系统提供全局时基、延迟服务、周期性任务调度的模块。该模块能够实现1ms的计时精度和延时精度，并且能够管理多个任务，让他们定时运行。相较于RTOS，该模块在牺牲了任务并行、非阻塞延迟、优先级抢占等功能的代价下，实现了相对精细的定时任务控制，为时间关键型任务例如PID控制、数据采集等任务提供了运行平台。

该模块的主要限制有：

1. 计时精度严重依赖于定时器中断，如果定时器中断长时间得不到响应，该模块将会出错。
2. 不能为中断优先级高于定时器中断优先级的函数提供计时和延时服务，因为定时器中断无法工作。事实上，您不应该在任何中断服务函数中使用任何阻塞式延时功能。
3. 调度器调度的任务必须是快速任务，应在1ms内执行完毕。如果一次定时器中断内执行任务的总时长超过2ms，就有可能错过下一次定时器中断，进而引发一定的时序混乱。因此，我们引入了“错位运行”机制，尽量使任务错峰运行，这有助于缓解这一情况，但使用时仍需特别注意。目前”自动推迟“功能已在计划中，未来将作为灰度测试加入。
4. 未对变量溢出进行处理，在PITMGR开始运行后约50天后，主计时变量会溢出。不过大部分情况下这并不构成影响。如有必要，将主计时变量设为uint64_t即可。



## 版本说明

### v1.0.0

改动说明：

- PITMGR从此版本起不再支持LTC。为了更好的兼容性，删除了64-bit LTC相关的功能。如果需要LTC，您仍可以自己调用系统底层函数。
- 增加了`D_KV10Z7`单片机的支持。
- 调整了代码的包含关系。
- 增加了`HITSIC_USE_PITMGR`宏，可以方便地启用或禁用本模块。

开发计划：

- 调度器增加任务超时时自动延迟至下一周期运行的功能，并可由`pptFlag`控制。
- 增加根据服务函数查找、编辑和删除任务的功能。这将使得中断列表支持插入等操作，也无需再手动保存任务指针。

已知问题：

- 暂无



## API文档

- **初始化函数 `static status_t pitMgr_t::init(void);`**

  该函数用于初始化PITMGR。调用该函数时，该函数会清空任务列表`std::list<pitMgr_t> pitMgr_t::isrSet`并复位毫秒计时器`uint32_t pitMgr_t::timer_ms`。如果使能了平台初始化功能（由宏`HITSIC_PITMGR_INITLIZE`控制），还会调用您在移植文件中定义的`void PITMGR_PlatformInit(void);`函数，用于初始化定时器硬件。通常情况下，定时器硬件可以用NXP官方配置工具自动生成初始化代码。当前版本中本函数总是返回`kStatus_Success`。

- **计时函数 `static uint32_t pitMgr_t::getTimer(void);`**

  该函数用于读取当前系统运行的毫秒数，可用于精度不高的计时。

- **阻塞延迟函数 `static void pitMgr_t::delay_ms(uint32_t _t);`**

  该函数用于阻塞延迟。不能在中断服务函数中使用。

- **插入任务 `static pitMgr_t* pitMgr_t::insert(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt);`**

  该函数用于向任务列表中插入一个任务。

  参数`_ms` ：表示该任务运行的周期，单位是毫秒。

  参数`_mso` ：表示该任务运行周期的偏移。例如，`_ms` = 10，`_mso` = 3时，该任务将在第3毫秒、第13毫秒、第23毫秒......运行。

  参数`_handler` ：任务的服务函数，在任务激活时调用。其原型为`typedef void (*handler_t)(void);`。

  参数`_ppt` ：属性列表。该参数的取值范围为为枚举变量`pitMgr_t::pptFlag_t`中各枚举量的按位或。

  返回值：指向该任务的指针。可用于重新配置任务或删除任务。插入任务失败时会返回空指针。

- **移除任务 `static status_t pitMgr_t::remove(pitMgr_t &_handle);`**

  该函数用于移除任务。如果任务存在且成功移除则返回`kStatus_Success`，如果任务不存在或移除失败，则返回`kStatus_Fail`。移除任务成功后指针将变为野指针，需要用户自行置为`NULL`。

- **设置任务 `void pitMgr_t::setup(uint32_t _ms, uint32_t _mso, handler_t _handler, uint32_t _ppt);`**

  该函数用于重设一个已有的任务。

- **设置任务 `void pitMgr_t::setEnable(bool _b);`**

  该函数控制`pptFlag`中启用属性的开关。

- **服务接口 `static void pitMgr_t::isr(void);`**

  该函数无需用户调用，是由系统自动在定时器中断服务函数中调用的。

  注意：该函数仅包含PITMGR的内部逻辑，不负责具体的硬件操作。定时器中断服务函数中除调用此函数外，还应包含清楚中断标志位等必须的操作。



模块内还包含用于兼容早期C语言版本PITMGR模块的C风格兼容层，仅供兼容旧代码使用，一旦旧代码迁移完成将删除该兼容层，因此禁止新代码使用。



## 设计文档



## 应用指南



## 移植指南

### 移植文件`sys_pitmgr_port.hpp`

主要包含以下项目：

- **宏：启用控制 `HITSIC_USE_PITMGR`**

  0：禁用PITMGR；其他：启用PITMGR。

- **宏：定时器计数频率 `HITSIC_PITMGR_CNTFREQ`**

  定时器计数的频率。注意：由于定时器内部分频器的存在，计数频率不一定等于定时器输入时钟的频率。

- **宏：平台初始化控制 `HITSIC_PITMGR_INITLIZE`**

  0：禁用平台初始化函数；其他：启用平台初始化函数。

- **函数：平台初始化函数 `inline void PITMGR_PlatformInit(void);`**

  平台初始化函数用于初始化定时器等PITMGR的相关硬件，如果启用平台初始化功能，该函数会在`status_t pitMgr_t::init(void);`中调用。如不适用平台初始化函数，您应在调用`status_t pitMgr_t::init(void);`前自行初始化定时器硬件。无论采用何种初始化方法，您应保证使定时器每1ms触发一次中断并调用中断服务函数，且中断优先级不能过低。推荐的中断优先级是2~4级（共16级优先级时）。

- **宏：启用移植文件内的中断服务函数 `HITSIC_PITMGR_DEFAULT_IRQ`**

  0：禁用；其他：启用。启用该宏将启用位于`sys_pitmgr_port.cpp`内的中断服务函数。您也可以在其他地方自行编写中断服务函数。例如，如果您更倾向于把中断服务函数集中在一个源文件内管理，就可以禁用此选项。
  
  

### 移植文件`sys_pitmgr_port.cpp`

主要包含以下项目：

- 函数：PITMGR中断服务函数

  一个在K66、KV10、KV58平台使用LPTMR定时器的例子是：

  ```c++
  #ifdef __cplusplus
  extern "C"{
  #endif
  
	#if defined(HITSIC_PITMGR_DEFAULT_IRQ) && (HITSIC_PITMGR_DEFAULT_IRQ > 0)
	void LPTMR0_IRQHandler(void)
	{
		LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
		pitMgr_t::isr();
	}
	#endif // ! HTISIC_PITMGR_USE_IRQHANDLER
	
	#ifdef __cplusplus
	}
	#endif
	```
	
	该中断函数应每1ms执行一次。

