#ifndef UTILITIES_APP_MENU_NVM_HPP
#define UTILITIES_APP_MENU_NVM_HPP
#include <hitsic_common.h>

#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)
#include <app_menu_def.h>
#include <stdio.h>
/*!
 * @addtogroup menu_nvm
 * @{
 */

 #define MENU_KVDB_REGISTRY_KEY "MENU_REGISTRY"

/**
 * @brief 生成键
 * 
 * @param {menuItem_t const *const} _item : 要生成键的菜单项指针。
 * @param {char *} _str : 存储键的字符串指针。
 * @param {uint32_t*} _size : 字符串指针的可用大小。
 * @retval {status_t} : 返回状态。成功返回kStatus_Success。当_size空间不足时返回kStatus_Fail。
 */
status_t MENU_KVDB_GenerateKey(menu_itemIfce_t const *const _item, char *_str, uint32_t _size);

/**
 * @brief 解析键
 * 
 * @param {char const * const} _str : 要解析的键字符串。
 * @param {uint32_t *const} _pptFlag : 解析出的存储区属性，可以为menuItem_data_global或menuItem_data_region。
 * @param {uint16_t *const} _saveAddr : 解析出的存储ID。
 * @retval {status_t} : 返回状态。总是返回kStatus_Success。
 */
status_t MENU_KVDB_ResolveKey(char const * const _str, uint32_t *const _pptFlag, uint16_t *const _saveAddr);


/**
 * @brief 初始化注册表。
 * 
 */
void MENU_KVDB_RegistryInit();

void MENU_KVDB_RegistryRead();

void MENU_KVDB_RegistrySave();



/* @} */

#endif // ! HITSIC_MENU_USE_NVM

#endif // ! UTILITIES_APP_MENU_NVM_HPP
