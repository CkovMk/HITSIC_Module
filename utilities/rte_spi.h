/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @file       		spi.h
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
#ifndef RTE_SPI_H
#define RTE_SPI_H

#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief   spi初始化
 *
 * @param  {SPI_Type*} base         : 外设基地址 SPI0 SPI1等等
 * @param  {uint32_t} whichpcs      : 片选 0 1 2 3 等等
 * @param  {uint32_t} baudRate_Bps_ : 波特率单位hz
 * @return {status_t}               : 错误代码
 * @SampleUsage
 *      SPI_SimpleInit(SPI2,0,1*1000000);
 */
status_t SPI_SimpleInit(SPI_Type* base, uint32_t whichpcs, uint32_t baudRate_Bps_);

/**
 * @brief   spi全双工传输
 *
 * @param  {SPI_Type*} base    : 外设基地址
 * @param  {uint32_t} whichpcs : 片选
 * @param  {uint8_t*} rxbuf    : 接收缓存地址 如果不需要接收为NULL
 * @param  {uint8_t*} txbuf    : 发送数据地址 如果不需要发送为NULL
 * @param  {uint32_t} len      : 长度
 * @return {status_t}          : 错误代码
 * @SampleUsage
 *      char txbufff[100];
 *      char rxbufff[100];
 *      SPI_FullDuplexXfer(SPI2,0,rxbufff,txbufff,60);
 *      SPI_FullDuplexXfer(SPI2,0,rxbufff,NULL,90);
 *      SPI_FullDuplexXfer(SPI2,0,NULL,txbufff,70);
 */
status_t SPI_FullDuplexXfer(SPI_Type* base, uint32_t whichpcs, uint8_t* rxbuf, uint8_t* txbuf, uint32_t len);

/*!
 * brief De-initializes the DSPI peripheral. Call this API to disable the DSPI clock.
 * param base DSPI peripheral address.
 */
void DSPI_Deinit(SPI_Type* base);



#ifdef __cplusplus
}
#endif

#endif // ! RTE_SPI_H

