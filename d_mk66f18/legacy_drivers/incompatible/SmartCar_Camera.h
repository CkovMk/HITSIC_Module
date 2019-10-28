#ifndef _SMARTCAR_MT9V032_H
#define _SMARTCAR_MT9V032_H

#define COL     188//图像宽度   范围1-752     K60采集不允许超过188
#define ROW     120//图像高度	范围1-480
#include "stdafx.h"

//#include "smartcar_drivers/uart.h"
//#include "smartcar_drivers/IRQ_manager.h"
#include "include.h"
//#include "SmartCar_Systick.h"
//#include "SmartCar_OLED.h"
#define SMARTCAR_CAMERA_BUFFER (1U)//定义为2的时候启用双缓存，1则是单缓存
#define DMAMUX_SELECT DMAMUX0
#define DMA_SELECT DMA0  
#define DMA_CHANNEL_SELECT 0 
/*场中断引脚PTC6*/
#define VSY_PORT PORTC
#define VSY_GPIO GPIOC
#define VSY_PIN 6u
//#define VSY_IRQn  PORTC_IRQn
//#define VSY_Prio 1u //较高优先级
//#define camera_VSYNC PORTC_IRQHandler
/*像素中断引脚PTC18*/
#define PIX_PORT PORTC
#define PIX_GPIO GPIOC
#define PIX_PIN 18u
#define PIX_DMA_CHANNEL_IRQn DMA0_DMA16_IRQn
#define DMAPORTCode kDmaRequestMux0PortC
#define PIX_DMA_Prio 2u //稍高优先级
#define DATALINE_PORT PORTC
#define DATALINE_GPIO GPIOC
#define DATALINE_GROUP GPIOC_BASE
#define DATALINE_PORT_NUM 8 //第一个引脚号，     例如ptc0~ptc7为0 ，ptc8~ptc15为8 ，依次类推
#define DATALINE_GROUP_PIN 1 //                 例如ptc0~ptc7为0 ，ptc8~ptc15为1 ，依次类推
#define DATALINE_PDIR_OFFSET 0x10 //Port Data Input Register offset 查K66芯片手册2191，通常不用改:Address: Base address + 10h offset
#define DATALINE_LOCATION GPIOC_BASE+DATALINE_PDIR_OFFSET+DATALINE_GROUP_PIN//由上面三条计算这几个数据线引脚的PDIR地址，强烈建议自己算
#define CAMERA_UART UART3
///*摄像头配置串口中断 UART_3 interrupt vector ID (number). */
//#define CAMERA_UART_RX_TX_IRQN UART3_RX_TX_IRQn
///*摄像头配置串口中断优先级 UART_1 interrupt vector priority. */
//#define CAMERA_UART_RX_TX_IRQ_PRIORITY 3
///*中断服务函数重定向*/
//#define camera_config_handle UART3_RX_TX_IRQHandler
//#define CAMERA_UART_PORT PORTC
//#define CAMERA_UART_CLOCK_SRC UART3_CLK_SRC
//#define CAMERA_UART_RX_PIN 16u
//#define CAMERA_UART_TX_PIN 17u   

//摄像头命令枚举
typedef enum
{
	INIT = 0,               //摄像头初始化命令
	AUTO_EXP,               //自动曝光命令
	EXP_TIME,               //曝光时间命令
	FPS,                    //摄像头帧率命令
	SET_COL,                //图像列命令
	SET_ROW,                //图像行命令
	LR_OFFSET,              //图像左右偏移命令
	UD_OFFSET,              //图像上下偏移命令
	GAIN,                   //图像偏移命令
	CONFIG_FINISH,          //非命令位，主要用来占位计数

	SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
	GET_STATUS,             //获取摄像头配置命令
	GET_VERSION,            //固件版本号命令

	SET_ADDR = 0XFE,        //寄存器地址命令
	SET_DATA                //寄存器数据命令
}CMD;

#define BOARD_InitCamera camera_init_MT9V032
extern volatile bool CameraON; //控制标志位，内部控制用，记录当前摄像头的是同情况
extern volatile bool CameraInitialized;//记录是否经过初始化
extern volatile uint8_t if_EDMA_Init;
extern volatile uint8_t if_DMAMUX_Init;
extern volatile uint8_t mt9v032_finish_flag;  //一场图像采集完成标志位，用于同步最新的一帧图像，完成时会被置1，需手动清0
extern uint8_t image_Buffer_0[ROW][COL];
#if defined(SMARTCAR_CAMERA_BUFFER)&&(SMARTCAR_CAMERA_BUFFER==2)
extern uint8_t image_Buffer_1[ROW][COL];
extern uint8_t image_Buffer_index;
#endif
int camera_init_MT9V032(void);
void camera_uart_init(void);
void camera_EDMA_Callback(edma_handle_t* handle, void* param, bool transferDone, uint32_t tcds);
void camera_config_get(void);
void camera_StopTransmittion();
void camera_StartTransmittion();
void camera_VSYNC(void);
void show_image_on_oled(uint8_t* image, uint16_t ro, uint16_t co);
uint8_t GetAvailableImageBufferIndex();


#endif
