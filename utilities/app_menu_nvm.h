#ifndef _APP_MENU_NVM_H_
#define _APP_MENU_NVM_H_

#include "app_menu_def.h"

#ifdef __cplusplus
extern "C"{
#endif 

	/**************************************
 ************ NVM存储常量定义 **********
 **************************************/

#define menu_nvm_rgCnt (3u) /// 局部存储区的数量

#define menu_nvm_sectorSize flash_sectorSize;
	/**
	 * @brief : 全局存储 Global Storage
	 */
	extern uint32_t menu_nvm_glSectCnt;	/// 全局存储区占用的扇区数
	extern uint32_t menu_nvm_glSectOffset; /// 全局存储区扇区偏移
	extern uint32_t menu_nvm_glAddrOffset;		 /// 全局存储区地址偏移
	/**
	 * @brief : 局部存储 Region Storage
	 */

	extern uint32_t menu_nvm_rgSectCnt;				/// 每个局部存储区占用的扇区数
	extern uint32_t menu_nvm_rgSectOffset[menu_nvm_rgCnt]; /// 三个局部存储区的扇区偏移

	extern uint32_t menu_nvm_rgAddrOffset[menu_nvm_rgCnt]; /// 三个局部存储区的地址偏移

	/**
	 * @brief : 菜单存储占用的总扇区数
	 */
	extern uint32_t menu_nvm_totalSectCnt;
	/**
	 * @brief : 每个菜单项保存时占用的字节数
	 */
	extern uint32_t menu_nvm_dataSize;

	/**
	 * @brief : 菜单项写入缓存。
	 * 当改写第N个扇区时，menu_nvm_cachedSector = N, menu_nvm_cache分配4KB缓存
	 * 并读入第N扇区的所有内容。此时能且仅能修改第N扇区的内容。对第N扇区内容的修改
	 * 将缓存至上述内存。
	 */
	extern uint8_t *menu_nvm_cache;
	extern uint32_t menu_nvm_cachedSector;

	/**************************************
 ************ NVM存储操作接口 **********
 **************************************/

	/**
	  * @brief : 从NVM中读取数据。
	  *
	  * @param  {uint32_t} _addr    : 要读取的NVM地址。
	  * @param  {void*} _buf        : 用于保存数据的缓冲区。
	  * @param  {uint32_t} _byteCnt : 要读取的字节数。
	  *
	  * @return {status_t}          : 返回运行结果。正常返回kStatus_Success。
	  */
	status_t MENU_NvmRead(uint32_t _addr, void *_buf, uint32_t _byteCnt);

	/**
	 * @brief : 检查地址是否在可缓存范围内。
	 * 如果当前未缓存任何扇区（menu_nvm_cache == NULL），始终返回true。如果当前已有缓存扇区，则根据保存的
	 * 扇区号判断输入地址是否在缓存范围内。
	 *
	 * @param  {uint32_t} _addr : 要检查的地址
	 * @return {bool}           : 可缓存返回true，不可缓存返回false。
	 */
	bool MENU_NvmCacheable(uint32_t _addr); //check if addr is ready to write.

	status_t MENU_NvmCacheSector(uint32_t _sect);

	/**
	 * @brief : 将数据写入缓存。
	 * 该函数不会检查输入地址及数据大小是否在可缓存范围内。
	 * 如果当前未缓存任何扇区，将创建缓存。
	 * @param  {uint32_t} _addr    :
	 * @param  {void*} _buf        :
	 * @param  {uint32_t} _byteCnt :
	 * @return {status_t}          : 成功写入返回kStatus_Success，失败返回kStatus_Fail。
	 */
	status_t MENU_NvmWriteCache(uint32_t _addr, void *_buf, uint32_t _byteCnt);

	/**
	 * @brief : 将缓存的修改写回NVM。
	 * 该函数先擦书缓存的扇区，然后将缓存的内容写回该扇区，最后释放缓存区内存，将缓存区指针赋为NULL。
	 * 缓存的扇区号将保留原值。
	 *
	 * @return {status_t}       : 成功写回返回kStatus_Success。
	 */
	status_t MENU_NvmUpdateCache(void);



#ifdef __cplusplus
}
#endif 


#endif // ! _APP_MENU_NVM_H_