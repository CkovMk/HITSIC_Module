/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		时间函数
 * @company	        哈工大智能车创新俱乐部
 * @author     		王学嘉 qq512924543     吴文华qq1280390945
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
				
    哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_Systick.h"

/**********************************************************************************************************************
*  @brief      systick延时函数
*  @param      time            需要延时的时间
*  @return     void
*  @since      v1.0
*  Sample usage:               systick_delay(1000);   *延时1000个内核时钟周期
**********************************************************************************************************************/
void systick_delay(uint32_t time)
{
    if(time == 0)   return;

    SysTick->CTRL = 0x00;//先关了 systick ,清标志位
    SysTick->LOAD = time;//设置延时时间..
    SysTick->VAL = 0x00;//清空计数器
    SysTick->CTRL = ( 0 | SysTick_CTRL_ENABLE_Msk         //使能 systick
                  //| SysTick_CTRL_TICKINT_Msk        //使能中断 (注释了表示关闭中断)
                    | SysTick_CTRL_CLKSOURCE_Msk      //时钟源选择 (core clk) K60只能选择(core clk)
                );
     while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));//等待时间到
}

/**********************************************************************************************************************
*  @brief      毫秒级systick延时函数
*  @param      ms              延时多少毫秒
*  @return     void
*  @since      v1.0
*  Sample usage:               systick_delay_ms(1000);   *延时1000毫秒
**********************************************************************************************************************/
void systick_delay_ms(uint32_t ms)
{
    //get_clk();//获取内核时钟便于后面设置
    uint32_t core_clk_mhz = CLOCK_GetCoreSysClkFreq();
    while(ms--) systick_delay(core_clk_mhz/1000);
}

/**********************************************************************************************************************
*  @brief      systick定时器
*  @param      time            定时时间(0-0x00ffffff)
*  @return     void
*  @since      v1.0
*  Sample usage:               systick_timing(1000);   *定时1000个内核时钟周期
**********************************************************************************************************************/
void systick_timing(uint32_t time)
{
    time &= 0x00ffffff;     	//数据强制变换为24位 
    SysTick->LOAD = time;       //设置延时时间
    SysTick->VAL = 0x00;       	//清空计数器
    SysTick->CTRL = ( 0 
					| SysTick_CTRL_ENABLE_Msk       //使能 systick
                    | SysTick_CTRL_TICKINT_Msk      //使能中断
                    | SysTick_CTRL_CLKSOURCE_Msk    //时钟源选择 (core clk) K60只能选择(core clk)
                );
}

/**********************************************************************************************************************
*  @brief      获得当前System tick timer的值
*  @return     返回当前System tick timer的值
*  @since      v1.0
*  Sample usage:               uint32_t tim = systick_getval();   
**********************************************************************************************************************/
uint32_t systick_getval(void)
{
    return SysTick->VAL;
}


