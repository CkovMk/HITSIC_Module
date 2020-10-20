#ifndef UTILITIES_DRV_CAM_ZF9V034_HPP
#define UTILITIES_DRV_CAM_ZF9V034_HPP

#include "hitsic_common.h"

#if (defined(HITSIC_USE_CAM_ZF9V034) && (HITSIC_USE_CAM_ZF9V034 > 0))
#include  "drv_cam_zf9v034_port.hpp"

typedef enum //TODO: Update these to enum class
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
}cam_zf9v034_cmd_t;
/*FIXME: fit these chaos...*/

__attribute__ ((__packed__))
struct cam_zf9v034_configPacket_t
{
    uint16_t autoExpSetCmd, autoExpSet;
    uint16_t autoExpTimeCmd, autoExpTime;
    uint16_t FrameRateCmd, FrameRate;
    uint16_t imageColCmd, imageCol;
    uint16_t imageRowCmd, imageRow;
    uint16_t HorizontalOffsetCmd, HorizontalOffset;
    uint16_t VerticalOffsetCmd, VerticalOffset;
    uint16_t imageGainCmd, imageGain;
    uint16_t initCmd, dummyData0;
};
void CAM_ZF9V034_GetDefaultConfig(cam_zf9v034_configPacket_t *config);
void CAM_ZF9V034_CfgWrite(const cam_zf9v034_configPacket_t *config);
void CAM_ZF9V034_CfgRead(cam_zf9v034_configPacket_t *config);
uint16_t CAM_ZF9V034_GetVersion(void);//TODO: use struct
uint16_t CAM_ZF9V034_SetExposeTime(uint16_t light);

// Receiver Config
#if (defined(ZF9V034_USE_DMADVP) && (ZF9V034_USE_DMADVP > 0U))
#include "drv_dmadvp.hpp"
void CAM_ZF9V034_GetReceiverConfig(dmadvp_config_t* config, const cam_zf9v034_configPacket_t *camConfig);
#elif(defined(ZF9V034_USE_RTCSI) && (ZF9V034_USE_RTCSI > 0U))
#include "drv_rtdvp.hpp"
void CAM_ZF9V034_GetReceiverConfig(csi_config_t* config, const cam_zf9v034_configPacket_t *camConfig); //FIXME
#endif // ! Receiver Config


#endif // ! HITSIC_USE_CAM_ZF9V034

#endif // ! UTILITIES_DRV_CAM_ZF9V034_HPP
