#include "textmenu_port.h"

/**
 * @name 事件任务接口
 * @ {
 */

/**
 * @brief 菜单事件服务前置定义。
 *
 * 该函数需要在事件处理线程中调用。每收到一次信号量，该线程激活一次，运行此函数。
 * 
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

/**
 * @brief 底层就绪函数。
 *
 * 调用该函数使能菜单定时触发器，并使能事件处理线程，使其可以接收信号量。
 * 
 */
void MENU_PORT_LowLevelResume(void)
{
    menu_pitHandle.pptFlag |= pitmgr_pptEnable;
    NVIC_EnableIRQ(TEXTMENU_SERVICE_IRQn);
}

/**
 * @brief 底层挂起函数。
 *
 * 调用该函数禁用菜单定时触发器，并禁用事件处理线程，使其无法接收信号量。
 * 
 */
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
#include <drv_disp_ssd1327.h>
#include <lib_graphic.h>

#include "textmenu_strbuf.h"

/**
 * @brief 向显示设备输出画面。
 *
 * 关于字符缓存的格式，启用或禁用色盘（Palette）时有不同的定义。详见相关文档。
 * 
 * @param _buf 要输出的字符缓存指针。
 */
void MENU_PORT_DisplayOutput(menu_strBuf_t *_buf)
{
    extern disp_ssd1327_fb_t dispBuffer;
    DISP_SSD1327_FB_Clear(&dispBuffer);
    for (uint8_t i = 0; i < TEXTMENU_DISPLAY_STRBUF_ROW; ++i)
    {
        DISP_SSD1327_FB_Print0608_Print_Overlay(&dispBuffer, i << 3U, 1U, 0x0fU/*f_color*/, 0x00U/*b_color*/, (const char*)_buf->strbuf[i]);
    }
    DISP_SSD1327_BufferUpload((uint8_t*) &dispBuffer);
}

/* @ } */


/**
 *  @name 键值数据库
 *  @ {
 */
#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

#include "easyflash.h"

/**
 * @brief 通过键获取对应值的大小。
 * 
 * @param _key 想要读取的键。
 * @param _size 保存大小的指针。
 * @return status_t 成功返回kStatus_Success，失败返回kStatus_Fail。
 */
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

/**
 * @brief 读取值。
 * 
 * @param _key 想要读取的键。
 * @param _data 存放读取数据的指针。
 * @param _size 提供的缓存区的大小。
 * @return status_t 成功返回kStatus_Success，失败（键不存在或缓存区太小）返回kStatus_Fail。
 */
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

/**
 * @brief 保存值。
 * 
 * @param _key 想要保存的键。
 * @param _data 存放保存数据的指针。
 * @param _size 保存数据的大小。
 * @return status_t 成功返回kStatus_Success，失败返回kStatus_Fail。
 */
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

/**
 * @brief 删除键和对应值。
 * 
 * @param _key 想要删除的键。
 * @return status_t 成功返回kStatus_Success，失败返回kStatus_Fail。
 */
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

