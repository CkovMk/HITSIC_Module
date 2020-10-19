#ifndef UTILITIES_DRV_CAM_ZF9V034_HPP
#define UTILITIES_DRV_CAM_ZF9V034_HPP

#include "hitsic_common.h"

#if (defined(HITSIC_USE_CAM_ZF9V034) && (HITSIC_USE_CAM_ZF9V034 > 0))

#ifdef __cplusplus
extern "C" {
#endif

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

void CAM_ZF9V034_CfgWrite(void);	//TODO: update this to accept cfg struct pointer.
void CAM_ZF9V034_CfgRead(void);     //TODO: update this to accept cfg struct pointer.
uint16_t CAM_ZF9V034_GetVersion(void);
uint16_t CAM_ZF9V034_SetExposeTime(uint16_t light);

// Receiver Config
#if (defined(ZF9V034_USE_EDMADVP) && (ZF9V034_USE_EDMADVP > 0U))
void CAM_ZF9V034_GetReceiverConfig(edmadvp_config_t* config);
#else if(defined(ZF9V034_USE_RTCSI) && (ZF9V034_USE_RTCSI > 0U))
void CAM_ZF9V034_GetReceiverConfig(csi_config_t* config); //FIXME
#endif // ! Receiver Config

#ifdef __cplusplus
}
#endif

#endif // ! HITSIC_USE_CAM_ZF9V034

#endif // ! UTILITIES_DRV_CAM_ZF9V034_HPP