/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		FTM
 * @company	        哈工大智能车创新俱乐部
 * @author     		王学嘉 qq512924543         吴文华qq1280390945
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
		    哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#include "SmartCar_Ftm.h"

#define FTM_CLOCK_SOURCE CLOCK_GetFreq(kCLOCK_BusClk)

#ifdef __cplusplus
extern "C" {
#endif
	

/**********************************************************************************************************************
*  @brief      FTM初始化
*  @return     void
*  @since      v1.0
*  Sample usage:          Ftm_PWM_Init(FTM0,kFTM_Chnl_1,20000,10.5);
**********************************************************************************************************************/
void Ftm_PWM_Init(FTM_Type *base, ftm_chnl_t chnlNumber, uint32_t dutyFre, float dutyCycle)
{
    //ftm初始化配置结构体
    ftm_config_t FTM_config = {
      .prescale = kFTM_Prescale_Divide_1,
      .bdmMode = kFTM_BdmMode_0,
      .pwmSyncMode = kFTM_SoftwareTrigger,
      .reloadPoints = 0,
      .faultMode = kFTM_Fault_Disable,
      .faultFilterValue = 0,
      .deadTimePrescale = kFTM_Deadtime_Prescale_1,
      .deadTimeValue = 0,
      .extTriggers = 0,
      .chnlInitState = 0,
      .chnlPolarity = 0,
      .useGlobalTimeBase = false
    };
    //ftmPWM设置占空比结构体
    ftm_chnl_pwm_signal_param_t FTM_pwmSignalParams[] = { 
      {
      .chnlNumber = chnlNumber,
      .level = kFTM_HighTrue,
      .dutyCyclePercent = dutyCycle
      }
    };
    //计算分频
    uint32_t pre_ps = FTM_CLOCK_SOURCE/dutyFre;
    uint32_t i = 0;
    for (i = 0;i <= 7;i++)
    {
        if ((pre_ps >> i) <= 65535)
        {
            break;
        }
    }
    switch (i)
    {
    case 0:
      FTM_config.prescale = kFTM_Prescale_Divide_1;
      break;
    case 1:
      FTM_config.prescale = kFTM_Prescale_Divide_2;
      break;
    case 2:
      FTM_config.prescale = kFTM_Prescale_Divide_4;
      break;
    case 3:
      FTM_config.prescale = kFTM_Prescale_Divide_8;
      break;
    case 4:
      FTM_config.prescale = kFTM_Prescale_Divide_16;
      break;
    case 5:
      FTM_config.prescale = kFTM_Prescale_Divide_32;
      break;
    case 6:
      FTM_config.prescale = kFTM_Prescale_Divide_64;
      break;
    case 7:
      FTM_config.prescale = kFTM_Prescale_Divide_128;
      break;
    default:
      break;
    }
    FTM_Init(base, &FTM_config);
    FTM_SetupPwm(base, FTM_pwmSignalParams, sizeof(FTM_pwmSignalParams) / sizeof(ftm_chnl_pwm_signal_param_t), kFTM_EdgeAlignedPwm, dutyFre, FTM_CLOCK_SOURCE);
    FTM_StartTimer(base, kFTM_SystemClock);
}


/**********************************************************************************************************************
*  @brief      ftm设定占空比      用于PWM模式
*  @param      base	           ftm模块选择
*  @param      chnlNumber      ftm通道选择
*  @param      dutyFre         ftm频率
*  @return     void
*  @since      v1.0
*  Sample usage:          Ftm_PWM_change(FTM0,kFTM_Chnl_1,20000,10.5);
**********************************************************************************************************************/
void Ftm_PWM_Change(FTM_Type *base, ftm_chnl_t chnlNumber, uint32_t dutyFre, float dutyCycle)
{
      /* Disable channel output before updating the dutycycle */
      //FTM_UpdateChnlEdgeLevelSelect(base, chnlNumber, 0U);
      /* Update PWM duty cycle */
      FTM_UpdatePwmDutycycle(base, chnlNumber, kFTM_CenterAlignedPwm, dutyCycle);
      /* Software trigger to update registers */
      FTM_SetSoftwareTrigger(base, true);
      /* Start channel output with updated dutycycle */
      //FTM_UpdateChnlEdgeLevelSelect(base, chnlNumber, kFTM_HighTrue);//待定，不知道有没有问题
      return;
}

/**********************************************************************************************************************
*  @brief      ftm正交编码器初始化
*  @return     void
*  @since      v1.0
*  Sample usage:          Ftm_QuadDecode_Init(FTM1);
**********************************************************************************************************************/
void Ftm_QuadDecode_Init(FTM_Type *base)
{
    const ftm_config_t FTM_2_config = {
      .prescale = kFTM_Prescale_Divide_16,
      .bdmMode = kFTM_BdmMode_0,
      .pwmSyncMode = kFTM_SoftwareTrigger,
      .reloadPoints = 0,
      .faultMode = kFTM_Fault_Disable,
      .faultFilterValue = 0,
      .deadTimePrescale = kFTM_Deadtime_Prescale_1,
      .deadTimeValue = 0,
      .extTriggers = 0,
      .chnlInitState = 0,
      .chnlPolarity = 0,
      .useGlobalTimeBase = false
    };
    const ftm_phase_params_t FTM_2_phaseAParams = { 
      .enablePhaseFilter = false,
      .phasePolarity = kFTM_QuadPhaseNormal

    };
    const ftm_phase_params_t FTM_2_phaseBParams = { 
      .enablePhaseFilter = false,
      .phasePolarity = kFTM_QuadPhaseNormal

    };

      FTM_Init(base, &FTM_2_config);
      FTM_SetQuadDecoderModuloValue(base, 0U,65535U);
      FTM_SetupQuadDecode(base, &FTM_2_phaseAParams, &FTM_2_phaseBParams, kFTM_QuadCountAndDir);//1为脉冲计数
      FTM_StartTimer(base, kFTM_SystemClock);
}

/**********************************************************************************************************************
*  @brief      ftm获取编码器计数值
*  @param      base	           ftm模块选择
*  @return     int16_t         编码器计数值
*  @since      v1.0
*  Sample usage:          Ftm_QuadDecode_Init(FTM1);
**********************************************************************************************************************/
int16_t Ftm_GetSpeed(FTM_Type *base)
{
    return (int16_t)FTM_GetQuadDecoderCounterValue(base);
}

/**********************************************************************************************************************
*  @brief      ftm清除编码器计数值
*  @param      base	           ftm模块选择
*  @return     void
*  @since      v1.0
*  Sample usage:          Ftm_ClearSpeed(FTM1);
**********************************************************************************************************************/
void Ftm_ClearSpeed(FTM_Type *base)
{
    FTM_ClearQuadDecoderCounterValue(base);
}

#ifdef __cplusplus
}
#endif