#include "app_menu_type.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief : 菜单项和菜单列表的计数器。
	 */
	uint32_t menu_itemCnt = 0;
	uint32_t menu_listCnt = 0;

	const int32_t menu_itemAdjustLut[] = {1, 10, 100, 1000};

	/**
	 * @brief : 占位类型菜单项的操作句柄及操作函数。
	 */
	void MENU_ItemConstruct_nullType(menu_itemIfce_t *_item, void *_data)
	{
		_item->handle.p_nullType = (menu_item_nullHandle_t*)malloc(sizeof(menu_item_nullHandle_t));
		_item->handle.p_nullType->data = _data;
	}
	void MENU_ItemGetData_nullType(menu_itemIfce_t *_item, void *_data)
	{
		//blank
	}
	void MENU_ItemSetData_nullType(menu_itemIfce_t *_item, void *_data)
	{
		//blank
	}
	//used when in menuList
	void MENU_ItemPrintSlot_nullType(menu_itemIfce_t *_item, uint32_t _slotNum)
	{
		snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " - - - - - - - - - - ");
	}
	void MENU_ItemDirectKeyOp_nullType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		*_op = 0;
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_nullType(menu_itemIfce_t *_item)
	{
		assert(0); //should never end up here.
	}
	void MENU_ItemKeyOp_nullType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		assert(0); //should never end up here.
	}

	/**
	 * @brief : 整数类型菜单项的操作句柄及操作函数。
	 */
	void MENU_ItemGetContent_variType(int32_t *const v, int32_t *const e, int32_t data)
	{
		*v = data;
		*e = 0;
		while (-10 < *e && *e < 10)
		{
			if (*v > 9999)
			{
				*v *= 0.1;
				++(*e);
				continue;
			}
			if (*v < -9999)
			{
				*v *= 0.1;
				++(*e);
				continue;
			}
			break;
		}
	}
	void MENU_ItemSetContent_variType(int32_t *const data, int32_t v, int32_t e)
	{
		//int32_t v = v, e = e;
		while (1)
		{
			if (e > 0)
			{
				v *= 10;
				e--;
				continue;
			}
			if (e < 0)
			{
				v *= 0.1;
				e++;
				continue;
			}
			break;
		}
		*data = v;
	}

	void MENU_ItemConstruct_variType(menu_itemIfce_t *_item, void *_data)
	{
		_item->handle.p_variType = (menu_item_variHandle_t*)malloc(sizeof(menu_item_variHandle_t));
		_item->handle.p_variType->data = (int32_t*)_data;
		_item->handle.p_variType->cur = 0;
		MENU_ItemGetContent_variType(&_item->handle.p_variType->v, &_item->handle.p_variType->e, *_item->handle.p_variType->data);
	}
	void MENU_ItemGetData_variType(menu_itemIfce_t *_item, void *_data)
	{
		*(int32_t *)_data = *(_item->handle.p_variType->data);
	}
	void MENU_ItemSetData_variType(menu_itemIfce_t *_item, void *_data)
	{
		if (!(_item->pptFlag & menuItem_data_ROFlag))
		{
			*(_item->handle.p_variType->data) = *(int32_t *)_data;
			HITSIC_MENU_PRINTF("Verbose: MENU: variType Data Updated %d\n", *(_item->handle.p_variType->data));
		}
	}
	//used when in menuList
	void MENU_ItemPrintSlot_variType(menu_itemIfce_t *_item, uint32_t _slotNum)
	{
		menu_item_variHandle_t *handle = _item->handle.p_variType;
		if (_item->pptFlag & menuItem_disp_noPreview)
		{
			menu_dispStrBuf[_slotNum][snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " %-16.16s  ->", _item->nameStr)] = ' ';
		}
		else if ((!(_item->pptFlag & menuItem_disp_forceSci)) && *handle->data < 1000000 && *handle->data > -1000000)
		{
			menu_dispStrBuf[_slotNum][snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " %-12.12s %+-7d", _item->nameStr, *(handle->data))] = ' ';
		}
		else
		{
			menu_dispStrBuf[_slotNum][snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " %-12.12s %+4.4d%+1.1d", _item->nameStr, handle->v, handle->e)] = ' ';
		}
	}
	void MENU_ItemDirectKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		menu_item_variHandle_t *handle = _item->handle.p_variType;
		switch (*_op)
		{
		case MENU_BUTTON_MAKE_OP(ok, shrt):
		{
			MENU_ItemGetContent_variType(&handle->v, &handle->e, *handle->data);
			menu_currItem = _item;
			*_op = 0;
			break;
		}
		case MENU_BUTTON_MAKE_OP(lf, shrt):
		case MENU_BUTTON_MAKE_OP(lf, long):
		case MENU_BUTTON_MAKE_OP(lf, lrpt):
		{
			if (!(_item->pptFlag & menuItem_data_ROFlag))
			{
				if (*handle->data < 10 && *handle->data > -10)
				{
					--*handle->data;
				}
				else
				{
					MENU_ItemGetContent_variType(&handle->v, &handle->e, *handle->data);
					MENU_ItemSetContent_variType(handle->data, handle->v - 100, handle->e);
				}
			}
			*_op = 0;
			break;
		}
		case MENU_BUTTON_MAKE_OP(rt, shrt):
		case MENU_BUTTON_MAKE_OP(rt, long):
		case MENU_BUTTON_MAKE_OP(rt, lrpt):
		{
			if (!(_item->pptFlag & menuItem_data_ROFlag))
			{
				if (*handle->data < 10 && *handle->data > -10)
				{
					++*handle->data;
				}
				else
				{
					MENU_ItemGetContent_variType(&handle->v, &handle->e, *handle->data);
					MENU_ItemSetContent_variType(handle->data, handle->v + 100, handle->e);
				}
			}
			*_op = 0;
			break;
		}
		default:
			//appui_menu_t::getInstance().currItem = NULL;
			break;
		}
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_variType(menu_itemIfce_t *_item)
	{
		menu_item_variHandle_t *handle = _item->handle.p_variType;
		MENU_ItemSetContent_variType(&handle->bData, handle->v, handle->e);
		menu_dispStrBuf[0][snprintf(menu_dispStrBuf[0], MENU_DISP_STRBUF_COL, "##%-12.12s*", _item->nameStr)] = ' ';
		menu_dispStrBuf[2][snprintf(menu_dispStrBuf[2], MENU_DISP_STRBUF_COL, "  Cur: %+-10d", *handle->data)] = ' ';
		menu_dispStrBuf[3][snprintf(menu_dispStrBuf[3], MENU_DISP_STRBUF_COL, "  Adj: %+-10d", handle->bData)] = ' ';
		menu_dispStrBuf[4][snprintf(menu_dispStrBuf[4], MENU_DISP_STRBUF_COL, "  Mod: %+4.4de%+1.1d", handle->v, handle->e)] = ' ';
		int32_t pos = handle->cur < 0 ? 12 - handle->cur : 11 - handle->cur;
		menu_dispStrBuf[5][pos] = '^';

		if (_item->pptFlag & menuItem_data_ROFlag)
		{
			menu_dispStrBuf[7][snprintf(menu_dispStrBuf[7], MENU_DISP_STRBUF_COL, "    RO")] = ' ';
		}
		else
		{
			menu_dispStrBuf[7][snprintf(menu_dispStrBuf[7], MENU_DISP_STRBUF_COL, "    SOK>AC LOK>WA")] = ' ';
		}
		if (_item->pptFlag & menuItem_data_global)
		{
			menu_dispStrBuf[0][16] = 'G';
			menu_dispStrBuf[0][17] = 'L';
		}
		else if (_item->pptFlag & menuItem_data_region)
		{
			menu_dispStrBuf[0][16] = 'R';
			menu_dispStrBuf[0][17] = 'G';
		}
		else
		{
			menu_dispStrBuf[0][16] = 'N';
			menu_dispStrBuf[0][17] = 'O';
		}
	}
	void MENU_ItemKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		menu_item_variHandle_t *handle = _item->handle.p_variType;
		switch (*_op)
		{
		case MENU_BUTTON_MAKE_OP(ok, shrt):
			if (!(_item->pptFlag & menuItem_data_ROFlag))
			{
				MENU_ItemSetContent_variType(handle->data, handle->v, handle->e);
			}
		case MENU_BUTTON_MAKE_OP(ok, long):
			menu_currItem = NULL;
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(lf, long):
			handle->cur = 4;
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(rt, long):
			handle->cur = -2;
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(lf, shrt):
			if (handle->cur < 4)
			{
				++handle->cur;
			}
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(rt, shrt):
			if (handle->cur > -2)
			{
				--handle->cur;
			}
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(up, shrt):
		case MENU_BUTTON_MAKE_OP(up, long):
		case MENU_BUTTON_MAKE_OP(up, lrpt):
			if (handle->cur == -2)
			{
				++handle->e;
			}
			else if (handle->cur == -1)
			{
				handle->e = -handle->e;
			}
			else if (handle->cur == 4)
			{
				handle->v = -handle->v;
			}
			else
			{
				handle->v += menu_itemAdjustLut[handle->cur];
			}
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(dn, shrt):
		case MENU_BUTTON_MAKE_OP(dn, long):
		case MENU_BUTTON_MAKE_OP(dn, lrpt):
			if (handle->cur == -2)
			{
				--handle->e;
			}
			else if (handle->cur == -1)
			{
				handle->e = -handle->e;
			}
			else if (handle->cur == 4)
			{
				handle->v = -handle->v;
			}
			else
			{
				handle->v -= menu_itemAdjustLut[handle->cur];
			}
			*_op = 0;
			break;
		default:
			//appui_menu_t::getInstance().currItem = NULL;
			break;
		}
	}

	/**
	 * @brief : 浮点类型菜单项的操作句柄及操作函数。
	 */

	void MENU_ItemGetContent_varfType(int32_t *const v, int32_t *const e, float data)
	{
		*e = 0;
		while (-10 < *e && *e < 10)
		{
			if (data > 0.0f && data < 999.0f)
			{
				data *= 10.0;
				--(*e);
				continue;
			}
			if (data > 9999.0f)
			{
				data /= 10.0f;
				++(*e);
				continue;
			}
			if (data < 0.0f && data > -999.0f)
			{
				data *= 10.0f;
				--(*e);
				continue;
			}
			if (data < -9999.0)
			{
				data /= 10.0f;
				++(*e);
				continue;
			}
			break;
		}
		*v = (int32_t)data;
	}
	void MENU_ItemSetContent_varfType(float *const data, int32_t v, int32_t e)
	{
		float buf = (float)v;
		while (1)
		{
			if (e > 0)
			{
				buf *= 10.0f;
				e--;
				continue;
			}
			if (e < 0)
			{
				buf /= 10.0f;
				e++;
				continue;
			}
			break;
		}
		*data = buf;
	}

	void MENU_ItemConstruct_varfType(menu_itemIfce_t *_item, void *_data)
	{
		_item->handle.p_varfType = (menu_item_varfHandle_t*)malloc(sizeof(menu_item_varfHandle_t));
		_item->handle.p_varfType->data = (float*)_data;
		_item->handle.p_varfType->cur = 0;
		MENU_ItemGetContent_varfType(&_item->handle.p_varfType->v, &_item->handle.p_varfType->e, *_item->handle.p_varfType->data);
	}
	void MENU_ItemGetData_varfType(menu_itemIfce_t *_item, void *_data)
	{
		*(float *)_data = *(_item->handle.p_varfType->data);
	}
	void MENU_ItemSetData_varfType(menu_itemIfce_t *_item, void *_data)
	{
		if (!(_item->pptFlag & menuItem_data_ROFlag))
		{
			*(_item->handle.p_varfType->data) = *(float *)_data;
			HITSIC_MENU_PRINTF("Verbose: MENU: varfType Data Updated %f\n", *(_item->handle.p_varfType->data));
		}
	}
	//used when in menuList
	void MENU_ItemPrintSlot_varfType(menu_itemIfce_t *_item, uint32_t _slotNum)
	{
		menu_item_varfHandle_t *handle = _item->handle.p_varfType;
		if (_item->pptFlag & menuItem_disp_noPreview)
		{
			menu_dispStrBuf[_slotNum][snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " %-16.16s  ->", _item->nameStr)] = ' ';
		}
		else if ((!(_item->pptFlag & menuItem_disp_forceSci)) && ((*handle->data < 10000 && *handle->data > 0.1f) || (*handle->data > -10000 && *handle->data < -0.1f)))
		{
			menu_dispStrBuf[_slotNum][snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " %-12.12s %+-7f", _item->nameStr, *(handle->data))] = ' ';
		}
		else
		{
			menu_dispStrBuf[_slotNum][snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " %-12.12s %+4.4d%+1.1d", _item->nameStr, handle->v, handle->e)] = ' ';
		}
	}
	void MENU_ItemDirectKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		menu_item_varfHandle_t *handle = _item->handle.p_varfType;
		switch (*_op)
		{
		case MENU_BUTTON_MAKE_OP(ok, shrt):
		{
			MENU_ItemGetContent_varfType(&handle->v, &handle->e, *handle->data);
			menu_currItem = _item;
			*_op = 0;
			break;
		}
		case MENU_BUTTON_MAKE_OP(lf, shrt):
		case MENU_BUTTON_MAKE_OP(lf, long):
		case MENU_BUTTON_MAKE_OP(lf, lrpt):
		{
			if (!(_item->pptFlag & menuItem_data_ROFlag))
			{
				MENU_ItemGetContent_varfType(&handle->v, &handle->e, *handle->data);
				MENU_ItemSetContent_varfType(handle->data, handle->v - 100, handle->e);
			}
			*_op = 0;
			break;
		}
		case MENU_BUTTON_MAKE_OP(rt, shrt):
		case MENU_BUTTON_MAKE_OP(rt, long):
		case MENU_BUTTON_MAKE_OP(rt, lrpt):
		{
			if (!(_item->pptFlag & menuItem_data_ROFlag))
			{
				MENU_ItemGetContent_varfType(&handle->v, &handle->e, *handle->data);
				MENU_ItemSetContent_varfType(handle->data, handle->v + 100, handle->e);
			}
			*_op = 0;
			break;
		}
		default:
			//appui_menu_t::getInstance().currItem = NULL;
			break;
		}
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_varfType(menu_itemIfce_t *_item)
	{
		menu_item_varfHandle_t *handle = _item->handle.p_varfType;
		MENU_ItemSetContent_varfType(&handle->bData, handle->v, handle->e);
		menu_dispStrBuf[0][snprintf(menu_dispStrBuf[0], MENU_DISP_STRBUF_COL, "##%-12.12s*", _item->nameStr)] = ' ';
		menu_dispStrBuf[2][snprintf(menu_dispStrBuf[2], MENU_DISP_STRBUF_COL, "  Cur: %+-10.4f", *handle->data)] = ' ';
		menu_dispStrBuf[3][snprintf(menu_dispStrBuf[3], MENU_DISP_STRBUF_COL, "  Adj: %+-10.4f", handle->bData)] = ' ';
		menu_dispStrBuf[4][snprintf(menu_dispStrBuf[4], MENU_DISP_STRBUF_COL, "  Mod: %+3.3de%+1.1d", handle->v, handle->e)] = ' ';
		int32_t pos = handle->cur < 0 ? 12 - handle->cur : 11 - handle->cur;
		//HITSIC_MENU_DEBUG_PRINTF("-Verbose MENU: cur = %d, pos = %d .\n",handle->cur, pos);
		menu_dispStrBuf[5][pos] = '^';

		if (_item->pptFlag & menuItem_data_ROFlag)
		{
			menu_dispStrBuf[7][snprintf(menu_dispStrBuf[7], MENU_DISP_STRBUF_COL, "    RO")] = ' ';
		}
		else
		{
			menu_dispStrBuf[7][snprintf(menu_dispStrBuf[7], MENU_DISP_STRBUF_COL, "    SOK>AC LOK>WA")] = ' ';
		}
		if (_item->pptFlag & menuItem_data_global)
		{
			menu_dispStrBuf[0][16] = 'G';
			menu_dispStrBuf[0][17] = 'L';
		}
		else if (_item->pptFlag & menuItem_data_region)
		{
			menu_dispStrBuf[0][16] = 'R';
			menu_dispStrBuf[0][17] = 'G';
		}
		else
		{
			menu_dispStrBuf[0][16] = 'N';
			menu_dispStrBuf[0][17] = 'O';
		}
	}
	void MENU_ItemKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		menu_item_varfHandle_t *handle = _item->handle.p_varfType;
		switch (*_op)
		{
		case MENU_BUTTON_MAKE_OP(ok, shrt):
			if (!(_item->pptFlag & menuItem_data_ROFlag))
			{
				MENU_ItemSetContent_varfType(handle->data, handle->v, handle->e);
			}
		case MENU_BUTTON_MAKE_OP(ok, long):
			menu_currItem = NULL;
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(lf, long):
			handle->cur = 4;
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(rt, long):
			handle->cur = -2;
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(lf, shrt):
			if (handle->cur < 4)
			{
				++handle->cur;
			}
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(rt, shrt):
			if (handle->cur > -2)
			{
				--handle->cur;
			}
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(up, shrt):
		case MENU_BUTTON_MAKE_OP(up, long):
		case MENU_BUTTON_MAKE_OP(up, lrpt):
			if (handle->cur == -2)
			{
				++handle->e;
			}
			else if (handle->cur == -1)
			{
				handle->e = -handle->e;
			}
			else if (handle->cur == 4)
			{
				handle->v = -handle->v;
			}
			else
			{
				handle->v += menu_itemAdjustLut[handle->cur];
			}
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(dn, shrt):
		case MENU_BUTTON_MAKE_OP(dn, long):
		case MENU_BUTTON_MAKE_OP(dn, lrpt):
			if (handle->cur == -2)
			{
				--handle->e;
			}
			else if (handle->cur == -1)
			{
				handle->e = -handle->e;
			}
			else if (handle->cur == 4)
			{
				handle->v = -handle->v;
			}
			else
			{
				handle->v -= menu_itemAdjustLut[handle->cur];
			}
			*_op = 0;
			break;
		default:
			//appui_menu_t::getInstance().currItem = NULL;
			break;
		}

		while (handle->v > 9999 || handle->v < -9999)
		{
			handle->v /= 10;
			++handle->e;
		}
		while ((handle->v > 0 && handle->v < 1000) || (handle->v < 0 && handle->v > -1000))
		{
			handle->v *= 10;
			--handle->e;
		}
	}

	/**
	 * @brief : 布尔类型菜单项的操作句柄及操作函数。
	 */
	void MENU_ItemConstruct_boolType(menu_itemIfce_t *_item, void *_data)
	{
		_item->handle.p_boolType = (menu_item_boolHandle_t*)malloc(sizeof(menu_item_boolHandle_t));
		_item->handle.p_boolType->data = (bool*)_data;
	}
	void MENU_ItemGetData_boolType(menu_itemIfce_t *_item, void *_data)
	{
	}
	void MENU_ItemSetData_boolType(menu_itemIfce_t *_item, void *_data)
	{
	}
	//used when in menuList
	void MENU_ItemPrintSlot_boolType(menu_itemIfce_t *_item, uint32_t _slotNum)
	{
		snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " bool Type - - - - ");
	}
	void MENU_ItemDirectKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_boolType(menu_itemIfce_t *_item)
	{
		assert(0); //should never end up here.
	}
	void MENU_ItemKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		assert(0); //should never end up here.
	}

	/**
	 * @brief : 函数类型菜单项的操作句柄及操作函数。
	 */
	void MENU_ItemConstruct_procType(menu_itemIfce_t *_item, void *_data)
	{
		_item->handle.p_procType = (menu_item_procHandle_t*)malloc(sizeof(menu_item_procHandle_t));
		_item->handle.p_procType->data = (void (*)(void))_data;
	}
	void MENU_ItemGetData_procType(menu_itemIfce_t *_item, void *_data)
	{
	}
	void MENU_ItemSetData_procType(menu_itemIfce_t *_item, void *_data)
	{
	}
	//used when in menuList
	void MENU_ItemPrintSlot_procType(menu_itemIfce_t *_item, uint32_t _slotNum)
	{
		menu_dispStrBuf[_slotNum][snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " //%-16.16s", _item->nameStr)] = ' ';
	}
	void MENU_ItemDirectKeyOp_procType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		menu_item_procHandle_t *handle = _item->handle.p_procType;
		switch (*_op)
		{
		case MENU_BUTTON_MAKE_OP(ok, shrt):
		{
			handle->data();
			*_op = 0;
			break;
		}

		default:
			break;
		}
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_procType(menu_itemIfce_t *_item)
	{
		assert(0); //should never end up here.
	}
	void MENU_ItemKeyOp_procType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		assert(0); //should never end up here.
	}

	/**
	 * @brief : 跳转类型菜单项的操作句柄及操作函数。
	 */
	void MENU_ItemConstruct_menuType(menu_itemIfce_t *_item, void *_data)
	{
		_item->handle.p_menuType = (menu_item_menuHandle_t*)malloc(sizeof(menu_item_menuHandle_t));
		_item->handle.p_menuType->data = (menu_list_t*)_data;
	}
	void MENU_ItemGetData_menuType(menu_itemIfce_t *_item, void *_data)
	{
	}
	void MENU_ItemSetData_menuType(menu_itemIfce_t *_item, void *_data)
	{
	}
	//used when in menuList
	void MENU_ItemPrintSlot_menuType(menu_itemIfce_t *_item, uint32_t _slotNum)
	{
		menu_dispStrBuf[_slotNum][snprintf(menu_dispStrBuf[_slotNum], MENU_DISP_STRBUF_COL, " [%s]", _item->nameStr)] = ' ';
	}
	void MENU_ItemDirectKeyOp_menuType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		switch (*_op)
		{
		case MENU_BUTTON_MAKE_OP(ok, shrt):
			menu_currList = _item->handle.p_menuType->data;
			*_op = 0;
			break;

		default:
			break;
		}
	}
	//used when in menuItem
	void MENU_ItemPrintDisp_menuType(menu_itemIfce_t *_item)
	{
		assert(0); //should never end up here.
	}
	void MENU_ItemKeyOp_menuType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		assert(0); //should never end up here.
	}

	/**
	 * ********** 菜单项操作接口 **********
	 */

	menu_itemIfce_t *MENU_ItemConstruct(menu_itemType_t _type, void *_data, const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag)
	{

		menu_itemIfce_t *item;
		item = (menu_itemIfce_t*)calloc(1, sizeof(menu_itemIfce_t));
		assert(item);
		item->type = _type;
		item->pptFlag = _pptFlag;
		item->unique_id = menu_itemCnt++;
		item->saveAddr = _saveAddr;
		strncpy(item->nameStr, _nameStr, menu_nameStrSize);
		item->nameStr[menu_nameStrSize - 1] = '\0';

		MENU_ITEM_SWITCH_CASE(MENU_ItemConstruct, item, _data);
		return item;
	}

	void MENU_itemDestruct(menu_itemIfce_t *_item)
	{
		free(&_item->handle); //TODO: fix this!
		free(_item);
		_item = NULL;
		--menu_itemCnt;
	}

	void MENU_ItemGetData(menu_itemIfce_t *_item, menu_nvmData_t *_data)
	{
		memcpy(_data->nameStr, _item->nameStr, menu_nameStrSize);
		_data->type = _item->type;
		MENU_ITEM_SWITCH_CASE(MENU_ItemGetData, _item, &_data->data);
	}

	void MENU_ItemSetData(menu_itemIfce_t *_item, menu_nvmData_t *_data)
	{
		if (0 == strncmp(_data->nameStr, _item->nameStr, menu_nameStrSize) && _data->type == (uint32_t)_item->type)
		{
			MENU_ITEM_SWITCH_CASE(MENU_ItemSetData, _item, &_data->data);
		}
	}

	void MENU_ItemPrintSlot(menu_itemIfce_t *_item, uint32_t _slotNum)
	{
		//HITSIC_MENU_PRINTF("-Verbose MENU: printing slot menu %s, type=%d, slot=%d.\n", _item->nameStr, _item->type, _slotNum);
		MENU_ITEM_SWITCH_CASE(MENU_ItemPrintSlot, _item, _slotNum);
	}

	void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemDirectKeyOp, _item, _op);
	}

	void MENU_ItemPrintDisp(menu_itemIfce_t *_item)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemPrintDisp, _item);
	}

	void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
	{
		MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, _item, _op);
	}

	/**
	 * ********** 菜单列表操作接口 **********
	 */

	menu_list_t *MENU_ListConstruct(const char *_nameStr, uint32_t _size, menu_list_t *_prev)
	{
		assert(_prev);
		menu_list_t *list;
		list = (menu_list_t*)calloc(1, sizeof(menu_list_t));
		assert(list);
		list->listSize = _size;
		list->listNum = 0;
		list->menu = (menu_itemIfce_t**)calloc(_size, sizeof(menu_itemIfce_t *));
		assert(list->menu);
		strncpy(list->nameStr, _nameStr, menu_nameStrSize);
		MENU_ListInsert(list, MENU_ItemConstruct(menuType, (void *)_prev, "Back", 0, 0));
		list->menu[0]->handle.p_menuType->data = _prev;
		++menu_listCnt;
		return list;
	}

	void MENU_ListDestruct(menu_list_t *_list)
	{
		free(_list->menu);
		free(_list);
		--menu_listCnt;
	}

	status_t MENU_ListInsert(menu_list_t *_list, menu_itemIfce_t *_item)
	{
		assert(_list);
		assert(_item);

		if (_list->listNum == _list->listSize)
		{
			return kStatus_Fail;
		}
		_list->menu[_list->listNum] = _item;
		_item->list_id = _list->listNum++;
		//_item->myList = _list;

		return kStatus_Success;
	}

	void MENU_ListPrintDisp(menu_list_t *_list)
	{
		menu_dispStrBuf[0][snprintf(menu_dispStrBuf[0], MENU_DISP_STRBUF_COL, "##%-12.12s*%2.2d/%2.2d", _list->nameStr, _list->slct_p, _list->listNum)] = ' ';
		uint32_t printCnt = _list->listNum < MENU_DISP_STRBUF_ROW - 1 ? _list->listNum : MENU_DISP_STRBUF_ROW - 1;
		for (uint8_t i = 0; i < printCnt; ++i)
		{
			MENU_ItemPrintSlot(_list->menu[_list->disp_p + i], i + 1);
		}
		menu_dispStrBuf[_list->slct_p - _list->disp_p + 1][0] = '>';
	}

	void MENU_ListKeyOp(menu_list_t *_list, menu_keyOp_t *const _op)
	{
		switch (*_op)
		{
		case MENU_BUTTON_MAKE_OP(ok, long):
			//return
			menu_currList = _list->menu[0]->handle.p_menuType->data;
		case MENU_BUTTON_MAKE_OP(ok, lrpt):
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(up, shrt):
		case MENU_BUTTON_MAKE_OP(up, long):
		case MENU_BUTTON_MAKE_OP(up, lrpt):
			//menu up
			if (_list->slct_p > 0)
			{
				--_list->slct_p;
			}
			while (_list->disp_p > _list->slct_p)
			{
				--_list->disp_p;
			}
			*_op = 0;
			break;
		case MENU_BUTTON_MAKE_OP(dn, shrt):
		case MENU_BUTTON_MAKE_OP(dn, long):
		case MENU_BUTTON_MAKE_OP(dn, lrpt):
			//menu dn
			if (_list->slct_p < _list->listNum - 1)
			{
				++_list->slct_p;
			}
			while (_list->slct_p >= 6 && _list->disp_p < _list->slct_p - 6) //TODO: fix this. 6 is a magic number.
			{
				++_list->disp_p;
			}
			*_op = 0;
			break;
		default:
			//direct op
			MENU_ItemDirectKeyOp(_list->menu[_list->slct_p], _op);
			break;
		}
	}

#ifdef __cplusplus
}
#endif
