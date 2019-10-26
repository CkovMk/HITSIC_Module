#pragma once 
#ifndef _APP_MENU_H_
#define _APP_MENU_H_
#include "stdafx.h"


/**
 * @brief 函数调用宏定义。
 */


#define MENU_ITEM_MAKE_FUNCTION(funcName,type)					(##funcName##_##type##)
#define MENU_ITEM_MAKE_HANDLE(p_Item,type)						((p_Item->handle).p_##type##)
#define MENU_ITEM_CALL_FUNCTION(funcName,p_Item,type,...)		(MENU_ITEM_MAKE_FUNCTION(funcName,type)((MENU_ITEM_MAKE_HANDLE(p_Item,type)),__VA_ARGS__))


/**
 * @brief : switch-case语句的宏定义。
 * 使用此语句简化menu_itemIfce操作函数内的switch-case调用。
 * 
 * @param funcName 	: 要分类切换的函数名称。
 * @param p_Item 	: 指向menu_itemIfce_t结构体的指针。
 * @param ... 		: 除上述指针以外的其他参数。没有其他参数则不写。
 * 
 * @example :
 * 
 * 		void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *_op)
 * 		{
 * 			MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, _item, _op);
 * 		}
 */
#define MENU_ITEM_SWITCH_CASE(funcName,p_Item,...)				\
		switch(p_Item->type)									\
		{														\
		case nullType:MENU_ITEM_CALL_FUNCTION(funcName,p_Item,nullType,__VA_ARGS__);break;	\
		case variType:MENU_ITEM_CALL_FUNCTION(funcName,p_Item,variType,__VA_ARGS__);break;	\
		case varfType:MENU_ITEM_CALL_FUNCTION(funcName,p_Item,varfType,__VA_ARGS__);break;	\
		case boolType:MENU_ITEM_CALL_FUNCTION(funcName,p_Item,boolType,__VA_ARGS__);break;	\
		case procType:MENU_ITEM_CALL_FUNCTION(funcName,p_Item,procType,__VA_ARGS__);break;	\
		case menuType:MENU_ITEM_CALL_FUNCTION(funcName,p_Item,menuType,__VA_ARGS__);break;	\
		default:break;	}


//test marco :
//MENU_ITEM_MAKE_FUNCTION(item, nullType, KeyOp);
//MENU_ITEM_CALL_FUNCTION(MENU_ItemConstruct, item, variType, &data);
//MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp,item,&op);





#ifndef __cplusplus
extern "C"{
#endif


/**
 * @brief : 按键操作宏定义。
 * TODO:以后应定义在按键驱动内，这里只留一个typedef。
 * 
 */
enum menu_keyOp_t
{
	key_up,
	key_dn,
	key_lf,
	key_rt,
};

/**
 * @brief : 函数类型菜单项的响应句柄。
 * 函数类型菜单项激活时会调用一个没有参数和返回值的函数。
 */
typedef void (*menu_itemProcHandler_t)(void);

/**
 * @brief : 菜单项和菜单列表名称的最大长度为16个字符。用于定义缓存区大小。
 */
uint32_t menu_nameStrSize = 16;

/**
 * @brief : 菜单项和菜单列表的计数器。
 */
extern uint32_t menu_itemCnt;
extern uint32_t menu_listCnt;








/**
 * **********菜单项 类型及变量声明 **********
 */


/**
 * @brief : 菜单项所支持的内容类型。
 */
enum menu_itemType_t
{
	nullType,
	variType,		//watch or set integer varibles
	varfType,		//watch or set float-point varibles
	boolType,
	procType,		//run certain process
	menuType,		//jump to another menu
};


/**
 * @brief : 菜单项属性枚举类型。
 * 标志位枚举。
 */
enum menu_itemPropety_t
{
	//data config
	menuItem_data_global = 1 << 0,			/// 该菜单项存储在全局数据区。
	menuItem_data_region = 1 << 1,			/// 该菜单项存储在局部数据区。
	menuItem_data_getPos = data_global | data_region,
	menuItem_data_ROFlag = 1 << 2,			/// 该菜单项为只读。只读菜单项不允许在菜单内修改，也不能保存到存储区或从存储区读取。
	menuItem_data_prioRW = 1 << 3,			/// 该菜单项需要先于其他菜单项读取。
	menuItem_data_getCfg = data_global | data_region | data_ROFlag | data_prioRW,

	//error mask
};


/**
 * @brief : 占位类型菜单项的操作句柄及操作函数。
 */
struct menu_item_nullHandle_t
{
	void *data;
};
void MENU_itemConstruct_nullType(menu_item_nullHandle_t _type, void *_data);
void *MENU_ItemGetData_nullType(menu_item_nullHandle_t *_item);
void MENU_ItemSetData_nullType(menu_item_nullHandle_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_nullType(menu_item_nullHandle_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_nullType(menu_item_nullHandle_t *_item, menu_keyOp_t *_op);
//used when in menuItem
void MENU_ItemPrintDisp_nullType(menu_item_nullHandle_t *_item);
void MENU_ItemKeyOp_nullType(menu_item_nullHandle_t *_item, menu_keyOp_t *_op);

/**
 * @brief : 整数类型菜单项的操作句柄及操作函数。
 */
struct menu_item_variHandle_t
{
	int32_t *data;
};
void MENU_itemConstruct_variType(menu_item_variHandle_t _type, void *_data);
void *MENU_ItemGetData_variType(menu_item_variHandle_t *_item);
void MENU_ItemSetData_variType(menu_item_variHandle_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_variType(menu_item_variHandle_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_variType(menu_item_variHandle_t *_item, menu_keyOp_t *_op);
//used when in menuItem
void MENU_ItemPrintDisp_variType(menu_item_variHandle_t *_item);
void MENU_ItemKeyOp_variType(menu_item_variHandle_t *_item, menu_keyOp_t *_op);

/**
 * @brief : 浮点类型菜单项的操作句柄及操作函数。
 */
struct menu_item_varfHandle_t
{
	float *data;
};
void MENU_itemConstruct_varfType(menu_item_varfHandle_t _type, void *_data);
void *MENU_ItemGetData_varfType(menu_item_varfHandle_t *_item);
void MENU_ItemSetData_varfType(menu_item_varfHandle_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_varfType(menu_item_varfHandle_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_varfType(menu_item_varfHandle_t *_item, menu_keyOp_t *_op);
//used when in menuItem
void MENU_ItemPrintDisp_varfType(menu_item_varfHandle_t *_item);
void MENU_ItemKeyOp_varfType(menu_item_varfHandle_t *_item, menu_keyOp_t *_op);

/**
 * @brief : 布尔类型菜单项的操作句柄及操作函数。
 */
struct menu_item_boolHandle_t
{
	bool *data;
};
void MENU_itemConstruct_boolType(menu_item_boolHandle_t _type, void *_data);
void *MENU_ItemGetData_boolType(menu_item_boolHandle_t *_item);
void MENU_ItemSetData_boolType(menu_item_boolHandle_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_boolType(menu_item_boolHandle_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_boolType(menu_item_boolHandle_t *_item, menu_keyOp_t *_op);
//used when in menuItem
void MENU_ItemPrintDisp_boolType(menu_item_boolHandle_t *_item);
void MENU_ItemKeyOp_boolType(menu_item_boolHandle_t *_item, menu_keyOp_t *_op);

/**
 * @brief : 函数类型菜单项的操作句柄及操作函数。
 */
struct menu_item_procHandle_t
{
	menu_itemProcHandler_t data;
};
void MENU_itemConstruct_procType(menu_item_procHandle_t _type, void *_data);
void *MENU_ItemGetData_procType(menu_item_procHandle_t *_item);
void MENU_ItemSetData_procType(menu_item_procHandle_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_procType(menu_item_procHandle_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_procType(menu_item_procHandle_t *_item, menu_keyOp_t *_op);
//used when in menuItem
void MENU_ItemPrintDisp_procType(menu_item_procHandle_t *_item);
void MENU_ItemKeyOp_procType(menu_item_procHandle_t *_item, menu_keyOp_t *_op);

/**
 * @brief : 跳转类型菜单项的操作句柄及操作函数。
 */
struct menu_item_menuHandle_t
{
	menu_list_t *data;
};
void MENU_itemConstruct_menuType(menu_item_menuHandle_t _type, void *_data);
void *MENU_ItemGetData_menuType(menu_item_menuHandle_t *_item);
void MENU_ItemSetData_menuType(menu_item_menuHandle_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_menuType(menu_item_menuHandle_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_menuType(menu_item_menuHandle_t *_item, menu_keyOp_t *_op);
//used when in menuItem
void MENU_ItemPrintDisp_menuType(menu_item_menuHandle_t *_item);
void MENU_ItemKeyOp_menuType(menu_item_menuHandle_t *_item, menu_keyOp_t *_op);


/**
 * @brief : 菜单项接口结构体。
 */
struct menu_itemIfce_t
{
	menu_itemType_t type;						/// 此菜单项的类型。
	menu_list_t *myList;						/// 此菜单项所属的菜单列表（暂时没用）。
	uint32_t pptFlag;							/// 此菜单项的属性标志位。
	uint32_t list_id, unique_id;				/// 此菜单项在本列表内的序号（从0开始）、全局唯一序号（从0开始）
	uint32_t saveAddr;							/// 此菜单在本区域内的偏移地址。从0开始，以1步进。注意，全局数据区和局部数据区的地址分开来算。
	char nameStr[menu_nameStrSize];				/// 此菜单项的名称字符串。最大长度为 menu_nameStrSize - 1 字节。
	union menu_itemIfce_handle_t				/// 菜单项操作句柄的共用体。使用时根据此菜单项的类型调取对应项访问。
	{
		menu_item_nullHandle_t *p_nullType;
		menu_item_variHandle_t *p_variType;
		menu_item_varfHandle_t *p_varfType;
		menu_item_boolHandle_t *p_boolType;
		menu_item_procHandle_t *p_procType;
		menu_item_menuHandle_t *p_menuType;
	}handle;
};


/**
 * ********** 菜单项操作接口 **********
 */

/**
 * @brief : 菜单项结构体的构造函数。
 * 该函数分配一块内存，根据传入的参数填入内容，然后返回所产生的菜单项结构体指针。
 * 
 * @param  {menu_itemType_t} _type : 要生成的菜单项类型。。
 * @param  {void*} _data           : 指向要操作的数据的指针。
 * @param  {char*} _nameStr        : 菜单项名称字符串，不超过最大长度，以'\0'结尾。
 * @param  {uint32_t} _saveAddr    : 变量要保存到的地址。对非数据类型及属性设置中只读的菜单项无效。
 * @param  {uint32_t} _pptFlag     : 属性标志。
 * 
 * @return {menu_itemIfce_t *}     : 返回所产生的菜单项结构体指针。
 */
menu_itemIfce_t *MENU_itemConstruct(menu_itemType_t _type, void *_data, const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag);


/**
 * @brief : 菜单项结构体析构函数。
 * 
 * @param  {menu_itemIfce_t*} _item : 要析构的菜单项结构体指针。析构后该指针会被自动赋值为NULL。
 */
void MENU_itemDestruct(menu_itemIfce_t *_item);



/**
 * @brief : 获取菜单项内存放的数据指针。
 * 
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
 * 
 * @return {void *}					: 返回数据指针。注意，无论何种类型，都会返回对应的数据指针。
 */
void *MENU_ItemGetData(menu_itemIfce_t *_item);


/**
 * @brief : 设置菜单项内指针指向的数据的值。
 * 注意，该函数对非数据类型菜单和只读数据菜单无效。
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
 * @param  {void*} _data            : 
 */
void MENU_ItemSetData(menu_itemIfce_t *_item, void *_data);


//used when in menuList


/**
 * @brief : 在菜单列表中打印一行菜单。
 * 
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
 * @param  {uint32_t} _slotNum      : 要打印位置（第几行）。对于12864 OLED而言，取值范围为0~7。
 */
void MENU_ItemPrintSlot(menu_itemIfce_t *_item, uint32_t _slotNum);


/**
 * @brief : 在菜单列表中响应直接按键操作。
 * 
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
 * @param  {menu_keyOp_t*} _op      : 按键操作指针。按键响应后会被清除为空操作。
 */
void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *_op);


//used when in menuItem

/**
 * @brief : 打印访问菜单项的二级页面。
 * 
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
 */
void MENU_ItemPrintDisp(menu_itemIfce_t *_item);

/**
 * @brief : 访问菜单项的二级页面时响应按键操作。
 * 
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
 * @param  {menu_keyOp_t*} _op      : 按键操作指针。按键响应后会被清除为空操作。
 */
void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *_op);






/**
 * 菜单列表结构体。
 */
struct menu_list_t
{
	menu_itemIfce_t *menu;				/// 菜单项指针的动态数组，用于存放指针。该数组内的指针析构时需要手动释放。
	uint32_t listSize, listNum;			/// 当前菜单项指针列表的大小、当前列表内的菜单项数量。
	uint32_t disp_p, slct_p;			/// 显示数组下标和选择数组下标。
	char nameStr[menu_nameStrSize];		/// 菜单列表名称字符串。
	menu_list_t *prev;					/// 指向上级菜单的指针。根菜单中该指针指向自身。
};

/**
 * @brief : 菜单列表的构造函数。
 * 
 * @param  {const char*} _nameStr : 
 * @param  {uint32_t} _listSize   : 
 * @param  {menu_list_t*} _prev   : 
 * 
 * @return {menu_list_t *}        : 返回构造的菜单列表结构体指针。
 */
menu_list_t *MENU_ListConstruct(const char *_nameStr, uint32_t _listSize , menu_list_t* _prev);

/**
 * @brief : 菜单列表的析构函数。
 * 
 * @param  {menu_list_t*} _list : 要析构的菜单列表结构体指针。析构后该指针会被自动赋为NULL。
 */
void MENU_ListDestruct(menu_list_t *_list);

/**
 * @brief : 插入一个菜单项。
 * 
 * @param  {menu_list_t*} _list     : 要访问的菜单列表的指针。
 * @param  {menu_itemIfce_t*} _item : 要插入的菜单项指针。该指针应为将亡值。
 * @return {status_t}               : 返回操作的结果。正常应返回kStatus_Success。
 */
status_t MENU_ListInsert(menu_list_t *_list, menu_itemIfce_t *_item);

/**
 * @brief : 打印菜单列表。
 * 
 * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
 */
void MENU_ListPrintDisp(menu_list_t *_list);

/**
 * @brief : 菜单列表中响应按键操作。
 * 
 * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
 * @param  {menu_keyOp_t*} _op  : 按键操作指针。按键响应后会被清除为空操作。
 */
void MENU_ListKeyOp(menu_list_t *_list, menu_keyOp_t* _op);


/**
 * menu_nvm
 */

//gl = global
//rg = region
//addr = address
//sect = sector
/**
 * @brief : 每个扇区包含的字节数
 */
uint32_t menu_nvm_sectorSize = 4096u;
/**
 * @brief : 全局存储 Global Storage
 */
uint32_t menu_nvm_glSectCnt = 2u;					/// 全局存储区占用的扇区数
uint32_t menu_nvm_glSectOffset = 2u; 				/// 全局存储区扇区偏移
uint32_t menu_nvm_glAddrOffset = menu_nvm_glSectOffset * menu_nvm_sectorSize;	/// 全局存储区地址偏移
/**
 * @brief : 局部存储 Region Storage
 */
uint32_t menu_nvm_rgCnt = 3u						/// 局部存储区的数量
uint32_t menu_nvm_rgSectCnt = 4u;					/// 每个局部存储区占用的扇区数
uint32_t menu_nvm_rgSectOffset[menu_nvm_rgCnt] = 	/// 三个局部存储区的扇区偏移
{
	menu_nvm_glSectOffset + menu_nvm_glSectCnt + 0u * menu_nvm_rgSectCnt,
	menu_nvm_glSectOffset + menu_nvm_glSectCnt + 1u * menu_nvm_rgSectCnt,
	menu_nvm_glSectOffset + menu_nvm_glSectCnt + 2u * menu_nvm_rgSectCnt,
};
uint32_t menu_nvm_rgAddrOffset[menu_nvm_rgCnt] = 	/// 三个局部存储区的地址偏移
{
	menu_nvm_rgSectOffset[0] * menu_nvm_sectorSize,
	menu_nvm_rgSectOffset[1] * menu_nvm_sectorSize,
	menu_nvm_rgSectOffset[2] * menu_nvm_sectorSize,
};
/**
 * @brief : 菜单存储占用的总扇区数
 */
uint32_t menu_nvm_totalSectCnt = menu_nvm_globalSectorCnt + menu_nvm_regionCnt * menu_nvm_regionSectorCnt;
/**
 * @brief : 每个菜单项保存时占用的字节数
 */
uint32_t menu_nvm_dataSize = 32u;

/**
 * @brief : 菜单项写入缓存。
 * 当改写第N个扇区时，menu_nvm_cachedSector = N, menu_nvm_cache分配4KB缓存
 * 并读入第N扇区的所有内容。此时能且仅能修改第N扇区的内容。对第N扇区内容的修改
 * 将缓存至上述内存。
 */
uint8_t *menu_nvm_cache;
uint32_t menu_nvm_cachedSector;



/**
 * @brief : 从NVM中读取数据。
 * 
 * @param  {uint32_t} _addr    : 要读取的NVM地址。
 * @param  {void*} _buf        : 用于保存数据的缓冲区。
 * @param  {uint32_t} _byteCnt : 要读取的字节数。
 * 
 * @return {status_t}          : 返回运行结果。正常返回kStatus_Success。
 */
status_t MENU_NvmRead(uint32_t _addr, void *_buf, uint32_t _byteCnt);

/**
 * @brief : 检查地址是否在可缓存范围内。
 * 如果当前未缓存任何扇区（menu_nvm_cache == NULL），始终返回true。如果当前已有缓存扇区，则根据保存的
 * 扇区号判断输入地址是否在缓存范围内。
 * 
 * @param  {uint32_t} _addr : 要检查的地址
 * @return {bool}           : 可缓存返回true，不可缓存返回false。
 */
bool MENU_NvmCacheable(uint32_t _addr);	//check if addr is ready to write.

/**
 * @brief : 将数据写入缓存。
 * 该函数会检查输入地址及数据大小是否在可缓存范围内。
 * 如果当前未缓存任何扇区，将创建缓存。
 * @param  {uint32_t} _addr    : 
 * @param  {void*} _buf        : 
 * @param  {uint32_t} _byteCnt : 
 * @return {status_t}          : 成功写入返回kStatus_Success，失败返回kStatus_Fail。
 */
status_t MENU_NvmWriteCache(uint32_t _addr, void *_buf, uint32_t _byteCnt);

/**
 * @brief : 将缓存的修改写回NVM。
 * 该函数先擦书缓存的扇区，然后将缓存的内容写回该扇区，最后释放缓存区内存，将缓存区指针赋为NULL。
 * 缓存的扇区号将保留原值。
 * 
 * @return {status_t}       : 成功写回返回kStatus_Success。
 */
status_t MENU_NvmUpdateCache(void);







/**
 * 菜单定义
 */

/**
 * @brief 菜单有效标志。
 * 菜单状态标志的最高8位用于表示菜单数据是否有效，正常值为0x55。24是需要左移的位数。
 * 如果修改菜单时改变了有效菜单项的存储地址，则应在下载程序前队flash进行擦除
 * 这样该标志会变为0xff，此时单片机复位后菜单将不再从flash读取数据，转而采用
 * 程序中初始化的数据。
 * 标志失效时，手动保存数据将会在所有存储区写入当前数据，并重设该标志。
 */
uint8_t menu_dataValid_flag = 0x55;
uint32_t menu_dataValid_mask = 24u;

/**
 * @brief 菜单状态标志位枚举变量。
 */
enum menu_status_t
{
	menu_data_valid = menu_dataValid_flag << menu_dataValid_mask,	/// 菜单状态标志

	menu_error_fatalError = 1 << 23,								/// 关键故障标志位。
	menu_warning_itemLost = 1 << 22,								/// 数据缺失标志位。读取数据时发现数据缺失时置位，须手动清除。该标志位使用16位参数，表示数据缺失的总个数。
	menu_message_cmbPrint = 1 << 21,								/// cm_backtrace错误打印标志位。发生cmb打印时设置，须手动清除。该标志位（目前）不使用参数。
	menu_message_usrPrint = 1 << 20,								/// 用户错误信息打印标志位，发生用户错误信息打印时设置，须手动清除。该标志位的参数由用户定义。
	menu_noUse4 = 1 << 19,													
	menu_noUse5 = 1 << 18,
	menu_noUse6 = 1 << 17,
	menu_noUse7 = 1 << 16,

	menu_param_Mask16 = 0xffff;										/// 低16位全16位掩码标志位，用于读取低16位参数。如果传一个参数且取值大于255，应使用16位参数。
	menu_param_Mask8h = 0xff00;										/// 低16位中高8位掩码标志位，用于读取16位参数中的高8位。如果要返回两个参数，或参数取值小于256，可以使用两个8位参数。
	menu_param_Mask8l = 0x00ff;										/// 低16位中高8位掩码标志位，用于读取16位参数中的低8位。同上。
};

/**
 * @brief 菜单控制结构体。
 */
struct menu_t
{
	menu_list_t *currList;
	menu_itemIfce_t *currItem;
	int32_t currRegionNum;
	int32_t statusFlag; 
};

menu_t menuInst;


/**
 * @brief : 菜单全局初始化。
 * 该函数会初始化必要的变量，创建根菜单和其他最基本的菜单列表和菜单项。
 * 
 * @param  {void} undefined : 
 */
void MENU_Init(void);

/**
 * @brief : 初始化各菜单列表和菜单项。
 * 注意：该函数在外部定义，不需初始化最基本的菜单列表和菜单项。该函数需要用户手动调用。
 * TODO: 究竟手动调用还是自动调用？或者干脆不定义这个函数，让用户自己去写？
 * 
 * @param  {void} undefined : 
 */
void MENU_DataSetUp(void);

/**
 * @brief : 打印屏幕。
 * 该函数是整个菜单的屏幕打印入口，将自动根据菜单的状态打印屏幕。
 * 
 * @param  {void} undefined : 
 */
void MENU_PrintDisp(void);

/**
 * @brief : 响应按键操作。
 * 该函数是整个菜单的按键响应入口，将自动根据菜单的状态调用按键响应函数。
 * 该函数不会更新屏幕显示。
 * 
 * @param  {void} undefined : 
 */
void MENU_KeyOp(void);

/**
 * @brief : 保存整个菜单到NVM。
 * 
 * @param  {int32_t} _region : 
 */
void MENU_Data_NvmSave(int32_t _region);

/**
 * @brief : 从NVM读取整个菜单。
 * 
 * @param  {int32_t} _region : 所选择的局部存储区。
 */
void MENU_Data_NvmRead(int32_t _region);

/**
 * @brief : 将一个局部存储区的数据拷贝到另一个局部存储区。
 * 
 * @param  {int32_t} _srcRegion : 源存储序号。
 * @param  {int32_t} _dstRegion : 目的存储区序号。
 */
void MENU_Data_NvmCopy(int32_t _srcRegion, int32_t _dstRegion);

/**
 * @brief : 读取NVM状态标志。
 * 
 * @return {int32_t}        : 返回读取到的状态标志。
 */
int32_t MENU_GetNvmStatus(void);

/**
 * @brief : 设置NVM状态标志。
 * 
 * @param  {int32_t} _status : 要设置的状态标志。
 */
void MENU_SetNvmStatus(int32_t _status);



#ifndef __cplusplus
}
#endif


#endif // ! _APP_MENU_H_