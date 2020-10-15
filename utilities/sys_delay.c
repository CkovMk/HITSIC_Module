#include "sys_delay.h"

#if defined(HITSIC_USE_SYS_DELAY) && (HITSIC_USE_SYS_DELAY>0)
#ifdef __cplusplus
extern "C"
{
#endif
#if defined(HITSIC_SYS_DELAY_FREE) && (HITSIC_SYS_DELAY_FREE > 0)
	void SysTick_Delay(uint32_t time)
	{
		if (time == 0) return;

		SysTick->CTRL = 0x00;
		SysTick->LOAD = time;//延时时间load
		SysTick->VAL = 0x00;
		SysTick->CTRL = (0 | SysTick_CTRL_ENABLE_Msk			//使能Systick
						//|SysTick_CTRL_TICK_Msk			//使能中断（注释表示关闭中断）
			| SysTick_CTRL_CLKSOURCE_Msk		//时钟源选择
			);
		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) ;	//等待时间到
	}

	void SysTick_Delay_Ms(uint32_t ms)
	{
		uint32_t core_clk_freq;
		core_clk_freq = CLOCK_GetCoreSysClkFreq();
		while (ms--)SysTick_Delay(core_clk_freq / 1000);
	}

	void SysTick_Timing(uint32_t time)
	{
		time &= 0x00ffffff;
		SysTick->LOAD = time;
		SysTick->VAL = 0x00;	//计数器清零
		SysTick->CTRL = (0 | SysTick_CTRL_ENABLE_Msk	//使能SysTick
			| SysTick_CTRL_TICKINT_Msk					//使能中断
			| SysTick_CTRL_CLKSOURECE_Msk				//时钟源选择
			);
	}

	void SysTick_Getval(void)
	{
		return SysTick->VAL;
	}
#endif
	void Sys_Pit_My_Init()
	{
		pit_config_t pitConfig;
		pitConfig.enableRunInDebug = 1;					//在debug模式下依然允许计时器计数
		PIT_Init(PIT, &pitConfig);
	}

	void Sys_Pit_Set_Period(PIT_Type* base, pit_chnl_t channel, uint32_t time)
	{
		uint32_t count;
		count = USEC_TO_COUNT(time, CLOCK_GetFreq(kCLOCK_BusClk));		//计算得到实际的计数
		PIT_SetTimerPeriod(base, channel, count);		//具体设置计时周期函数在这里更改
	}

	void Sys_Pit_Start_Timer(PIT_Type* base, pit_chnl_t channel)
	{
		PIT_StartTimer(base, channel);
	}

	void Sys_Pit_Stop_Timer(PIT_Type* base, pit_chnl_t channel)
	{
		PIT_StopTimer(base, channel);
	}
	uint32_t Sys_Pit_Flag_Get(PIT_Type* base, pit_chnl_t channel)
	{
		return PIT_GetStatusFlags(base, channel);
	}

#endif