#ifndef _SMARTCAR_AD_H__
#define _SMARTCAR_AD_H__
#include "stdafx.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ADC
{
  PORT_Type* PORT;
  uint8_t    PIN;
  ADC_Type*	 ADC;
} Adc_t;

void AD_Init(void);
uint32_t ADC_Get(ADC_Type *base, uint8_t channelGroup, uint8_t channel);
#ifdef __cplusplus
}
#endif
#endif
