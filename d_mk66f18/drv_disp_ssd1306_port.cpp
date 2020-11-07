#include "drv_disp_ssd1306_port.hpp"

dspi_transfer_t oled_spi_xfer =
        {
            .txData = NULL,
            .rxData = NULL,
            .dataSize = 1,
            .configFlags = OLED_SPI_MasterCtarn | OLED_SPI_MasterPcsn | kDSPI_MasterPcsContinuous,
        };

dspi_master_edma_handle_t g_dspi_edma_m_handle;
edma_handle_t dspiEdmaMasterRxRegToRxDataHandle;
#if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
edma_handle_t dspiEdmaMasterTxDataToIntermediaryHandle;
#endif
edma_handle_t dspiEdmaMasterIntermediaryToTxRegHandle;
