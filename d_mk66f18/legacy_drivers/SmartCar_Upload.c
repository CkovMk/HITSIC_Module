/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		上位机程序
 * @company	        哈工大智能车创新俱乐部
 * @author     		吴文华 qq1280390945        刘睿智qq786240762
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
		NUM_VAR为需要发送到上位机的变量个数,UP_UART为上位机UART口。Send_Variable()适用于名优科创上位机，
                void Img_Upload(void)适用于红树伟业上位机
		
		哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_Upload.h"

#define NUM_VAR  6
#define UP_UART  UART0

#ifdef __cplusplus
extern "C" {
#endif

float Variable[NUM_VAR];//发送缓存数组
extern uint8_t image_Buffer_0[120][188];

/**********************************************************************************************************************
*  @brief      向上位机发送一个字节
*  @return     void
*  @since      v1.0
*  @Sample usage:          Putchar(temp)
**********************************************************************************************************************/
void Putchar(char temp)
{
    UART_WriteBlocking(UP_UART,(uint8_t*)&temp,1); //根据实际的串口号来修改
}

/**********************************************************************************************************************
*  @brief      上位机帧头
*  @return     void
*  @since      v1.0
*  @Sample usage:          Send_Begin();
*  @note   用来通知上位机新的一组数据开始，要保存数据必须发送它
**********************************************************************************************************************/
void Send_Begin(void)
{
  Putchar(0x55);
  Putchar(0xaa);
  Putchar(0x11);
}

/**********************************************************************************************************************
*  @brief      上传数据
*  @return     void
*  @since      v1.0
*  @Sample usage:          Send_Variable();
**********************************************************************************************************************/
void  Send_Variable(void)//发送实时变量
{
  uint8_t i=0,ch=0;
  float temp=0;
  Send_Begin();
  Variable[0] = 2;			//此处将需要发送的变量赋值到Variable
  Variable[1] = 13;
  Variable[2] = 16;
  Variable[3] = 15;
  Variable[4] = 6;
  Variable[5] = 8;	
  Putchar(0x55);
  Putchar(0xaa);
  Putchar(0x11);
  Putchar(0x55);
  Putchar(0xaa);
  Putchar(0xff);
  Putchar(0x01);
  Putchar(NUM_VAR);
 for(i=0;i<NUM_VAR;i++)
  {
    temp=Variable[i];
    ch=BYTE0(temp);
      Putchar(ch);
    ch=BYTE1(temp);
      Putchar(ch);
    ch=BYTE2(temp);
      Putchar(ch);
    ch=BYTE3(temp);
      Putchar(ch);
  }
     Putchar(0x01);
}

/**********************************************************************************************************************
*  @brief      上传数据
*  @return     void
*  @since      v1.0
*  @Sample usage:          Img_Upload();
**********************************************************************************************************************/
void Img_Upload(void) //向上位机发送图像
{
  uint8_t* pImg;
  uint8_t image_Buffer[120][188];
  memcpy(image_Buffer,image_Buffer_0,22560);
  pImg =(uint8_t *)image_Buffer;   
  int i,j;
  Putchar(0xff);
  for(i=0;i<120;i++)
  {
    for(j=0;j<188;j++)
    {
      uint8_t xxx=*(pImg+i*188+j);//pImg[i][j]
      if(xxx==0xff)
      {
      xxx=0xfe;
      }
      Putchar(xxx);
    }

  }
}
#define CMD_WARE     ((uint8_t)3u)
#define NCMD_WARE     ((uint8_t)(~3u))

void Img_Upload_wxj(uint8_t* upload_img) //向上位机发送图像
{
	uint8_t image_Buffer[120][188];//用于缓存数据，防止传输过程被更改
	memcpy(image_Buffer, upload_img, 22560);//缓存数组防止传输途中被更改
	uint8_t cmdf[2] = { CMD_WARE, NCMD_WARE };    //串口调试 使用的前命令
	uint8_t cmdr[2] = { NCMD_WARE, CMD_WARE };    //串口调试 使用的后命令
	UART_WriteBlocking(UP_UART, cmdf, sizeof(cmdf)); //先发送前命令
	UART_WriteBlocking(UP_UART, &image_Buffer[0][0], 22560);    //发送数据
	UART_WriteBlocking(UP_UART, cmdr, sizeof(cmdr));    //发送后命令

}

#ifdef __cplusplus
}
#endif
