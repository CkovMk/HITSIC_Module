/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @file       		i2c.h
 * @company	        哈工大智能车创新俱乐部
 * @author     		肖日涛 qq1761690868
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		k66
 * @date       		ver1.0 2019年9月22日
 * @note：
 * @note：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#pragma once
#ifndef RTE_I2C_H
#define RTE_I2C_H
#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"



#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief   i2c初始化
 *
 * @param  {I2C_Type*} base         : I2C外设基地址比如 I2C0,I2C1
 * @param  {uint32_t} baudRate_Bps_ : 波特率，单位hz
 * @return {status_t}               : 错误代码
 * @SampleUsage
 *      I2C_SimpleInit(I2C0,400*1000);
 */
status_t I2C_SimpleInit(I2C_Type* base, uint32_t baudRate_Bps_);

/**
 *
 * @param  {I2C_Type*} base       : 已经初始化I2C外设基地址比如 I2C0,I2C1
 * @param  {uint8_t} SlaveAddress : 从机地址
 * @param  {uint8_t} reg          : 从机的寄存器
 * @param  {uint8_t*} data        : 读取数据的存放地址
 * @param  {uint32_t} size        : 读取数据的大小
 * @return {status_t}             : 错误代码
 * @SampleUsage
 * 			uint8_t buf[10];
 * 			I2C_SimpleReadBlocking(I2C0,0x68,0x6A,buf,1);
 */
status_t I2C_SimpleReadBlocking(I2C_Type* base, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size);

/**
 *
 * @param  {I2C_Type*} base       : 已经初始化I2C外设基地址比如 I2C0,I2C1
 * @param  {uint8_t} SlaveAddress : 从机地址
 * @param  {uint8_t} reg          : 从机的寄存器
 * @param  {uint8_t*} data        : 写入数据的存放地址
 * @param  {uint32_t} size        : 写入数据的大小
 * @return {status_t}             : 错误代码
 * @SampleUsage
 * 			uint8_t buf[10];
 * 			I2C_SimpleWriteBlocking(I2C0,0x68,0x6A,buf,1);
 */
status_t I2C_SimpleWriteBlocking(I2C_Type* base, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size);


/*!
 * @brief De-initializes the I2C master peripheral. Call this API to gate the I2C clock.
 * The I2C master module can't work unless the I2C_MasterInit is called.
 * @param base I2C base pointer
 */
static void I2C_SimpleDeinit(I2C_Type* base)
{
	I2C_MasterDeinit(base);
}




#ifdef __cplusplus
}
#endif


#endif // ! RTE_I2C_H

