/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		uart.h
 * @company	        哈工大智能车创新俱乐部
 * @author     		肖日涛 qq1761690868
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		k66
 * @date       		ver1.0 2019年9月22日
 * @note：仅实现了串口的polling，其他中断方式和dma方式自行参考官方例程开发
 * @note：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/
#pragma once

#ifndef RTE_USRT_H
#define RTE_USRT_H
#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化串口
 * 
 * @param  {UART_Type*} base        : 串口基地址，可选的有UART0 UART1 UART2 等等
 * @param  {uint32_t} baudRate_Bps_ : 波特率，常用的为115200
 * @return {status_t}               : 返回错误代码，0为成功初始化
 * @SampleUsage 
 *      UART_SimpleInit(UART0,115200);
 */
status_t UART_SimpleInit(UART_Type* base, uint32_t baudRate_Bps_);

/**
 * @brief   串口printf，用法同printf
 * 
 * @param  {UART_Type*} base : 串口基地址，可选的有UART0 UART1 UART2 等等
 * @param  {char*} Format    : 格式化字符串
 * @param  {...} undefined   : 可变参数列表
 * @return {int}             : 字符串长度
 * @SampleUsage    
 *      	UART_Printf(UART0, "ax=%f\r\n", icm.acc.x);
 *			UART_Printf(UART0, "ay=%f\r\n", icm.acc.y);
 *			UART_Printf(UART0, "az=%f\r\n", icm.acc.z);
 *			UART_Printf(UART0, "gx=%f\r\n", icm.gyro.x);
 *			UART_Printf(UART0, "gy=%f\r\n", icm.gyro.y);
 *			UART_Printf(UART0, "gz=%f\r\n", icm.gyro.z);
 *			UART_Printf(UART0, "T =%f\r\n", icm.temp);
 *			UART_Printf(UART0, "\r\n");
 */
int UART_Printf(UART_Type* base, const char* Format, ...);

/**
 * @brief Read RX data register using a blocking method.
 *
 * This function polls the RX register, waits for the RX register to be full or for RX FIFO to
 * have data, and reads data from the TX register.
 * 
 * @param  {UART_Type*} base : 串口基地址，可选的有UART0 UART1 UART2 等等
 * @param  {uint8_t*} data   : 接收的数组的指针
 * @param  {size_t} length   : 接收的数据的长度
 * @return {status_t}        : 错误代码
 * @retval kStatus_UART_RxHardwareOverrun Receiver overrun occurred while receiving data.
 * @retval kStatus_UART_NoiseError A noise error occurred while receiving data.
 * @retval kStatus_UART_FramingError A framing error occurred while receiving data.
 * @retval kStatus_UART_ParityError A parity error occurred while receiving data.
 * @retval kStatus_Success Successfully received all data. 
 * @SampleUsage
 *		char ch;
 *		UART_ReadBlocking(UART0, &ch, 1);
 */
extern status_t UART_ReadBlocking(UART_Type* base, uint8_t* data, size_t length);

/**
 * @brief Writes to the TX register using a blocking method.
 *
 * This function polls the TX register, waits for the TX register to be empty or for the TX FIFO
 * to have room and writes data to the TX buffer.
 *
 * @note This function does not check whether all data is sent out to the bus.
 * Before disabling the TX, check kUART_TransmissionCompleteFlag to ensure that the TX is
 * finished.
 * 
 * @param  {UART_Type*} base : 串口基地址，可选的有UART0 UART1 UART2 等等
 * @param  {uint8_t*} data   : 发送的数组的指针
 * @param  {size_t} length   : 发送的数组的长度
 * @SampleUsage 
 *		char stringbuff[20] = "1234567890qwer";
 *		UART_WriteBlocking(UART0,stringbuff,20); 
 */
extern void UART_WriteBlocking(UART_Type* base, const uint8_t* data, size_t length);

/*!
 * brief Deinitializes a UART instance.
 *
 * This function waits for TX complete, disables TX and RX, and disables the UART clock.
 *
 * param base UART peripheral base address.
 */
extern void UART_Deinit(UART_Type* base);

#ifdef __cplusplus
}
#endif


#endif // ! RTE_USRT_H

