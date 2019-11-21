/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,哈工大智能车创新俱乐部
 * All rights reserved.
 * 
 * @file       		ADC
 * @company	        哈工大智能车创新俱乐部
 * @author     		吴文华 qq1280390945
 * @version    		v1.0
 * @Software 		IAR 7.7+
 * @Target core		K66
 * @date       		2019-9-27
 *
 * @note：
		ADC_NUM为使用的ADC引脚数量
		.resolution为ADC位数，可调整精度
		.clockDivider为时钟预分频系数，值越低速度越快，ADC采集的值越不稳定
		上面两个参数的调整可参见芯片手册或MCUXpresso
	哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/
#include "SmartCar_ADC.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_NUM 2

Adc_t  Adcs[ADC_NUM] = 
{
	{
	  .PORT = PORTB,
	  .PIN  = 0,
	  .ADC  = ADC0
	},
	{
	  .PORT = PORTB,
	  .PIN  = 1,
	  .ADC  = ADC0	
		
	}
};


adc16_channel_config_t ADC_channelsConfig = {
  
	.enableDifferentialConversion = false,
	.enableInterruptOnConversionCompleted = false
  
};

const adc16_config_t ADC16config = {
  .referenceVoltageSource = kADC16_ReferenceVoltageSourceVref,
  .clockSource = kADC16_ClockSourceAsynchronousClock,
  .enableAsynchronousClock = true,
  .clockDivider = kADC16_ClockDivider1,
  .resolution = kADC16_ResolutionSE8Bit,
  .longSampleMode = kADC16_LongSampleDisabled,
  .enableHighSpeed = false,
  .enableLowPower = false,
  .enableContinuousConversion = false
};

const adc16_channel_mux_mode_t ADC16_1_muxMode = kADC16_ChannelMuxA;
const adc16_hardware_average_mode_t ADC16_1_hardwareAverageMode = kADC16_HardwareAverageDisabled;

/**********************************************************************************************************************
*  @brief      ADC初始化
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:          AD_Init();
**********************************************************************************************************************/
void AD_Init(void)
{
  uint8_t i;
  for(i=0;i<ADC_NUM;i++)
  {
  PORT_SetPinMux(Adcs[i].PORT, Adcs[i].PIN, kPORT_PinDisabledOrAnalog);
  ADC16_Init(Adcs[i].ADC, &ADC16config);
  /* Make sure, that software trigger is used */
  ADC16_EnableHardwareTrigger(Adcs[i].ADC, false);
  /* Configure hardware average mode */
  ADC16_SetHardwareAverage(Adcs[i].ADC, ADC16_1_hardwareAverageMode);
  /* Configure channel multiplexing mode */
  ADC16_SetChannelMuxMode(Adcs[i].ADC, ADC16_1_muxMode);
  }
}

/**********************************************************************************************************************
*  @brief      获取AD转化值
*  @param      base	          	 ADC模块选择
*  @param      channelGroup       ADC模组，0或1，即A或B，按照MCUXpresso上选
*  @param      channel            ADC通道选择
*  @return     uint32_t   ADC采集数值
*  @since      v1.1
*  Sample usage:          ADC_Get(ADC0,0,4);
**********************************************************************************************************************/
uint32_t ADC_Get(ADC_Type *base, uint8_t channelGroup, uint8_t channel)
{
	ADC_channelsConfig.channelNumber = channel;
	ADC16_SetChannelConfig(base, channelGroup, &ADC_channelsConfig);
	while (0U == (kADC16_ChannelConversionDoneFlag &
		ADC16_GetChannelStatusFlags(base, channelGroup)));
	return  ADC16_GetChannelConversionValue(base, channelGroup);
}
#ifdef __cplusplus
}
#endif

