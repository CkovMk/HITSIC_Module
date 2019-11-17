/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @file       		i2c_soft.h软件模拟i2c
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
#ifndef RTE_SI2C_H
#define RTE_SI2C_H

#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"
#include "clock_config.h"
#include "pin_mux.h"

typedef struct _si2c_master_t {
	GPIO_Type* SDA;
	uint32_t SDA_pin;
	GPIO_Type* SCL;
	uint32_t SCL_pin;
	uint32_t nDELAY;//cpu延时用，一般为20的时候波特率大概是400k
}si2c_master_t;


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	软件i2c初始化
 *
 * @param  {si2c_master_t*} base : si2c_master_t对象句柄
 * @return {status_t}         : 错误代码
 * @SampleUsage
 * 			si2c_master_t i2cs0;
 *			i2cs0.nDELAY = 5;
 *			i2cs0.SCL = GPIOD;
 *			i2cs0.SDA = GPIOD;
 *			i2cs0.SDA_pin = 9;
 *			i2cs0.SCL_pin = 8;
 *			SI2C_Init(&i2cs0);
 */
status_t SI2C_Init(si2c_master_t* base);

/**
 *
 * @param  {si2c_master_t*} p        : si2c_master_t对象句柄
 * @param  {uint8_t} SlaveAddress : 从机地址
 * @param  {uint8_t} reg          : 从机寄存器
 * @param  {uint8_t*} data        : 缓存数组指针
 * @param  {uint32_t} size        : 数据大小
 * @return {status_t}             : 错误代码
 * @SampleUsage
 * 			uint8_t buf[10];
 * 			SI2C_MasterReadBlocking(i2cs0,0x68,0x6A,buf,1);
 */
status_t SI2C_MasterReadBlocking(si2c_master_t* p, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size);

/**
 *
 * @param  {si2c_master_t*} p        : si2c_master_t对象句柄
 * @param  {uint8_t} SlaveAddress : 从机地址
 * @param  {uint8_t} reg          : 从机寄存器
 * @param  {uint8_t*} data        : 数组指针
 * @param  {uint32_t} size        : 数据大小
 * @return {status_t}             : 错误代码
 * @SampleUsage
 * 			uint8_t buf[10];
 * 			SI2C_MasterWriteBlocking(i2cs0,0x68,0x6A,buf,1);
 */
status_t SI2C_MasterWriteBlocking(si2c_master_t* p, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size);

/**
 * @brief	内部使用，返回gpio对应的port
 *
 * @param  {GPIO_Type*} GPIOx :
 * @return {PORT_Type*}       :
 */
PORT_Type* PORTxGet(GPIO_Type* GPIOx);

////软件IIC所有操作函数
//static void IIC_Delay(si2c_master_t* p);				//IIC延时函数
//static void IIC_Delay2(si2c_master_t* p);				//IIC延时函数
//static void IIC_Start(si2c_master_t* p);				//发送IIC开始信号
//static void IIC_Stop(si2c_master_t* p);	  			//发送IIC停止信号
//static void IIC_Send_Byte(si2c_master_t* p,uint8_t txd);			//IIC发送一个字节
//static uint8_t IIC_Read_Byte(si2c_master_t* p,uint8_t ack);//IIC读取一个字节
//static uint8_t IIC_Wait_Ack(si2c_master_t* p); 				//IIC等待ACK信号
//static void IIC_Ack(si2c_master_t* p);					//IIC发送ACK信号
//static void IIC_NAck(si2c_master_t* p);				//IIC不发送ACK信号
//static void SCL_out(si2c_master_t* base);
//static void SDA_out(si2c_master_t* base);
//static void SDA_in(si2c_master_t* base);
//static void SCL_in(si2c_master_t* base);
//static void SDA_H(si2c_master_t* base);
//static void SDA_L(si2c_master_t* base);
//static void SCL_H(si2c_master_t* base);
//static void SCL_L(si2c_master_t* base);
//static uint32_t SDA_val(si2c_master_t* base);
//static uint32_t SCL_val(si2c_master_t* base);



#ifdef __cplusplus
}
#endif


#endif // ! RTE_SI2C_H

