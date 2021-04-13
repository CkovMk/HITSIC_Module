# DRV_BUTTON

[TOC]

## 简介

按钮（BUTTON）是用于检测独立按键输入的组件。利用一个定时中断和外部中断，BUTTON组件可以实现基本的按键输入消抖、单击触发、长按触发和长按时连续触发的功能。



## 版本说明

### v0.3.0

by CkovMk @hitsic 2021.04.02

改动说明：

- 适配新版EXTINT和PITMGR

开发计划：

- 创建移植接口
- 新增支持双击检测
- 删除按钮事件CLER状态。因为按钮事件产生后总是立即调用按钮事件响应句柄，不存在异步操作。

已知问题：

- 在KV1x系列单片机上未经过测试。



### v0.2.2

by CkovMk @hitsic 2021.10.29



### v0.2-beta.1

by CkovMk @hitsic 2020.04.20



### v0.1.0

by CkovMk @hitsic 2020.02.15

改动说明：

- 初始版本

开发计划：

- 创建移植接口
- 新增支持双击检测
- 删除按钮事件CLER状态。因为按钮事件产生后总是立即调用按钮事件响应句柄，不存在异步操作。

已知问题：

- 在KV1x系列单片机上未经过测试。



## API文档

-  **初始化函数 `void BUTTON_Setup(button_t *_inst, GPIO_Type *_gpio, uint32_t _pin);`**

   用于初始化或重新设置一个已有的按钮对象。

- **构造函数 `button_t *BUTTON_Construct(GPIO_Type *_gpio, uint32_t _pin);`**

  构造一个按钮对象，并返回指针。如果构造失败会返回空指针。

  初始化（构造）时按钮必须处于释放状态，否则函数将一直等待直到按钮释放。初始化（构造）函数不负责初始化定时中断和外部中断，您需要手动向EXTINT添加外部中断句柄、手动向PITMGR添加定时中断句柄。

- **设置按钮事件响应句柄 ` void BUTTON_InstallHandler(button_t *_inst, button_handler_t _handler);`**

  设置您用于响应按钮事件的句柄函数。您设置的句柄函数会在按钮事件触发时被调用。该句柄函数的原型为`typedef void(*button_handler_t)(button_t *_inst);`。您可在句柄函数中通过访问`_inst->status`变量判断按钮事件的类型。注意：短按事件`BUTTON_SHRT_PRES`触发时句柄函数会在外部中断处理函数内调用，长按事件`BUTTON_LONG_PRES`和长按重复事件`BUTTON_LRPT_PRES`触发时句柄函数会在定时中断处理函数内调用，因而具有较高的优先级，应迅速退出，不能在句柄函数内处理复杂事务。
  ~~句柄函数在处理完按钮事件后，应将`_inst->status`变量标记为对应的已响应标志。~~

- **移除按钮事件响应句柄 ` void BUTTON_UninstallHandler(button_t *_inst);` **

  删除您设置的句柄函数。

- **设置中断方式 ` void BUTTON_SetInterrupt(button_t *_inst, port_interrupt_t _int);`**

  设置外部中断方式的接口，大多数时候您不需要调用此函数。

- **读取按键电平 `uint32_t BUTTON_ReadPin(button_t *_inst);`**

  大多数时候您不需要调用此函数。

- **外部中断响应函数 `void BUTTON_ExtIsr(button_t *_inst);`**

  当一个按钮的外部中断触发时，您应在外部中断服务函数内调用此函数。

- **定时中断响应函数 `void BUTTON_PitIsr(button_t *_inst);`**

  您应在定时中断服务函数内周期性调用此函数。调用周期为`BUTTON_PITMGR_TIME_MS`毫秒。



## 设计文档

### 宏定义介绍

```c++
#define DRV_BUTTON_VERSION (HITSIC_MAKE_VERSION(0u, 1u, 1u))

#define BUTTON_TIME_SHRT 50u    ///< 短按最短时间。要产生短按，按下事件必须小于短按的最长时间。
#define BUTTON_SHRT_TOUT 200u	///< 短按最长时间。
#define BUTTON_TIME_LONG 250u   ///< 长按最短时间。要产生短按，按下事件必须小于长按的最长时间。
#define BUTTON_TIME_LRPT 100u   ///< 长按重复时间。在产生长按事件或长按重复事件之后，每隔此时间产生一次长按重复事件。
#define BUTTON_REPT_TOUT 300u   ///< 长按最长时间。
#define BUTTON_TIME_INTV 50u	///< 长按重复消抖时间。进行长按重复判断时。按键释放时间低于此时间不会终止长按重复判定。

#define BUTTON_PRESSDN_LOGIC 0u
#define BUTTON_RELEASE_LOGIC 1u
#define BUTTON_PRESSDN_EXTINT kPORT_InterruptFallingEdge //define which edge indicates button pressed.
#define BUTTON_RELEASE_EXTINT kPORT_InterruptRisingEdge  //define which edge indicates button release.

#define BUTTON_PITMGR_TIME_MS 10

#define BUTTON_TIMER_MS     (PITMGR_GetTimer_ms())
```



### 类型介绍

```c++
typedef enum 
{
    BUTTON_STAT_NONE = 0, ///< button no operation
    BUTTON_SHRT_PRES = 1, ///< button short press
    BUTTON_SHRT_CLER = 2, ///< service responded short press
    BUTTON_LONG_PRES = 3, ///< button long press
    BUTTON_LONG_CLER = 4, ///< service responded long press
    BUTTON_LRPT_PRES = 5, ///< button long_repeat press
    BUTTON_LRPT_CLER = 6, ///< service responded long_repeat press
}button_status_t;
```
按钮事件类型定义。`BUTTON_STAT_NONE`代表空事件。`BUTTON_XXXX_XXXX`中，中缀`SHRT`代表短按（Short）,中缀`LONG`代表长按（Long）,中缀`LRPT`代表长按重复（Long Repeat）；后缀`PRES`代表刚刚产生的事件，后缀`CLER`代表按钮事件响应句柄已经响应过的事件。



```c++
typedef struct 
{
    GPIO_Type *gpio;
    //PORT_Type* port;
    uint32_t pin;
    port_interrupt_t intCfg;
    uint64_t msCnt;
    button_status_t status;
    button_handler_t handler;
}button_t;
```
按钮对象定义。存储了按钮所在的GPIO地址和引脚号、当前的中断方式配置、计时器变量、当前状态、按钮事件响应句柄。



```c++
typedef void(*button_handler_t)(button_t *_inst);
```
按钮事件响应句柄的原型定义。



### 按钮响应过程

- 初始化过程

  初始化函数将GPIO地址和引脚号存入按钮对象，等待按钮释放（`while(BUTTON_ReadPin(_inst) == BUTTON_PRESSDN_LOGIC){}`）后设置对应GPIO、引脚的中断方式为等待按下中断（`BUTTON_SetInterrupt(_inst, BUTTON_PRESSDN_EXTINT);`）。
  
- 按下中断过程

  等待按下中断（`BUTTON_PRESSDN_EXTINT`）触发后，按钮外部中断响应函数被调用，进入按下中断过程。首先判断与上次触发的时间间隔，如果时间过短（`if (t < BUTTON_TIME_INTV)`），判断为抖动，直接退出程序（`return;`）。如果满足消抖条件，则将中断方式修改为等待释放中断（`BUTTON_SetInterrupt(_inst, BUTTON_RELEASE_EXTINT);`）。如果判断不是由于长按时的抖动产生的按下事件（`if (!((_inst->status == BUTTON_LONG_PRES || _inst->status == BUTTON_LRPT_PRES) && (t < BUTTON_TIME_INTV)))`），则清除按键事件标志（`_inst->status = BUTTON_STAT_NONE;`）。最后记录当前时间（`_inst->msCnt = BUTTON_TIMER_MS;`），退出程序。

- 释放中断过程（短按)

  等待释放中断（`BUTTON_RELEASE_EXTINT`）触发后，按钮外部中断响应函数被调用，进入释放中断过程。将中断方式修改为等待按下中断（`BUTTON_SetInterrupt(_inst, *BUTTON_PRESSDN_EXTINT);`），判断与上次触发的时间间隔并记录当前时间，如果当前按钮事件标志为空，且时间间隔符合短按的特征（`if (BUTTON_TIME_SHRT <= t && t < BUTTON_SHRT_TOUT && _inst->status == BUTTON_STAT_NONE){...}`），则设置按钮事件标志（`_inst->status = BUTTON_SHRT_PRES;`）并调用按钮事件响应句柄（如果存在）。否则，判断为已经产生了事件，直接将按钮事件标志置空并退出程序。这是为了防止在定时中断已经产生长按事件或长按重复事件后，随着按键释放再次产生短按事件。

- 定时中断

  进入定时中断后，首先计算当前按钮已被按下的时间。随后进行外部中断有效性检测：如果按钮当前电平为释放状态，说明此时按钮没有被按下。但是，如果此时中断配置为等待释放，则说明单片机漏检了释放中断。为了避免此类问题，退出程序前重新设置中断配置为等待按下。然后检测长按重复超时：如果当前按钮状态为已触发长按事件或已触发长按重复事件，但当前距离按键上次事件响应的时间超过了规定消抖值（`t >= BUTTON_TIME_INTV`），则结束本次长按重复逻辑。这是为了防止长按过程的抖动导致按钮短暂释放。

  如果外部中断有效，且中断配置为等待释放，说明当前按钮确实处于按下状态。

  - 如果当前按钮状态为未触发过任何事件，且当前按钮已被按下的时间符合长按的特征（`if (BUTTON_TIME_LONG <= t && t < BUTTON_REPT_TOUT){...}`），则设置按钮事件标志（`_inst->status = BUTTON_LONG_PRES;`），并调用按钮事件响应句柄（如果存在），然后记录当前时间，退出程序。可以看出，长按事件仅能在按键按下后没有任何事件触发过的情况下触发。
  - 否则，再判断如果当前按钮当前状态为已触发长按事件或已触发长按重复事件，且已被按下的时间符合长按重复触发的特征，则设置按钮事件标志（`_inst->status = BUTTON_LRPT_PRES;`），并调用按钮事件响应句柄（如果存在），然后记录当前时间，退出程序。可以看出，长按重复事件仅能在已触发长按事件或长按重复事件的情况下触发。



## 应用指南



## 移植指南

### 基于本组件库的移植

BUTTON组件的GPIO操作完全依赖EXTINT，无需移植。

### 独立于本组件库的移植

如果要脱离本组件库移植本组件，需要修改以下内容：

- 定义按键按下和释放时的逻辑状态和GPIO中断方式

  ```c++
  #define BUTTON_PRESSDN_LOGIC 0u
  #define BUTTON_RELEASE_LOGIC 1u
  #define BUTTON_PRESSDN_EXTINT kPORT_InterruptFallingEdge //define which edge indicates button pressed.
  #define BUTTON_RELEASE_EXTINT kPORT_InterruptRisingEdge  //define which edge indicates button release.
  ```

- 适配用于设置GPIO中断方式的函数`void BUTTON_SetInterrupt(button_t *_inst, port_interrupt_t _int);`

- 适配系统计时函数`#define BUTTON_TIMER_MS     (PITMGR_GetTimer_ms())`。




