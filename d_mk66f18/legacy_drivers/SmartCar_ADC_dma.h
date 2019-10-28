/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @brief           用dma采集adc，不占用cpu时间。
 * @file       		SmartCar_ADC_dma.h
 * @company	        哈工大智能车创新俱乐部
 * @author     		肖日涛 (QQ:1761690868)
 * @version    		v1.0
 * @Software
 * @Target_core		k66
 * @date       		ver1.0 2019年10月3日 by 肖日涛
 * @date       		ver1.0.1 2019.10.20 fixbug
 *
 * @note：不知道怎么用看示例
 * @note：如果需要ADC1上的dma采集的话依葫芦画瓢即可
 * @note：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/
#ifndef __AD_DMA_H__
#define __AD_DMA_H__
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_adc16.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "board.h"
#include "list_cycle.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "SmartCar_Camera.h"
#define ADC_DMA_ADC_BASEADDR ADC0	//本驱动只能选一个ADC0
#define ADC_DMA_DMAMUX_BASEADDR DMAMUX0//没得选，就一个
#define ADC_DMA_DMA_BASEADDR DMA0		//选DMA0，其实可以不用重命名，没得选，就一个   
//#define ADC_DMA_DMA_CHANNEL_adc0 0U			//DMA通道，选0，如果有多个要区分,注意不要和摄像头冲突
#define ADC_DMA_DMA_CHANNEL_adc0 (DMA_CHANNEL_SELECT+1)			//DMA通道，选0，如果有多个要区分,注意不要和摄像头冲突
#define ADC_DMA_DMA_ADC_SOURCE_adc0 40U     //DMA触发源，这里为ADC0的触发源，也要去数据手册查找。adc1的触发源为41
#define ADC_DMA_ADC_SAMPLE_COUNT 32U  //每次采样多少次
//#define ADC_DMA_resolution kADC16_ResolutionSE8Bit  //设置为8位adc，满值是2^8-1 = 255
//#define ADC_DMA_resolution kADC16_ResolutionSE12Bit //设置为12位adc，满值是2^12-1 = 4095
#define ADC_DMA_resolution kADC16_Resolution16Bit   //设置为16位adc，满值是2^16-1 = 65535

extern volatile uint8_t if_EDMA_Init;
extern volatile uint8_t if_DMAMUX_Init;
typedef struct _ADC_DMA
{
	uint8_t TransferDone;
	ADC_Type* ADCx;				//只能选一个ADC0
	uint8_t channelNumber;		//ADC通道
	uint8_t channelGroup;		//没特殊情况都选0
	uint32_t SampleDataArray[ADC_DMA_ADC_SAMPLE_COUNT];//数据缓存，dma采集的数据丢在这里
	void (*userCallBack)(struct _ADC_DMA* self);	//采集完成的回调函数，自行适配
} AD_DMA_t;
/**
 * @brief	AD_DMA初始化，初始化会覆盖adc polling方式的初始化
 * 
 * @param  {void} undefined : 
 * @return {int}            : 错误代码
 */
int AD_DMA_Init(void);//初始化
/**
 * @brief	将需要采集的引脚添加到采集队列中（采集队列最大容量为32），之后会自动开始一次采集。
 * 		完成之后需要再次手动添加到采集队列才能采集下一次。
 * 
 * @param  {AD_DMA_t*} s : AD_DMA_t指针
 * @return {int}         : 错误代码（-10表示采集队列满了）
 */
int AD_DMA_StartTransfer(AD_DMA_t* s);
/* -------------------------------------------------------------------------- */
/* -------------------------------示例--------------------------------------- */
/* -------------------------------------------------------------------------- */
extern AD_DMA_t ADC_DMA_example;
void AD_DMA_DEMO(void);
void AD_DMA_userCallBack_demo(AD_DMA_t* self);
void AD_DMA_userCallBack_demo2(AD_DMA_t* self);
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

//以下为内部变量和函数，外部禁止访问

static edma_handle_t EDMA_Handle_adc0;
static edma_transfer_config_t transferConfig_adc0;
static volatile uint8_t stopping = 0;
static list_t AdcDmaList;
static adc16_channel_config_t adcChnConfig_adc0;
typedef struct _ADC_DMA_sys
{
	AD_DMA_t* p_userAD_DMA_t;
} AD_DMA_Sys_t;
static void ADC_DMA_NEXT(void);
static void ADC_Edma_Callback(edma_handle_t* handle, void* userData, bool transferDone, uint32_t tcds);
static volatile int ADC_DMA_CRITICAL_COUNT = 0;
static void ADC_DMA_ENTER_CRITICAL();
static void ADC_DMA_EXIT_CRITICAL();

#endif
