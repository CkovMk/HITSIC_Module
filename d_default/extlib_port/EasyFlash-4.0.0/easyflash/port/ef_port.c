/*
 * This file is part of the EasyFlash Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-01-16
 */

#include <easyflash.h>
#include <stdarg.h>

#include "hitsic_common.h"
#include "drv_ftfx_flash.hpp"
     
int status = (0x50 << 24);

/* default environment variables set for user */
static const ef_env default_env_set[] = {
	//{"board_name", "hitsic_mk66f18", 0},
	{"ef_status", &status, sizeof(int32_t)},
};

/**
 * Flash port for hardware initialize.
 *
 * @param default_env default ENV set for user
 * @param default_env_size default ENV size
 *
 * @return result
 */
EfErrCode ef_port_init(ef_env const **default_env, size_t *default_env_size)
{
	EfErrCode result = EF_NO_ERR;

	*default_env = default_env_set;
	*default_env_size = sizeof(default_env_set) / sizeof(default_env_set[0]);

	return result;
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
EfErrCode ef_port_read(uint32_t addr, uint32_t *buf, size_t size)
{
	EfErrCode result = EF_NO_ERR;

	EF_ASSERT(size % (4) == 0);

	/* You can add your code under here. */
	EF_INFO("Verbose: Read Addr 0x%8.8x, Size %4.4d bytes\n", addr, size);
	if (kStatus_FTFx_Success != FLASH_AddressRead(addr, (uint8_t *)buf, size))
	{
		result = EF_READ_ERR;
		EF_INFO("Warning: Read Failed !\n");
	}

	return result;
}

/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
EfErrCode ef_port_erase(uint32_t addr, size_t size)
{
	EfErrCode result = EF_NO_ERR;

	/* make sure the start address is a multiple of EF_ERASE_MIN_SIZE */
	EF_ASSERT(addr % EF_ERASE_MIN_SIZE == 0);

	/* You can add your code under here. */
	EF_ASSERT(size % EF_ERASE_MIN_SIZE == 0);
	uint32_t sectorNum = size / EF_ERASE_MIN_SIZE;
	for (uint32_t i = 0; i < sectorNum; ++i)
	{
		EF_INFO("Verbose: Erase Addr 0x%8.8x, Size %4.4d bytes\n", addr, size);
		if (kStatus_FTFx_Success != FLASH_SectorErase(addr / EF_ERASE_MIN_SIZE + i))
		{
			result = EF_ERASE_ERR;
			EF_INFO("Warning: Erase Failed !\n");
		}
	}

	return result;
}
/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
EfErrCode ef_port_write(uint32_t addr, const uint32_t *buf, size_t size)
{
	EfErrCode result = EF_NO_ERR;

	EF_ASSERT(size % (EF_WRITE_GRAN/8) == 0);

	/* You can add your code under here. */
	EF_INFO("Verbose: Write Addr 0x%8.8x, Size %4.4d bytes\n", addr, size);
	if (kStatus_FTFx_Success != FLASH_AddressProgram(addr, (uint8_t *)buf, size))
	{
		result = EF_WRITE_ERR;
		EF_INFO("Warning: Write Failed !\n");
	}

	return result;
}

/**
 * lock the ENV ram cache
 */
void ef_port_env_lock(void)
{

	/* You can add your code under here. */
	HAL_EnterCritical();
}

/**
 * unlock the ENV ram cache
 */
void ef_port_env_unlock(void)
{

	/* You can add your code under here. */
	HAL_ExitCritical();
}

/**
 * This function is print flash debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 *
 */
void ef_log_debug(const char *file, const long line, const char *format, ...)
{

#ifdef PRINT_DEBUG

	va_list args;

	/* args point to the first variable parameter */
	va_start(args, format);

	/* You can add your code under here. */
	vprintf(format, args);

	va_end(args);

#endif
}

/**
 * This function is print flash routine info.
 *
 * @param format output format
 * @param ... args
 */
void ef_log_info(const char *format, ...)
{
	va_list args;

	/* args point to the first variable parameter */
	va_start(args, format);

	/* You can add your code under here. */
	vprintf(format, args);

	va_end(args);
}
/**
 * This function is print flash non-package info.
 *
 * @param format output format
 * @param ... args
 */
void ef_print(const char *format, ...)
{
	va_list args;

	/* args point to the first variable parameter */
	va_start(args, format);

	/* You can add your code under here. */
	vprintf(format, args);

	va_end(args);
}
