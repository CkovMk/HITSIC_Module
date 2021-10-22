#include <app_menu_port.h>

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

#if defined(HITSIC_MENU_USE_FRAME_BUFFER) && (HITSIC_MENU_USE_FRAME_BUFFER > 0)

extern disp_ssd1327_fb_t dispBuffer;

void MENU_FrameBufferClear(void)
{
    DISP_SSD1327_FB_Clear(&dispBuffer);
}

void MENU_FrameBufferPrint(uint16_t x, uint16_t y, char *str)
{
    DISP_SSD1327_FB_Print0608_Print_Overlay(&dispBuffer, y << 3U, x, 0x0f, 0U, str);
}

void MENU_FrameBufferUpdate(void)
{
    DISP_SSD1327_BufferUpload(&dispBuffer);
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

#endif // ! HITSIC_USE_APP_MENU


