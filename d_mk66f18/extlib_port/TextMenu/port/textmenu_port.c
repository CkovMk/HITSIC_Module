#include "textmenu_port.h"

/**
 * @name 事件任务接口
 * @ {
 */
void MENU_EventService(void);

#ifdef __cplusplus
extern "C"
{
#endif

void TEXTMENU_SERVICE_IRQHandler(void)
{
    TEXTMENU_SERVICE_SEM_TAKE();
    MENU_EventService();
}

#ifdef __cplusplus
}
#endif

#include "sys_pitmgr.h"

extern pitmgr_t pitmgr_main;
extern pitmgr_handle_t menu_pitHandle;

void MENU_PORT_LowLevelResume(void)
{
    menu_pitHandle.pptFlag |= pitmgr_pptEnable;
    NVIC_EnableIRQ(TEXTMENU_SERVICE_IRQn);
}

void MENU_PORT_LowLevelSuspend(void)
{
    menu_pitHandle.pptFlag &= ~pitmgr_pptEnable;
    NVIC_DisableIRQ(TEXTMENU_SERVICE_IRQn);
}

/* @ } */

/**
 * @name 屏幕打印接口
 * @ {
 */
#include <drv_disp_ssd1306.h>
#include <lib_graphic.h>

#include "textmenu_strbuf.h"

extern disp_ssd1306_fb_t dispBuffer;
void MENU_PORT_DisplayOutput(menu_strBuf_t *_buf)
{
    DISP_SSD1306_FB_Clear(&dispBuffer);
    for (uint8_t i = 0; i < TEXTMENU_DISPLAY_STRBUF_ROW; ++i)
    {
        DISP_SSD1306_FB_Print0608_Print_Overlay(&dispBuffer, i << 3U, 1U, 1U/*f_color*/, 0U/*b_color*/, (const char*)_buf->strbuf[i]);
    }
    DISP_SSD1306_BufferUpload((uint8_t*) &dispBuffer);
}

/* @ } */


/**
 *  @name 键值数据库
 *  @ {
 */
#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

#include "easyflash.h"

status_t MENU_PORT_KVDB_GetSize(char const *_key, uint32_t *_size)
{
    ef_get_env_blob(_key, NULL, 0, _size);
    if(0U == *_size)
    {
        return kStatus_Fail;
    }
    else
    {
        return kStatus_Success;
    }
}

status_t MENU_PORT_KVDB_ReadValue(char const *_key, void *_data , uint32_t _size)
{
    uint32_t dataLen = ef_get_env_blob(_key, _data, _size, NULL);
    if(0U == dataLen)
    {
        return kStatus_Fail;
    }
    else if (dataLen > _size)
    {
        return kStatus_Fail;
    }
    else
    {
        return kStatus_Success;
    }
}

status_t MENU_PORT_KVDB_SaveValue(char const *_key, void const *_data, uint32_t _size)
{
    if(0U != ef_set_env_blob(_key, _data, _size))
    {
        return kStatus_Fail;
    }
    else
    {
        return kStatus_Success;
    }
}

status_t MENU_PORT_KVDB_DeltValue(char const *_key)
{
    if(0U !=  ef_del_env(_key))
    {
        return kStatus_Fail;
    }
    else
    {
        return kStatus_Success;
    }
}

#endif // ! TEXTMENU_USE_KVDB

/* @ } */

