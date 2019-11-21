#ifndef _APP_MENU_DEF_H_
#define _APP_MENU_DEF_H_
#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"

#include "app_menu_port.h"
#include "drv_button.h"
#include "drv_disp_ssd1306.h"
#include "drv_ftfx_flash.h"
#include "lib_list.h"



#ifdef __cplusplus
extern "C"
{
#endif

/**
	 * @brief : 菜单项和菜单列表名称的最大长度为16个字符。用于定义缓存区大小。
	 */
#define menu_nameStrSize (16u)


/** string buffer size defnition */
#define MENU_DISP_STRBUF_ROW (8u)
#define MENU_DISP_STRBUF_COL (22u)

/** format controll used by data print */
#define MENU_DISP_NAME_COL (1u)
#define MENU_DISP_NAME_LEN (12u)
#define MENU_DISP_DATA_COL (14u)
#define MENU_DISP_DATA_LEN (7u)
//TODO: deprecate this

/**
	 * @brief : 菜单项属性枚举类型。
	 * 标志位枚举。
	 */
typedef enum
{
    /** data config */
    menuItem_data_global = 1 << 0, ///< 该菜单项存储在全局数据区。
    menuItem_data_region = 1 << 1, ///< 该菜单项存储在局部数据区。
    menuItem_data_getPos = menuItem_data_global | menuItem_data_region,
    menuItem_data_ROFlag = 1 << 2, ///< 该菜单项为只读。只读菜单项不允许在菜单内修改。
    menuItem_data_NoSave = 1 << 3, ///< 该菜单项默认不保存到NVM。
    menuItem_data_getCfg = menuItem_data_global | menuItem_data_region | menuItem_data_ROFlag/* | menuItem_data_prioRW*/,

    /** display config */
    menuItem_disp_forceSci = 1 << 8,  ///< 该菜单项强制使用科学计数法，适用于variType和varfType。
    //menuItem_disp_bitFlag = 1 << 9,   ///< 该菜单项为按位标志位，仅适用于variType。此时数据指针将被视为uint32_t*。
    menuItem_disp_noPreview = 1 << 10 ///< 该菜单项不会在菜单列表中显示数据。数据区将显示占位字符。注意此选项对标记为按位标志位的variType无效，因为这类菜单项从不在菜单列表显示数据。

    /** error mask */
} menu_itemPropety_t;

/**
	  * @brief : 菜单项所支持的内容类型。
	  */
typedef enum
{
    nullType,
    variType, //watch or set integer varibles
    varfType, //watch or set float-point varibles
    boolType,
    procType, //run certain process
    menuType, //jump to another menu
} menu_itemType_t;

/**
	 * @brief : 函数类型菜单项的响应句柄。
	 * 函数类型菜单项激活时会调用一个没有参数和返回值的函数。
	 */
typedef void (*menu_itemProcHandler_t)(void);

/** 菜单列表结构体 前置定义 */
typedef struct _menu_list_t menu_list_t;

/******************************************
 * **********菜单项类型及变量声明 **********
 ******************************************/

/** @brief : 占位类型菜单项句柄。 */
typedef struct
{
    void *data;
} menu_item_nullHandle_t;

/** @brief : 整数类型菜单项句柄。 */
typedef struct
{
    int32_t *data;
    int32_t bData;
    int32_t v,e;
    int32_t cur;
} menu_item_variHandle_t;

/** @brief : 浮点类型菜单项句柄。 */
typedef struct
{
    float *data;
    float bData;
    int32_t v,e;
    int32_t cur;
} menu_item_varfHandle_t;

/** @brief : 布尔类型菜单项句柄。 */
typedef struct
{
    bool *data;
} menu_item_boolHandle_t;

/** @brief : 函数类型菜单项句柄。 */
typedef struct
{
    menu_itemProcHandler_t data;
} menu_item_procHandle_t;

/** @brief : 跳转类型菜单项句柄。 */
typedef struct
{
    menu_list_t *data;
    //void *data; //TODO: fix this.
} menu_item_menuHandle_t;

/** @brief : 菜单项接口结构体。 */
typedef struct _menu_itemIfce_t
{
    menu_itemType_t type; /// 此菜单项的类型。
    //menu_list_t *myList;			/// 此菜单项所属的菜单列表（暂时没用）。
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

/**
	 * @brief : 菜单项和菜单列表的计数器。
	 */
extern uint32_t menu_itemCnt;
extern uint32_t menu_listCnt;

extern menu_list_t *menu_currList;
extern menu_itemIfce_t *menu_currItem;
extern menu_list_t *menu_menuRoot;
extern int32_t menu_currRegionNum;
extern int32_t menu_statusFlag;
extern uint32_t menu_nvm_statusFlagAddr;

extern char menu_dispStrBuf[MENU_DISP_STRBUF_ROW][MENU_DISP_STRBUF_COL];

/*************************************
 ************ 菜单列表结构体 **********
 *************************************/
typedef struct _menu_list_t
{
    menu_itemIfce_t **menu;         /// 菜单项指针的动态数组，用于存放指针。该数组内的指针析构时需要手动释放。
    uint32_t listSize, listNum;     /// 当前菜单项指针列表的大小、当前列表内的菜单项数量。
    uint32_t disp_p, slct_p;        /// 显示数组下标和选择数组下标。
    char nameStr[menu_nameStrSize]; /// 菜单列表名称字符串。
                                    //_menu_list_t *prev;		    /// 指向上级菜单的指针。根菜单中该指针指向自身。
} menu_list_t;

/**
 * MENU
 */

/**
	  * @brief 菜单有效标志。
	  * 菜单状态标志的最高8位用于表示菜单数据是否有效，正常值为0x55。24是需要左移的位数。
	  * 如果修改菜单时改变了有效菜单项的存储地址，则应在下载程序前队flash进行擦除
	  * 这样该标志会变为0xff，此时单片机复位后菜单将不再从flash读取数据，转而采用
	  * 程序中初始化的数据。
	  * 标志失效时，手动保存数据将会在所有存储区写入当前数据，并重设该标志。
	  */
#define menu_dataValid_flag (0x55)
#define menu_dataValid_mask (24u)

/**
	 * @brief 菜单状态标志位枚举变量。
	 */
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

    /**
	 * @brief 菜单控制结构体。deprecated
	 */
//typedef struct
//{
//	menu_list_t* menu_currList;
//	menu_itemIfce_t* menu_currItem;
//	menu_list_t* menu_menuRoot;
//	int32_t menu_currRegionNum;
//	int32_t menu_statusFlag;
//}menu_t;

//menu_t menuInst;

extern menu_list_t *menu_currList;
extern menu_itemIfce_t *menu_currItem;
extern menu_list_t *menu_menuRoot;
extern int32_t menu_currRegionNum;
extern int32_t menu_statusFlag;
extern uint32_t menu_nvm_statusFlagAddr;
extern int32_t menu_nvmCopySrc, menu_nvmCopyDst;

extern char menu_dispStrBuf[MENU_DISP_STRBUF_ROW][MENU_DISP_STRBUF_COL];

/**
 * BUTTON
 */

/**
	 * @brief : 按键操作宏定义。
	 *
	 */
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





typedef struct _menu_nvmData_t
{
    uint32_t head;
    char nameStr[menu_nameStrSize];
    uint32_t type;
    uint32_t data;
    uint32_t tail;
}menu_nvmData_t;


#ifdef __cplusplus
}
#endif



#endif // ! _APP_MENU_DEF_H_




