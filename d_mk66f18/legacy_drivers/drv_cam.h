/*
 *	@by:		SmartCar SJMC
 *	@name:		drvcam.h
 *	@layer:		driver
 *	@owner:		C.M.@SJMC
 *	@modified:	C.M.@SJMC 2018.12.30 05:44
 */

#pragma once
#ifndef _DRVCAM_H_
#define _DRVCAM_H_
#include "inc_stdlib.h"
#include "inc_fsl_mk66f18.h"
#include "sys_pitmgr.h"
#include "sys_extint.h"
#include "drv_cam_def.h"

#define systick_delay_ms    PITMGR_Delay_ms

#define CAM_TYPE				MT9V032
#define CAM_UART_BASE			UART3
#define CAMERA_UART_CLOCK_SOURCE CLOCK_GetFreq(UART3_CLK_SRC)
#define CAM_DMA_REQUEST_SOURCE 	kDmaRequestMux0PortD

#define DATALINE_GROUP_PIN 1
#define DATALINE_PDIR_OFFSET 0x10

#define GPIO_CAM_D0_GPIO		RTEPIN_BOARD_CAM_D0_GPIO
#define GPIO_CAM_D0_PORT		RTEPIN_BOARD_CAM_D0_PORT
#define GPIO_CAM_D0_PIN			RTEPIN_BOARD_CAM_D0_PIN

#define CAM_DATA_DMA_ADDR		((uint32_t)GPIO_CAM_D0_GPIO)+0x10+GPIO_CAM_D0_PIN/8

#define GPIO_CAM_PCLK_GPIO		RTEPIN_BOARD_CAM_PCLK_GPIO
#define GPIO_CAM_PCLK_PORT		RTEPIN_BOARD_CAM_PCLK_PORT
#define GPIO_CAM_PCLK_PIN		RTEPIN_BOARD_CAM_PCLK_PIN
// #define CAM_DATA_HREF_GPIO		GPIOC
// #define CAM_DATA_HREF_PORT		PORTC
// #define CAM_DATA_HREF_PIN		14
#define GPIO_CAM_VSNC_GPIO		RTEPIN_BOARD_CAM_VSNC_GPIO
#define GPIO_CAM_VSNC_PORT		RTEPIN_BOARD_CAM_VSNC_PORT
#define GPIO_CAM_VSNC_PIN		RTEPIN_BOARD_CAM_VSNC_PIN

#define CAM_DMA_Chnl 0
#define CAM_DMAMUX_Chnl 0
#define CAM_PCLK_DMA_IRQn DMA0_DMA16_IRQn
#define CAM_PCLK_DMA_PRIO 2 //higher priority required !


#define camera_enable_vsync_irq() (PORT_SetPinInterruptConfig(GPIO_CAM_VSNC_PORT, GPIO_CAM_VSNC_PIN, kPORT_InterruptFallingEdge))
#define camera_disable_vsync_irq() (PORT_SetPinInterruptConfig(GPIO_CAM_VSNC_PORT, GPIO_CAM_VSNC_PIN, kPORT_InterruptOrDMADisabled))

#define camera_enable_pclk_irq()	(EnableIRQ(CAM_PCLK_DMA_IRQn))
#define camera_disable_pclk_irq()	(DisableIRQ(CAM_PCLK_DMA_IRQn))

#ifndef CAM_IMG_R
#define  CAM_IMG_R 120
#endif
#ifndef CAM_IMG_C
#define  CAM_IMG_C 188
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    CAM_CMD_INIT = 0,               //����ͷ��ʼ������
    CAM_CMD_AUTO_EXP,               //�Զ��ع�����
    CAM_CMD_EXP_TIME,               //�ع�ʱ������
    CAM_CMD_FPS,                    //����ͷ֡������
    CAM_CMD_SET_COL,                //ͼ��������
    CAM_CMD_SET_ROW,                //ͼ��������
    CAM_CMD_LR_OFFSET,              //ͼ������ƫ������
    CAM_CMD_UD_OFFSET,              //ͼ������ƫ������
    CAM_CMD_GAIN,                   //ͼ����������
    CAM_CMD_CONFIG_FINISH,          //������λ����Ҫ����ռλ����
    
    CAM_CMD_SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    CAM_CMD_GET_STATUS,             //��ȡ����ͷ��������
    CAM_CMD_GET_VERSION,            //�̼��汾������
    
    CAM_CMD_SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    CAM_CMD_SET_DATA                //�Ĵ�����������
}camera_cmd_t;



extern camera_img_t camera_imgBuf;
extern uint16_t camera_version;
extern bool camera_finishFlag;

void CAMERA_Init();
void CAMERA_CfgWrite(void);	
void CAMERA_CfgRead(void);
uint16_t CAMERA_GetVersion(void);
uint16_t CAMERA_SetExposeTime(uint16_t light);
void CAMERA_BeginXfer(void);
void CAMERA_VSNC_ExtIntHandler(void);
void CAMERA_Xfer_EdmaCallback(edma_handle_t * handle, void * param, bool transferDone, uint32_t tcds);


#ifdef __cplusplus
}
#endif

#endif // ! _DRVCAM_H_


