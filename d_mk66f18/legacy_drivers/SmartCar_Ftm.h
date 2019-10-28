#ifndef _SMARTCAR_FTM_H
#define _SMARTCAR_FTM_H
#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif

void Ftm_QuadDecode_Init(FTM_Type *base);
void Ftm_PWM_Init(FTM_Type *base, ftm_chnl_t chnlNumber, uint32_t dutyFre, float dutyCycle);
void Ftm_PWM_Change(FTM_Type *base, ftm_chnl_t chnlNumber, uint32_t dutyFre, float dutyCycle);
void Ftm_QuadDecode_Init(FTM_Type *base);
int16_t Ftm_GetSpeed(FTM_Type *base);
void Ftm_ClearSpeed(FTM_Type *base);
#ifdef __cplusplus
}
#endif
#endif