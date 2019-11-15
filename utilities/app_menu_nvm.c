#include "app_menu_nvm.h"




#ifdef __cplusplus
extern "C"{
#endif 

	/**
	 * ********** NVM存储变量定义 **********
	 */

	//gl = global
	//rg = region
	//addr = address
	//sect = sector
	/**
	  * @brief : 每个扇区包含的字节数
	  */
#define menu_nvm_sectorSize flash_sectorSize;
	/**
	 * @brief : 全局存储 Global Storage
	 */
	uint32_t menu_nvm_glSectCnt = 2u;	/// 全局存储区占用的扇区数
	uint32_t menu_nvm_glSectOffset = 2u; /// 全局存储区扇区偏移
	uint32_t menu_nvm_glAddrOffset;		 /// 全局存储区地址偏移
	/**
	 * @brief : 局部存储 Region Storage
	 */

	uint32_t menu_nvm_rgSectCnt = 4u;				/// 每个局部存储区占用的扇区数
	uint32_t menu_nvm_rgSectOffset[menu_nvm_rgCnt]; /// 三个局部存储区的扇区偏移

	uint32_t menu_nvm_rgAddrOffset[menu_nvm_rgCnt]; /// 三个局部存储区的地址偏移

	/**
	 * @brief : 菜单存储占用的总扇区数
	 */
	uint32_t menu_nvm_totalSectCnt;
	/**
	 * @brief : 每个菜单项保存时占用的字节数
	 */
	uint32_t menu_nvm_dataSize = 32u;

	/**
	 * @brief : 菜单项写入缓存。
	 * 当改写第N个扇区时，menu_nvm_cachedSector = N, menu_nvm_cache分配4KB缓存
	 * 并读入第N扇区的所有内容。此时能且仅能修改第N扇区的内容。对第N扇区内容的修改
	 * 将缓存至上述内存。
	 */
	uint8_t *menu_nvm_cache = NULL;
	uint32_t menu_nvm_cachedSector = 0;

	/**
	 * ********** NVM存储操作接口 **********
	 */

	status_t MENU_NvmRead(uint32_t _addr, void *_buf, uint32_t _byteCnt)
	{
		return FLASH_AddressRead(_addr, _buf, _byteCnt);
	}

	bool MENU_NvmCacheable(uint32_t _addr)
	{
        if(menu_nvm_cache == NULL || _addr / flash_sectorSize == menu_nvm_cachedSector)
        {
            return true;
        }
		else
		{
			return false;
		}
	}

	status_t MENU_NvmCacheSector(uint32_t _sect)
	{
		if (menu_nvm_cache != NULL)
		{
			return kStatus_Fail;
		}
		menu_nvm_cache = malloc(flash_sectorSize);
		if (menu_nvm_cache == NULL)
		{
			return kStatus_Fail;
		}
		if (kStatus_FTFx_Success != FLASH_SectorRead(menu_nvm_cachedSector, (void *)menu_nvm_cache))
		{
			free(menu_nvm_cache);
			menu_nvm_cache = NULL;
			return kStatus_Fail;
		}
		return kStatus_Success;
	}

	status_t MENU_NvmWriteCache(uint32_t _addr, void *_buf, uint32_t _byteCnt)
	{
		if (menu_nvm_cache == NULL)
		{
			menu_nvm_cachedSector = _addr / flash_sectorSize;
			MENU_NvmCacheSector(menu_nvm_cachedSector);
		}
		memcpy(_buf, menu_nvm_cache + _addr % flash_sectorSize, _byteCnt);
		return kStatus_Success;
	}

	status_t MENU_NvmUpdateCache(void)
	{
		if (kStatus_FTFx_Success != FLASH_SectorWrite(menu_nvm_cachedSector, menu_nvm_cache))
		{
			return kStatus_Fail;
		}
		free(menu_nvm_cache);
		menu_nvm_cache = NULL;
		return kStatus_Success;
	}



#ifdef __cplusplus
}
#endif 