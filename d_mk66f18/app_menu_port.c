#include <app_menu_port.h>

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

extern disp_ssd1306_fb_t dispBuffer;
void MENU_DisplayOutput(menu_strBuf_t *_buf)
{
    DISP_SSD1306_FB_Clear(&dispBuffer);
	for (uint8_t i = 0; i < HITSIC_MENU_DISPLAY_STRBUF_ROW; ++i)
	{
        DISP_SSD1306_FB_Print0608_Print_Overlay(&dispBuffer, i << 3U, 1U, 1U/*f_color*/, 0U/*b_color*/, (const char*)_buf->strbuf);
	}
	DISP_SSD1306_BufferUpload((uint8_t*) &dispBuffer);
}

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


