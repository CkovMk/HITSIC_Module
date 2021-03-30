/**
 * Copyright 2018 - 2020 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * flash_driver.c
 *
 *  Created on: 2019年7月25日
 *      Author: 17616
 */


#include <drv_ftfx_flash.h>

#if defined(HITSIC_USE_FTFX_FLASH) && (HITSIC_USE_FTFX_FLASH > 0)

#define SYSLOG_TAG  ("FTFX_FLASH")
#define SYSLOG_LVL  (3U)
#include <inc_syslog.h>

#if defined(__cplusplus)
extern "C" {
#endif




	ftfx_security_state_t flash_securityStatus;
	flash_config_t flash_config;
	ftfx_cache_config_t flash_cacheConfig;
	uint32_t flash_blockBaseAddr = 0;
	uint32_t flash_totalSize = 0;
	uint32_t flash_sectorSize = 0;


	status_t FLASH_SimpleInit(void)
	{
	    SYSLOG_I("Init Begin. v%d.%d.%d",HITSIC_VERSION_MAJOR(DRV_FTFX_FLASH_VERSION),
	            HITSIC_VERSION_MINOR(DRV_FTFX_FLASH_VERSION), HITSIC_VERSION_PATCH(DRV_FTFX_FLASH_VERSION));
		/* Return code from each flash driver function */
		status_t result;
		flash_securityStatus = kFTFx_SecurityStateNotSecure;
		/* Clean up Flash, Cache driver Structure*/
		memset(&flash_config, 0, sizeof(flash_config_t));
		memset(&flash_cacheConfig, 0, sizeof(ftfx_cache_config_t));

		/* Setup flash driver structure for device and initialize variables. */
		result = FLASH_Init(&flash_config);

		/* Setup flash cache driver structure for device and initialize variables. */
		result = FTFx_CACHE_Init(&flash_cacheConfig);

		/* Get flash properties*/
		FLASH_GetProperty(&flash_config, kFLASH_PropertyPflash0BlockBaseAddr, &flash_blockBaseAddr);
		FLASH_GetProperty(&flash_config, kFLASH_PropertyPflash0TotalSize, &flash_totalSize);
		FLASH_GetProperty(&flash_config, kFLASH_PropertyPflash0SectorSize, &flash_sectorSize);

		/* Print flash information - PFlash. */
		SYSLOG_D("Flash Information:");
		SYSLOG_D("Flash Size:\t%d KB, Hex: (0x%x)", (flash_totalSize / 1024), flash_totalSize);
		SYSLOG_D("Sector Size:\t%d KB, Hex: (0x%x)", (flash_sectorSize / 1024), flash_sectorSize);
		/* Check security status. */
		result = FLASH_GetSecurityState(&flash_config, &flash_securityStatus);

		/* Print security status. */
		switch (flash_securityStatus)
		{
		case kFTFx_SecurityStateNotSecure:
		    SYSLOG_D("Flash is UNSECURE!");
			break;
		case kFTFx_SecurityStateBackdoorEnabled:
		    SYSLOG_D("Flash is SECURE, BACKDOOR is ENABLED!");
			break;
		case kFTFx_SecurityStateBackdoorDisabled:
		    SYSLOG_D("Flash is SECURE, BACKDOOR is DISABLED!");
			break;
		default:
			break;
		}
		SYSLOG_I("Init Complete.");
		return result;
	}


	uint32_t FLASH_GetPhysicalAddress(uint32_t _addr)
	{
		/*Calculate the destAdrss*/
#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
	/* Note: we should make sure that the sector shouldn't be swap indicator sector*/
		return (flash_blockBaseAddr + (flash_totalSize - (((_addr / flash_sectorSize) + 1) * flash_sectorSize * 2)) + (_addr % flash_sectorSize));
#else
		return (flash_blockBaseAddr + (flash_totalSize - (((_addr / flash_sectorSize) + 1) * flash_sectorSize)) + (_addr % flash_sectorSize));
#endif
	}





	status_t FLASH_AddressRead(uint32_t _addr, void* _buf, uint32_t _byteCnt)
	{
		assert(_buf);
		assert(_byteCnt);
		uint32_t destAdrss = FLASH_GetPhysicalAddress(_addr);
		if (kFTFx_SecurityStateNotSecure != flash_securityStatus)
		{
			return flash_securityStatus;
		}
		if ((_addr + _byteCnt - 1) / flash_sectorSize != _addr / flash_sectorSize)
		{
			return kStatus_FTFx_SizeError;
		}

		/* Post-preparation work about flash Cache/Prefetch/Speculation. */
		FTFx_CACHE_ClearCachePrefetchSpeculation(&flash_cacheConfig, false);

#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
		L1CACHE_InvalidateCodeCache();
#endif /* FSL_FEATURE_HAS_L1CACHE */

#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
		/* Clean the D-Cache before reading the flash data*/
		SCB_CleanInvalidateDCache();
#endif

		for (uint32_t i = 0; i < _byteCnt; i++)
		{
			((uint8_t*)_buf)[i] = *(volatile uint8_t*)(destAdrss + i);
		}
		return kStatus_FTFx_Success;

	}





	status_t FLASH_AddressProgram(uint32_t _addr, void* _buf, uint32_t _byteCnt)
	{
		assert(_buf);
        assert(_byteCnt);
		uint32_t destAdrss = FLASH_GetPhysicalAddress(_addr);
		status_t result;
		uint32_t failAddr, failDat;
		if (kFTFx_SecurityStateNotSecure != flash_securityStatus)
		{
			return flash_securityStatus;
		}
		if ((_addr + _byteCnt - 1) / flash_sectorSize != _addr / flash_sectorSize)
		{
			return kStatus_FTFx_SizeError;
		}

		/* Pre-preparation work about flash Cache/Prefetch/Speculation. */
		FTFx_CACHE_ClearCachePrefetchSpeculation(&flash_cacheConfig, true);

		/* Program user buffer into flash*/
		result = FLASH_Program(&flash_config, destAdrss, (uint8_t*)_buf, _byteCnt);

		/* Verify programming by Program Check command with user margin levels */
		result = FLASH_VerifyProgram(&flash_config, destAdrss, _byteCnt, (const uint8_t*)_buf, kFTFx_MarginValueUser, &failAddr, &failDat);

		return result;
	}




	status_t FLASH_SectorErase(uint32_t _sect)
	{
		uint32_t destAdrss = FLASH_GetPhysicalAddress(_sect * flash_sectorSize);
		status_t result;
		if (kFTFx_SecurityStateNotSecure != flash_securityStatus)
		{
			return flash_securityStatus;
		}

		/* Pre-preparation work about flash Cache/Prefetch/Speculation. */
		FTFx_CACHE_ClearCachePrefetchSpeculation(&flash_cacheConfig, true);

		/* Erase a sector from destAdrss. */
		result = FLASH_Erase(&flash_config, destAdrss, flash_sectorSize, kFTFx_ApiEraseKey);

		/* Verify sector if it's been erased. */
		result = FLASH_VerifyErase(&flash_config, destAdrss, flash_sectorSize, kFTFx_MarginValueUser);

		return result;
	}





	status_t FLASH_SectorRead(uint32_t sector, void* _buf)
	{
		return FLASH_AddressRead(sector * flash_sectorSize, _buf, flash_sectorSize);
	}





	status_t FLASH_SectorProgram(uint32_t sector, void* _buf)
	{
		return FLASH_AddressProgram(sector * flash_sectorSize, _buf, flash_sectorSize);
	}





	status_t FLASH_SectorWrite(uint32_t sector, void* _buf )
	{
		status_t result;
		result = FLASH_SectorErase(sector);
		result = FLASH_AddressProgram(sector * flash_sectorSize, _buf, flash_sectorSize);
		return result;
	}




	/*
	status_t FLASH_Ram2Flash(uint32_t dst, void* src, uint32_t size)
	{
		assert(src);
		if (kFTFx_SecurityStateNotSecure != flash_securityStatus)
		{
			return flash_securityStatus;
		}
		uint8_t* buff = (uint8_t*)malloc(flash_sectorSize * sizeof(uint8_t));
		if (buff == NULL)
		{
			PRINTF("内存分配失败，检查堆大小\r\n");
			return -1;
		}
		uint32_t head, end;
		head = dst / flash_sectorSize;
		end = (dst + size) / flash_sectorSize;
		if (head > 31)head = 31;
		if (end > 31)head = 31;
		uint8_t* src_start = src;
		uint32_t buff_start = 0;
		uint32_t src_size = 0;
		status_t result;
		for (uint32_t i = head; i < end + 1; i++)
		{
			src_start += src_size;
			dst += src_size;
			size -= src_size;
			buff_start = dst % flash_sectorSize;
			if (size >= flash_sectorSize)
			{
				src_size = flash_sectorSize - buff_start;
			}
			else
			{
				src_size = size;
			}
			//读取再写回
			ErrorCheck(result, FLASH_SectorRead(buff, i));
			memcpy(buff + buff_start, src_start, src_size);
			ErrorCheck(result, FLASH_SectorWrite(buff, i));
		}
		free(buff);
		return kStatus_FTFx_Success;
	}





	status_t FLASH_Flash2Ram(void* dst, uint32_t src, uint32_t size)
	{
		assert(dst);
		if (kFTFx_SecurityStateNotSecure != flash_securityStatus)
		{
			return flash_securityStatus;
		}
		uint8_t* buff = (uint8_t*)malloc(flash_sectorSize * sizeof(uint8_t));
		if (buff == NULL)
		{
			PRINTF("内存分配失败，检查堆大小\r\n");
			return -1;
		}
		uint32_t head, end;
		head = src / flash_sectorSize;
		end = (src + size) / flash_sectorSize;
		if (head > 31)head = 31;
		if (end > 31)head = 31;
		uint8_t* dst_start = dst;
		uint32_t buff_start = 0;
		uint32_t dst_size = 0;
		status_t result;
		for (uint32_t i = head; i < end + 1; i++)
		{
			dst_start += dst_size;
			src += dst_size;
			size -= dst_size;
			buff_start = src % flash_sectorSize;
			if (size >= flash_sectorSize)
			{
				dst_size = flash_sectorSize - buff_start;
			}
			else
			{
				dst_size = size;
			}
			//读取
			ErrorCheck(result, FLASH_SectorRead(buff, i));
			memcpy(dst_start, buff + buff_start, dst_size);
		}
		free(buff);
		return kStatus_FTFx_Success;
	}
	*/


	


#ifdef __cplusplus
}
#endif

#endif // ! HITSIC_USE_FTFX_FLASH
