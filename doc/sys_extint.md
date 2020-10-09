# SYS_EXTINT

[TOC]


## 简介

外部中断管理器（EXTINT，External Interrupt Manager），是用于响应外部中断请求的模块。相较于常规的外部中断时常常是一个PORT对应一个中断服务函数，甚至多个PORT对应一个中断服务函数的情况，EXTINT通过二次判断中断标志位，实现精确到每个PIN的中断路由管理。这为多个模块共用一个PORT上的中断、一个模块使用多个PORT上的中断等复杂情况提供了极大的便利。



## 版本说明

### v1.0-beta0

by CkovMk @hitsic 2019.07.25

改动说明：

- 全新C++ API
- 使用std::map作为存储容器，查询速度更快。
- 增加了对`d_kv10z7`单片机的支持

开发计划：

- 仿照PITMGR添加`HITSIC_USE_EXTINT`，可以快速禁用EXTINT模块。

已知问题：

- 暂无



## API文档

- **初始化 `static status_t init(void);`**

  该函数用于初始化EXTINT。

- **插入任务`static extInt_t* insert(INTC_Type* _gpio, uint32_t _pin, handler_t _handler);`**

  该函数用于向任务列表插入指定GPIO引脚上的触发任务。

- **移除任务`static status_t remove(INTC_Type* _gpio, uint32_t _pin);`**

  该函数用于移除指定GPIO引脚上的触发任务。

- **移除任务 `static status_t remove(extInt_t* _inst);`**

  该函数用于移除指针指向的触发任务。移除成功后返回`kStatus_Success`，且指针将变为野指针，您需要手动将其置为NULL。

- **服务接口 `static void isr(INTC_Type* _gpio);`**

  该函数用于为某一个端口提供中断服务。参数类型`INTC_Type*`是指本MCU上用于管理外部中断的外设类型，应在`sys_extint_port.hpp`中定义。例如，对于K66、KV58等单片机，`#define INTC_Type PORT_Type`，而对于RT1052等单片机，`#define INTC_Type GPIO_Type`。您也可以使用`typedef`关键字。

- **设置任务 `void setup(INTC_Type* _gpio, uint32_t _pin, handler_t _handler);`**

- **设置任务 `void setMode(interrupt_mode_t _mode);`**



模块内还包含用于兼容早期C语言版本EXTINT模块的C风格兼容层，仅供兼容旧代码使用，一旦旧代码迁移完成将删除该兼容层，因此禁止新代码使用。



## 设计文档

## 应用指南

## 移植指南

### 移植文件`sys_extint_port.hpp`

- **移植平台接口**

  ```c++
  //HAL marco
  #define INTC_Type 			PORT_Type
  #define interrupt_mode_t 		port_interrupt_t
  #define EXTINT_SetInterruptConfig(_intc, _pin, _cfg) 	PORT_SetPinInterruptConfig(_intc, _pin, _cfg)
  #define EXTINT_GetInterruptFlags(_intc) 				PORT_GetPinsInterruptFlags(_intc)
  #define EXTINT_ClearInterruptFlags(_intc, _mask) 				PORT_ClearPinsInterruptFlags(_intc, _mask)
  ```

  这里主要是移植一些平台相关的定义和函数。

  

- **设置功能**

  ```c++
  #define HITSIC_EXTMGR_INITLIZE 		(1U)
  
  #if defined(HITSIC_EXTMGR_INITLIZE) && (HITSIC_EXTMGR_INITLIZE > 0)
  inline void EXTINT_PlatformInit(void)
  {
  	NVIC_SetPriority(PORTA_IRQn, 6);
  	NVIC_SetPriority(PORTB_PORTC_PORTD_PORTE_IRQn, 6);
  	EnableIRQ(PORTA_IRQn);
  	EnableIRQ(PORTB_PORTC_PORTD_PORTE_IRQn);
  }
  #endif // ! HITSIC_EXTMGR_INITLIZE
  
  ```
  `HITSIC_EXTMGR_INITLIZE`宏控制平台初始化代码。启用该宏会在EXTINT初始化代码中进行相关硬件的初始化。如果已经在配置工具中自行生成了外部中断的初始化，则不需要启用该宏。

  

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



### 移植文件`sys_extint_port.hpp`

- **中断服务函数**

  ```c++
  #ifdef __cplusplus
  extern "C"{
  #endif
  
  #if defined(HTISIC_EXTINT_DEFAULT_IRQ) && (HTISIC_EXTINT_DEFAULT_IRQ > 0)
  void PORTA_IRQHandler(void){pitMgr_t::isr(PORTA);}
  void PORTB_PORTC_PORTD_PORTE_IRQHandler(void)
  {
  	pitMgr_t::isr(PORTB);
      pitMgr_t::isr(PORTC);
      pitMgr_t::isr(PORTD);
      pitMgr_t::isr(PORTE);
  	
  }
  #endif // ! HTISIC_EXTMGR_USE_IRQHANDLER
  
  #ifdef __cplusplus
  }
  #endif
  ```
```
  
  您需要在这里实现每个外部中断对应的中断服务函数。在这些中断服务函数中，只需调用`static void pitMgr_t::isr(INTC_Type* _gpio);`，传入当前中断的外设地址即可。如果有多个外设的中断被绑定到了同一个中断服务函数，如上例所示，则需要在该中断服务函数中调用多次`static void isr(INTC_Type* _gpio);`，依次传入所有外设地址。还有一种情况是一个外设的中断被绑定到了多个中断服务函数（例如RT052系列GPIOx 0~15号引脚和16~31号引脚各对应一个中断服务函数），则需要在所有属于该外设的中断服务函数内调用`static void isr(INTC_Type* _gpio);`，并传入对应外设地址。
  
  如果不希望对特定外设启用EXTINT，也可以在这里略过。
```