#include "app_menu_port.hpp"

#if defined(HITSIC_MENU_USE_FRAME_BUFFER) && (HITSIC_MENU_USE_FRAME_BUFFER > 0)

extern disp_ssd1306_frameBuffer_t dispBuffer;
extern graphic::bufPrint0608_t<disp_ssd1306_frameBuffer_t> bufPrinter;

void MENU_FrameBufferClear(void)
{
    dispBuffer.Clear();
}

void MENU_FrameBufferPrint(uint16_t x, uint16_t y, char *str)
{
    bufPrinter.Print(y << 3U, x, true, false, str);
}

void MENU_FrameBufferUpdate(void)
{
    DISP_SSD1306_BufferUpload((uint8_t*) &dispBuffer);
}

#else // HITSIC_MENU_USE_FRAME_BUFFER
// nothing here.
#endif // ! HITSIC_MENU_USE_FRAME_BUFFER

void MENU_EventService(void);

#ifdef __cplusplus
extern "C"
{
#endif

void HITSIC_MENU_SERVICE_IRQHandler(void)
{
    HITSIC_MENU_SERVICE_SEM_TAKE();
    MENU_EventService();
}

#ifdef __cplusplus
}
#endif


