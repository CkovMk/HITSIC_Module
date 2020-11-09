# APP_MENU

[TOC]



## 简介

APP_MENU隶属于HITSIC_MODULE的应用层，主要实现一个轻量级的调参菜单。该菜单可以支持32位有符号整数型、32位有符号浮点型变量的监视与调节，具有子菜单管理，且可以插入函数。每个菜单项的属性可以单独设置，数据可以在Flash等NVM中保存。该菜单还将成为数据日志和调试上位机访问各种数据的接口。

### 组件

#### 本体组件

菜单通用类型定义（MENU_DEF），保存菜单公用的定义。

菜单顶层逻辑（MENU），使用时只需包含此模块的头文件。其他头文件均被此文件包含。

菜单按键处理逻辑（MENU_BUTTON）。

菜单非易失性存储（MENU_NVM），在K66平台指内部Flash。

菜单类型逻辑（MENU_TYPE），包含各种类型的菜单项的处理逻辑、菜单项接口的处理逻辑、菜单列表的处理逻辑等。

#### 环境依赖

**此菜单依赖于：**

PIT任务管理器（PITMGR），用于计时、延时和按键PIT管理；

I/O中断管理器（EXTINT），用于管理按键外部中断；

Flash读写驱动（FTFX_FLASH），可选，用于保存和读取菜单；

屏幕驱动（DISP_SSD1306），用于屏幕显示输出；

按键驱动（BUTTON），可选，用于管理按键功能；



## 版本说明

### v0.1.6

by：CkovMk @hitsic 2020.11.09

**改动说明**

- 适配DISP_SSD1306驱动更新，新增全缓存屏幕移植接口。现在屏幕可支持全缓存打印。
- 仿照C++虚表，修改了菜单项适配器和类型多态原理。现在只有菜单项构造需要使用`swich-case`结构，其他函数调用均由查表得到。
- 现在`menuItem_dataExt_HasMinMax`扩展属性已对NVM读取生效。
- 调整默认事件中断优先级为12。
- 补全移植文档。

**开发计划**

- 适配全缓存屏幕API，创建面向不同屏幕的移植接口。
- 创建文件存储接口。

**已知问题**

- 局部数据区间的数据拷贝功能仍不可用。



### v0.1.5

by：CkovMk @hitsic 2020.10.30

功能完善。

**改动说明**

- 新增了两个顶层API：`MENU_Suspend(void)`和`MENU_Resume(void);`，用于暂时地禁用/启用菜单。
- 菜单项属性Flag调整/新增：`menuItem_dataExt_HasMinMax`属性，调整`menuItem_data_ROFlag`、`menuItem_data_NoSave`、`menuItem_data_NoLoad`等的含义
- 修改了部分菜单项的显示格式
- 修复了一些Bug。现在，在使用“disp_noPreview”属性时，快速调参将被禁用。
- 所有代码由`*.x`更新为`*.xpp`。

**开发计划**

- 适配全缓存屏幕API，创建面向不同屏幕的移植接口。
- 创建文件存储接口。
- （下一个大版本）调整菜单类型的多态方式，使用函数指针+结构体的方式替代现有的宏定义方式。这可能需要花费非常多的时间。

**已知问题**

- 局部数据区间的数据拷贝功能仍不可用。
- `menuItem_dataExt_HasMinMax`扩展属性对数据初值、NVM读取无效。



### v0.1.4

by：CkovMk @hitsic 2020.10.09

继续完善功能

**改动说明**

- 修改了`procType`菜单的运行逻辑，现在`procType`类型的菜单具有可进入的二级菜单，仅在二级菜单内短按`OK`键才会执行函数；任何时候在二级菜单内长按`OK`键将直接退出二级菜单。新增了两个属性Flag：`menuItem_proc_runOnce`表示该函数仅运行一次就自动退出。所有没有此Flag的`procType菜单`短按OK键后都不会退出，只能由函数内逻辑退出或长按OK键退出。`menuItem_proc_uiDisplay`属性Flag表示该函数具有打印屏幕的功能，将禁用菜单默认的界面显示。
- 修改了菜单刷新逻辑。现在将有一个PITMGR任务以4Hz的频率写入屏幕刷新事件（`menu_message_printDisp`）并触发菜单响应。另外，按键操作也将写入屏幕刷新事件（`menu_message_buttonOp`）。
- 优化了一些不需要的全局变量，现在已被宏定义替代。

**开发计划**

- 创建面向不同屏幕的移植接口。

**已知问题**

- 菜单内在不同局部存储区拷贝数据的功能存在问题，可能会造成数据丢失。
- 在使用“disp_noPreview”属性时，仍可进行快速调参。



### v0.1.3

by：CkovMk @hitsic 2020.07.31

增加了一些属性Flag。

**改动说明**

- 增加了`HITSIC_USE_APP_MENU`宏，用于启用或禁用整个APP_MENU模块。
- 增加了`HITSIC_MENU_USE_BUTTON`宏，用于启用或禁用菜单依赖的DRV_BUTTON模块和自带的MENU_BUTTON按键事件模块。禁用该宏后，您需要手动向菜单发送按键消息。
- 增加了`HITSIC_MENU_USE_NVM`宏，用于启用或禁用自带的MENU_NVM非易失性存储模块。禁用该宏后，菜单将无法以任何形式保存参数。
- 在启用`HITSIC_MENU_USE_NVM`宏时，您现在可以自定义数据的存储方式。在`app_menu_port.h`内提供了对各存储区大小、位置的快速设置。您还可以自定义NVM后端驱动。例如，通过适配，您现在可以将数据存在外部EEPROM或SPI Flash上。目前，MENU_NVM仅支持块级存储，不支持文件存储。
- 由于HITSIC_Modules上层结构调整，删除掉了`hitsic_def.h`，APP_MENU不再依赖该头文件。

**开发计划**

- 增加对文件存储的支持
- 编写全面的移植指南，整理应用指南。
- 属性Flag提供更多选项，并在运行过程中指示该变量是否已修改。
- 优化头文件包含关系，只包含必要的库，且所有平台相关的头文件全部移动到移植文件中。

**已知问题**

- 菜单内在不同局部存储区拷贝数据的功能存在问题，可能会造成数据丢失。



### v0.1.2

by：CkovMk @hitsic 2019.11.22

主要功能初步完善。

**改动说明**

- 菜单存储功能已可以正常使用。菜单内含1个全局存储区和3个局部存储区，全局存储区用于存储公共参数，而三个局部存储区可存储三组不同参数。当前所使用的参数组可以在菜单内选择。单片机上电后可以自动读取保存的数据。**注意：存储区间的数据拷贝功能还不完善，可能造成数据丢失。**
- 调整及新增了一些属性Flag。现在，`menuItem_data_ROFlag` 标志仅表示该变量不能从菜单修改，但仍会被保存到对应的Nvm存储区。如要禁止保存该变量，使用新增的`menuItem_data_NoSave` 标志。**注意：此标志仅代表在保存菜单时不会保存该变量，用户仍可以对具有此标志的菜单项分配存储空间，并手动保存改变量到分配的地址。**

**开发计划**

- 完善菜单存储功能

- 完善函数类型菜单

- 属性Flag提供更多选项，并在运行过程中指示该变量是否已修改。

**已知问题**

- 存储区间的数据拷贝功能还不完善，可能造成数据丢失。





### v0.1.1

by：CkovMk @hitsic 2019.11.02

第一个正式版。经过早期测试没有严重Bug。一些功能仍未完善。

**主要功能**

- 使用单条语句构造和析构菜单和菜单项。菜单项的插入和删除与类型无关。

- 低优先级中断操作，不干扰高优先级中断，也不占用`main()`函数。

- 所有按键均支持短按、长按、长按重复三种操作。在任意菜单列表内长按OK返回上一级菜单。也可以使用每个菜单列表顶部的[back]菜单项返回上级菜单。

- 可以使用空类型（分割线）、整形（int32_t）、浮点型（float）、菜单跳转、函数调用五种菜单项。

  - 参数类型（整数和浮点）支持在菜单列表内快速调参，其中int32_t型在仅有一位数时调整第一位有效数字，在大于等于两位数时固定调整第二位有效数字；float型全范围内固定调节第二位有效数字。
  - 参数类型在菜单列表内具有自适应显示。（-10^5 ，+10^5）内的整型参数和（-10^4，-10^-1）∪（+10^-1，+10^4）内的浮点参数将直接显示，这些范围外的参数将采用四位有效数字的“准”科学计数法显示。该显示方式为`±abcd±e`的形式，为了节约显示空间没有使用标准的小数点表示法并省掉了有效数字与数量级之间的字符E，表示`±abcd × 10^±e`。
  - 参数类型具有二级调参页面，可以科学计数法的方式对前四位有效数字进行精确设置。在二级调参页面中，短按OK键可以保存并退出，长按OK键可以放弃更改并退出。
  - 所有菜单项都有独立的属性控制，可以设置只读、可写、保存区域、显示方式等属性，部分属性在此版本中不可用，在后续的更新中将陆续实现。
- 可以支持任意数量的子菜单和菜单项，一个子菜单可以有一个或多个入口。

**已知问题**

- 目前菜单仅能保证基本使用，有些功能仅做了接口，并没有实现。使用上可能会引起困惑。
- 该菜单没用添加定时任务，故仅在有按键操作时刷新菜单。相关接口已经留好，后续更新中实现。
- 菜单的Flash保存功能还在开发中，暂时无法使用。菜单内的数据在复位后会丢失，需要手动记录。
- 删去了紫丁香LOGO的相关数据，无法显示紫丁香LOGO。



## API文档

### 顶层API（`APP_MENU`）

- 菜单初始化
  
  ```c
  /** @brief : 菜单全局初始化。
   * 该函数会初始化必要的变量，创建根菜单和其他最基本的菜单列表和菜单项。
   *
   * @param  {void} undefined :
   */
  void MENU_Init(void);
  ```

  将进行初始化菜单运行所必需的变量、初始化NVM（如果启用）相关变量、初始化按钮驱动（如果启用）、添加管理菜单等操作，并调用`void MENU_DataSetUp(void);`函数配置用户菜单。使用菜单前必须首先调用。
  
  

- 设置菜单数据

  ```C
  /**
   * @brief : 初始化各菜单列表和菜单项。
   * 注意：该函数在外部定义，不需初始化最基本的菜单列表和菜单项。该函数禁止用户手动调用。
   *
   * @param  {void} undefined :
   */
  void MENU_DataSetUp(void);
  ```

  在这里配置菜单。本模组内包含了该函数的一个弱符号实现，仅用于菜单测试。您可以创建您自己的实现，并在其中插入您所需的数据、子菜单和函数等项目。

  

- 打印屏幕

  ```C
  /**
   * @brief : 打印屏幕。
   * 该函数是整个菜单的屏幕打印入口，将自动根据菜单的状态打印屏幕。
   *
   * @param  {void} undefined :
   */
  void MENU_PrintDisp(void);
  ```

  用于刷新屏幕。大多数时候屏幕刷新都是自动进行的，您无需手动刷新。

  

- 按键操作

  ```C
  /**
   * @brief : 响应按键操作。
   * 该函数是整个菜单的按键响应入口，将自动根据菜单的状态调用按键响应函数。
   * 该函数不会更新屏幕显示。
   *
   * @param  {void} undefined :
   */
  void MENU_KeyOp(menu_keyOp_t * const _op);
  ```

  用于解析按键操作并根据菜单逻辑给出响应。如果执行按键操作后需要刷新屏幕，其将自动产生一次屏幕刷新事件，但可能不会立即进行屏幕刷新。在使用本模组自带的按键处理函数时，您无需手动调用该函数。反之，如果您自行编写了按键识别代码，您可以通过调用此函数向菜单传输指令。

  

- 保存数据

  ```C
  /**
   * @brief : 保存整个菜单到NVM。
   *
   * @param  {int32_t} _region :  所选择的局部存储区。
   */
  void MENU_Data_NvmSave(int32_t _region);
  
  /**
   * @brief : 保存整个菜单到NVM。
   * 该函数将使用全局变量 menu_currRegionNum 中保存的局部存储区号。
   * 
   * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
   */
  void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op);
  ```

  保存菜单内的数据到NVM（非易失性存储）。仅在启用NVM功能时有效。本模组不会自动保存参数到NVM。在程序中，您应该调用`void MENU_Data_NvmSave(int32_t _region);`来执行保存操作。`int32_t menu_currRegionNum;`是当前菜单选中的局部存储区。您也可以人为指定存储区，取值范围为0 ≤ `_region` ＜ `HITSIC_MENU_NVM_REGION_CNT`。`void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op);`是程序类型菜单项的服务函数，将被自动添加至管理菜单。您可以通过菜单执行数据保存操作。

  

- 读取数据

  ```C
  /**
   * @brief : 从NVM读取整个菜单。
   *
   * @param  {int32_t} _region : 所选择的局部存储区。
   */
  void MENU_Data_NvmRead(int32_t _region);
  
  /**
   * @brief : 从NVM读取整个菜单。
   * 该函数将使用全局变量 menu_currRegionNum 中保存的局部存储区号。
   *
   * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
   */
  void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op);
  ```

  从NVM（非易失性存储）中读取数据到当前菜单。仅在启用NVM功能时有效。本模组不会自动读取参数到NVM。在程序中，您可以在初始化后立即调用`void MENU_Data_NvmSave(int32_t _region);`来执行读取操作。您可以人为指定存储区，取值范围为0 ≤ `_region` ＜ `HITSIC_MENU_NVM_REGION_CNT`，或先读取NVM中保存的分区号，再执行读取操作。`void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op);`是程序类型菜单项的服务函数，将被自动添加至管理菜单。您可以通过菜单执行数据读取操作。

  **注意：菜单内的数据在NVM中按地址存储，且设置了校验机制。如果修改了菜单结构或菜单名称，可能会导致数据丢失。请在修改菜单结构前备份数据。**

  

- 读写局部存储区设置

  ```c
  /**
   * @brief : 保存当前局部存储区号到NVM。
   * 该数值设置为不自动保存。
   * 
   */
  void MENU_Data_NvmSaveRegionConfig(void);
  
  /**
   * @brief : 保存当前局部存储区号到NVM。
   * 该数值设置为不自动保存。
   *
   * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
   */
  void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op);
  ```

  用于保存局部存储区编号到NVM（非易失性存储）。该变量不会随其他变量一起保存。`void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op);`函数用于通过菜单保存该编号。

  

- 在数据区之间拷贝数据（不可用）

  ```C
  /**
   * @brief : 将一个局部存储区的数据拷贝到另一个局部存储区。
   *
   * @param  {int32_t} _srcRegion : 源存储序号。
   * @param  {int32_t} _dstRegion : 目的存储区序号。
   */
  void MENU_Data_NvmCopy(int32_t _srcRegion, int32_t _dstRegion);
  
  /**
   * @brief : 将一个局部存储区的数据拷贝到另一个局部存储区。
   * 该函数将使用全局变量 menu_nvmCopySrc 和 menu_nvmCopyDst 中存储的值。
   * 
   * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
   */
  void MENU_Data_NvmCopy_Boxed(menu_keyOp_t *const _op);
  ```

- 读取NVM状态标志（不可用）

  ```C
  /**
   * @brief : 读取NVM状态标志。
   *
   * @return {int32_t}        : 返回读取到的状态标志。
   */
  int32_t MENU_GetNvmStatus(void);
  ```

- 设置NVM状态标志（不可用）

  ```C
  /**
   * @brief : 设置NVM状态标志。
   *
   * @param  {int32_t} _status : 要设置的状态标志。
   */
  void MENU_SetNvmStatus(int32_t _status);
  ```

- 菜单挂起与恢复

	```c++
	/**
	 * @brief 挂起菜单。这将暂时禁用菜单的事件处理，包括按键输入和屏幕打印。
	 *          注意：调用多次MENU_Suspend()将需要调用相同次数的MENU_Resume()才能将菜单恢复至运行状态。
	 */
	void MENU_Suspend(void);
	
	/**
	 * @brief 恢复菜单。这将启用菜单的事件处理，包括按键输入和屏幕打印。
	 *          注意：调用多次MENU_Suspend()将需要调用相同次数的MENU_Resume()才能将菜单恢复至运行状态。
	 */
	void MENU_Resume(void);
	```
	
	调用这些函数以禁用和恢复菜单运行。



### 菜单项（`APP_MENU_TYPE`）

- 构造菜单项
	```c++
	/**
	 * @brief : 菜单项结构体的构造函数。
	 * 该函数分配一块内存，根据传入的参数填入内容，然后返回所产生的菜单项结构体指针。
	 *
	 * @param  {menu_itemType_t} _type : 要生成的菜单项类型。
	 * @param  {void*} _data           : 指向要操作的数据的指针。
	 * @param  {char*} _nameStr        : 菜单项名称字符串，不超过最大长度，以'\0'结尾。
	 * @param  {uint32_t} _saveAddr    : 变量要保存到的地址。对非数据类型及属性设置中只读的菜单项无效。
	 * @param  {uint32_t} _pptFlag     : 属性标志。
	 *
	 * @return {menu_itemIfce_t *}     : 返回所产生的菜单项结构体指针。
	 */
	menu_itemIfce_t* MENU_ItemConstruct(menu_itemType_t _type, void *_data,
	        const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag);
	```
	
	目前支持的菜单项类型有：分隔线/空类型（`nullType`）、`int32_t`参数型（`variType`）、`float`参数型（`varfType`）、菜单跳转型（`menuType`）、运行程序型（`procType`）。
	
	- `nullType`
	
	​	_type：menu_nulllType
	
	​	_data：空指针。
	
	​	_nameStr：显示在菜单中的名称。该位置将显示一条带有该名称的分隔线。
	
	​	_saveAddr：对此类菜单项无意义。
	
	​	_pptFlag：对此类菜单项无意义。
	
	- `variType`
	
	  _type：menu_variType
	
	  _data：指向要操作的int32_t型变量的指针。
	
	  _nameStr：显示在菜单列表和二级页面的名称。
	
	  _saveAddr：数据保存的地址号。同一数据区内的编号不能重复。
	
	  _pptFlag：属性标志。多个互不冲突的标志位用按位或`|`连接。支持的属性标志位有：
	
	  - `menuItem_data_global`：该变量存储在全局数据区。具有该属性的变量将在其二级调参页面的右上角显示`*GL`字样。
	
	  - `menuItem_data_region`：该变量存储在局部数据区。具有该属性的变量将在其二级调参页面的右上角显示`*RG`字样。
	
	    > 需要保存至NVM的变量应至少设置以上两项其中之一。未设置以上两项中任何一项属性的变量将在其二级调参页面的右上角显示`*NO`字样，且后续所有与NVM操作相关的属性均不生效。
	
	  - `menuItem_data_ROFlag`：该变量为只读，无法通过菜单修改。该属性不会影响NVM操作。
	
	  - `menuItem_data_NoSave`：该变量默认不保存。管理菜单内名称为的`RegnSel(0-2)`的菜单项就应用了该属性。
	
	  - `menuItem_data_NoLoad`：该变量默认不读取。管理菜单内名称为的`RegnSel(0-2)`的菜单项就应用了该属性。
	
	  - `menuItem_disp_forceSci`：强制以科学计数法显示。默认为自适应模式，数字位数过多自动切换为科学计数法显示。
	
	  - `menuItem_disp_noPreview`：关闭菜单列表内的变量数值显示，这将允许显示更长的菜单项名称。
	
	  - `menuItem_dataExt_HasMinMax`：该（变量）具有最小/最大值的扩展属性。最小值存储在`data+1`地址，最大值存储在`data+2`地址。
	
	- `varfType`
	
	  _type：varfType
	
	  _data：指向要操作的float型变量的指针。
	
	  _nameStr：显示在菜单列表和二级页面的名称。
	
	  _saveAddr：数据保存的地址号。同一数据区内的编号不能重复。
	
	  _pptFlag：属性标志。多个互不冲突标志位用按位或`|`连接。支持的属性标志位与`variType`相同。
	
	- `menuType`
	
	  _type：menuType
	
	  _data：指向要跳转到的菜单列表的指针。
	
	  _nameStr：显示在菜单列表和二级页面的名称。显示时会自动用方括号`[]`括起来以提高辨识度。
	
	  _saveAddr：对此类菜单项无意义。
	
	  _pptFlag：对此类菜单项无意义。
	
	- `procType`
	
	  _type：procType
	
	  _data：指向要运行的程序的函数指针。该函数必须是`void (*)(menu_keyOp_t* cosnt)`类型。
	
	  _nameStr：显示在菜单列表和二级页面的名称。显示时会自动在左边加冒号`:`以提高辨识度。
	
	  _saveAddr：对此类菜单项无意义。
	
	  _pptFlag：属性标志。多个互不冲突标志位用按位或`|`连接。支持的属性标志位有：
	  
	  - `menuItem_proc_runOnce`：该程序仅运行一次就立刻退出。
	  - `menuItem_proc_uiDisplay`：该程序具备打印屏幕的功能。这将暂时关闭菜单在屏幕的显示。
	
	
	
- 析构菜单项

  ```c++
  /**
   * @brief : 菜单项结构体析构函数。
   *
   * @param  {menu_itemIfce_t*} _item : 要析构的菜单项结构体指针。析构后该指针会被自动赋值为NULL。
   */
  void MENU_itemDestruct(menu_itemIfce_t *_item);
  ```

- 获取菜单项内存放的数据指针
  ```c++
  /**
   * @brief : 获取菜单项内存放的数据指针。
   * TODO : 更新注释
   * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
   *
   * @return {void *}					: 返回数据指针。注意，无论何种类型，都会返回对应的数据指针。
   */
  void MENU_ItemGetData(menu_itemIfce_t *_item, menu_nvmData_t *_data);
  ```

- 设置菜单项内指针指向的数据的值
  ```c++
  /**
   * @brief : 设置菜单项内指针指向的数据的值。
   * 注意，该函数对非数据类型菜单和只读数据菜单无效。
   * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
   * @param  {void*} _data            :
   */
  void MENU_ItemSetData(menu_itemIfce_t *_item, menu_nvmData_t *_data);
  ```

- 在菜单列表中打印一行菜单
  ```c++
  //used when in menuList
  
  /**
   * @brief : 在菜单列表中打印一行菜单。
   *
   * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
   * @param  {uint32_t} _slotNum      : 要打印位置（第几行）。对于12864 OLED而言，取值范围为0~7。
   */
  void MENU_ItemPrintSlot(menu_itemIfce_t *_item, uint32_t _slotNum);
  ```

- 在菜单列表中响应直接按键操作
  ```c++
  /**
   * @brief : 在菜单列表中响应直接按键操作。
   *
   * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
   * @param  {menu_keyOp_t*} _op      : 按键操作指针。按键响应后会被清除为空操作。
   */
  void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op);
  ```

- 打印访问菜单项的二级页面
  ```c++
  /**
   * @brief : 打印访问菜单项的二级页面。
   *
   * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
   */
  void MENU_ItemPrintDisp(menu_itemIfce_t *_item);
  ```

- 访问菜单项的二级页面时响应按键操作
  ```c++
  /**
   * @brief : 访问菜单项的二级页面时响应按键操作。
   *
   * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
   * @param  {menu_keyOp_t*} _op      : 按键操作指针。按键响应后会被清除为空操作。
   */
  void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op);
  ```



### 菜单列表（`APP_MENU_TYPE`）

- 构造菜单列表
	```c++
	/**
	 * @brief : 菜单列表的构造函数。
	 *
	 * @param  {const char*} _nameStr : 菜单列表的名称，显示在页面顶部。
	 * @param  {uint32_t}    _size	  : 菜单列表的长度。须预留一位用于存储[Back]返回项。
	 * @param  {menu_list_t*} _prev   : 指向该菜单列表的上级菜单列表，仅供返回时使用。
	 *
	 * @return {menu_list_t *}        : 返回构造的菜单列表结构体指针。
	 */
	menu_list_t* MENU_ListConstruct(const char *_nameStr, uint32_t _size,
	        menu_list_t *_prev);
	```
	
	
	
- 析构菜单列表
  ```c++
  /**
   * @brief : 菜单列表的析构函数。
   *
   * @param  {menu_list_t*} _list : 要析构的菜单列表结构体指针。析构后该指针会被自动赋为NULL。
   */
  void MENU_ListDestruct(menu_list_t *_list);
  ```

- 插入菜单项
  ```c++
  /**
   * @brief : 插入一个菜单项。
   *
   * @param  {menu_list_t*} _list     : 要访问的菜单列表的指针。
   * @param  {menu_itemIfce_t*} _item : 要插入的菜单项指针。该指针应为将亡值。
   * @return {status_t}               : 返回操作的结果。正常应返回kStatus_Success。
   */
  status_t MENU_ListInsert(menu_list_t *_list, menu_itemIfce_t *_item);
  ```

- 向显存中打印菜单列表
  ```c++
  /**
   * @brief : 打印菜单列表。
   *
   * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
   */
  void MENU_ListPrintDisp(menu_list_t *_list);
  ```

- 菜单列表响应按键操作
  ```c++
  /**
   * @brief : 菜单列表中响应按键操作。
   *
   * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
   * @param  {menu_keyOp_t*} _op  : 按键操作指针。按键响应后会被清除为空操作。
   */
  void MENU_ListKeyOp(menu_list_t *_list, menu_keyOp_t *const _op);
  ```



### 其他API

- 其他API的说明参见代码注释。





## 设计文档

### 术语与概述

- 菜单：管理菜单项和菜单表的结构。处理菜单的操作、显示、数据。
- 菜单项：对数据进行操作的单元，表现为菜单列表中的元素。可接受操作。
- 菜单项界面：显示一个菜单项的详细内容，供用户对数据进行精细操作的界面。
- 菜单列表：对菜单项进行管理的结构，表现为菜单项组成的列表。可接受操作。
- 菜单列表界面：用于显示多个菜单项，供用户选择和调节的界面。
- 操作：按键等输入设备对菜单的输入。
- 显示槽：一个菜单项在菜单表界面中的显示空间。

### 核心逻辑

#### 菜单项与菜单列表（命令模式）

- 菜单项结构体
  
  定义如下结构体用于存放一个菜单项所包含的全部信息：
  
  ```c
  /** @brief : 菜单项接口结构体。 */
  typedef struct _menu_itemIfce_t
  {
      menu_itemType_t type; ///< 此菜单项的类型。
      uint32_t pptFlag; ///< 此菜单项的属性标志位。
      uint32_t list_id, unique_id; ///< 此菜单项在本列表内的序号（从0开始）、全局唯一序号（从0开始）
      uint32_t saveAddr; ///< 此菜单在本区域内的偏移地址。从0开始，以1步进。注意，全局数据区和局部数据区的地址分开来算。
      char nameStr[menu_nameStrSize]; ///< 此菜单项的名称字符串。最大长度为menu_nameStrSize - 1 字节。
      union menu_itemIfce_handle_t ///< 菜单项操作句柄的共用体。使用时根据此菜单项的类型调取对应项访问。
      {
          void *p_void;
          menu_item_nullHandle_t *p_nullType;
          menu_item_variHandle_t *p_variType;
          menu_item_varfHandle_t *p_varfType;
          menu_item_boolHandle_t *p_boolType;
          menu_item_procHandle_t *p_procType;
          menu_item_menuHandle_t *p_menuType;
      } handle;
      const menu_itemAdapter_t* adapter; ///< 指向存放菜单项命令函数指针的结构体。参考C++虚表
  } menu_itemIfce_t;
  ```
  
  `pptFlag` ：一个标志位枚举的实例，用于设置该菜单项的属性。参考枚举类型 `menu_itemPropety_t` 。
  
  `handle`：这是一个共用体，其内容根据菜单类型变化。例如，`variType` 的菜单项，`handle` 中有效的指针为 `p_variType` ，依此类推。而 `p_variType` 的类型 `menu_item_variHandle_t` 是 `variType` 的适配器类型，包含该类型所需的目标指针和所有辅助数据。`handle` 中的指针所指向的内存在构造时分配，析构时释放。
  
- 菜单项适配器

  这里以整数类型为例：

  ```c
  /** @brief : 整数类型菜单项句柄。 */
  typedef struct
  {
      int32_t *data;
      int32_t bData;
      int32_t v,e;
      int32_t cur;
  } menu_item_variHandle_t;
  ```

  该结构体包含了指向被操作变量的指针 `int32_t *data;`、被操作变量的副本 `int32_t bData;`、科学计数法使用的变量 `int32_t v,e;`、调参指针 `int32_t cur;`。这些变量被对应菜单项类型的操作接口访问和使用。

- 菜单项操作接口

  这里以整数类型为例：
  
  ```c
  /** @brief : 整数类型菜单项操作函数。 */
  const menu_itemAdapter_t menu_itemAdapter_variType =
  {
      .ItemConstruct = MENU_ItemConstruct_variType,
      .ItemGetData = MENU_ItemGetData_variType,
      .ItemSetData = MENU_ItemSetData_variType,
      .ItemPrintSlot = MENU_ItemPrintSlot_variType,
      .ItemDirectKeyOp = MENU_ItemDirectKeyOp_variType,
      .ItemPrintDisp = MENU_ItemPrintDisp_variType,
      .ItemKeyOp = MENU_ItemKeyOp_variType,
  };
  void MENU_ItemConstruct_variType(menu_itemIfce_t *_item, void *_data);
  void MENU_ItemGetData_variType(menu_itemIfce_t *_item, void *_data);
  void MENU_ItemSetData_variType(menu_itemIfce_t *_item, void *_data);
  //used when in menuList
  void MENU_ItemPrintSlot_variType(menu_itemIfce_t *_item, uint32_t _slotNum);
  void MENU_ItemDirectKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
  //used when in menuItem
  void MENU_ItemPrintDisp_variType(menu_itemIfce_t *_item);
  void MENU_ItemKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
  ```
  
  每个菜单类型都应包含上述7个命令函数。这些函数建议完全按照此格式命名。菜单项中的`adapter`将会指向由这些函数的函数指针构成的结构体`menu_itemAdapter_variType`。在调用一个特定的`menuItem_t`的命令函数时，将会根据其中存储的`adapter`调用这些命令函数。
  
- 菜单表结构体

  引入如下菜单表结构体来管理菜单表：

  ```c
  typedef struct _menu_list_t
  {
      menu_itemIfce_t **menu;         /// 菜单项指针的动态数组，用于存放指针。该数组内的指针析构时需要手动释放。
      uint32_t listSize, listNum;     /// 当前菜单项指针列表的大小、当前列表内的菜单项数量。
      uint32_t disp_p, slct_p;        /// 显示数组下标和选择数组下标。
      char nameStr[menu_nameStrSize]; /// 菜单列表名称字符串。
                                      //_menu_list_t *prev;		    /// 指向上级菜单的指针。根菜单中该指针指向自身。
  } menu_list_t;
  ```

  `menu_itemIfce_t **menu;` ：一个元素类型为 `menu_itemIfce_t*` 的数组。该数组的大小由构造函数指定，并在构造时分配，一旦分配不再更改，直到析构时释放内存。

- 菜单表操作接口

#### 菜单状态变量（状态机）

- 顶层变量

  - 顶层处理变量

    ```c
    /**
     * @brief : 菜单状态机。
     * @ {
     */
    extern menu_list_t *menu_currList;          ///< 状态变量：指向当前所在的菜单列表
    extern menu_itemIfce_t *menu_currItem;      ///< 状态变量：指向当前所在的菜单项，仅位于菜单项
    extern menu_list_t *menu_menuRoot;          ///< 根菜单指针。
    extern int32_t &menu_currRegionNum;         ///< 当前局部存储区号
    extern int32_t menu_statusFlag;             ///< 状态标志位
    extern uint32_t menu_nvm_statusFlagAddr;    ///< 存储状态标志位的NVM存储地址
    extern int32_t &menu_nvmCopySrc, &menu_nvmCopyDst;
    /**
     * @ }
     */
    ```

  - 菜单状态标志位定义

    ```c
    enum menu_status_t
    {
        menu_data_valid = menu_dataValid_flag << menu_dataValid_mask, /// 菜单状态标志
    
        menu_error_fatalError = 1 << 23,  ///> 关键故障标志位。
        menu_warning_itemLost = 1 << 22,  ///> 数据缺失标志位。读取数据时发现数据缺失时置位，须手动清除。该标志位使用16位参数，表示数据缺失的总个数。
        menu_datalog_cmbPrint = 1 << 21,  ///> cm_backtrace错误打印标志位。发生cmb打印时设置，须手动清除。该标志位（目前）不使用参数。
        menu_datalog_usrPrint = 1 << 20,  ///> 用户错误信息打印标志位，发生用户错误信息打印时设置，须手动清除。该标志位的参数由用户定义。
        menu_message_buttonOp = 1 << 19,  ///> 按键操作消息
        menu_message_printDisp = 1 << 18, ///> 屏幕打印消息
        menu_noUse6 = 1 << 17,
        menu_noUse7 = 1 << 16,
    
        menu_param_Mask16 = 0xffff, /// 低16位全16位掩码标志位，用于读取低16位参数。如果传一个参数且取值大于255，应使用16位参数。
        menu_param_Mask8h = 0xff00, /// 低16位中高8位掩码标志位，用于读取16位参数中的高8位。如果要返回两个参数，或参数取值小于256，可以使用两个8位参数。
        menu_param_Mask8l = 0x00ff, /// 低16位中高8位掩码标志位，用于读取16位参数中的低8位。同上。
    };
    ```

    


### 外围组件（责任链模式）

#### 按键输入

- 按键操作定义

  ```c
  typedef enum _menu_keyOpCode_t
  {
      menuOpCode_nl = 0, ///< NULL
      menuOpCode_ok = 1,
      menuOpCode_up,
      menuOpCode_dn,
      menuOpCode_lf,
      menuOpCode_rt,
  } menu_keyOpCode_t;
  
  typedef enum _menu_keyOpType_t
  {
      menuOpType_shrt = 1 << 8,
      menuOpType_long = 2 << 8,
      menuOpType_lrpt = 3 << 8,
  } menu_keyOpType_t;
  
  typedef uint32_t menu_keyOp_t;
  
  extern menu_keyOp_t menu_keyOpBuff;
  ```

  

- 按键消息处理

  根据上述枚举定义，每条按键消息都由两部分组成：位于低八位的按键码和位于高八位的操作码。一旦有按键消息从按键处理模块产生，该按键消息将被写入缓存区 `menu_keyOpBuff`，然后置位菜单中断标志位中的按键消息位，最后置位NVIC中对应的中断有效标志。



#### 屏幕输出

字符缓存

全缓存（可选）



#### NVM存储

- NVM变量

  - NVM存储结构体

    ```c
    typedef struct _menu_nvmData_t
    {
        uint32_t head;
        char nameStr[menu_nameStrSize];
        uint32_t type;
        uint32_t data;
        uint32_t tail;
    }menu_nvmData_t;
    ```

  - NVM缓存管理

		```c
		/**
		 * @brief : 菜单项写入缓存。
		 * 当改写第N个扇区时，menu_nvm_cachedSector = N, menu_nvm_cache分配4KB缓存
		 * 并读入第N扇区的所有内容。此时能且仅能修改第N扇区的内容。对第N扇区内容的修改
		 * 将缓存至上述内存。
		 */
		extern uint8_t *menu_nvm_cache;
		extern uint32_t menu_nvm_cachedSector;
		```

    

- NVM存储接口（惰性缓存）

  每个保存的菜单项都占用32个字节的Flash空间。在保存菜单数据时，为了节约NVM（一般是Flash存储器）的寿命，采用惰性缓存的方法：开辟一块与扇区大小相同的内存作为缓存，并开辟一个整型变量用于存储所缓存的扇区号。如要将数据写入地址 `Addr`，分三种情况：1. 如果 `Addr` 所在扇区已被缓存，则直接修改缓存区中对应位置的值。 2. 若 `Addr` 所在扇区未被缓存，且当前未缓存任何扇区，则缓存 `Addr` 所在扇区，执行情况1。 3. 若 `Addr` 所在扇区未被缓存，且当前已缓存其他扇区，则擦除已缓存扇区后将缓存写回该扇区，执行情况2。**注意：最后一次写NVM操作结束后，需要手动将已缓存扇区写回NVM。**



## 应用指南

### 依赖项与初始化

by：CkovMk @hitsic 2020.10.13

内容已更新以适配v0.1.4版本。

#### 初始化步骤

- 在主函数初始化阶段依次调用PITMGR、EXTMGR、FLASH、OLED、MENU的初始化函数。这些函数都没有参数，具体内容详见头文件内的函数注释，在此不再说明。 **注意：按键BUTTON的初始化已包含在MENU内部，不需要手动初始化。**

  > 代码清单1
  >
  > ```c
  > void main()
  > {
  >   	/** 初始化阶段，关闭总中断 */
  >   	HAL_EnterCritical();
  > 	
  >   	/** 其他初始化代码 */
  > 	
  >   	/** 初始化PIT中断管理器 */
  >   	PITMGR_Init();
  >   	/** 初始化I/O中断管理器 */
  >   	EXTINT_Init();
  >   	/** 初始化ftfx_Flash */
  >   	FLASH_SimpleInit();
  >   	/** 初始化OLED屏幕 */
  >   	OLED_Init();
  >   	/** 初始化菜单 */
  >   	MENU_Init();
  >       /** 读取NVM中保存的存储区号 */
  >   	MENU_Data_NvmReadRegionConfig();
  >     /** 读取全局存储区和局部存储区 */
  >   	MENU_Data_NvmRead(menu_currRegionNum);
  >   	
  > 	/** 其他初始化代码 */
  >   	
  > 	/** 初始化结束，开启总中断 */
  >   	HAL_ExitCritical();
  >   	while(1)
  >   	{
  >   	      /** 无限循环 */
  >   	}
  >   }
  > ```
  

  
- 此时应该就可以编译通过并运行了。屏幕上将显示Demo界面。

#### 注意事项

- 菜单初始化时必须关闭总中断。事实上，整个初始化阶段均应关闭总中断。如无特殊情况，开启总中断应该是主函数进入死循环前的最后一条语句。
- 菜单本体初始化前必须完成OLED模组及NVM模组（一般是Flash）的初始化，且所有按键所在引脚必须为释放状态。**注意：如果按键初始化时为按下状态，初始化将被永久阻塞，直到按键释放。**



### 创建自己的菜单

by：CkovMk @hitsic 2020.10.13

内容已更新以适配v0.1.4版本。

- 在合适的位置创建`void MENU_DataSetUp(void)`新的实现。原函数已声明为弱符号`__WEAK`，新定义将自动覆盖旧定义，不会产生重复定义的错误。**注意：禁止在`app_menu.c`中直接修改代码！**该函数会在初始化时被`MENU_Init(void);`函数调用，禁止手动调用。

- 添加自己的菜单项。

  - 构造菜单项：调用`MENU_ItemConstruct`。
  - 添加菜单项：调用`MENU_ListInsert`。
  - 添加子菜单：先调用`MENU_ListConstruct`构造菜单列表，再调用`MENU_ItemConstruct`和`MENU_ListInsert`向上级菜单插入跳转菜单项。

  > 代码清单1
  >
  > ```c
  > /** 在其他位置创建的MENU_DataSetup函数 */
  > 
  > /** 创建子菜单指针 */
  > menu_list_t* myList_1;
  > 
  > /** 这些变量仅用于示例 */
  > int32_t testInt = 19981214;
  > float testFlt = 3.1415926f;
  > 
  > void ExampleHandler(menu_keyOp_t* const _op)
  > {
  >     *_op = 0;
  > }
  > 
  > void MENU_DataSetUp(void)
  > {
  >     MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(nullType, NULL, "", 0, 0));
  > 
  > /** 子菜单指针初始化 */
  >     myList_1 = MENU_ListConstruct(
  >         "myList_1",     ///> 菜单标题，在菜单列表中的第一行显示，最大12字符。
  >         50,             ///> 菜单列表的大小，须预留1位用于返回上一级的[back]。
  >         menu_menuRoot   ///> 该菜单的上级菜单指针。注意：该指针仅用于返回上级菜单，并不会将子菜单插入上级菜单。
  >     );
  > /** 检查内存分配是否成功 */
  >     assert(testList);
  > /** 将子菜单的跳转入口插入其上级菜单 */
  >     MENU_ListInsert(
  >         menu_menuRoot,  ///> 要插入的上级菜单。
  >         MENU_ItemConstruct(
  >         menuType,   ///> 类型标识，指明这是一个菜单跳转类型的菜单项。
  >         myList_1,   ///> 数据指针，这里指向要跳转到的菜单列表。
  >         "TestList", ///> 菜单项名称，在菜单列表中显示。
  >         0,          ///> 数据的保存位置，对于非数据类型填0即可。
  >         0           ///> 属性Flag，无任何属性填0。
  >     ));
  >     {   //这里加这组括号只是为了缩进方便，其内部的语句用于向myList_1插入菜单项。
  >         MENU_ListInsert(myList_1, MENU_ItemConstruct(
  >             variType,  ///> 类型标识，指明这是一个整数类型的菜单项
  >             &testInt,  ///> 数据指针，这里指向要操作的整数。必须是int32_t类型。
  >             "T_int",   ///> 菜单项名称，在菜单列表中显示。
  >             10,        ///> 数据的保存地址，不能重复且尽可能连续，步长为1。
  >                        ///> 全局数据区0~9的地址为保留地址，不能使用。
  >             menuItem_data_global | menuItem_data_ROFlag
  >                        ///> 属性flag。此flag表示该变量存储于全局数据区，且为只读变量。
  >         ));
  >         MENU_ListInsert(myList_1, MENU_ItemConstruct(
  >             varfType,  ///> 类型标识，指明这是一个浮点类型的菜单项
  >             &testFlt,  ///> 数据指针，这里指向要操作的整数。必须是float类型。
  >             "T_float", ///> 菜单项名称，在菜单列表中显示。
  >             0,         ///> 数据的保存地址，不能重复且尽可能连续，步长为1。
  >             menuItem_data_region
  >                        ///> 属性flag。此flag表示该变量存储于局部数据区。局部数据区内的数据没有保留地址。
  >         ));
  >     }
  >     MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(
  >           procType,  ///> 类型标识，指明这是一个浮点类型的菜单项
  >           &ExampleHandler,///> 数据指针，这里指向要操作的整数。必须是float类型。
  >           "T_proc ", ///> 菜单项名称，在菜单列表中显示。
  >           0,         ///> 数据的保存地址，不能重复且尽可能连续，步长为1。
  >           menuItem_proc_runOnce
  >                      ///> 属性flag。此flag表示该该程序运行一次就退出。
  >     ));
  > }
  > 
  > ```

- 设置好自己的菜单后，编译下载，就可以看到自己定义的菜单了。



### 菜单第一次启动

by CkovMk @hitsic 2020.10.30

菜单第一次启动时，由于NVM中未存储任何数据，将不会从NVM中加载数据。此时应对菜单的各个局部数据区分别执行一次保存操作，以初始化NVM存储。再次之后，菜单将在启动时读取NVM中存储的数据。





## 移植指南

### 顶层移配置

- MENU组件

  启用/禁用：由宏`HITSIC_USE_APP_MENU`控制。0为禁用。

- 调试输出

  启用/禁用：由宏`HITSIC_MENU_PRINT_ENABLE`控制。

  输出语句：由宏`HITSIC_MENU_PRINTF`配置。

- 根菜单最大容量

  由于本组件中的菜单列表需要在初始化时指定其大小，根菜单的大小由宏`HITSIC_MENU_ROOT_SIZE`决定。

- 事件任务接口

  本组件目前利用未使用的中断服务来模拟事件处理。需要配置的项有：

  ```c++
  /**
   * @brief : 菜单消息调用所使用的中断信息。
   * 可以使用任何当前工程中未使用的中断。注意中断号和中断服务函数
   * 必须对应。优先级不可过高。
   */
  #define HITSIC_MENU_SERVICE_IRQHandler (Reserved85_IRQHandler)	///< 要使用的中断服务函数
  #define HITSIC_MENU_SERVICE_IRQn (Reserved85_IRQn) 				///< 要使用的中断号
  #define HITSIC_MENU_SERVICE_IRQPrio (12u) 						///< 中断优先级，需要设置一个较低的值，以免打断重要任务。
  ```



### 按键输入接口

- 启用/禁用

  由宏`HITSIC_MENU_USE_BUTTON`控制。如果不使用菜单自带的按键处理，您也可以调用顶层API`void MENU_KeyOp(menu_keyOp_t *const _op);`手动传入按键操作。**菜单自带的按键处理依赖于BUTTON组件。**

- GPIO配置

  由宏`HITSIC_MENU_BUTTON_5DIR_BSP_INIT`配置。该宏定义了一个长度为5的`button_t`结构体数组，该结构体的定义详见BUTTON组件。

  在MENU的移植文件中，每一个结构体仅需修改前两项：第一项为按键所在的GPIO外设，第二项为按键所在的GPIO编号。其他项保持默认即可。

  五个结构体按顺序依次为：确定、向上、向下、向左、向右。

  > 示例：
  >
  > ```c++
  > /** @brief : 菜单使用的五向按键初始化。每组数据前两个是GPIO和Pin，其余数据为0。 */
  > #define HITSIC_MENU_BUTTON_5DIR_BSP_INIT  \
  >     {                                     \
  >         {                                 \
  >             RTEPIN_DIGITAL_BUTTON_OK_GPIO, \
  >             RTEPIN_DIGITAL_BUTTON_OK_PIN,   \
  >             kPORT_InterruptOrDMADisabled, \
  >             0,                            \
  >             BUTTON_STAT_NONE,             \
  >             NULL,                         \
  >         },                                \
  >         {                                 \
  >         	RTEPIN_DIGITAL_BUTTON_UP_GPIO,  \
  > 			RTEPIN_DIGITAL_BUTTON_UP_PIN,   \
  >             kPORT_InterruptOrDMADisabled, \
  >             0,                            \
  >             BUTTON_STAT_NONE,             \
  >             NULL,                         \
  >         },                                \
  >         {                                 \
  >         	RTEPIN_DIGITAL_BUTTON_DN_GPIO,  \
  > 			RTEPIN_DIGITAL_BUTTON_DN_PIN,   \
  >             kPORT_InterruptOrDMADisabled, \
  >             0,                            \
  >             BUTTON_STAT_NONE,             \
  >             NULL,                         \
  >         },                                \
  >         {                                 \
  >         	RTEPIN_DIGITAL_BUTTON_LF_GPIO,  \
  > 			RTEPIN_DIGITAL_BUTTON_LF_PIN,   \
  >             kPORT_InterruptOrDMADisabled, \
  >             0,                            \
  >             BUTTON_STAT_NONE,             \
  >             NULL,                         \
  >         },                                \
  >         {                                 \
  >         	RTEPIN_DIGITAL_BUTTON_RT_GPIO,  \
  > 			RTEPIN_DIGITAL_BUTTON_RT_PIN,   \
  >             kPORT_InterruptOrDMADisabled, \
  >             0,                            \
  >             BUTTON_STAT_NONE,             \
  >             NULL,                         \
  >         },                                \
  >     }
  > ```



### 屏幕显示接口

- 帧缓存

  启用/禁用：由宏`HITSIC_MENU_USE_FRAME_BUFFER`控制。

#### 帧缓存模式

首先需要在这里包含所需头文件,然后将三个宏函数连接到三个函数适配器。

```c++
#include "drv_disp_ssd1306.hpp"
#include "lib_graphic.hpp"
#define HITSIC_MENU_DISPLAY_BUFFER_CLEAR() (MENU_FrameBufferClear())
#define HITSIC_MENU_DISPLAY_PRINT(row, col, str) (MENU_FrameBufferPrint(row, col, str))
#define HITSIC_MENU_DISPLAY_BUFFER_UPDATE() (MENU_FrameBufferUpdate())
```

随后在`app_menu_port.hpp`内实现这三个函数。

`void MENU_FrameBufferClear(void);`用于清除缓存区。

`void MENU_FrameBufferPrint(uint16_t x, uint16_t y, char *str);`用于向缓存区打印字符串。

`void MENU_FrameBufferUpdate(void);`用于将缓存区上传到屏幕。

#### 无缓存模式

只需要适配宏`HITSIC_MENU_DISPLAY_PRINT`。该接口用于将字符打印至屏幕。

其他两个宏定义`HITSIC_MENU_DISPLAY_BUFFER_CLEAR`和`HITSIC_MENU_DISPLAY_BUFFER_UPDATE`置为`(0)`即可。



### NVM存储接口

- NVM存储

  启用/禁用：由宏`HITSIC_MENU_USE_NVM`控制。

- NVM接口正在施工中，文档暂缓更新...