/**
 * @brief 陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 * @author xiao qq1761690868
 * @doc drv_imu_invensense.md
 * @version v1.0
 * @date 2020-10-16
 */

#ifndef D_DEFAULT_DRV_IMU_INVENSENSE_PORT_HPP
#define D_DEFAULT_DRV_IMU_INVENSENSE_PORT_HPP

#define HITSIC_IMU_INV_DEBUG 1
#define HITSIC_IMU_INV_YES_TRACE 0
#define HITSIC_IMU_INV_NO_DEBUG 1
//#define INV_PRINTF printf

#if (defined(HITSIC_IMU_INV_DEBUG) && (HITSIC_IMU_INV_DEBUG > 0))

#if (defined(HITSIC_IMU_INV_YES_TRACE) && (HITSIC_IMU_INV_YES_TRACE > 0))
#define INV_TRACE_(fmt, ...) \
    INV_PRINTF("[I]imu_inv line:%d:trace: " fmt "%s\r\n",  __LINE__, __VA_ARGS__)
#define INV_TRACE(...) INV_TRACE_(__VA_ARGS__, "")
#else
#define INV_TRACE(...)
#endif//(defined(HITSIC_IMU_INV_YES_TRACE)&&(HITSIC_IMU_INV_YES_TRACE>0))

#if !(defined(HITSIC_IMU_INV_NO_DEBUG) && (HITSIC_IMU_INV_NO_DEBUG > 0))
#define INV_DEBUG_(fmt, ...) \
    INV_PRINTF("[E]imu_inv line:%d:debug: " fmt "%s\r\n",  __LINE__, __VA_ARGS__)
#define INV_DEBUG(...) INV_DEBUG_(__VA_ARGS__, "")
#else
#define INV_DEBUG(...)
#endif//!(defined(HITSIC_IMU_INV_NO_DEBUG)&&(HITSIC_IMU_INV_NO_DEBUG>0))

#endif//(defined(HITSIC_IMU_INV_DEBUG)&&(HITSIC_IMU_INV_DEBUG>0))

namespace inv {
    //i2c接口
    class i2cInterface_t {
    public:
        /**
         * @param  _context          :调用函数指针传入的用户参数
         * @param  _readBlocking     :约定如下，阻塞读
         *  **************************************************
         *  * @brief   这里的函数指针的参数约定
         *  * @param  {void*}                : 用户参数
         *  * @param  {uint8_t}        : iic从机地址
         *  * @param  {uint8_t}        : 从机寄存器地址
         *  * @param  {const unsigned* char} : 缓存地址
         *  * @param  {unsigned int}         : 数据长度
         *  * @return {int}                  : 错误码
         *  **************************************************
         * @param  _writeBlocking    :约定同上，阻塞写
         * @param  _readNonBlocking  :约定同上，非阻塞读
         * @param  _writeNonBlocking :约定同上，非阻塞写
         */
        i2cInterface_t(void *_context,
                       int (*_readBlocking)(void *context,
                                            uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len),
                       int (*_writeBlocking)(void *context,
                                             uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len),
                       int (*_readNonBlocking)(void *context,
                                               uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len),
                       int (*_writeNonBlocking)(void *context,
                                                uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len))
                : context(_context), readBlocking(_readBlocking), writeBlocking(_writeBlocking),
                  readNonBlocking(_readNonBlocking), writeNonBlocking(_writeNonBlocking) {}

        /**
         * @brief imu_t会调用此方法实现读写iic
         *          此方法将调用构造时传入的函数指针
         * @param  {unsigned} char  :
         * @param  {unsigned} char  :
         * @param  {unsigned*} char :
         * @param  {unsigned} int   :
         * @return {int}            : 错误码
         */
        int ReadBlocking(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len) {
            return (*readBlocking)(context, addr, reg, val, len);
        }

        /**
         * @brief imu_t会调用此方法实现读写iic
         *          此方法将调用构造时传入的函数指针
         * @param  {unsigned} char        :
         * @param  {unsigned} char        :
         * @param  {const unsigned*} char :
         * @param  {unsigned} int         :
         * @return {int}                  : 错误码
         */
        int WriteBlocking(uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len) {
            return (*writeBlocking)(context, addr, reg, val, len);
        }

        /**
         * @brief imu_t会调用此方法实现异步读写iic
         *          此方法将调用构造时传入的函数指针
         * @param  {unsigned} char  :
         * @param  {unsigned} char  :
         * @param  {unsigned*} char :
         * @param  {unsigned} int   :
         * @return {int}            : 错误码
         */
        int ReadNonBlocking(uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len) {
            return (*readNonBlocking)(context, addr, reg, val, len);
        }

        /**
         * @brief imu_t会调用此方法实现异步读写iic
         *          此方法将调用构造时传入的函数指针
         * @param  {unsigned} char        :
         * @param  {unsigned} char        :
         * @param  {const unsigned*} char :
         * @param  {unsigned} int         :
         * @return {int}                  : 错误码
         */
        int WriteNonBlocking(uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len) {
            return (*writeNonBlocking)(context, addr, reg, val, len);
        }

    protected:
        void *context;
        int (*readBlocking)(void *context,
                            uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);
        int (*writeBlocking)(void *context,
                             uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len);
        int (*readNonBlocking)(void *context,
                               uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);
        int (*writeNonBlocking)(void *context,
                                uint8_t addr, uint8_t reg, const uint8_t *val, unsigned int len);
    };
}
#endif //D_DEFAULT_DRV_IMU_INVENSENSE_PORT_HPP
