/******************************************************************************************************************************
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
***************************************************************************************************************************/
#pragma once
#ifndef DRV_FLASH_H
#define DRV_FLASH_H
#include "include.h" 
#include "clock_config.h"
#include "fsl_debug_console.h"

#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
#include "fsl_cache.h"
#endif
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
status_t FLASH_Ram2Flash(uint32_t dst, void* src, uint32_t size);

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
status_t FLASH_Flash2Ram(void* dst, uint32_t src, uint32_t size);

/**
 * @brief 返回flash一块扇区多少字节，初始flash之后才知道多少字节
 *
 * @param  {void} undefined :
 * @return {uint32_t}       : 返回flash一块扇区多少字节，一般是4096
 */
uint32_t FLASH_GetSectorSize(void);

/**
 * @brief   flash扇区读
 *
 * @param  {uint8_t*} dst     : 读出数据的目标地址
 * @param  {uint32_t} size    : 数据大小，不能超过扇区大小
 * @param  {uint32_t} sectors : 扇区号，0是k66倒数一块扇区，1是倒数第2块扇区，以此类推
 * @return {status_t}         : 错误代码
 * @SampleUsage		status_t result;
 *					uint8_t a[4096] = {1};
 *					uint8_t b[4096] = {0};
 *					result = FLASH_SectorWrite(a,4096,0);
 *					result = FLASH_SectorRead(b,4096,0);
 */
status_t FLASH_SectorRead(uint8_t* dst, uint32_t size, uint32_t sectors);

/**
 * @brief   flash扇区写
 *
 * @param  {uint8_t*} src     : 写入数据源地址
 * @param  {uint32_t} size    : 写入大小，不能超过扇区大小
 * @param  {uint32_t} sectors : 扇区号，0是k66倒数一块扇区，1是倒数第2块扇区，以此类推
 * @return {status_t}         : 错误代码
 * @SampleUsage		status_t result;
 *					uint8_t a[4096] = {1};
 *					uint8_t b[4096] = {0};
 *					result = FLASH_SectorWrite(a,4096,0);
 *					result = FLASH_SectorRead(b,4096,0);
 */
status_t FLASH_SectorWrite(uint8_t* src, uint32_t size, uint32_t sectors);

#endif /* DRV_FLASH_H */