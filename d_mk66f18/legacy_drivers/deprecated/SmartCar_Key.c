/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		按键检测
 * @company	        哈工大智能车创新俱乐部
 * @author     		吴文华 qq1280390945
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
		KEY结构体定义各个按键对应GPIO口和对应引脚，使用时按照“Key.h”中
		Key_e的顺序依次列出,KEY_NUM为按键数量
		
		哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_Key.h"
#include "fsl_gpio.h"

#define   KEY_NUM	5

KEY Key[KEY_NUM] =
{
  {
    .GPIO = GPIOE,
    .PORT = PORTE,
    .PIN  = 8
  },
  {
    .GPIO = GPIOE,
    .PORT = PORTE,
    .PIN  = 6
  },
  {
    .GPIO = GPIOE,
    .PORT = PORTE,
    .PIN  = 9
  },
  {
    .GPIO = GPIOE,
    .PORT = PORTE,
    .PIN  = 7
  },
  {
    .GPIO = GPIOE,
    .PORT = PORTE,
    .PIN  = 10
  },
};

gpio_pin_config_t Key_config = {kGPIO_DigitalInput, 0};
port_pin_config_t Key_port_config = {
     kPORT_PullUp,
     kPORT_FastSlewRate,
     kPORT_PassiveFilterDisable,
     kPORT_OpenDrainDisable,
     kPORT_LowDriveStrength,
     kPORT_MuxAsGpio,
     kPORT_UnlockRegister,
};
/**********************************************************************************************************************
*  @brief      按键检测函数
*  @param      key        需要检测的按键
*  @return     返回0按键按下，返回1按键未按下
*  @since      v1.0
*  Sample usage:          Key_Init();
**********************************************************************************************************************/
void Key_Init(void)
{
  uint8_t i;
  for( i = 0;i<KEY_NUM;i++)
  {
  PORT_SetPinMux(Key[i].PORT, Key[i].PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(Key[i].GPIO, Key[i].PIN, &Key_config);
  PORT_SetPinConfig(Key[i].PORT, Key[i].PIN, &Key_port_config);
  }
}
/**********************************************************************************************************************
*  @brief      按键检测函数
*  @param      key        需要检测的按键
*  @return     返回0按键按下，返回1按键未按下
*  @since      v1.0
*  Sample usage:          Key_Check(KEY_ENTER);  
**********************************************************************************************************************/

KEY_STATUS Key_Check(KEY_e key)
{
    if((((Key[key].GPIO->PDIR) >> Key[key].PIN) & 0x01U)==KEY_P_DOWN )
    {
        DELAY_MS(50);
        if((((Key[key].GPIO->PDIR) >> Key[key].PIN) & 0x01U)==KEY_P_DOWN )
        return KEY_P_DOWN;
    }
    return KEY_P_UP;
}


