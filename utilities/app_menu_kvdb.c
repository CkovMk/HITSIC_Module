#include <app_menu_kvdb.h>

#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)

/*!
 * @addtogroup menu_kvdb
 * @{
 */
#define SYSLOG_TAG  ("MENU.KVDB")
#define SYSLOG_LVL  (HITSIC_MENU_KVDB_LOG_LVL)
#include <inc_syslog.h>

status_t  MENU_KVDB_GenerateKey(menu_itemIfce_t const *const _item, char *const _str, uint32_t _size)
{//FIXME: functional but not optimal.
    if(_size < 10U)
    {
        SYSLOG_W("Key generation failed! - no enouth space.");
        return kStatus_Fail; 
    }
    if(_item->pptFlag & menuItem_data_global)
        { snprintf(_str, _size, "MENUG%4.4x", _item->saveAddr); }
    else if(_item->pptFlag & menuItem_data_region)
        { snprintf(_str, _size, "MENUR%4.4x", _item->saveAddr); }
    return kStatus_Success;
}

status_t MENU_KVDB_ResolveKey(char const * const _str, uint32_t *const _pptFlag, uint16_t *const _saveAddr)
{//FIXME: functional but not optimal.
    char c = ' ';
    sscanf(_str, "MENU%c%x", &c, &_saveAddr);
    if('G' == c){*_pptFlag = menuItem_data_global;}
    else if('R' == c){*_pptFlag = menuItem_data_global;}
    return kStatus_Success;
}

/* @} */

#endif // ! HITSIC_MENU_USE_NVM
