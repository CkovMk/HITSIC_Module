/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		蜂鸣器
 * @company	        哈工大智能车创新俱乐部
 * @author     		吴文华qq1280390945
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
				
    哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/
#include "SmartCar_Beep.h"

/**********************************************************************************************************************
*  @brief      蜂鸣器初始化
*  @param      port		蜂鸣器端口号
*  @param      gpio		蜂鸣器GPIO号
*  @param      pin		引脚号
*  @return     void
*  @since      v1.0
*  Sample usage:        Beep_Init(PORTB, GPIOB,1);
**********************************************************************************************************************/
void Beep_Init(PORT_Type *port, GPIO_Type *gpio,uint32_t pin)
{
	gpio_pin_config_t Beep_config = {kGPIO_DigitalOutput, 0};
	port_pin_config_t Beep_port_config = {
		 kPORT_PullUp,
		 kPORT_FastSlewRate,
		 kPORT_PassiveFilterDisable,
		 kPORT_OpenDrainDisable,
		 kPORT_LowDriveStrength,
		 kPORT_MuxAsGpio,
		 kPORT_UnlockRegister,
	};
	PORT_SetPinMux(port, pin, kPORT_MuxAsGpio);
	GPIO_PinInit(gpio, pin, &Beep_config);
	PORT_SetPinConfig(port, pin, &Beep_port_config);
}

/**********************************************************************************************************************
*  @brief      蜂鸣器打开
*  @param      gpio		蜂鸣器GPIO号
*  @param      pin		引脚号
*  @return     void
*  @since      v1.0
*  Sample usage:        Beep_On(GPIOB,1);
**********************************************************************************************************************/
void Beep_On(GPIO_Type *gpio,uint32_t pin)
{
	GPIO_PortSet(gpio, 1U << pin);
}

/**********************************************************************************************************************
*  @brief      蜂鸣器关闭
*  @param      gpio		蜂鸣器GPIO号
*  @param      pin		引脚号
*  @return     void
*  @since      v1.0
*  Sample usage:        Beep_On(GPIOB,1);
**********************************************************************************************************************/
void Beep_Off(GPIO_Type *gpio,uint32_t pin)
{
	GPIO_PortClear(gpio, 1U << pin);
}
/**********************************************************************************************************************
*  @brief      蜂鸣器翻转
*  @param      gpio		蜂鸣器GPIO号
*  @param      pin		引脚号
*  @return     void
*  @since      v1.0
*  Sample usage:        Beep_Toggle(GPIOB,1);
**********************************************************************************************************************/
void Beep_Toggle(GPIO_Type *gpio,uint32_t pin)
{
  GPIO_PortToggle(gpio, 1U << pin);
}