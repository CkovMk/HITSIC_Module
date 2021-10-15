#include <app_menu_kvdb.h>

#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)

/*!
 * @addtogroup menu_kvdb
 * @{
 */
#define SYSLOG_TAG  ("MENU.KVDB")
#define SYSLOG_LVL  (HITSIC_MENU_KVDB_LOG_LVL)
#include <inc_syslog.h>

status_t MENU_KVDB_MetadataInit(menu_kvdb_metadata_t *_data)
{
    _data->swVersion = APP_MENU_VERSION;
    _data->globalItemCnt = 0U;
    _data->regionItemCnt = 0U;
    _data->regionCnt = HITSIC_MENU_NVM_REGION_CNT;

    menu_iterator_t *iter = MENU_IteratorConstruct();
    if(NULL == iter)
    {
        SYSLOG_W("Metadata init failed! Iterator constructor failed.");
        return kStatus_Fail;
    }

    do
    {
        menuItemIfce_t *item = MENU_IteratorDerefItem(iter);

        //Type constrain, only vari & varf will be saved.
        if((item->type != variType) && (item->type != varfType))
        {
            continue;
        }

        //Flag constrain, only global or region defined will be saved.
        if(item->pptFlag | menuItem_data_global)
        {
            ++_data->globalItemCnt;
            continue;
        }
        else if(item->pptFlag | menuItem_data_global)
        {
            ++_data->regionItemCnt;
            continue;
        }

    }while(kStatus_Success == MENU_IteratorIncrease(iter))

    MENU_IteratorDestruct(iter);

    SYSLOG_I("Metadata init success.");
    return kStatus_Success;
}

status_t MENU_KVDB_MetadataRead(menu_kvdb_metadata_t *_data)
{
    uint32_t size = 0U;
    if(kStatus_Success != MENU_KVDB_GetSize(MENU_KVDB_METADATA_KEY, _size) || (0U == *_size))
    {
        SYSLOG_I("Metadata read failed! Metadata not found.");
        return kStatus_Fail;
    }
    else if(sizeof(menu_kvdb_metadata_t) != size)
    {
        SYSLOG_I("Metadata read failed! Metadata size incorrect.");
        return kStatus_Fail;
    }
    if(kStatus_Success != MENU_KVDB_ReadValue(MENU_KVDB_METADATA_KEY, _data))
    {
        SYSLOG_W("Metadata read failed! Read value failed.");
        return kStatus_Fail;
    }

    SYSLOG_I("Metadata read success.");
    return kStatus_Success;
} 

status_t MENU_KVDB_MetadataSave(menu_kvdb_metadata_t *_data)
{
    assert(_data);
    if(kStauts_Success != (MENU_KVDB_METADATA_KEY, _data, sizeof(menu_kvdb_metadata_t)))
    {
        SYSLOG_W("Metadata save failed! Save value failed.")
        return kStatus_Fail;
    }
    else
    {
        SYSLOG_I("Metadata save success.");
        return kStatus_Success;
    }
} 

status_t  MENU_KVDB_GenerateKey(menu_itemIfce_t const *const _item, char *const _str, uint32_t _size)
{//FIXME: functional but not optimal.
    if(_size < 10U)
    {
        SYSLOG_W("Key generation failed! - no enough space.");
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
    sscanf(_str, "MENU%c%x", &c, _saveAddr);
    if('G' == c){*_pptFlag = menuItem_data_global;}
    else if('R' == c){*_pptFlag = menuItem_data_global;}
    return kStatus_Success;
}

status_t MENU_KVDB_RegistryInit(void *_data, uint32_t *_size)
{
    assert(NULL == _data);

    // first scan, determine how much ram to allocate
    menu_kvdb_metadata_t meta;
    MENU_KVDB_MetadataInit(&meta);

    *_size = (meta.globalItemCnt + meta.regionItemCnt) * MENU_KVDB_REG_SIZE;

    _data = malloc(*_size);
    if(NULL == _data)
    {
        *_size = 0U;
        SYSLOG_W("Registry init failed! Mem allocation failed.");
        return kStatus_Fail;
    }


    // second scan, generete registry
    char *str = (char*)_data;

    menu_iterator_t *iter = MENU_IteratorConstruct();
    if(NULL == iter)
    {
        SYSLOG_W("Registry init failed! Iterator constructor failed.");
        return kStatus_Fail;
    }

    do
    {
        menuItemIfce_t *item = MENU_IteratorDerefItem(iter);

        //Type constrain, only vari & varf will be saved.
        if((item->type != variType) && (item->type != varfType))
        {
            continue;
        }

        //Flag constrain, only global defined will be saved.
        if(item->pptFlag | menuItem_data_global)
        {
            MENU_KVDB_GenerateKey(item, str, MENU_KVDB_REG_SIZE);
            SYSLOG_V("Registry add: %s", str);
            str += MENU_KVDB_REG_SIZE;
            continue;
        }

    }while(kStatus_Success == MENU_IteratorIncrease(iter))

    MENU_IteratorSetup(iter); // Reset iter to beginning

    do
    {
        menuItemIfce_t *item = MENU_IteratorDerefItem(iter);

        //Type constrain, only vari & varf will be saved.
        if((item->type != variType) && (item->type != varfType))
        {
            continue;
        }

        //Flag constrain, only region defined will be saved.
        if(item->pptFlag | menuItem_data_global)
        {
            MENU_KVDB_GenerateKey(item, str, MENU_KVDB_REG_SIZE);
            SYSLOG_V("Registry add: %s", str);
            str += MENU_KVDB_REG_SIZE;
            continue;
        }

    }while(kStatus_Success == MENU_IteratorIncrease(iter))

    MENU_IteratorDestruct(iter);

    SYSLOG_I("Registry init success.");
}

status_t MENU_KVDB_RegistryRead(void *_data, uint32_t *_size)
{
    assert(_data == NULL);

    if(kStatus_Success != MENU_KVDB_GetSize(MENU_KVDB_REGISTRY_KEY, _size) || (0U == *_size))
    {
        SYSLOG_I("Registry read failed! Registry not found.");
        return kStatus_Fail;
    }

    _data = malloc(*_size);
    if(NULL == data)
    {
        SYSLOG_W("Registry read failed! Mem allocation failed.");
        return kStatus_Fail;
    }

    if(kStatus_Success != MENU_KVDB_ReadValue(MENU_KVDB_REGISTRY_KEY, data))
    {
        free(data);
        SYSLOG_W("Registry read failed! Read value failed.");
        return kStatus_Fail;
    }

    SYSLOG_I("Registry read success. %6.6d byte(s) received.", *_size);
    return kStatus_Success;
}

status_t MENU_KVDB_RegistrySave(void *_data, uint32_t _size)
{
    assert(_data);
    if(kStauts_Success != MENU_KVDB_SaveValue(MENU_KVDB_REGISTRY_KEY, _data, uint32_t _size))
    {
        SYSLOG_W("Registry save failed! Save value failed.")
        return kStatus_Fail;
    }
    else
    {
        SYSLOG_I("Registry save success. %6.6d byte(s) saved.", _size);
        return kStatus_Success;
    }
}

status_t MENU_KVDB_Init(void)
{

}

/* @} */

#endif // ! HITSIC_MENU_USE_NVM
