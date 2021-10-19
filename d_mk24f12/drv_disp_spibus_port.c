#include <drv_disp_spibus_port.h>

dspi_transfer_t oled_spi_xfer =
{
    .txData = NULL,
    .rxData = NULL,
    .dataSize = 1,
    .configFlags = OLED_SPI_MasterCtarn | OLED_SPI_MasterPcsn | kDSPI_MasterPcsContinuous,
};

dspi_master_edma_handle_t oled_dspi_edma_m_handle;
edma_handle_t disp_dspiEdmaMasterRxRegToRxDataHandle;
#if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
edma_handle_t disp_dspiEdmaMasterTxDataToIntermediaryHandle;
#endif
edma_handle_t disp_dspiEdmaMasterIntermediaryToTxRegHandle;

/**
 * @brief SPI接口发送多个字节。
 *
 * @param data 要发送的数据
 * @param size 数据大小
 */
status_t DISP_SPIBUS_spiWrite(uint8_t *data, uint32_t size)
{

    oled_spi_xfer.txData = data;
    oled_spi_xfer.dataSize = size;
    return DSPI_MasterTransferBlocking(OLED_SPI_BASE, &oled_spi_xfer);
}

#if defined(HITSIC_DISP_SSD1306_DMA) && (HITSIC_DISP_SSD1306_DMA > 0U)

void DISP_SPIBUS_spiDmaWriteCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData);

status_t DISP_SSD1306_spiDmaInit(void)
{
    DMAMUX_SetSource(DMAMUX, OLED_SPI_DMA_RX_Chnl,
                         (uint8_t)OLED_SPI_DMA_RX_REQSRC);
    DMAMUX_EnableChannel(DMAMUX, OLED_SPI_DMA_RX_Chnl);
    DMAMUX_SetSource(DMAMUX, OLED_SPI_DMA_TX_Chnl,
                         (uint8_t)OLED_SPI_DMA_TX_REQSRC);
    DMAMUX_EnableChannel(DMAMUX, OLED_SPI_DMA_TX_Chnl);

//    EDMA_SetBandWidth(DMA0, OLED_SPI_DMA_RX_Chnl,
//                kEDMA_BandwidthStall8Cycle); //休息
//    EDMA_SetBandWidth(DMA0, OLED_SPI_DMA_TX_Chnl,
//                kEDMA_BandwidthStall8Cycle); //休息

    /* Set up dspi master */
        memset(&(disp_dspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(disp_dspiEdmaMasterRxRegToRxDataHandle));
    #if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
        memset(&(disp_dspiEdmaMasterTxDataToIntermediaryHandle), 0, sizeof(disp_dspiEdmaMasterTxDataToIntermediaryHandle));
    #endif
        memset(&(disp_dspiEdmaMasterIntermediaryToTxRegHandle), 0, sizeof(disp_dspiEdmaMasterIntermediaryToTxRegHandle));

        EDMA_CreateHandle(&(disp_dspiEdmaMasterRxRegToRxDataHandle), DMA0, OLED_SPI_DMA_RX_Chnl);
    #if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
        EDMA_CreateHandle(&(disp_dspiEdmaMasterTxDataToIntermediaryHandle), DMA0, OLED_SPI_DMA_IM_Chnl);
    #endif
        EDMA_CreateHandle(&(disp_dspiEdmaMasterIntermediaryToTxRegHandle), DMA0, OLED_SPI_DMA_TX_Chnl);
    #if (defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET)
        DSPI_MasterTransferCreateHandleEDMA(OLED_SPI_BASE, &oled_dspi_edma_m_handle, DISP_SPIBUS_spiDmaWriteCallback,
                                            NULL, &disp_dspiEdmaMasterRxRegToRxDataHandle, NULL,
                                            &disp_dspiEdmaMasterIntermediaryToTxRegHandle);
    #else
        DSPI_MasterTransferCreateHandleEDMA(OLED_SPI_BASE, &oled_dspi_edma_m_handle, DISP_SPIBUS_spiDmaWriteCallback,
                                            NULL, &disp_dspiEdmaMasterRxRegToRxDataHandle,
                                            &disp_dspiEdmaMasterTxDataToIntermediaryHandle,
                                            &disp_dspiEdmaMasterIntermediaryToTxRegHandle);
    #endif
        return kStatus_Success;
}

status_t DISP_SPIBUS_spiDmaWrite(uint8_t* data, uint32_t size)
{
    oled_spi_xfer.txData = data;
    oled_spi_xfer.dataSize = size;
    return DSPI_MasterTransferEDMA(OLED_SPI_BASE, &oled_dspi_edma_m_handle, &oled_spi_xfer);
}

void DISP_SPIBUS_spiDmaWriteCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        //PRINTF("This is DSPI master edma transfer completed callback. \r\n\r\n");
    }
    //isTransferCompleted = true;
}

#endif // ! HITSIC_DISP_SSD1306_DMA
