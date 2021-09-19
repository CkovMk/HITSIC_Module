#ifndef UTILITIES_APP_MENU_DEF_HPP
#define UTILITIES_APP_MENU_DEF_HPP
#include <hitsic_common.h>

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

/*!
 * @addtogroup menu_def
 * @{
 */

#include <inc_stdlib.h>
#include <app_menu_port.h>
#include <sys_pitmgr.h>

/**
 * BUTTON
 */

/**
 * @brief : 按键操作宏定义。
 *
 */
typedef enum _menu_keyOpCode_t
{
    /* 五向按键，UP+DN+LF+RT+OK */
    menuOpCode_5wayStick_nl = 0, ///< NULL
    menuOpCode_5wayStick_ok = 1,
    menuOpCode_5wayStick_up = 2,
    menuOpCode_5wayStick_dn = 3,
    menuOpCode_5wayStick_lf = 4,
    menuOpCode_5wayStick_rt = 5,

    /* 三向按键，LF+RT+OK */
    menuOpCode_3wayStick_ok = 6,
    menuOpCode_3wayStick_lf = 7,
    menuOpCode_3wayStick_rt = 8,

    /* 3*4矩阵键盘，0~9 + '*' + '#' */
    menuOpCode_numpad3b4_n0 = 10,
    menuOpCode_numpad3b4_n1 = 11,
    menuOpCode_numpad3b4_n2 = 12,
    menuOpCode_numpad3b4_n3 = 13,
    menuOpCode_numpad3b4_n4 = 14,
    menuOpCode_numpad3b4_n5 = 15,
    menuOpCode_numpad3b4_n6 = 16,
    menuOpCode_numpad3b4_n7 = 17,
    menuOpCode_numpad3b4_n8 = 18,
    menuOpCode_numpad3b4_n9 = 19,
    menuOpCode_numpad3b4_as = 20, // '*', asterisk
    menuOpCode_numpad3b4_hs = 21, // '#', hash

    /* 双层编码器 */
    menuOpCode_2dEncoder_hp = 30, // 高层编码器，加
    menuOpCode_2dEncoder_hn = 31, // 高层编码器，减
    menuOpCode_2dEncoder_lp = 32, // 低层编码器，加
    menuOpCode_2dEncoder_ln = 33, // 低层编码器，减
    menuOpCode_2dEncoder_ok = 34, // 编码器按键按下

    /* 通用功能 */
    menuOpCode_genericfn_home = 200, // home
    menuOpCode_genericfn_back = 201, // back
    menuOpCode_genericfn_acpt = 202, // accept
    menuOpCode_genericfn_deny = 203, // deny
    menuOpCode_genericfn_info = 204, // info
} menu_keyOpCode_t;
typedef enum _menu_keyOpType_t
{
    menuOpType_shrt = 1 << 8, 
    menuOpType_long = 2 << 8, 
    menuOpType_lrpt = 3 << 8, 
    menuOpType_doub = 4 << 8,
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
#define MENU_NAME_STR_SIZE (16u)

/** format controll used by data print */
//#define MENU_DISP_NAME_COL (1u)
//#define MENU_DISP_NAME_LEN (12u)
//#define MENU_DISP_DATA_COL (14u)
//#define MENU_DISP_DATA_LEN (7u)
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
    menuItem_data_getPos = menuItem_data_global | menuItem_data_region, ///< 如果既没有定义全局存储区，又没有定义局部存储区，则该菜单项无法保存。
    menuItem_data_ROFlag = 1 << 2, ///< 该菜单项为只读。只读菜单项不允许在菜单内修改。
    menuItem_data_NoSave = 1 << 3, ///< 该菜单项默认不保存到NVM，需要手动保存。
    menuItem_data_NoLoad = 1 << 4, ///< 该菜单项默认不从NVM读取，需要手动读取。
    menuItem_data_getCfg = menuItem_data_global | menuItem_data_region
            | menuItem_data_ROFlag | menuItem_data_NoSave
            | menuItem_data_NoLoad,
    menuItem_dataExt_HasMinMax = 1 << 5, ///< 数据属性扩展：最小/最大值限制。该属性使用data指针偏移+1和+2的地址。

    /** display config */
    menuItem_disp_forceSci = 1 << 8, ///< 该菜单项强制使用科学计数法，适用于variType和varfType。
    menuItem_disp_noPreview = 1 << 10, ///< 该菜单项不会在菜单列表中显示数据。建议可在菜单项名称过长时使用。

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
typedef struct _menu_list menu_list_t;

typedef struct _menu_itemAdapter menu_itemAdapter_t;

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
typedef struct _menu_itemIfce
{
    menu_itemType_t type;               ///< 此菜单项的类型。
    uint32_t pptFlag;                   ///< 此菜单项的属性标志位。
    uint16_t list_id, unique_id;        ///< 此菜单项在本列表内的序号（从0开始）、全局唯一序号（从0开始）
    uint16_t saveAddr;                  ///< 此菜单在本区域内的偏移地址。从0开始，以1步进。注意，全局数据区和局部数据区的地址分开来算。
    char nameStr[MENU_NAME_STR_SIZE];     ///< 此菜单项的名称字符串。最大长度为MENU_NAME_STR_SIZE - 1 字节。
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
    menu_itemAdapter_t const *adapter;  ///< 指向存放菜单项命令函数指针的结构体。参考C++虚表
} menu_itemIfce_t;

struct _menu_itemAdapter
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
extern int32_t menu_currRegionNum[3];    ///< 当前局部存储区号
extern int32_t menu_statusFlag;             ///< 状态标志位
extern uint32_t menu_nvm_statusFlagAddr;    ///< 存储状态标志位的NVM存储地址
extern int32_t menu_nvmCopySrc[3];
extern int32_t menu_nvmCopyDst[3];
/**
 * @ }
 */

/** 字符缓存 */

typedef struct _menu_strBuf
{
    char strbuf[HITSIC_MENU_DISPLAY_STRBUF_ROW][HITSIC_MENU_DISPLAY_STRBUF_COL];
#if defined(HITSIC_MENU_USE_PALETTE) && (HITSIC_MENU_USE_PALETTE > 0)
    uint8_t fcolor[HITSIC_MENU_DISPLAY_STRBUF_ROW][HITSIC_MENU_DISPLAY_STRBUF_COL];
    uint8_t bcolor[HITSIC_MENU_DISPLAY_STRBUF_ROW][HITSIC_MENU_DISPLAY_STRBUF_COL];
    menu_dispColor_t palette[HITSIC_MENU_DISPLAY_PALETTE_SIZE];
#else

#endif // ! HITSIC_MENU_USE_PALETTE
}menu_strBuf_t;

extern menu_strBuf_t menu_dispStrBuf;

/*************************************
 ************ 菜单列表结构体 **********
 *************************************/
struct _menu_list
{
    menu_itemIfce_t **menu; /// 菜单项指针的动态数组，用于存放指针。该数组内的指针析构时需要手动释放。
    uint32_t listSize, listNum; /// 当前菜单项指针列表的大小、当前列表内的菜单项数量。
    uint32_t disp_p, slct_p; /// 显示数组下标和选择数组下标。
    char nameStr[MENU_NAME_STR_SIZE]; /// 菜单列表名称字符串。
};

/**
 * MENU
 */

/**
 * @brief 菜单状态标志位枚举变量。
 */
enum menu_status_t
{
    //menu_data_valid = menu_dataValid_flag << menu_dataValid_mask, /// 菜单状态标志

    menu_error_fatalError = 1 << 23,        ///> 关键故障标志位。
    menu_warning_itemLost = 1 << 22,        ///> 数据缺失标志位。读取数据时发现数据缺失时置位，须手动清除。该标志位使用16位参数，表示数据缺失的总个数。
    menu_noUse2 = 1 << 21,                  ///> cm_backtrace错误打印标志位。发生cmb打印时设置，须手动清除。该标志位使用16位参数，表示数据的大小（字节数）。
    //menu_message_strBufOverride = 1 << 20,  ///> 字符串缓存超控标志位。该标志位置位时，菜单顶层逻辑将忽略字符串缓存，直接打印全缓存。进行一帧打印后自动清除。
    menu_message_buttonOp = 1 << 19,        ///> 按键操作消息。置位时将进行按键处理，处理完成后自动清除，并自动产生屏幕打印消息。
    menu_message_printDisp = 1 << 18,       ///> 屏幕打印消息。置位时将进行屏幕打印，处理完成后自动清除。
    menu_noUse6 = 1 << 17,
    menu_noUse7 = 1 << 16,

    menu_param_Mask16 = 0xffff, ///> 低16位全16位掩码标志位，用于读取低16位参数。如果传一个参数且取值大于255，应使用16位参数。
    menu_param_Mask8h = 0xff00, ///> 低16位中高8位掩码标志位，用于读取16位参数中的高8位。如果要返回两个参数，或参数取值小于256，可以使用两个8位参数。
    menu_param_Mask8l = 0x00ff, ///> 低16位中高8位掩码标志位，用于读取16位参数中的低8位。同上。
};

typedef struct _menu_nvmData_t
{
    uint32_t head;
    char nameStr[MENU_NAME_STR_SIZE];
    uint32_t type;
    uint32_t data;
    uint32_t tail;
} menu_nvmData_t;


/**
 * @brief 菜单项迭代器结构体
 */
typedef struct _menu_iterator_t
{
    menu_list_t **listQue;
    uint32_t listQueSize;
    uint32_t listNum;
    uint32_t itemNum;
}menu_iterator_t;


/**
 * @brief 设置MENU状态标志位
 */
static inline void MENU_StatusFlagSet(uint32_t _mask)
{
    menu_statusFlag |= _mask;
}

/**
 * @brief 清除MENU状态标志位
 */
static inline void MENU_StatusFlagClr(uint32_t _mask)
{
    menu_statusFlag &= (~_mask);
}

/* @} */

#endif // ! HITSIC_USE_APP_MENU

#endif // ! UTILITIES_APP_MENU_DEF_HPP
