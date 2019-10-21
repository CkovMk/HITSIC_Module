#include "rte_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

status_t SPI_SimpleInit(SPI_Type* base, uint32_t whichpcs, uint32_t baudRate_Bps_)
{
	dspi_master_config_t masterConfig;
	/* Master config */
	masterConfig.whichCtar = (dspi_ctar_selection_t)whichpcs;
	masterConfig.ctarConfig.baudRate = baudRate_Bps_;
	masterConfig.ctarConfig.bitsPerFrame = 8U;
	masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / baudRate_Bps_;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / baudRate_Bps_;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / baudRate_Bps_;

	masterConfig.whichPcs = (dspi_which_pcs_t)(1 << whichpcs);
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK = false;
	masterConfig.enableRxFifoOverWrite = false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint = kDSPI_SckToSin0Clock;
	DSPI_MasterInit(base, &masterConfig, CLOCK_GetFreq(BUS_CLK));
	return 0;
}

status_t SPI_FullDuplexXfer(SPI_Type* base, uint32_t whichpcs, uint8_t* rxbuf, uint8_t* txbuf, uint32_t len)
{
	dspi_transfer_t masterXfer;
	masterXfer.txData = txbuf;
	masterXfer.rxData = rxbuf;
	masterXfer.dataSize = len;
	masterXfer.configFlags = (whichpcs << DSPI_MASTER_CTAR_SHIFT) | (whichpcs << DSPI_MASTER_PCS_SHIFT) | kDSPI_MasterPcsContinuous;
	return DSPI_MasterTransferBlocking(base, &masterXfer);
}

#ifdef __cplusplus
}
#endif
