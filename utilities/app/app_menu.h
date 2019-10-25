#pragma once 
#ifndef _APPUI_H_
#define _APPUI_H_
#include "include.h"
#include "sysirq.h"
#include "drvflash.h"
#include "drvkey.h"
#include "drvoled.h"
#include "drvcam.h"
#include "drvadc.h"
#include "appctrl.h"
#include "appfilter.h"
#include "appimg.h"
#include "systask.h"
#include "drvtof.h"
#include "drvtcs.h"

#if defined(OLED_RES_128x64)
#define UI_MENU_MAX_ROW 8
#elif defined(OLED_RES_128x128)
#define UI_MENU_MAX_ROW 16
#else
#error "oled resolution unknown !"
#endif // ! OLED_RES

//----------ui---------

#define UI_MODE_SW0  KEY_SW01
#define UI_MODE_SW1  KEY_SW02

enum class ui_mode_t : uint8_t
{
	off_mode,
	menu_idle_mode,	//oled auto-refresh disable, will only refresh after key operation
	menu_ctns_mode,	//oled auto-refresh enable
	img_mode,
};

extern ui_mode_t ui_mode;
extern uint32_t ui_sysTimer_irqPos;
extern bool ui_updateFlag;

void ui_init(void);
void ui_print_oled(void);
void ui_key_op(key_btOp_t *_op);
void ui_update(void);
void ui_sysTimer_irqHandler(void);

/*
caution: "ui_print_oled()" will print the title of current page. This will occupy the first row of oled.
	So "print_oled()" functions in former classes are only allowed to print the other part of the page.
	for instance:
		-> "ui_menuItem_variType_t::print_oled()" will only need to print the v & e numbers and adjusting cursor.
		-> "ui_menuItem_procType_t::print_oled()" will print a message and allow certain process to print the oled.
			(e.g. the "oled_show_image()" process.)
		-> "ui_menuList_t::print_oled()" will print maximum 7 menu items on oled. (for fffffuture 128*128 oled, this nubmer is 15.)
*/

void ui_key_op(key_btOp_t *_op);


//----------ui_menu----------

//flash: 4096 blocks per sector. each block is an uint8_t.
//for uint64_t operation, 512 numbers can be stored in one sector.
//Each menu item uses 2 uint64_t, so 256 items can be stored in one sector.

#define UI_MENU_FLASH_BLOCK_OFFSET 1
#define UI_MENU_FLASH_RESERVE_BLOCK 10
#define UI_MENU_FLASH_ITEM_SIZE 2	// each write is a uint64 !!!
#define UI_MENU_FLASH_ITEM_PER_BLOCK 256
#define UI_MENU_FLASH_EOF_MSDW 0xffff
#define UI_MENU_FLASH_EOF_LSDW 0xeeee

class ui_menuItem_t;
class ui_menuItem_variType_t;
class ui_menuItem_varfType_t;
class ui_menuItem_procType_t;
class ui_menuItem_menuType_t;
class ui_menuList_t;


enum class ui_menuItem_type_t : uint8_t
{
	nullType,
	variType,		//watch or set integer varibles
	varfType,		//watch or set float-point varibles
	optnType,		//select one option
	chosType,		//select multiple options
	procType,		//run certain process
	menuType,		//jump to another menu
};

void ui_menuInit(void);

void ui_menuGetContent(void);
void ui_menuSetContent(void);
void ui_menuReadContent(void);
void ui_menuReadContent_old(void);
void ui_menuSaveContent(void);
void ui_menuGetSaveContent(void);
void ui_menuReadSetContent(void);

enum class ui_menuItem_propety_t : uint32_t
{
	menuModify		= 1 << 0,
	enableflashRW	= 1 << 1,
	forceFlashRW	= 1 << 2,
};

class ui_menuItem_t
{
public:
	static const uint8_t nameLen = 16;
	//static uint16_t cnt;
	uint8_t lid;	//list id
	bool readOnly;	//will abandon !!!!!!
	uint32_t porpetyFlag;
	uint16_t flashBlockIdx,flashSectorIdx;
	ui_menuItem_type_t type;
	void* ptr;
	char name[nameLen];
	ui_menuItem_t(void);
	ui_menuItem_t(const char* uname, void * uptr, ui_menuItem_type_t utype, uint32_t propety);
	~ui_menuItem_t(void);
	
	uint32_t hash(void);
	//menu operation
	void getContent(void);
	void setContent(void);
	//sdcard/flash operation
	void readContent(void);
	void saveContent(void);
	//api
	void print_oled(void);
	void key_op(key_btOp_t *_op);
};

class ui_menuItem_variType_t
{
public:
	int32_t *ptr;
	int16_t v, e;
	int8_t cur;		//cursor used when adjusting numbers. 0,1,2 for v x1,x10,x100 numbers, -1 for e
	ui_menuItem_variType_t(int32_t* uptr);
	//var operation
	void writeContent(int16_t _v, int16_t _e);
	void writeContent(int32_t _var);
	//menu operation
	void getContent(void);
	void setContent(void);
	//sdcard operation
	void readContent(void);
	void saveContent(void);
	//api
	void print_oled(void);
	void key_op(key_btOp_t *_op);
};
class ui_menuItem_varfType_t
{
public:
	float *ptr;
	int16_t v, e;
	int8_t cur;		//cursor used when adjusting numbers. 0,1,2 for v x1,x10,x100 numbers, -1 for e

	ui_menuItem_varfType_t(float* uptr);
	//var operation
	void writeContent(int16_t _v, int16_t _e);
	void writeContent(float _var);
	//menu operation
	void getContent(void);
	void setContent(void);
	//sdcard operation
	void readContent(void);
	void saveContent(void);
	//api
	void print_oled(void);
	void key_op(key_btOp_t *_op);
};
//class ui_menuItem_optnType_t
//{
//public:
//	static const uint8_t nameLen = 16;		//name length
//	int32_t* ptr;
//	uint8_t num;		//number of selections
//	char** name;		//name of each selection
//	int32_t* val;
//	uint8_t sel;
//
//	ui_menuItem_optnType_t(uint32_t* _ptr, uint8_t _num, char** _name, int32_t* _val, uint8_t _sel);
//	~ui_menuItem_optnType_t();
//	//var operation
//	void writeContent(uint8_t _optnSel);
//	//menu operation
//	void getContent(void);
//	void setContent(void);
//	//sdcard operation
//	void readContent(void);
//	void saveContent(void);
//	//api
//	print_oled(void);
//};
//class ui_menuItem_chosType_t
//{
//public:
//
//};
typedef int32_t(*ui_menuItem_procHandler_t)(key_btOp_t *_op);
enum ui_menuItem_procMsg_t : int32_t
{
	// return 0 to exit.
	msg_procCtnue = 1<<0,	//contiune run
	msg_noLRepeat_LUP = 1<<2,
	msg_noLRepeat_LDN = 1<<3,
	msg_noLRepeat_LLF = 1<<4,
	msg_noLRepeat_LRT = 1<<5,
	msg_noLRepeat_LOK = 1<<6,
};
class ui_menuItem_procType_t
{
public:
	ui_menuItem_procHandler_t ptr;
	ui_menuItem_procType_t(ui_menuItem_procHandler_t _ptr);
	bool returnEnable;
	int32_t handlerReturn;
	void print_oled(void);
	void key_op(key_btOp_t *_op);
};
class ui_menuItem_menuType_t
{
public:
	ui_menuList_t *ptr;
	ui_menuItem_menuType_t(ui_menuList_t* _ptr);
	//void print_oled(void);
	//void key_op(key_btOp_t *_op);
};

class ui_menuList_t
{
public:
	//static uint32_t cnt;
	vector<ui_menuItem_t> menu;
	//uint32_t uid;
	uint32_t disp_p, slct_p;
	char name[16];
	ui_menuList_t(void);
	ui_menuList_t(const char* _name, ui_menuList_t* _prev);
	//~ui_menuList_t(void);
	void insert(ui_menuItem_t item);
	void print_oled(void);
	void key_op(key_btOp_t *_op);

};

extern ui_menuList_t* ui_menu_currList;
extern ui_menuItem_t* ui_menu_currItem;
extern ui_menuList_t ui_menu_root;






//----------ui_msgBox---------
class ui_msgBox_t
{
public:

};


//hotfix 2019.04.16

#endif // ! _APPUI_H_