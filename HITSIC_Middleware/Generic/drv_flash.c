#include "drv_flash.h"

/*
 * flash_driver.c
 *
 *  Created on: 2019年7月25日
 *      Author: 17616
 */

#define ErrorCheck(result,expression)  result = expression;if(0!= result)return result

ftfx_security_state_t securityStatus;
flash_config_t s_flashDriver;
ftfx_cache_config_t s_cacheDriver;
uint32_t pflashBlockBase = 0;
uint32_t pflashTotalSize = 0;
uint32_t pflashSectorSize = 0;
status_t FLASH_Ram2Flash(uint32_t dst, void* src, uint32_t size)
{
	assert(src);
	if (kFTFx_SecurityStateNotSecure != securityStatus)
	{
		return securityStatus;
	}
	uint8_t* buff = (uint8_t*)malloc(pflashSectorSize * sizeof(uint8_t));
	if (buff == NULL)
	{
		PRINTF("内存分配失败，检查堆大小\r\n");
		return -1;
	}
	uint32_t head, end;
	head = dst / pflashSectorSize;
	end = (dst + size) / pflashSectorSize;
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
		buff_start = dst % pflashSectorSize;
		if (size >= pflashSectorSize)
		{
			src_size = pflashSectorSize - buff_start;
		}
		else
		{
			src_size = size;
		}
		//读取再写回
		ErrorCheck(result, FLASH_SectorRead(buff, pflashSectorSize, i));
		memcpy(buff + buff_start, src_start, src_size);
		ErrorCheck(result, FLASH_SectorWrite(buff, pflashSectorSize, i));
	}
	free(buff);
	return kStatus_FTFx_Success;
}

status_t FLASH_Flash2Ram(void* dst, uint32_t src, uint32_t size)
{
	assert(dst);
	if (kFTFx_SecurityStateNotSecure != securityStatus)
	{
		return securityStatus;
	}
	uint8_t* buff = (uint8_t*)malloc(pflashSectorSize * sizeof(uint8_t));
	if (buff == NULL)
	{
		PRINTF("内存分配失败，检查堆大小\r\n");
		return -1;
	}
	uint32_t head, end;
	head = src / pflashSectorSize;
	end = (src + size) / pflashSectorSize;
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
		buff_start = src % pflashSectorSize;
		if (size >= pflashSectorSize)
		{
			dst_size = pflashSectorSize - buff_start;
		}
		else
		{
			dst_size = size;
		}
		//读取
		ErrorCheck(result, FLASH_SectorRead(buff, pflashSectorSize, i));
		memcpy(dst_start, buff + buff_start, dst_size);
	}
	free(buff);
	return kStatus_FTFx_Success;
}

status_t FLASH_SimpleInit(void)
{
	/* Return code from each flash driver function */
	status_t result;
	securityStatus = kFTFx_SecurityStateNotSecure;
	/* Clean up Flash, Cache driver Structure*/
	memset(&s_flashDriver, 0, sizeof(flash_config_t));
	memset(&s_cacheDriver, 0, sizeof(ftfx_cache_config_t));

	/* Setup flash driver structure for device and initialize variables. */
	ErrorCheck(result, FLASH_Init(&s_flashDriver));

	/* Setup flash cache driver structure for device and initialize variables. */
	ErrorCheck(result, FTFx_CACHE_Init(&s_cacheDriver));

	/* Get flash properties*/
	FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0BlockBaseAddr, &pflashBlockBase);
	FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0TotalSize, &pflashTotalSize);
	FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0SectorSize, &pflashSectorSize);

	/* Print flash information - PFlash. */
	PRINTF("\r\n PFlash Information: ");
	PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
	PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);
	/* Check security status. */
	ErrorCheck(result, FLASH_GetSecurityState(&s_flashDriver, &securityStatus));

	/* Print security status. */
	switch (securityStatus)
	{
	case kFTFx_SecurityStateNotSecure:
		PRINTF("\r\n Flash is UNSECURE!");
		break;
	case kFTFx_SecurityStateBackdoorEnabled:
		PRINTF("\r\n Flash is SECURE, BACKDOOR is ENABLED!");
		break;
	case kFTFx_SecurityStateBackdoorDisabled:
		PRINTF("\r\n Flash is SECURE, BACKDOOR is DISABLED!");
		break;
	default:
		break;
	}
	PRINTF("\r\n");
	return 0;
}

uint32_t FLASH_GetSectorSize(void)
{
	return pflashSectorSize;
}

status_t FLASH_SectorRead(uint8_t* dst, uint32_t size, uint32_t sectors)
{
	assert(dst);
	uint32_t destAdrss;
	if (kFTFx_SecurityStateNotSecure != securityStatus)
	{
		return securityStatus;
	}
	if (size > pflashSectorSize)
	{
		size = pflashSectorSize;
	}
	/*Calculate the destAdrss*/
#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
	/* Note: we should make sure that the sector shouldn't be swap indicator sector*/
	destAdrss = pflashBlockBase + (pflashTotalSize - ((sectors + 1) * pflashSectorSize * 2));
#else
	destAdrss = pflashBlockBase + (pflashTotalSize - ((sectors + 1) * pflashSectorSize));
#endif

	/* Post-preparation work about flash Cache/Prefetch/Speculation. */
	FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, false);

#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
	L1CACHE_InvalidateCodeCache();
#endif /* FSL_FEATURE_HAS_L1CACHE */

#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
	/* Clean the D-Cache before reading the flash data*/
	SCB_CleanInvalidateDCache();
#endif

	for (uint32_t i = 0; i < size; i++)
	{
		dst[i] = *(volatile uint8_t*)(destAdrss + i);
	}
	return kStatus_FTFx_Success;
}

status_t FLASH_SectorWrite(uint8_t* src, uint32_t size, uint32_t sectors)
{
	assert(src);
	uint32_t destAdrss;
	status_t result;
	uint32_t failAddr, failDat;
	if (kFTFx_SecurityStateNotSecure != securityStatus)
	{
		return securityStatus;
	}
	if (size > pflashSectorSize)
	{
		size = pflashSectorSize;
	}
	/*Calculate the destAdrss*/
#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
	/* Note: we should make sure that the sector shouldn't be swap indicator sector*/
	destAdrss = pflashBlockBase + (pflashTotalSize - ((sectors + 1) * pflashSectorSize * 2));
#else
	destAdrss = pflashBlockBase + (pflashTotalSize - ((sectors + 1) * pflashSectorSize));
#endif

	/* Pre-preparation work about flash Cache/Prefetch/Speculation. */
	FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, true);

	/* Erase a sector from destAdrss. */
	ErrorCheck(result, FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFTFx_ApiEraseKey));

	/* Verify sector if it's been erased. */
	ErrorCheck(result, FLASH_VerifyErase(&s_flashDriver, destAdrss, pflashSectorSize, kFTFx_MarginValueUser));

	/* Program user buffer into flash*/
	ErrorCheck(result, FLASH_Program(&s_flashDriver, destAdrss, (uint8_t*)src, size));

	/* Verify programming by Program Check command with user margin levels */
	ErrorCheck(result, FLASH_VerifyProgram(&s_flashDriver, destAdrss, size, (const uint8_t*)src, kFTFx_MarginValueUser, &failAddr, &failDat));

	return kStatus_FTFx_Success;
}