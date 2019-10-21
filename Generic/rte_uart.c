#include "rte_uart.h"


#ifdef __cplusplus
extern "C" {
#endif

status_t UART_SimpleInit(UART_Type* base, uint32_t baudRate_Bps_)
{
	uart_config_t config;
	/*
	 * config.baudRate_Bps = 115200U;
	 * config.parityMode = kUART_ParityDisabled;
	 * config.stopBitCount = kUART_OneStopBit;
	 * config.txFifoWatermark = 0;
	 * config.rxFifoWatermark = 1;
	 * config.enableTx = false;
	 * config.enableRx = false;
	 */
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = baudRate_Bps_;
        config.txFifoWatermark = 0;
	config.enableTx = true;
	config.enableRx = true;

	uint32_t clkFreq = 0;

	switch ((uint32_t)base)
	{
	case UART0_BASE:
	case UART1_BASE:
		clkFreq = CLOCK_GetFreq(SYS_CLK);
		break;
	default:
		clkFreq = CLOCK_GetFreq(BUS_CLK);
		break;
	}

	return UART_Init(base, &config, clkFreq);
}

int UART_Printf(UART_Type* base, const char* Format, ...)
{
	char buf[128];
	int len;
	va_list args;
	va_start(args, Format);
	len = vsnprintf(buf, 128, Format, args);
	if (len > sizeof(buf))
	{
		PRINTF("message");
		PRINTF("meemory leak\r\n");
	}
	//len = vsprintf_s(buf,sizeof(buf), Format, args);
	va_end(args);
	UART_WriteBlocking(base, (uint8_t*)buf, len);
	return len;
}



#ifdef __cplusplus
}
#endif