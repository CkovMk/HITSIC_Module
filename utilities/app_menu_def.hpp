#ifndef _APP_MENU_DEF_H_
#define _APP_MENU_DEF_H_
#include "inc_stdlib.hpp"
#include "hitsic_common.h"

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

/*!
 * @addtogroup menu_def
 * @{
 */

#include "app_menu_port.hpp"
#include "drv_disp_ssd1306.hpp"
#include "lib_list.h"
#include "sys_pitmgr.hpp"

#define MENU_LOG_A(...)                                                        \
    (HITSIC_MENU_PRINTF("[A] MENU: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) +      \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LOG_E(...)                                                        \
    (HITSIC_MENU_PRINTF("[E] MENU: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) +      \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LOG_W(...)                                                        \
    (HITSIC_MENU_PRINTF("[W] MENU: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) +      \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LOG_I(...)                                                        \
    (HITSIC_MENU_PRINTF("[I] MENU: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) +      \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LOG_D(...)                                                        \
    (HITSIC_MENU_PRINTF("[D] MENU: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) +      \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LOG_V(...)                                                        \
    (HITSIC_MENU_PRINTF("[V] MENU: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) +      \
     HITSIC_MENU_PRINTF("\n"))

#define MENU_NVM_LOG_A(...)                                                    \
    (HITSIC_MENU_PRINTF("[A] MENU.NVM : ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_NVM_LOG_E(...)                                                    \
    (HITSIC_MENU_PRINTF("[E] MENU.NVM : ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_NVM_LOG_W(...)                                                    \
    (HITSIC_MENU_PRINTF("[W] MENU.NVM : ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_NVM_LOG_I(...)                                                    \
    (HITSIC_MENU_PRINTF("[I] MENU.NVM : ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_NVM_LOG_D(...)                                                    \
    (HITSIC_MENU_PRINTF("[D] MENU.NVM : ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_NVM_LOG_V(...)                                                    \
    (HITSIC_MENU_PRINTF("[V] MENU.NVM : ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))

#define MENU_ITEM_LOG_A(...)                                                   \
    (HITSIC_MENU_PRINTF("[A] MENU.ITEM: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_ITEM_LOG_E(...)                                                   \
    (HITSIC_MENU_PRINTF("[E] MENU.ITEM: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_ITEM_LOG_W(...)                                                   \
    (HITSIC_MENU_PRINTF("[W] MENU.ITEM: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_ITEM_LOG_I(...)                                                   \
    (HITSIC_MENU_PRINTF("[I] MENU.ITEM: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_ITEM_LOG_D(...)                                                   \
    (HITSIC_MENU_PRINTF("[D] MENU.ITEM: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_ITEM_LOG_V(...)                                                   \
    (HITSIC_MENU_PRINTF("[V] MENU.ITEM: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))

#define MENU_LIST_LOG_A(...)                                                   \
    (HITSIC_MENU_PRINTF("[A] MENU.LIST: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LIST_LOG_E(...)                                                   \
    (HITSIC_MENU_PRINTF("[E] MENU.LIST: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LIST_LOG_W(...)                                                   \
    (HITSIC_MENU_PRINTF("[W] MENU.LIST: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LIST_LOG_I(...)                                                   \
    (HITSIC_MENU_PRINTF("[I] MENU.LIST: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LIST_LOG_D(...)                                                   \
    (HITSIC_MENU_PRINTF("[D] MENU.LIST: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_LIST_LOG_V(...)                                                   \
    (HITSIC_MENU_PRINTF("[V] MENU.LIST: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))

#define MENU_BUTTON_LOG_A(...)                                                 \
    (HITSIC_MENU_PRINTF("[A] MENU.BUTN: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_BUTTON_LOG_E(...)                                                 \
    (HITSIC_MENU_PRINTF("[E] MENU.BUTN: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_BUTTON_LOG_W(...)                                                 \
    (HITSIC_MENU_PRINTF("[W] MENU.BUTN: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_BUTTON_LOG_I(...)                                                 \
    (HITSIC_MENU_PRINTF("[I] MENU.BUTN: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_BUTTON_LOG_D(...)                                                 \
    (HITSIC_MENU_PRINTF("[D] MENU.BUTN: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))
#define MENU_BUTTON_LOG_V(...)                                                 \
    (HITSIC_MENU_PRINTF("[V] MENU.BUTN: ") + HITSIC_MENU_PRINTF(__VA_ARGS__) + \
     HITSIC_MENU_PRINTF("\n"))

#ifdef __cplusplus
extern "C"
{
#endif

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
    menuOpType_shrt = 1 << 8, menuOpType_long = 2 << 8, menuOpType_lrpt = 3 << 8,
} menu_keyOpType_t;

typedef uint32_t menu_keyOp_t;

/**
 * @brief : 按键操作生成宏
 */
#define MENU_BUTTON_MAKE_OP(code, type) (menuOpCode_##code | menuOpType_##type)

extern menu_keyOp_t menu_keyOpBuff;

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
// TODO: deprecate this

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
    menuItem_data_NoLoad = 1 << 4, ///< 该菜单项默认不从NVM读取。
    menuItem_data_getCfg = menuItem_data_global | menuItem_data_region
            | menuItem_data_ROFlag | menuItem_data_NoSave
            | menuItem_data_NoLoad,
    menuItem_dataExt_HasMinMax = 1 << 5, ///< 数据属性扩展：最小/最大值限制。该属性使用data指针偏移+1和+2的地址。

    /** display config */
    menuItem_disp_forceSci = 1 << 8, ///< 该菜单项强制使用科学计数法，适用于variType和varfType。
    menuItem_disp_noPreview = 1 << 10, ///< 该菜单项不会在菜单列表中显示数据。数据区将显示占位字符。注意此选项对标记为按位标志位的variType无效，因为这类菜单项从不在菜单列表显示数据。

    menuItem_proc_runOnce = 1 << 11, ///< 该菜单项只会运行一次。仅适用于procType。
    menuItem_proc_uiDisplay = 1 << 12, ///< 该菜单项会自行打印屏幕。仅适用于procType。

    /** error mask */
} menu_itemPropety_t;

/**
 * @brief : 菜单项所支持的内容类型。
 */
typedef enum
{
    nullType, // null type
    variType, // watch or set integer varibles
    varfType, // watch or set float-point varibles
    boolType,
    procType, // run certain process
    menuType, // jump to another menu
} menu_itemType_t;



/**
 * @brief : 函数类型菜单项的响应句柄。
 * 函数类型菜单项激活时会调用一个没有参数和返回值的函数。
 */
typedef void (*menu_itemProcHandler_t)(menu_keyOp_t *const _op);

/** 菜单列表结构体 前置定义 */
typedef struct _menu_list_t menu_list_t;

struct menu_itemAdapter_t;

/******************************************
 ************菜单项类型及变量声明 **********
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
    int32_t v, e;
    int32_t cur;
} menu_item_variHandle_t;

/** @brief : 浮点类型菜单项句柄。 */
typedef struct
{
    float *data;
    float bData;
    int32_t v, e;
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
} menu_item_menuHandle_t;

/** @brief : 菜单项接口结构体。 */
typedef struct _menu_itemIfce_t
{
    menu_itemType_t type;               ///< 此菜单项的类型。
    uint32_t pptFlag;                   ///< 此菜单项的属性标志位。
    uint32_t list_id, unique_id;        ///< 此菜单项在本列表内的序号（从0开始）、全局唯一序号（从0开始）
    uint32_t saveAddr;                  ///< 此菜单在本区域内的偏移地址。从0开始，以1步进。注意，全局数据区和局部数据区的地址分开来算。
    char nameStr[menu_nameStrSize];     ///< 此菜单项的名称字符串。最大长度为menu_nameStrSize - 1 字节。
    union menu_itemIfce_handle_t        ///< 菜单项操作句柄的共用体。使用时根据此菜单项的类型调取对应项访问。
    {
        void *p_void;
        menu_item_nullHandle_t *p_nullType;
        menu_item_variHandle_t *p_variType;
        menu_item_varfHandle_t *p_varfType;
        menu_item_boolHandle_t *p_boolType;
        menu_item_procHandle_t *p_procType;
        menu_item_menuHandle_t *p_menuType;
    } handle;
    const menu_itemAdapter_t* adapter;  ///< 指向存放菜单项命令函数指针的结构体。参考C++虚表
} menu_itemIfce_t;

struct menu_itemAdapter_t
{
    void (*ItemConstruct)(menu_itemIfce_t *_item, void *_data);
    void (*ItemGetData)(menu_itemIfce_t *_item, void *_data);
    void (*ItemSetData)(menu_itemIfce_t *_item, void *_data);
    void (*ItemPrintSlot)(menu_itemIfce_t *_item, uint32_t _slotNum);
    void (*ItemDirectKeyOp)(menu_itemIfce_t *_item, menu_keyOp_t *const _op);
    void (*ItemPrintDisp)(menu_itemIfce_t *_item);
    void (*ItemKeyOp)(menu_itemIfce_t *_item, menu_keyOp_t *const _op);
};

/**
 * @brief : 菜单项和菜单列表的计数器。
 * @ {
 */
extern uint32_t menu_itemCnt;               ///< 菜单项计数器
extern uint32_t menu_listCnt;               ///< 菜单列表计数器
/**
 * @ }
 */

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

/** 字符缓存 */
extern char menu_dispStrBuf[MENU_DISP_STRBUF_ROW][MENU_DISP_STRBUF_COL];

/*************************************
 ************ 菜单列表结构体 **********
 *************************************/
typedef struct _menu_list_t
{
    menu_itemIfce_t **menu; /// 菜单项指针的动态数组，用于存放指针。该数组内的指针析构时需要手动释放。
    uint32_t listSize, listNum; /// 当前菜单项指针列表的大小、当前列表内的菜单项数量。
    uint32_t disp_p, slct_p; /// 显示数组下标和选择数组下标。
    char nameStr[menu_nameStrSize]; /// 菜单列表名称字符串。
                                    //_menu_list_t *prev;		    ///
                                    //指向上级菜单的指针。根菜单中该指针指向自身。
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

    menu_error_fatalError = 1 << 23, ///> 关键故障标志位。
    menu_warning_itemLost = 1 << 22, ///>
                                     ///数据缺失标志位。读取数据时发现数据缺失时置位，须手动清除。该标志位使用16位参数，表示数据缺失的总个数。
    menu_datalog_cmbPrint = 1 << 21, ///>
                                     /// cm_backtrace错误打印标志位。发生cmb打印时设置，须手动清除。该标志位（目前）不使用参数。
    menu_datalog_usrPrint = 1 << 20, ///>
                                     ///用户错误信息打印标志位，发生用户错误信息打印时设置，须手动清除。该标志位的参数由用户定义。
    menu_message_buttonOp = 1 << 19,  ///> 按键操作消息
    menu_message_printDisp = 1 << 18, ///> 屏幕打印消息
    menu_noUse6 = 1 << 17,
    menu_noUse7 = 1 << 16,

    menu_param_Mask16 = 0xffff, /// 低16位全16位掩码标志位，用于读取低16位参数。如果传一个参数且取值大于255，应使用16位参数。
    menu_param_Mask8h = 0xff00, /// 低16位中高8位掩码标志位，用于读取16位参数中的高8位。如果要返回两个参数，或参数取值小于256，可以使用两个8位参数。
    menu_param_Mask8l = 0x00ff, /// 低16位中高8位掩码标志位，用于读取16位参数中的低8位。同上。
};

extern char menu_dispStrBuf[MENU_DISP_STRBUF_ROW][MENU_DISP_STRBUF_COL];

typedef struct _menu_nvmData_t
{
    uint32_t head;
    char nameStr[menu_nameStrSize];
    uint32_t type;
    uint32_t data;
    uint32_t tail;
} menu_nvmData_t;

#ifdef __cplusplus
}
#endif

/* @} */

#endif // ! HITSIC_USE_APP_MENU

#endif // ! _APP_MENU_DEF_H_
