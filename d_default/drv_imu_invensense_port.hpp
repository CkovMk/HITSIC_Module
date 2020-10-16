/**
 * @brief 陀螺仪驱动，适用于mpu6050,mpu9250,icm20602
 * @author xiao qq1761690868
 * @doc drv_imu_invensense.md
 * @version v1.0
 * @date 2020-10-16
 */

#ifndef REMOTEIIC_DRV_IMU_INVENSENSE_PORT_HPP
#define REMOTEIIC_DRV_IMU_INVENSENSE_PORT_HPP
#if defined(INV_IMU_DEBUG)
#if defined(__linux__)
#include<iostream>
#define INV_PRINTF printf
#define INV_YES_TRACE
#undef INV_NO_DEBUG
#endif//defined(__linux__)
#else
#define INV_PRINTF(...)
#endif//defined(INV_IMU_DEBUG)
#if !defined(INV_PRINTF)&&defined(INV_IMU_DEBUG)
#error "plase def INV_PRINTF for debug and trace or undef INV_IMU_DEBUG"
#elif defined(INV_PRINTF)&&defined(INV_IMU_DEBUG)
#ifdef INV_YES_TRACE
#define INV_TRACE_(fmt, ...) \
    INV_PRINTF("%s:%d:trace: " fmt "%s\r\n", __FILE__, __LINE__, __VA_ARGS__)
#define INV_TRACE(...) INV_TRACE_(__VA_ARGS__, "")
#else
#define INV_TRACE(...)
#endif //INV_YES_TRACE
#ifndef INV_NO_DEBUG
#define INV_DEBUG_(fmt, ...) \
    INV_PRINTF("%s:%d:debug: " fmt "%s\r\n", __FILE__, __LINE__, __VA_ARGS__)
#define INV_DEBUG(...) INV_DEBUG_(__VA_ARGS__, "")
#else
#define INV_DEBUG(...)
#endif//INV_NO_DEBUG
#else
#define INV_TRACE(...)
#define INV_DEBUG(...)
#endif//!defined(INV_PRINTF)&&defined(INV_IMU_DEBUG)
namespace inv{
    //i2c接口
    class i2cInterface_t {
    public:
        /**
         * @param  _context          :调用函数指针传入的用户参数
         * @param  _readBlocking     :约定如下，阻塞读
         *  **************************************************
         *  * @brief   这里的函数指针的参数约定
         *  * @param  {void*}                : 用户参数
         *  * @param  {unsigned char}        : iic从机地址
         *  * @param  {unsigned char}        : 从机寄存器地址
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
                                            unsigned char addr, unsigned char reg,
                                            unsigned char *val, unsigned int len),
                       int (*_writeBlocking)(void *context,
                                             unsigned char addr, unsigned char reg,
                                             const unsigned char *val, unsigned int len),
                       int (*_readNonBlocking)(void *context,
                                               unsigned char addr, unsigned char reg,
                                               unsigned char *val, unsigned int len),
                       int (*_writeNonBlocking)(void *context,
                                                unsigned char addr, unsigned char reg,
                                                const unsigned char *val, unsigned int len))
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
        int ReadBlocking(unsigned char addr, unsigned char reg,
                         unsigned char *val, unsigned int len) {
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
        int WriteBlocking(unsigned char addr, unsigned char reg,
                          const unsigned char *val, unsigned int len) {
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
        int ReadNonBlocking(unsigned char addr, unsigned char reg,
                            unsigned char *val, unsigned int len) {
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
        int WriteNonBlocking(unsigned char addr, unsigned char reg,
                             const unsigned char *val, unsigned int len) {
            return (*writeNonBlocking)(context, addr, reg, val, len);
        }

    protected:
        void *context;
        int (*readBlocking)(void *context,
                            unsigned char addr, unsigned char reg,
                            unsigned char *val, unsigned int len);
        int (*writeBlocking)(void *context,
                             unsigned char addr, unsigned char reg,
                             const unsigned char *val, unsigned int len);
        int (*readNonBlocking)(void *context,
                               unsigned char addr, unsigned char reg,
                               unsigned char *val, unsigned int len);
        int (*writeNonBlocking)(void *context,
                                unsigned char addr, unsigned char reg,
                                const unsigned char *val, unsigned int len);
    };
}
#endif //REMOTEIIC_DRV_IMU_INVENSENSE_PORT_HPP
