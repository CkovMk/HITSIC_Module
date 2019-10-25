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
 * @brief : 
 * 
 * @param  {menu_itemType_t} _type : 
 * @param  {void*} _data           : 
 * @param  {char*} _nameStr        : 
 * @param  {uint32_t} _saveAddr    : 
 * @param  {uint32_t} _pptFlag     : 
 * 
 * @return {menu_itemIfce_t *}     :
 */
menu_itemIfce_t *MENU_itemConstruct(menu_itemType_t _type, void *_data, const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag);


/**
 * @brief : 
 * 
 * @param  {menu_itemIfce_t*} _item : 
 */
void MENU_itemDestruct(menu_itemIfce_t *_item);



/**
 * @brief : 
 * 
 * @param  {menu_itemIfce_t*} _item : 
 * 
 * @return {void *}					:
 */
void *MENU_ItemGetData(menu_itemIfce_t *_item);


/**
 * @brief : 
 * 
 * @param  {menu_itemIfce_t*} _item : 
 * @param  {void*} _data            : 
 */
void MENU_ItemSetData(menu_itemIfce_t *_item, void *_data);


//used when in menuList


/**
 * @brief : 
 * 
 * @param  {menu_itemIfce_t*} _item : 
 * @param  {uint32_t} _slotNum      : 
 */
void MENU_ItemPrintSlot(menu_itemIfce_t *_item, uint32_t _slotNum);


/**
 * @brief : 
 * 
 * @param  {menu_itemIfce_t*} _item : 
 * @param  {menu_keyOp_t*} _op      : 
 */
void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *_op);


//used when in menuItem

/**
 * @brief : 
 * 
 * @param  {menu_itemIfce_t*} _item : 
 */
void MENU_ItemPrintDisp(menu_itemIfce_t *_item);

/**
 * @brief : 
 * 
 * @param  {menu_itemIfce_t*} _item : 
 * @param  {menu_keyOp_t*} _op      : 
 */
void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *_op);







struct menu_list_t
{
	menu_itemIfce_t *menu;
	uint32_t listSize, listNum;
	uint32_t disp_p, slct_p;
	char nameStr[menu_nameStrSize];
	menu_list_t *prev;
};


/**
 * @brief : 
 * 
 * @param  {menu_list_t*} _list   : 
 * @param  {const char*} _nameStr : 
 * @param  {uint32_t} _listSize   : 
 * @param  {menu_list_t*} _prev   : 
 */
void MENU_ListConstruct(menu_list_t *_list, const char *_nameStr, uint32_t _listSize , menu_list_t* _prev);

/**
 * @brief : 
 * 
 * @param  {menu_list_t*} _list : 
 */
void MENU_ListDestruct(menu_list_t *_list);

/**
 * 
 * @param  {menu_list_t*} _list     : 
 * @param  {menu_itemIfce_t*} _item : 
 * @return {status_t}               : 
 */
status_t MENU_ListInsert(menu_list_t *_list, menu_itemIfce_t *_item);

/**
 * @brief : 
 * 
 * @param  {menu_list_t*} _list : 
 */
void MENU_ListPrintDisp(menu_list_t *_list);

/**
 * @brief : 
 * 
 * @param  {menu_list_t*} _list    : 
 * @param  {appVar_keyBTOp_t*} _op : 
 */
void MENU_ListKeyOp(menu_list_t *_list, appVar_keyBTOp_t* _op);


/**
 * menu_nvm
 */

//gl = global
//rg = region
//addr = address
//sect = sector
uint32_t menu_nvm_sectorSize = 4096u;
//global storage
uint32_t menu_nvm_glSectCnt = 2u;
uint32_t menu_nvm_glSectOffset = 2u; 
uint32_t menu_nvm_glAddrOffset = menu_nvm_glSectOffset * menu_nvm_sectorSize;
//region storage
uint32_t menu_nvm_rgCnt = 3u, menu_nvm_rgSectCnt = 4u;
uint32_t menu_nvm_rgSectOffset[menu_nvm_rgCnt] = 
{
	menu_nvm_glSectOffset + menu_nvm_glSectCnt + 0u * menu_nvm_rgSectCnt,
	menu_nvm_glSectOffset + menu_nvm_glSectCnt + 1u * menu_nvm_rgSectCnt,
	menu_nvm_glSectOffset + menu_nvm_glSectCnt + 2u * menu_nvm_rgSectCnt,
};
uint32_t menu_nvm_rgAddrOffset[menu_nvm_rgCnt] = 
{
	menu_nvm_rgSectOffset[0] * menu_nvm_sectorSize,
	menu_nvm_rgSectOffset[1] * menu_nvm_sectorSize,
	menu_nvm_rgSectOffset[2] * menu_nvm_sectorSize,
};

uint32_t menu_nvm_totalSectorCnt = menu_nvm_globalSectorCnt + menu_nvm_regionCnt * menu_nvm_regionSectorCnt;

uint32_t menu_nvm_dataSize = 32u;

uint8_t *menu_nvm_cache;
uint32_t menu_nvm_cachedSector;



/**
 * @brief : 
 * 
 * @param  {uint32_t} _addr    : 
 * @param  {void*} _buf        : 
 * @param  {uint32_t} _byteCnt : 
 * @return {status_t}          : 
 */
status_t MENU_NvmRead(uint32_t _addr, void *_buf, uint32_t _byteCnt);

/**
 * @brief : 
 * 
 * @param  {uint32_t} _addr : 
 * @return {bool}           : 
 */
bool MENU_NvmCacheable(uint32_t _addr);	//check if addr is ready to write.

/**
 * @brief : 
 * 
 * @param  {uint32_t} _addr    : 
 * @param  {void*} _buf        : 
 * @param  {uint32_t} _byteCnt : 
 * @return {status_t}          : 
 */
status_t MENU_NvmWriteCache(uint32_t _addr, void *_buf, uint32_t _byteCnt);

/**
 * @brief : 
 * 
 * @param  {void} undefined : 
 * @return {status_t}       : 
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
 * @brief : 
 * 
 * @param  {void} undefined : 
 */
void MENU_Init(void);

/**
 * @brief : 
 * 
 * @param  {void} undefined : 
 */
void MENU_DataSetUp(void);

/**
 * @brief : 
 * 
 * @param  {void} undefined : 
 */
void MENU_PrintDisp(void);

/**
 * @brief : 
 * 
 * @param  {void} undefined : 
 */
void MENU_KeyOp(void);

/**
 * @brief : 
 * 
 * @param  {int32_t} _region : 
 */
void MENU_Data_NvmSave(int32_t _region);

/**
 * @brief : 
 * 
 * @param  {int32_t} _region : 
 */
void MENU_Data_NvmRead(int32_t _region);

/**
 * @brief : 
 * 
 * @param  {int32_t} _srcRegion : 
 * @param  {int32_t} _dstRegion : 
 */
void MENU_Data_NvmCopy(int32_t _srcRegion, int32_t _dstRegion);

/**
 * @brief : 
 * 
 * @param  {void} undefined : 
 * @return {int32_t}        : 
 */
int32_t MENU_GetNvmStatus(void);

/**
 * @brief : 
 * 
 * @param  {int32_t} _status : 
 */
void MENU_SetNvmStatus(int32_t _status);



#ifndef __cplusplus
}
#endif


#endif // ! _APP_MENU_H_