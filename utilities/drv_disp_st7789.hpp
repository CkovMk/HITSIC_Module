#pragma once
#ifndef UTILITIES_DRV_DISP_ST7789_HPP
#define UTILITIES_DRV_DISP_ST7789_HPP

#include "hitsic_common.h"
#include "sys_pitmgr.hpp"

#if defined(HITSIC_USE_DISP_ST7789) && (HITSIC_USE_DISP_ST7789 > 0)




class st7789_t
{
public:
	enum statusFlag_t : uint8_t
	{
		edmaLock = 1 << 0,
		edmaDone = 1 << 1,
		edmaError = 1 << 2,
	};
	uint8_t statusFlag;
	static st7789_t& getInst(void)
	{
		static st7789_t inst;
		return inst;
	}

	LPSPI_Type* base;
	uint32_t edmaChnl;

	edma_handle_t edmaTxHandle;

	static void edmaCallBack(LPSPI_Type* base, lpspi_master_edma_handle_t* handle, status_t status, void* userData);


	status_t spiTxPoll(uint8_t* data, uint32_t size);
	status_t spiTxEdma(uint8_t* data, uint32_t size);
	void setConfigPoll(uint8_t* cmd, uint8_t* dat, uint32_t size);
	void setRST(uint8_t b);
	void setD_C(uint8_t b);
	void setBKL(uint8_t b);


	void setup(void);
	void setRange(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2);
	void fill(uint16_t col);


private:
	st7789_t(void);


};

#endif // ! HITSIC_USE_DISP_ST7789

#endif // ! UTILITIES_DRV_DISP_ST7789_HPP
