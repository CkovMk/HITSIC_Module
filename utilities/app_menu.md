# APP_MENU

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

Flash读写驱动（FTFX_FLASH），用于保存和读取菜单；

屏幕驱动（DISP_SSD1306），用于屏幕显示输出；

按键驱动（BUTTON），用于管理按键功能；

**直接包含的外部头文件：**

`stdafx.h` 预编译头文件

`hitsic_common.h` K66平台环境头文件

**间接包含的主要文件：**

`hitsic_def.h` 配置头文件，用于控制各模块的功能，并适配模块内的各种接口。

`hitsic_common.h` 模块通用头文件。

`inc_fsl_mk66f18.h` K66平台NXP官方库。

`inc_stdlib.h` C/C++标准库。

`inc_generic.h` 平台无关的符号定义。

## 版本说明

### 开发计划

完善菜单存储功能

完善函数类型菜单

属性Flag提供更多选项，并在运行过程中指示该变量是否已修改。



### v0.1.2

by：Chekhov Mark @hitsic 2019.11.22

主要功能初步完善。

更新内容：

- 菜单存储功能已可以正常使用。菜单内含1个全局存储区和3个局部存储区，全局存储区用于存储公共参数，而三个局部存储区可存储三组不同参数。当前所使用的参数组可以在菜单内选择。单片机上电后可以自动读取保存的数据。**注意：存储区间的数据拷贝功能还不完善，可能造成数据丢失。**
- 调整及新增了一些属性Flag。现在，`menuItem_data_ROFlag` 标志仅表示该变量不能从菜单修改，但仍会被保存到对应的Nvm存储区。如要禁止保存该变量，使用新增的`menuItem_data_NoSave` 标志。**注意：此标志仅代表在保存菜单时不会保存该变量，用户仍可以对具有此标志的菜单项分配存储空间，并手动保存改变量到分配的地址。**





### v0.1.1

by：Chekhov Mark @hitsic 2019.11.02

第一个正式版。经过早期测试没有严重Bug。一些功能仍未完善。

主要功能：

- 使用单条语句构造和析构菜单和菜单项。菜单项的插入和删除与类型无关。

- 低优先级中断操作，不干扰高优先级中断，也不占用`main()`函数。

- 所有按键均支持短按、长按、长按重复三种操作。在任意菜单列表内长按OK返回上一级菜单。也可以使用每个菜单列表顶部的[back]菜单项返回上级菜单。

- 可以使用空类型（分割线）、整形（int32_t）、浮点型（float）、菜单跳转、函数调用五种菜单项。

  - 参数类型（整数和浮点）支持在菜单列表内快速调参，其中int32_t型在仅有一位数时调整第一位有效数字，在大于等于两位数时固定调整第二位有效数字；float型全范围内固定调节第二位有效数字。
  - 参数类型在菜单列表内具有自适应显示。（-10^5 ，+10^5）内的整型参数和（-10^4，-10^-1）∪（+10^-1，+10^4）内的浮点参数将直接显示，这些范围外的参数将采用四位有效数字的“准”科学计数法显示。该显示方式为`±abcd±e`的形式，为了节约显示空间没有使用标准的小数点表示法并省掉了有效数字与数量级之间的字符E，表示`±abcd × 10^±e`。
  - 参数类型具有二级调参页面，可以科学计数法的方式对前四位有效数字进行精确设置。在二级调参页面中，短按OK键可以保存并退出，长按OK键可以放弃更改并退出。
  - 所有菜单项都有独立的属性控制，可以设置只读、可写、保存区域、显示方式等属性，部分属性在此版本中不可用，在后续的更新中将陆续实现。
- 可以支持任意数量的子菜单和菜单项，一个子菜单可以有一个或多个入口。

存在问题：

- 目前菜单仅能保证基本使用，有些功能仅做了接口，并没有实现。使用上可能会引起困惑。
- 该菜单没用添加定时任务，故仅在有按键操作时刷新菜单。相关接口已经留好，后续更新中实现。
- 菜单的Flash保存功能还在开发中，暂时无法使用。菜单内的数据在复位后会丢失，需要手动记录。
- 删去了紫丁香LOGO的相关数据，无法显示紫丁香LOGO。

### v0.1-beta.0

此版本是一个预览版本，Bug很多，代码仅完成了基本的结构。

## API文档

### 顶层API

- 菜单初始化
  
  ```c
  /** @brief : 菜单全局初始化。
   * 该函数会初始化必要的变量，创建根菜单和其他最基本的菜单列表和菜单项。
   *
   * @param  {void} undefined :
   */
  void MENU_Init(void);
  ```

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
   */
  void MENU_Data_NvmSave_Boxed(void);
  ```

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
   */
  void MENU_Data_NvmRead_Boxed(void);
  ```

- 读写局部存储区设置

  ```c
  /**
   * @brief : 保存当前局部存储区号到NVM。
   * 该数值设置为不自动保存。
   * 
   */
  void MENU_Data_NvmSaveRegionConfig(void);
  
  /**
   * @brief : 从NVM中读取当前局部存储区号。
   * 该数值设置为不自动保存。
   * 
   */
  void MENU_Data_NvmReadRegionConfig(void);
  ```

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
   */
  void MENU_Data_NvmCopy_Boxed(void);
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

### 其他API

- 其他API的说明参见代码注释。





## 设计文档

### 术语与概述

- 菜单：管理菜单项和菜单表的结构。处理菜单的操作、显示、数据。
- 菜单项：对数据进行操作的单元，表现为菜单表中的元素。可接受操作。
- 菜单项界面：显示一个菜单项的详细内容，供用户对数据进行精细操作的界面。
- 菜单表：对菜单项进行管理的结构，表现为菜单项组成的列表。可接受操作。
- 菜单表界面：用于显示多个菜单项，供用户选择和调节的界面。
- 操作：按键等输入设备对菜单的输入。
- 显示槽：一个菜单项在菜单表界面中的显示空间。

### 核心逻辑

#### 菜单项与菜单表

- 菜单项结构体
  
  定义如下结构体用于存放一个菜单项所包含的全部信息：
  
  ```c
  /** @brief : 菜单项接口结构体。 */
  typedef struct _menu_itemIfce_t
  {
      menu_itemType_t type;           /// 此菜单项的类型。
      //menu_list_t *myList;          /// 此菜单项所属的菜单列表（暂时没用）。
      uint32_t pptFlag;               /// 此菜单项的属性标志位。
      uint32_t list_id, unique_id;    /// 此菜单项在本列表内的序号（从0开始）、全局唯一序号（从0开始）
      uint32_t saveAddr;              /// 此菜单在本区域内的偏移地址。从0开始，以1步进。注意，全局数据区和局部数据区的地址分开来算。
      char nameStr[menu_nameStrSize]; /// 此菜单项的名称字符串。最大长度为 menu_nameStrSize - 1 字节。
      union menu_itemIfce_handle_t    /// 菜单项操作句柄的共用体。使用时根据此菜单项的类型调取对应项访问。
      {
          void *p_void;
          menu_item_nullHandle_t *p_nullType;
          menu_item_variHandle_t *p_variType;
          menu_item_varfHandle_t *p_varfType;
          menu_item_boolHandle_t *p_boolType;
          menu_item_procHandle_t *p_procType;
          menu_item_menuHandle_t *p_menuType;
      } handle;
      //void* pHandle;
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

  `menu_itemIfce_t **menu;` ：一个元素类型为 `menu*` 的数组。该数组的大小由构造函数指定，并在构造时分配，一旦分配不再更改，直到析构时释放内存。

- 菜单表操作接口

#### 顶层逻辑

- 顶层变量

  - 顶层处理变量

    ```c
    extern uint32_t menu_itemCnt;
    extern uint32_t menu_listCnt;
      
    extern menu_list_t *menu_currList;
    extern menu_itemIfce_t *menu_currItem;
    extern menu_list_t *menu_menuRoot;
    extern int32_t menu_currRegionNum;
    extern int32_t menu_statusFlag;
    extern uint32_t menu_nvm_statusFlagAddr;
      
    extern char menu_dispStrBuf[MENU_DISP_STRBUF_ROW][MENU_DISP_STRBUF_COL];
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

    

- 顶层操作接口

### 外围组件

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

    

- NVM存储接口

  每个保存的菜单项都占用32个字节的Flash空间。在保存菜单数据时，为了节约NVM（一般是Flash存储器）的寿命，采用惰性缓存的方法：则开辟一块与扇区大小相同的内存作为缓存，并开辟一个整型变量用于存储所缓存的扇区号。如要将数据写入地址 `Addr`，分三种情况：1. 如果 `Addr` 所在扇区已被缓存，则直接修改缓存区中对应位置的值。 2. 若 `Addr` 所在扇区未被缓存，且当前未缓存任何扇区，则缓存 `Addr` 所在扇区，执行情况1。 3. 若 `Addr` 所在扇区未被缓存，且当前已缓存其他扇区，则擦除已缓存扇区后将缓存写回该扇区，执行情况2。**注意：最后一次写NVM操作结束后，需要手动将已缓存扇区写回NVM。**

### 附录1：符号说明

- 菜单项类型

  目前支持的数据类型有：

  ```c
  typedef enum
  {
      nullType,
      variType, //整数类型，data应传入（int*）或(int32_t*)。
      varfType, //浮点类型，data应传入(float*)。
      procType, //函数类型，可以运行函数，data应传入(void*)(void(*)(void))。
      menuType, //菜单类型，用于跳转菜单列表。data应传入(menu_list_t*)。
  } menu_itemType_t;
  ```

- 属性Flag

  目前支持的属性Flag有：

  ```c
  typedef enum
  {
      /** data config 数据属性配置 */
      menuItem_data_global = 1 << 0, ///< 该菜单项存储在全局数据区。所有参数类型的菜单都应包括此项，除非使用了ROFlag。
      menuItem_data_ROFlag = 1 << 2, ///< 该菜单项为只读。只读菜单项不允许在菜单内修改，也不能保存到存储区或从存储区读取。
  
      /** display config 显示属性配置 */
      menuItem_disp_forceSci = 1 << 8,  ///< 该菜单项强制使用科学计数法，适用于variType和varfType。
  
      /** error mask */
  } menu_itemPropety_t;
  ```

  互不冲突的属性Flag之间用按位或运算符"|"连接。例如`(menuItem_data_ROFlag | menuItem_disp_forceSci)`表示该数据属性为”该数据为只读，且 强制使用科学计数法显示“。

## 应用指南

### APP_MENU v0.1.1 移植说明

For hello_world_v2.x

by：Chekhov Mark @hitsic 2019.11.02

内容已更新以适配v0.1.2版本。

#### 移植须知

**注意事项**

移植前必须备份工程！该菜单使用了一些与`hello_world_v2.x`版本工程功能重复却不兼容的模块，移植过程中需要重写使用了不兼容模块的已有代码，如果移植失败对原有工程影响较大。除Flash驱动外，本菜单附带的所有驱动均包含了原工程内冲突驱动的所有功能，移植成功后只需对原有代码做少量修改。事实上一部分驱动仅修改了原有代码的名称，使其更符合代码规范。不过PITMGR与原有工程区别较大，需特别注意。

**移植步骤**

- 从原工程中移除冲突的模块：flash_driver（被ftfx_flash替换）、IRQ_manager（被extint替换）、sys_pitmgr（这个可以不移除，和菜单自带的pitmgr一样。如果这里不移除此模块，添加文件时注意不要重复添加）、SmartCar_Pit（与pitmgr冲突）、SmartCar_Systick（可以不移除，但pitmgr已经提供计时和延时的功能且更加安全，该模块已不再必要）。注释或暂时移除受影响的模块，使得工程能编译通过。

- 菜单初始化时必须关闭总中断。事实上，整个初始化阶段均应关闭总中断。如无特殊情况，开启总中断应该是主函数进入死循环前的最后一条语句。

- 将菜单本体及依赖项复制到合适的位置，添加包含路径，加入工程，包含相关头文件。在主函数初始化阶段依次调用PITMGR、EXTMGR、FLASH、OLED、MENU的初始化函数。这些函数都没有参数，具体内容详见头文件内的函数注释，在此不再说明。 **注意：按键BUTTON的初始化已包含在MENU内部，不需要手动初始化。**

  > 代码清单1
  >
  > ```c
  > void main()
  > {
  >      /** 初始化阶段，关闭总中断 */
  >      __disable_irq();
  >  
  >      /** 其他初始化代码 */
  >  
  >      /** 初始化PIT中断管理器 */
  >      PITMGR_Init();
  >      /** 初始化I/O中断管理器 */
  >      EXTINT_Init();
  >      /** 初始化ftfx_Flash */
  >      FLASH_SimpleInit();
  >      /** 初始化OLED屏幕 */
  >      OLED_Init();
  >      /** 初始化菜单 */
  >      MENU_Init();
  >      /** 打印第一帧菜单 */
  >      MENU_PrintDisp();
  >  
  >      /** 其他初始化代码 */
  >  
  >      /** 初始化结束，开启总中断 */
  >      __enable_irq();
  >      while(1)
  >      {
  >            /** 无限循环 */
  >      }
  > }
  > ```

- 修改`hitsic_def.h`文件。前面的数字表示行号，仅供参考。

  073：宏`HITSIC_MENU_DEBUG_ENABLE`：是否开启调试信息输出。为避免浪费打印输出的时间，将其置0。

  080：打印输出语句的名称。如要使用调试输出功能，根据自己的平台适配一个printf格式的函数。

  086：根菜单的容量。为了简化代码，菜单内存采用一次性动态分配的方式，因此创建菜单时需要指定菜单大小。如果需要在根菜单插入大量菜单项，可适当将其改大，够用即可。推荐在根菜单插入几个子菜单，在子菜单中插入菜单项。

  089：菜单按键所使用GPIO定义。每组只需修改前两个，第一个是GPIO实例，第二个是GPIO编号。例如A15端口，对应{GPIOA, 15, ... } 。五组定义的顺序为：确认键、上方向、下方向、左方向、右方向。**不要修改每组中的其他定义！**

  137：这三行是菜单所使用的中断服务定义，一般不需要修改。如有必要，合理设置中断优先级即可。菜单优先级一般较低。

  144：这是OLED打印函数的接口定义。如果不使用MENU中自带的OLED驱动，需要将名称改为`SmartCar_OLED`中的函数名称`OLED_P6x8Str`（都是学嘉的代码，MENU这个改了改名，其实大同小异）。

- 此时应该可以编译通过并运行了。屏幕上将显示和Demo文件一样的界面。不能编译通过的再找找问题。





### 创建自己的菜单

内容已更新以适配v0.1.2版本。

- 添加自己的菜单项。~~找到`app_menu.c`内的`void MENU_DataSetUp(void)`函数，将其注释掉（仅注释源文件中的函数实现，保留头文件中的函数声明），然后在main()函数后~~创建`void MENU_DataSetUp(void)`新的实现。原函数已声明为`__WEAK`，新定义将自动覆盖旧定义。**注意：禁止在`app_menu.c`中直接修改代码！**该函数会被`MENU_Init(void);`函数调用，禁止手动调用。

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
  > void MENU_DataSetUp(void)
  > {
  >     MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(nullType, NULL, "", 0, 0));
  >
  >  /** 子菜单指针初始化 */
  >     myList_1 = MENU_ListConstruct(
  >      "myList_1",     ///> 菜单标题，在菜单列表中的第一行显示，最大12字符。
  >      50,             ///> 菜单列表的大小，须预留1位用于返回上一级的[back]。
  >      menu_menuRoot  ///> 该菜单的上级菜单指针。
  >  );
  >  /** 检查内存分配是否成功 */
  >     assert(testList);
  >  /** 将子菜单的跳转入口插入其上级菜单 */
  >     MENU_ListInsert(
  >      menu_menuRoot, ///> 要插入的上级菜单。
  >      MENU_ItemConstruct(
  >          menuType,  ///> 类型标识，指明这是一个菜单跳转类型的菜单项。
  >          myList_1,  ///> 数据指针，这里指向要跳转到的菜单列表。
  >          "TestList", ///> 菜单项名称，在菜单列表中显示。
  >          0,          ///> 数据的保存位置，对于非数据类型填0即可。
  >          0           ///> 属性Flag，无任何属性填0。
  >      ));
  >     {   //这里加这组括号只是为了缩进方便，其内部的语句用于向myList_1插入菜单项。
  >         MENU_ListInsert(myList_1, MENU_ItemConstruct(
  >          variType,  ///> 类型标识，指明这是一个整数类型的菜单项
  >          &testInt,  ///> 数据指针，这里指向要操作的整数。必须是int32_t类型。
  >          "T_int",   ///> 菜单项名称，在菜单列表中显示。
  >          10,        ///> 数据的保存地址，对于数据类型不能重复且尽可能连续，步长为1。
  >                     ///> 10以内的地址为保留地址，不能使用。
  >          menuItem_data_global
  >                     ///> 属性flag。此flag表示该变量存储于全局数据区。在多组参数功
  >                     ///> 能写好之前维持此值。其他的Flag参见附录。
  >      ));
  >          MENU_ListInsert(myList_1, MENU_ItemConstruct(
  >          varfType,  ///> 类型标识，指明这是一个浮点类型的菜单项
  >          &testFlt,  ///> 数据指针，这里指向要操作的整数。必须是float类型。
  >          "T_float", 
  >          11,
  >          menuItem_data_global
  >      ));
  >     }
  > }
  >
  > ```

- 设置好自己的菜单后，编译下载，就可以看到自己定义的菜单了。

