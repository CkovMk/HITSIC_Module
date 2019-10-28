/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		数字电位器5206
 * @company	        哈工大智能车创新俱乐部
 * @author     		钱宇坤2248285537  刘睿智786240762	吴文华 qq1280390945
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
    哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_AD5206.h"

#define CS_LOW() GPIO_PortClear(CS_GPIO,1U<<CS_PIN)
#define CS_HIGH() GPIO_PortSet(CS_GPIO,1U<<CS_PIN)
#define DIN_LOW() GPIO_PortClear(DIN_GPIO,1U<<DIN_PIN)
#define DIN_HIGH() GPIO_PortSet(DIN_GPIO,1U<<DIN_PIN)
#define CLK_LOW() GPIO_PortClear(CLK_GPIO,1U<<CLK_PIN)
#define CLK_HIGH() GPIO_PortSet(CLK_GPIO,1U<<CLK_PIN)

gpio_pin_config_t AD5206_GPIO_out_config = {
    .pinDirection = kGPIO_DigitalOutput,
    .outputLogic = 1U
};

void Delay()
{
    int i,j;
    for(i=0;i<10;i++)
      for(j=0;j<50;j++);

}

/**********************************************************************************************************************
*  @brief      拨码开关初始化
*  @param      address	电阻地址
*  @param      dat		设定值
*  @return     NULL
*  @since      v1.0
*  Sample usage:    WriteAD5206(R0,100);
**********************************************************************************************************************/
void AD5206_init()
{
      PORT_SetPinMux(CS_PORT, CS_PIN, kPORT_MuxAsGpio);
	  PORT_SetPinMux(DIN_PORT, DIN_PIN, kPORT_MuxAsGpio);
	  PORT_SetPinMux(CLK_PORT, CLK_PIN, kPORT_MuxAsGpio);
      GPIO_PinInit(CS_GPIO, CS_PIN, &AD5206_GPIO_out_config);
      GPIO_PinInit(DIN_GPIO, DIN_PIN, &AD5206_GPIO_out_config);
      GPIO_PinInit(CLK_GPIO, CLK_PIN, &AD5206_GPIO_out_config);
}

/**********************************************************************************************************************
*  @brief      AD5206设定电阻值
*  @param      address	电阻地址
*  @param      dat		设定值
*  @return     NULL
*  @since      v1.0
*  Sample usage:    WriteAD5206(R0,100);
**********************************************************************************************************************/
void WriteAD5206(unsigned char address,unsigned char dat)
{
    uint8_t i;
    
    CS_LOW();    //拉低片选线，选中器件
    //发送地址
    for (i=0;i<3;i++)        //移位循环3次              
    {   
       CLK_LOW();       //清零时钟总线
       if(address&0x80)
        DIN_HIGH();
       else
        DIN_LOW();
       address<<=1;             //左移一位
       Delay();
       CLK_HIGH();      //时钟上升沿，发送地址
       Delay();
    }
    //发送数据
    for (i=0;i<8;i++)        //移位循环8次              
    {   
       CLK_LOW();       //清零时钟总线
       if(dat&0x80)
         DIN_HIGH();
       else
         DIN_LOW();
       dat<<=1;             //左移一位
       Delay();
       CLK_HIGH();      //时钟上升沿，发送地址
        Delay();
    }
   CS_HIGH();     //发送结束，上升沿锁存数据   
}




 