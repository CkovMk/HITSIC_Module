/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		tf_mini uart底层
 * @company	        哈工大智能车创新俱乐部
 * @author     		刘兴达 高嘉华 
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-30
 *
 * @note：
		该底层适用于tfmini及tfminiplus，使用时根据所用引脚更改tf.h中的宏定义
		
		哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_TF.h"
  const uart_config_t UART_tf_config = {
  .baudRate_Bps = 115200,
  .parityMode = kUART_ParityDisabled,
  .stopBitCount = kUART_OneStopBit,
  .txFifoWatermark = 0,
  .rxFifoWatermark = 1,
  .idleType = kUART_IdleTypeStartBit,
  .enableTx = true,
  .enableRx = true
};

void my_getchar (UART_Type *base, uint8_t *ch)   //uart接收一个字符
{
    while(!(base->S1 & UART_S1_RDRF_MASK));     
    *ch =  base->D; 
    
}

int tf_distance;
int tf_strength;
uint8_t tf_arry[9];
/**********************************************************************************************************************
*  @brief      TOF串口初始化
*  @param      NULL
*  @return     NULL
*  @since      v1.0
*  Sample usage:    uart_tf_init()
**********************************************************************************************************************/
void uart_tf_init(void)                               //中断初始化                   
{
  UART_Init(UART_tf_PERIPHERAL, &UART_tf_config, UART_tf_CLOCK_SOURCE);
  UART_EnableInterrupts(UART_tf_PERIPHERAL, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
  /* Interrupt vector UART1_RX_TX_IRQn priority settings in the NVIC */
  NVIC_SetPriority(UART_tf_SERIAL_RX_TX_IRQN, UART_tf_SERIAL_RX_TX_IRQ_PRIORITY);
  /* Enable interrupt UART1_RX_TX_IRQn request in the NVIC */
  EnableIRQ(UART_tf_SERIAL_RX_TX_IRQN);
}
/**********************************************************************************************************************
*  @brief      获取TOF距离
*  @param      NULL
*  @return     int   距离值
*  @since      v1.0
*  Sample usage:    uart_tf_distance()
**********************************************************************************************************************/
int uart_tf_distance()
{
  return tf_distance;
}
/**********************************************************************************************************************
*  @brief      TOF串口中断服务函数
*  @param      NULL
*  @return     NULL
*  @since      v1.0
*  Sample usage:    NULL
**********************************************************************************************************************/
void UART_tf_SERIAL_RX_TX_IRQHANDLER()        //uart中斷服務函數
{
  static int TF_cnt=0;
  static int TF_check=0;
  static  uint8_t cache_buff[9];
 if(( kUART_RxDataRegFullFlag| kUART_RxOverrunFlag) & UART_GetStatusFlags(UART_tf_PERIPHERAL))
  {
    my_getchar(UART_tf_PERIPHERAL,&cache_buff[TF_cnt]);      //接收一個字符到數組
    if(TF_cnt==0 || TF_cnt==1)
    {
      if(cache_buff[TF_cnt]==0x59)     
           TF_cnt++;       
    }
    else
    {
      TF_cnt++;
    }
    if(TF_cnt>8)
    {
      for(int i = 0;i<8;i++)
      {
        TF_check += cache_buff[i];
      }
      if(cache_buff[8] == (TF_check&0xff))//校驗
      {
        memcpy(tf_arry, cache_buff, sizeof(cache_buff));   //拷贝数组
        tf_strength = tf_arry[4]+(tf_arry[5]<<8);     //由强度的高8位和低8位得到强度值
        if(tf_strength>100 && tf_strength!=65535)     //信号强度满足条件时才可信
          tf_distance = tf_arry[2]+(tf_arry[3]<<8);   //由距离的高8位和低8位得到距离值
        else tf_distance = 9999;
      }
      TF_cnt = 0;//无论是否校验成功，都重新开始读取数据
      TF_check = 0;
    }
  }
  
}

