# SYS_EXTINT

[TOC]


## 简介

外部中断管理器（EXTINT，External Interrupt Manager），是用于响应外部中断请求的模块。相较于常规的外部中断时常常是一个PORT对应一个中断服务函数，甚至多个PORT对应一个中断服务函数的情况，EXTINT通过二次判断中断标志位，实现精确到每个PIN的中断路由管理。这为多个模块共用一个PORT上的中断、一个模块使用多个PORT上的中断等复杂情况提供了极大的便利。



## 版本说明

### v2.0.0

by CkovMk @hitsic 2021.04.03

**改动说明**

- 使用C语言重构代码，提高了兼容性。
- 采用多实例的写法，您现在可以根据外部中断硬件控制器的数量添加多个EXTINT实例。
- 简化了接口。描述任务的结构体（即任务描述符）不再保存于EXTINT实例的结构体中，而是由用户创建和管理。EXTINT实例只持有指向任务描述符的指针。

**开发计划**

- 暂无

**已知问题**

- 暂无



### v1.0.1

by CkovMk @hitsic 2020.10.30

**改动说明**

- 仿照PITMGR添加`HITSIC_USE_EXTINT`，可以快速禁用EXTINT模块。
- 修改了服务函数接口，新增了用户参数，添加了`setUserData`方法。

**开发计划**

- 优化查询速度。

**已知问题**

- 暂无



### v1.0-beta.0

by CkovMk @hitsic 2020.07.25

**改动说明**

- 全新C++ API
- 使用std::map作为存储容器，查询速度更快。
- 增加了对`d_kv10z7`单片机的支持

**开发计划**

- 仿照PITMGR添加`HITSIC_USE_EXTINT`，可以快速禁用EXTINT模块。

**已知问题**

- 暂无



### v0.2.1

by CkovMk @hitsic 2019.11.05



### v0.2-beta.0

by CkovMk @hitsic 2019.10.20

### ......

### v0.1.0

by CkovMk @hitsic 2018.12.23



## API文档

- **初始化函数 `status_t EXTINT_Init(extint_t *_inst);`**

  该函数用于初始化EXTINT。调用该函数时，该函数会清空任务列表。当前版本中本函数总是返回`kStatus_Success`。

- **解初始化函数 `void EXTINT_Deinit(extint_t *_inst);`**

- **插入任务 `status_t EXTINT_HandleInsert(extint_t *_inst, extint_handle_t *_handle);`**

  该函数用于向任务列表中插入一个任务。

  返回值：成功返回kStatus_Success，异常返回kStatus_Fail。

- **移除任务 `status_t EXTINT_HandleRemove(extint_t *_inst, extint_handle_t *_handle);`**

  该函数用于移除任务。如果任务存在且成功移除则返回`kStatus_Success`，如果任务不存在或移除失败，则返回`kStatus_Fail`。

- **服务接口 `EXTINT_Isr(extint_t *_inst, uint32_t flag)`**

  该函数无需用户在使用EXTINT时调用，而是由系统在定时器中断服务函数中调用的。调用时须传入中断标志位。

  注意：该函数仅包含EXTINT的内部逻辑，不负责具体的硬件操作。定时器中断服务函数中除调用此函数外，还应包含清除中断标志位等必需的操作。



## 设计文档



## 应用指南

### 初始化

- 根据外部中断控制器的数量创建EXTINT实例：`extint_t extint_porta, extint_portb;`。

  > 例如：
  >
  > 所有GPIO都可以产生外部中断：例如MK66等平台，外部中断控制器为PORT外设，则应按照PORT外设的数量来创建EXTINT实例。当然，如果不使用部分PORT外设的外部中断功能，也可省略这些外设所对应的EXTINT实例。
  >
  > 由专门的外设控制外部中断：例如STM32平台，外部中断由EXTI外设控制，则仅需创建一个EXTINT实例即可。
  >
  > 特别地，对于I.MX RT平台，在每个GPIO最多拥有32个引脚的情况下，其中断服务函数被进一步分为了GPIOx_0_15中断和GPIOx_16_31中断。此时，应为每个GPIO外设创建两个EXTINT实例。

- 调用初始化函数：`EXTINT_Init(&extint_porta);`即可。

- 注意：EXTINT仅负责中断事件的转发，不负责硬件的初始化。有关引脚路由、引脚中断方式选择、启用中断、设置中断优先级等初始化须由用户执行。



### 基础使用


- 创建服务函数：
  创建一个符合`typedef void (*extint_handler_t)(void *userData);`定义的服务函数。

- 创建服务描述符：

  ```c
  typedef struct _extint_handle
  {
      uint32_t index;				/*< 引脚号 */
      extint_handler_t handler;   /*< 指向中断服务函数的指针 */
      void *userData;  			/*< 用户变量 */
  }extint_handle_t;
  ```

  

  

- 将服务函数注册至列表

  调用`status_t EXTINT_HandleInsert(extint_t *_inst, extint_handle_t *_handle);`函数，即可注册该服务函数。执行此函数不会修改服务描述符。

  成功返回kStatus_Success，异常返回kStatus_Fail。

- 从列表中删除服务函数

  调用`status_t EXTINT_HandleRemove(extint_t *_inst, extint_handle_t *_handle);`函数，即可取消该服务描述符。执行此函数不会修改服务描述符。

  成功返回kStatus_Success，异常返回kStatus_Fail。



### 变更属性、使用用户参数

直接修改注册任务时使用的`extint_handle_t`实例。



### 注意事项

暂无



## 移植指南

### 移植文件`sys_extint_port.h`

- **移植平台接口**

  ```c++
  typedef PORT_Type INTC_Type;
  typedef port_interrupt_t extInt_interruptMode_t;
  
  #define EXTINT_InterruptOrDMADisabled	kPORT_InterruptOrDMADisabled
  #define EXTINT_DMARisingEdge 			kPORT_DMARisingEdge
  #define EXTINT_DMAFallingEdge 			kPORT_DMAFallingEdge
  #define EXTINT_DMAEitherEdge 			kPORT_DMAEitherEdge
  #define EXTINT_InterruptLogicZero 		kPORT_InterruptLogicZero
  #define EXTINT_InterruptRisingEdge 		kPORT_InterruptRisingEdge
  #define EXTINT_InterruptFallingEdge 	kPORT_InterruptFallingEdge
  #define EXTINT_InterruptEitherEdge 		kPORT_InterruptEitherEdge
  #define EXTINT_InterruptLogicOne		kPORT_InterruptLogicOne
  
  #define EXTINT_SetInterruptConfig(_intc, _pin, _cfg) 	PORT_SetPinInterruptConfig(_intc, _pin, _cfg)
  #define EXTINT_GetInterruptFlags(_intc) 				PORT_GetPinsInterruptFlags(_intc)
  #define EXTINT_ClearInterruptFlags(_intc, _mask) 	    PORT_ClearPinsInterruptFlags(_intc, _mask)
  ```

  这里主要是移植一些平台相关的定义和函数。

  

  ```c++
  #define HTISIC_EXTINT_DEFAULT_IRQ 	(1U)
  ```

  该宏控制是否启用`sys_extint_port.hpp`中的中断服务函数。

  

- **查询函数**

  ```c++
  inline INTC_Type *EXTINT_GetPortInst(GPIO_Type *gpio)
  {
  	static PORT_Type *lut[] = PORT_BASE_PTRS;
  	return lut[((uint32_t)gpio - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)];
  }
  ```
  此函数的功能是根据控制GPIO输入、输出的外设地址查询对应的控制中断的外设地址。如果此单片机中控制GPIO输入、输出等功能的外设与控制外部中断的外设不同（例如K66的GPIO和PORT），则需要适配此函数。
  
  对于控制GPIO输入、输出等功能的外设与控制外部中断的外设相同的单片机（例如RT1052），由于`INTC_Type`实际上就是`GPIO_Type`，直接返回传入的参数即可。



### 移植文件`sys_extint_port.c`

- **中断服务函数**

  ```c++
  #include <sys_extint.h>
  
  #if defined(HITSIC_USE_EXTINT) && (HITSIC_USE_EXTINT > 0)
  
  #ifdef __cplusplus
  extern "C"{
  #endif
  
  extern extint_t extint_porta, extint_portb, extint_portc, extint_portd, extint_porte;
  
  extint_t *EXTINT_GetInst(INTC_Type *base)
  {
      switch((uint32_t)base)
      {
      case PORTA_BASE:
          return &extint_porta;
      case PORTB_BASE:
              return &extint_portb;
      case PORTC_BASE:
              return &extint_portc;
    case PORTD_BASE:
              return &extint_portd;
      case PORTE_BASE:
              return &extint_porte;
      default:
          return NULL;
      }
  }
  
  #if defined(HTISIC_EXTINT_DEFAULT_IRQ) && (HTISIC_EXTINT_DEFAULT_IRQ > 0)
  
  void PORTA_IRQHandler(void)
  {
      uint32_t flag = PORT_GetPinsInterruptFlags(PORTA);
      EXTINT_Isr(&extint_porta, flag);
      PORT_ClearPinsInterruptFlags(PORTA, 0xffff);
  }
  
  void PORTB_IRQHandler(void)
  {
      uint32_t flag = PORT_GetPinsInterruptFlags(PORTB);
      EXTINT_Isr(&extint_portb, flag);
      PORT_ClearPinsInterruptFlags(PORTB, 0xffff);
  }
  
  void PORTC_IRQHandler(void)
  {
      uint32_t flag = PORT_GetPinsInterruptFlags(PORTC);
      EXTINT_Isr(&extint_portc, flag);
      PORT_ClearPinsInterruptFlags(PORTC, 0xffff);
  }
  
  void PORTD_IRQHandler(void)
  {
      uint32_t flag = PORT_GetPinsInterruptFlags(PORTD);
      EXTINT_Isr(&extint_portd, flag);
      PORT_ClearPinsInterruptFlags(PORTD, 0xffff);
  }
  
  void PORTE_IRQHandler(void)
  {
      uint32_t flag = PORT_GetPinsInterruptFlags(PORTE);
      EXTINT_Isr(&extint_porte, flag);
      PORT_ClearPinsInterruptFlags(PORTE, 0xffff);
  }
  #endif // ! HTISIC_EXTMGR_USE_IRQHANDLER
  
  #ifdef __cplusplus
  }
  #endif
  
  #endif // ! HITSIC_USE_EXTINT
  
  ```
  
  
