/**
 * Copyright 2018 - 2019 HITSIC
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

/**************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @file       		flash_driver.h
 * @company	      哈工大智能车创新俱乐部
 * @author     		肖日涛 qq1761690868
 * @version    		v1.1
 * @Software 		  IAR 7.7+
 * @Target core		K66
 * @date       		ver1.0 2019年7月25日
 * @date       		ver1.1 2019年9月22日
 * @note：按字节读写flash和按扇区读写二选一，
 *        按字节读写的函数是基于按扇区读写的二次封装。
 * @note：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
**************************************************************************************************/
#pragma once
#ifndef _DRV_FTFX_FLASH_H_
#define _DRV_FTFX_FLASH_H_
#include "inc_fsl_mk66f18.h"
#include "inc_stdlib.h"
#include "hitsic_common.h"

#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
#include "fsl_cache.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

	extern flash_config_t flash_config;
	extern uint32_t flash_blockBaseAddr;
	extern uint32_t flash_totalSize;
	extern uint32_t flash_sectorSize;


	/**
	 * @brief 初始化flash
	 *
	 * @param  {void} undefined :
	 * @return {status_t}       : 错误代码
	 * @SampleUsage  	status_t result;
	 *					result = FLASH_SimpleInit();
	 */
	status_t FLASH_SimpleInit(void);



	/**
	 * @brief 将给定的逻辑地址转换为物理地址
	 * 
	 * @param _addr  待转换的逻辑地址
	 * @return uint32_t 转换后的物理地址
	 */
	uint32_t FLASH_GetPhysicalAddress(uint32_t _addr);





	/**
	 * @brief   flash地址读取
	 * 
	 * 注意：K66的Flash仅支持8字节对齐操作。
	 *
	 * @param  {uint8_t*} dst     : 读出数据的目标地址
	 * @param  {uint32_t} sectors : 扇区号，0是k66倒数一块扇区，1是倒数第2块扇区，以此类推
	 * @return {status_t}         : 错误代码
	 * @SampleUsage		status_t result;
	 *					uint8_t a[4096] = {1};
	 *					uint8_t b[4096] = {0};
	 *					result = FLASH_SectorWrite(a,0);
	 *					result = FLASH_SectorRead(b,0);
	 */
	status_t FLASH_AddressRead(uint32_t _addr, void* _buf, uint32_t _byteCnt);




	/**
	 * @brief   flash地址写入
	 * 
	 * 注意：K66的Flash仅支持8字节对齐操作。
	 *
	 * @param  {uint8_t*} dst     : 读出数据的目标地址
	 * @param  {uint32_t} sectors : 扇区号，0是k66倒数一块扇区，1是倒数第2块扇区，以此类推
	 * @return {status_t}         : 错误代码
	 * @SampleUsage		status_t result;
	 *					uint8_t a[4096] = {1};
	 *					uint8_t b[4096] = {0};
	 *					result = FLASH_SectorWrite(a,0);
	 *					result = FLASH_SectorRead(b,0);
	 */
	status_t FLASH_AddressProgram(uint32_t _addr, void* _buf, uint32_t _byteCnt);




	/**
	 * @brief   flash扇区擦除
	 *
	 * @param  {uint8_t*} dst     : 读出数据的目标地址
	 * @param  {uint32_t} sectors : 扇区号，0是k66倒数一块扇区，1是倒数第2块扇区，以此类推
	 * @return {status_t}         : 错误代码
	 * @SampleUsage		status_t result;
	 *					uint8_t a[4096] = {1};
	 *					uint8_t b[4096] = {0};
	 *					result = FLASH_SectorWrite(a,0);
	 *					result = FLASH_SectorRead(b,0);
	 */
	status_t FLASH_SectorErase(uint32_t _sect);




	/**
	 * @brief   flash扇区读取
	 *
	 * @param  {uint8_t*} dst     : 读出数据的目标地址
	 * @param  {uint32_t} sectors : 扇区号，0是k66倒数一块扇区，1是倒数第2块扇区，以此类推
	 * @return {status_t}         : 错误代码
	 * @SampleUsage		status_t result;
	 *					uint8_t a[4096] = {1};
	 *					uint8_t b[4096] = {0};
	 *					result = FLASH_SectorWrite(a,0);
	 *					result = FLASH_SectorRead(b,0);
	 */
	status_t FLASH_SectorRead(uint32_t sectors, void* _buf);



	/**
	 * @brief   flash扇区写入
	 *
	 * @param  {uint8_t*} dst     : 读出数据的目标地址
	 * @param  {uint32_t} sectors : 扇区号，0是k66倒数一块扇区，1是倒数第2块扇区，以此类推
	 * @return {status_t}         : 错误代码
	 */
	status_t FLASH_SectorProgram(uint32_t sector, void* _buf);

	/**
	 * @brief   flash扇区擦除并写入
	 *
	 * @param  {uint8_t*} src     : 写入数据源地址
	 * @param  {uint32_t} sectors : 扇区号，0是k66倒数一块扇区，1是倒数第2块扇区，以此类推
	 * @return {status_t}         : 错误代码
	 */
	status_t FLASH_SectorWrite(uint32_t sector, void* _buf);



	/**
	 * @brief   flash按字节写
	 *
	 * 写的时候最好是准备好一大批连续数据，再连续写，节省flash寿命。
	 *
	 * @param  {uint32_t} dst  :目标flash字节地址，flash字节空间地址范围 0 - ( 32 * 4096 - 1 )，也就是能存128k的数据
	 * @param  {void*} src     :写入数据源地址（也就是内存地址）
	 * @param  {uint32_t} size :写入数据的大小
	 * @return {status_t}      :错误代码
	 * @SampleUsage  	status_t result;
	 *					float a = 1.23456789;
	 *					float b = 0;
	 *					result = FLASH_Ram2Flash(0,&a,sizeof(float));
	 *					result = FLASH_Flash2Ram(&a,0,sizeof(float));
	 *					if(a!=b){PRINTF("error\n");}
	 */
	//status_t FLASH_Ram2Flash(uint32_t dst, void* src, uint32_t size);

	/**
	 * @brief   flash按字节读
	 *
	 * @param  {void*} dst     : 读出的数据目标地址（也就是内存地址）
	 * @param  {uint32_t} src  : 数据源flash字节地址，flash字节空间地址范围 0 - ( 32 * 4096 - 1 )，也就是能存128k的数据
	 * @param  {uint32_t} size : 读取数据的大小
	 * @return {status_t}      : 错误代码
	 * @SampleUsage  	status_t result;
	 *					float a = 1.23456789;
	 *					flaot b = 0;
	 *					result = FLASH_Ram2Flash(0,&a,sizeof(float));
	 *					result = FLASH_Flash2Ram(&a,0,sizeof(float));
	 *					if(a!=b){PRINTF("error\n");}
	 */
	//status_t FLASH_Flash2Ram(void* dst, uint32_t src, uint32_t size);





#ifdef __cplusplus
}
#endif

#endif // ! _DRV_FTFX_FLASH_H_
