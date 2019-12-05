/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @file       		IRQ_manager.h 外部中断管理器
 * @company	        哈工大智能车创新俱乐部
 * @author     		肖日涛 qq1761690868
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		ver1.0 2019年9月26日
 * 
 * @note：使用外部中断之前一定要先初始化外部中断管理器
 * 
 * @note：gpio是通用输入输出端口的英文, 是指单一根脚。port是指一组gpio,看单片机的端口规划例如8051就有0~3个port。
 * k66有5个port。每个port有一个中断服务函数。
 * 
 * 			这样一组gpio上的所有中断共用一个中断服务函数。
 * 
 * 			外部中断管理器作用是接管这5个port中断服务函数，并且为每一个gpio单独设置gpio中断服务函数。
 * 
 * 			当外部中断发生时，先进入这5个port中断服务函数中的某一个，然后获取中断的flag并且清理中断标志位
 * 然后根据这个flag，判断是哪个脚的中断，然后调用对应的gpio中断服务函数
 * 
 * @note：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/
#pragma once

#ifndef UTILITIES_SYS_EXTINT_H_
#define UTILITIES_SYS_EXTINT_H_
#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "hitsic_common.h"
#include "sys_extint_port.hpp"

#include "lib_list.h"

#if defined(D_MK66F18_SYS_EXTINT_PORT_HPP_) //CPU Selection


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _extint_irqNode_t
{
 	uint32_t pin_mask;
	uint32_t priority; //0为先
	void (*callback)(void);
}extint_irqNode_t;

extern list_t PORTA_irq_list;
extern list_t PORTB_irq_list;
extern list_t PORTC_irq_list;
extern list_t PORTD_irq_list;
extern list_t PORTE_irq_list;

/**
 * @brief	外部中断管理器初始化，使用之前一定要初始化
 * 
 * @return {status_t}  : 
 */
status_t EXTINT_Init();

/**
 * @brief	配置io口中断并且设置其中断函数
 * 
 * @param  {PORT_Type*} PORTx                      : PORT口，PORTA,PORTB等等
 * @param  {uint32_t} pin                          : io口编号，0-32等
 * @param  {uint32_t} priority_                    : 优先级，相对于PORT中断里头的GPIO口中断的优先级，0为最高
 * @param  {void(*)(void)} gpio_interrupt_callback : 引脚对应的中断服务函数
 * @return {status_t}                              : 错误代码
 * @SampleUsage 
 * 		EXTINT_PortSetup(GPIOE, 6, kPORT_InterruptFallingEdge, 0, gpio_callback);
 */
status_t EXTINT_PortSetup(PORT_Type* PORTx, uint32_t pin, uint32_t priority_,void (*gpio_interrupt_callback)(void));

/**
 * @brief	取消io口中断
 * 
 * @param  {PORT_Type*} PORTx : PORT口，PORTA,PORTB等等
 * @param  {uint32_t} pin     : io口编号，0-32等
 * @return {status_t}         : 错误代码
 * @SampleUsage
 * 		EXTINT_PortRemove(GPIOE, 6);
 */
status_t EXTINT_PortRemove(PORT_Type* PORTx, uint32_t pin);

/**
 * @brief	内部函数，返回port对应的链表
 * 
 * @param  {PORT_Type*} PORTx : 
 * @return {list_t*}          : 
 */
list_t* EXTINT_ListGet(PORT_Type* PORTx);

/**
 * @brief	PORTX中断函数
 * @param  {PORT_Type*} _port : port实例
 * @param  {list_t*}    _list : port中断管理链表
 */
void PORTX_IRQHandler(PORT_Type* _port, list_t* _list);


#ifdef __cplusplus
}
#endif


#else	// CPU Selection
#error "C API does NOT support this CPU!"
#endif 	// CPU Selection


#endif // ! UTILITIES_SYS_EXTINT_H_


