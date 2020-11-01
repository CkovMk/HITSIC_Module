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

#include "hitsic_common.h"
#include "drv_softi2c_port.h"

#define PORTxGet(...) (EXTINT_GetPortInst(__VA_ARGS__))

typedef struct _SI2C_Type {
	GPIO_Type* SDA;
	uint32_t SDA_pin;
	GPIO_Type* SCL;
	uint32_t SCL_pin;
	uint32_t nDELAY;//cpu延时用，一般为20的时候波特率大概是400k
}SI2C_Type;


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	软件i2c初始化
 *
 * @param  {SI2C_Type*} base : SI2C_Type对象句柄
 * @return {status_t}         : 错误代码
 * @SampleUsage
 * 			SI2C_Type i2cs0;
 *			i2cs0.nDELAY = 5;
 *			i2cs0.SCL = GPIOD;
 *			i2cs0.SDA = GPIOD;
 *			i2cs0.SDA_pin = 9;
 *			i2cs0.SCL_pin = 8;
 *			SI2C_Init(&i2cs0);
 */
status_t SI2C_Init(SI2C_Type* base);

/**
 *
 * @param  {SI2C_Type*} p        : SI2C_Type对象句柄
 * @param  {uint8_t} SlaveAddress : 从机地址
 * @param  {uint8_t} reg          : 从机寄存器
 * @param  {uint8_t*} data        : 缓存数组指针
 * @param  {uint32_t} size        : 数据大小
 * @return {status_t}             : 错误代码
 * @SampleUsage
 * 			uint8_t buf[10];
 * 			SI2C_MasterReadBlocking(i2cs0,0x68,0x6A,buf,1);
 */
status_t SI2C_MasterReadBlocking(SI2C_Type* p, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size);

/**
 *
 * @param  {SI2C_Type*} p        : SI2C_Type对象句柄
 * @param  {uint8_t} SlaveAddress : 从机地址
 * @param  {uint8_t} reg          : 从机寄存器
 * @param  {uint8_t*} data        : 数组指针
 * @param  {uint32_t} size        : 数据大小
 * @return {status_t}             : 错误代码
 * @SampleUsage
 * 			uint8_t buf[10];
 * 			SI2C_MasterWriteBlocking(i2cs0,0x68,0x6A,buf,1);
 */
status_t SI2C_MasterWriteBlocking(SI2C_Type* p, uint8_t SlaveAddress, uint8_t reg, uint8_t* data, uint32_t size);

/**
 * @brief	内部使用，返回gpio对应的port
 *
 * @param  {GPIO_Type*} GPIOx :
 * @return {PORT_Type*}       :
 */
PORT_Type* PORTxGet(GPIO_Type* GPIOx);

////软件IIC所有操作函数
//static void IIC_Delay(SI2C_Type* p);				//IIC延时函数
//static void IIC_Delay2(SI2C_Type* p);				//IIC延时函数
//static void IIC_Start(SI2C_Type* p);				//发送IIC开始信号
//static void IIC_Stop(SI2C_Type* p);	  			//发送IIC停止信号
//static void IIC_Send_Byte(SI2C_Type* p,uint8_t txd);			//IIC发送一个字节
//static uint8_t IIC_Read_Byte(SI2C_Type* p,uint8_t ack);//IIC读取一个字节
//static uint8_t IIC_Wait_Ack(SI2C_Type* p); 				//IIC等待ACK信号
//static void IIC_Ack(SI2C_Type* p);					//IIC发送ACK信号
//static void IIC_NAck(SI2C_Type* p);				//IIC不发送ACK信号
//static void SCL_out(SI2C_Type* base);
//static void SDA_out(SI2C_Type* base);
//static void SDA_in(SI2C_Type* base);
//static void SCL_in(SI2C_Type* base);
//static void SDA_H(SI2C_Type* base);
//static void SDA_L(SI2C_Type* base);
//static void SCL_H(SI2C_Type* base);
//static void SCL_L(SI2C_Type* base);
//static uint32_t SDA_val(SI2C_Type* base);
//static uint32_t SCL_val(SI2C_Type* base);



#ifdef __cplusplus
}
#endif


#endif // ! RTE_SI2C_H

