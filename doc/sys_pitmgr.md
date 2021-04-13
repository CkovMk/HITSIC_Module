# SYS_PITMGR

[TOC]

## 简介

定时中断管理器（PITMGR，Periodic Interrupt Timer Manager），是利用定时器计时产生的稳定中断服务，为系统提供全局时基、延迟服务、周期性任务调度的模块。该模块能够实现1ms的计时精度和延时精度，并且能够管理多个任务，让他们定时运行。相较于RTOS，该模块在牺牲了任务并行、非阻塞延迟、优先级抢占等功能的代价下，实现了相对精细的定时任务控制，为时间关键型任务例如PID控制、数据采集等任务提供了运行平台。

该模块的主要限制有：

1. 计时精度严重依赖于定时器中断，如果定时器中断长时间得不到响应，该模块将会出错。
3. 调度器调度的任务必须是快速任务，应在一个中断周期内执行完毕。如果一次定时器中断内执行任务的总时长超过一个终端周期，就有可能错过下一次定时器中断，进而引发一定的时序混乱。因此，我们引入了“错位运行”机制，尽量使任务错峰运行，这有助于缓解这一情况，但使用时仍需特别注意。
4. 未对变量溢出进行处理。主计时变量为uint64_t，一般可以认为永不溢出。

## 版本说明

### v2.0.0

by CkovMk @hitsic 2021.03.27

**改动说明**

- 使用C语言重构代码，提高了兼容性。
- 采用多实例的写法，您现在可以添加多个PITMGR实例。
- 简化了接口。描述任务的结构体（即任务描述符）不再保存于PITMGR实例的结构体中，而是由用户创建和管理。PITMGR实例只持有指向任务描述符的指针。
- 支持自定义PITMGR的中断周期。

**开发计划**

- 暂无

**已知问题**

- 暂无



### v1.0.1

by CkovMk @hitsic 2020.10.30

**改动说明**

- 增加了`HITSIC_USE_PITMGR`宏，可以方便地启用或禁用本模块。
- 修改了服务函数接口，新增了用户参数，添加了`setUserData`方法。

**开发计划**

- 调度器增加任务超时时自动延迟至下一周期运行的功能，并可由`pptFlag`控制。
- 增加根据服务函数查找、编辑和删除任务的功能。这将使得中断列表支持插入等操作，也无需再手动保存任务指针。

**已知问题**

- 暂无



### v1.0.0

by CkovMk @hitsic 2020.07.25

**改动说明**

- PITMGR从此版本起不再支持LTC。为了更好的兼容性，删除了64-bit LTC相关的功能。如果需要LTC，您仍可以自行调用系统底层函数。
- 增加了`D_KV10Z7`单片机的支持。
- 调整了代码的包含关系。

**开发计划**

- 调度器增加任务超时时自动延迟至下一周期运行的功能，并可由`pptFlag`控制。
- 增加根据服务函数查找、编辑和删除任务的功能。这将使得中断列表支持插入等操作，也无需再手动保存任务指针。

**已知问题**

- 暂无



### v0.4.1

by CkovMk @hitsic 2019.11.02



### v0.4-beta.0

by CkovMk @hitsic 2019.10.28

### ......

### v0.1.0

by CkovMk @hitsic 2018.12.20



## API文档

- **初始化函数 `status_t PITMGR_Init(pitmgr_t *_inst, uint32_t _period_us);`**

  该函数用于初始化PITMGR。调用该函数时，该函数会清空任务列表并复位计时器。当前版本中本函数总是返回`kStatus_Success`。

- **解初始化函数 `void PITMGR_Deinit(pitmgr_t *_inst);`**

- **插入任务 `status_t PITMGR_HandleInsert(pitmgr_t *_inst, pitmgr_handle_t *_handle);`**

  该函数用于向任务列表中插入一个任务。

  返回值：成功返回`kStatus_Success`，异常返回`kStatus_Fail`。

- **移除任务 `status_t PITMGR_HandleRemove(pitmgr_t *_inst, pitmgr_handle_t *_handle);`**

  该函数用于移除任务。如果任务存在且成功移除则返回`kStatus_Success`，如果任务不存在或移除失败，则返回`kStatus_Fail`。

- **服务接口 `void PITMGR_Isr(pitmgr_t *_inst);`**

  该函数无需用户在使用PITMGR时调用，而是由系统在定时器中断服务函数中调用的。

  注意：该函数仅包含PITMGR的内部逻辑，不负责具体的硬件操作。定时器中断服务函数中除调用此函数外，还应包含清除中断标志位等必需的操作。



## 设计文档





## 应用指南

### 初始化

- 创建PITMGR实例：`pitmgr_t pitMain;`
- 调用初始化函数：`PITMGR_Init(&pitMain, 1000U);`即可。



### 基础使用


- 创建服务函数：
  创建一个符合`typedef void (*pitMgr_t::handler_t)(void *userData);`定义的服务函数。
  
- 创建服务描述符：
  ```c
  pitmgr_handle_t imu_invensense_test_pitHandle =
  {
      .tickInterval = 5UL, ///< 运行间隔
      .tickOffset = 1UL, ///< 运行偏移
      .handler = myHandler, ///< 服务函数指针
      .pptFlag = pitmgr_pptEnable, ///< 属性Flag
      .userData = NULL, ///< 用户数据，不用则留空
  };
  ```

  

  

- 将服务函数注册至列表

  调用`status_t PITMGR_HandleInsert(pitmgr_t *_inst, pitmgr_handle_t *_handle);`函数，即可注册该服务函数。执行此函数不会修改服务描述符。

  成功返回kStatus_Success，异常返回kStatus_Fail。

- 从列表中删除服务函数

  调用`status_t PITMGR_HandleRemove(pitmgr_t *_inst, pitmgr_handle_t *_handle);`函数，即可取消该服务描述符。执行此函数不会修改服务描述符。
  
  成功返回kStatus_Success，异常返回kStatus_Fail。



### 变更属性、使用用户参数

直接修改注册任务时使用的`pitmgr_handle_t`实例。



### 注意事项

暂无



## 移植指南

### 移植文件`sys_pitmgr_port.h`

主要包含以下项目：

- **宏：启用移植文件内的中断服务函数 `HITSIC_PITMGR_DEFAULT_IRQ`**

  0：禁用；其他：启用。启用该宏将启用位于`sys_pitmgr_port.cpp`内的中断服务函数。您也可以在其他地方自行编写中断服务函数。例如，如果您更倾向于把中断服务函数集中在一个源文件内管理，就可以禁用此选项。
  
  

### 移植文件`sys_pitmgr_port.c`

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
	    extern pitmgr_t pitmgr_main;
		LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
		PITMGR_Isr(&pitmgr_main);
	}
	#endif // ! HTISIC_PITMGR_USE_IRQHANDLER
	
	#ifdef __cplusplus
	}
	#endif
	```
	
	

