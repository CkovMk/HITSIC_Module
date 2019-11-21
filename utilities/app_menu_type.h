#ifndef _APP_MENU_TYPE_H_
#define _APP_MENU_TYPE_H_

#include "app_menu_def.h"
#include "app_menu_button.h"

/**
 * @brief : 菜单项类别函数命名宏定义
 */
#define MENU_ITEM_MAKE_FUNCTION(funcName, type) (funcName##_##type)
#define MENU_ITEM_MAKE_HANDLE(p_Item, type) ((p_Item->handle).p_##type##)
#define MENU_ITEM_CALL_FUNCTION(funcName, type, p_Item, ...) (MENU_ITEM_MAKE_FUNCTION(funcName, type)(p_Item, ##__VA_ARGS__))


/**
 * @brief : 菜单项分类调用switch-case语句的宏定义。
 * 使用此语句简化menu_itemIfce操作函数内的switch-case调用。
 *
 * @param funcName 	: 要分类切换的函数名称。
 * @param p_Item 	: 指向menu_itemIfce_t结构体的指针。
 * @param ... 		: 除上述指针以外的其他参数。没有其他参数则不写。
 *
 * @example :
 *
 * 		void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t * const _op)
 * 		{
 * 			MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, _item, _op);
 * 		}
 */
#define MENU_ITEM_SWITCH_CASE(funcName, p_Item, ...)                        \
    switch (p_Item->type)                                                   \
    {                                                                       \
    case nullType:                                                          \
        MENU_ITEM_CALL_FUNCTION(funcName, nullType, p_Item, ##__VA_ARGS__); \
        break;                                                              \
    case variType:                                                          \
        MENU_ITEM_CALL_FUNCTION(funcName, variType, p_Item, ##__VA_ARGS__); \
        break;                                                              \
    case varfType:                                                          \
        MENU_ITEM_CALL_FUNCTION(funcName, varfType, p_Item, ##__VA_ARGS__); \
        break;                                                              \
    case boolType:                                                          \
        MENU_ITEM_CALL_FUNCTION(funcName, boolType, p_Item, ##__VA_ARGS__); \
        break;                                                              \
    case procType:                                                          \
        MENU_ITEM_CALL_FUNCTION(funcName, procType, p_Item, ##__VA_ARGS__); \
        break;                                                              \
    case menuType:                                                          \
        MENU_ITEM_CALL_FUNCTION(funcName, menuType, p_Item, ##__VA_ARGS__); \
        break;                                                              \
    default:                                                                \
        break;                                                              \
    }

//test marco :
//MENU_ITEM_MAKE_FUNCTION(item, nullType, KeyOp);
//MENU_ITEM_CALL_FUNCTION(MENU_ItemConstruct, item, variType, &data);
//MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, item, &op);

#ifdef __cplusplus
extern "C"
{
#endif






    /** @brief : 占位类型菜单项操作函数。 */
    void MENU_ItemConstruct_nullType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemGetData_nullType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemSetData_nullType(menu_itemIfce_t *_item, void *_data);
    //used when in menuList
    void MENU_ItemPrintSlot_nullType(menu_itemIfce_t *_item, uint32_t _slotNum);
    void MENU_ItemDirectKeyOp_nullType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
    //used when in menuItem
    void MENU_ItemPrintDisp_nullType(menu_itemIfce_t *_item);
    void MENU_ItemKeyOp_nullType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);

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

    /** @brief : 浮点类型菜单项操作函数。 */
    void MENU_ItemConstruct_varfType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemGetData_varfType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemSetData_varfType(menu_itemIfce_t *_item, void *_data);
    //used when in menuList
    void MENU_ItemPrintSlot_varfType(menu_itemIfce_t *_item, uint32_t _slotNum);
    void MENU_ItemDirectKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
    //used when in menuItem
    void MENU_ItemPrintDisp_varfType(menu_itemIfce_t *_item);
    void MENU_ItemKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);

    /** @brief : 布尔类型菜单项操作函数。 */
    void MENU_ItemConstruct_boolType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemGetData_boolType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemSetData_boolType(menu_itemIfce_t *_item, void *_data);
    //used when in menuList
    void MENU_ItemPrintSlot_boolType(menu_itemIfce_t *_item, uint32_t _slotNum);
    void MENU_ItemDirectKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
    //used when in menuItem
    void MENU_ItemPrintDisp_boolType(menu_itemIfce_t *_item);
    void MENU_ItemKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);

    /** @brief : 函数类型菜单项操作函数。 */
    void MENU_ItemConstruct_procType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemGetData_procType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemSetData_procType(menu_itemIfce_t *_item, void *_data);
    //used when in menuList
    void MENU_ItemPrintSlot_procType(menu_itemIfce_t *_item, uint32_t _slotNum);
    void MENU_ItemDirectKeyOp_procType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
    //used when in menuItem
    void MENU_ItemPrintDisp_procType(menu_itemIfce_t *_item);
    void MENU_ItemKeyOp_procType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);

    /** @brief : 跳转类型菜单项操作函数。 */
    void MENU_ItemConstruct_menuType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemGetData_menuType(menu_itemIfce_t *_item, void *_data);
    void MENU_ItemSetData_menuType(menu_itemIfce_t *_item, void *_data);
    //used when in menuList
    void MENU_ItemPrintSlot_menuType(menu_itemIfce_t *_item, uint32_t _slotNum);
    void MENU_ItemDirectKeyOp_menuType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
    //used when in menuItem
    void MENU_ItemPrintDisp_menuType(menu_itemIfce_t *_item);
    void MENU_ItemKeyOp_menuType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);

    /*************************************
 ************ 菜单项操作接口 **********
 *************************************/

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
    menu_itemIfce_t *MENU_ItemConstruct(menu_itemType_t _type, void *_data, const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag);

    /**
	 * @brief : 菜单项结构体析构函数。
	 *
	 * @param  {menu_itemIfce_t*} _item : 要析构的菜单项结构体指针。析构后该指针会被自动赋值为NULL。
	 */
    void MENU_itemDestruct(menu_itemIfce_t *_item);

    /**
	 * @brief : 获取菜单项内存放的数据指针。
	 * TODO : 更新注释
	 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
	 *
	 * @return {void *}					: 返回数据指针。注意，无论何种类型，都会返回对应的数据指针。
	 */
    void MENU_ItemGetData(menu_itemIfce_t *_item, menu_nvmData_t *_data);

    /**
	 * @brief : 设置菜单项内指针指向的数据的值。
	 * 注意，该函数对非数据类型菜单和只读数据菜单无效。
	 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
	 * @param  {void*} _data            :
	 */
    void MENU_ItemSetData(menu_itemIfce_t *_item, menu_nvmData_t *_data);

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
    void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t * const _op);

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
    void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t * const _op);



    /**************************************
 ************ 菜单列表操作接口 **********
 **************************************/

    /**
	  * @brief : 菜单列表的构造函数。
	  *
	  * @param  {const char*} _nameStr :
	  * @param  {uint32_t}    _size	   :
	  * @param  {menu_list_t*} _prev   :
	  *
	  * @return {menu_list_t *}        : 返回构造的菜单列表结构体指针。
	  */
    menu_list_t *MENU_ListConstruct(const char *_nameStr, uint32_t _size, menu_list_t *_prev);

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
    void MENU_ListKeyOp(menu_list_t *_list, menu_keyOp_t * const _op);

#ifdef __cplusplus
}
#endif

#endif // ! _APP_MENU_TYPE_H_
