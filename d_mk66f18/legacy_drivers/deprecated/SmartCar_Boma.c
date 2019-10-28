/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		拨码开关检测
 * @company	        哈工大智能车创新俱乐部
 * @author     		吴文华 qq1280390945
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
		Boma_Switch结构体定义拨码按键对应GPIO口和引脚,BOMA_NUM为拨码按键数量，
		换用不同拨码开关时记得修改
    哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_Boma.h"

#define  BOMA_NUM	4

Boma_Switch Boma[BOMA_NUM] =
{
  {
    .GPIO = GPIOA,
    .PORT = PORTA,
    .PIN  = 19
  },
  {
    .GPIO = GPIOA,
    .PORT = PORTA,
    .PIN  = 16
  },
  {
    .GPIO = GPIOA,
    .PORT = PORTA,
    .PIN  = 14
  },
  {
    .GPIO = GPIOA,
    .PORT = PORTA,
    .PIN  = 12
  }
};

gpio_pin_config_t Boma_config = {kGPIO_DigitalInput, 0};
port_pin_config_t Boma_port_config = {
     kPORT_PullUp,
     kPORT_FastSlewRate,
     kPORT_PassiveFilterDisable,
     kPORT_OpenDrainDisable,
     kPORT_LowDriveStrength,
     kPORT_MuxAsGpio,
     kPORT_UnlockRegister,
};

/**********************************************************************************************************************
*  @brief      拨码开关初始化
*  @param      NULL
*  @return     NULL
*  @since      v1.0
*  Sample usage:    Boma_Init()
**********************************************************************************************************************/
void Boma_Init(void)
{
  uint8_t i;
  for( i = 0;i<BOMA_NUM;i++)
  {
  PORT_SetPinMux(Boma[i].PORT, Boma[i].PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(Boma[i].GPIO, Boma[i].PIN, &Boma_config);
  PORT_SetPinConfig(Boma[i].PORT, Boma[i].PIN, &Boma_port_config);
  }
}

/**********************************************************************************************************************
*  @brief      拨码开关检测函数
*  @param      num        拨码开关位
*  @return     返回1，开关打开，返回0，开关关闭
*  @since      v1.0
*  Sample usage:    Boma_Check(1)检测1号拨码开关
**********************************************************************************************************************/
uint8_t Boma_Check(uint8_t num)
{
  return !GPIO_PinRead(Boma[num-1].GPIO,Boma[num-1].PIN);
}
