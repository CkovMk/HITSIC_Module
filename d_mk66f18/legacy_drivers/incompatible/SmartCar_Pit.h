#ifndef _SMARTCAR_PIT_H
#define _SMARTCAR_PIT_H

#include "fsl_pit.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_ftm.h"
#include "clock_config.h"
#include "pin_mux.h"

void Pit_Init(void);
void Pit_Interrupt_Init(pit_chnl_t channel, uint32_t msec);
void Time_Start(pit_chnl_t channel);
uint32_t Get_Time(pit_chnl_t channel);
uint32_t Get_Time_us(pit_chnl_t channel);
void Pit_ChangeTime(pit_chnl_t channel, uint32_t msec);//纯属娱乐，这个函数好像没什么用。。。。

#endif