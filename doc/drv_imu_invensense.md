# DRV_IMU_InvenSense
[toc]


## 1.简介
这是给mpu6050,mpu9250,icm20602等InvenSense的imu设计的驱动，鉴于i2c的简便和可靠性本驱动仅设计了以i2c为接口。

注意，自检的时候保持传感器静止，自检不通过说明传感器内部微机械结构已经出问题了，建议更换传感器

返回类型为int的函数返回错误码，为0代表正常，其余都是错误。

返回类型为bool的函数返回true代表成功，false失败，和返回int的函数刚好相反

override方法的注释请参看它在基类中的注释

命名空间为inv

## 2.类图
![类图](inv_imu_invensense.assets/ClassDiagram.png)

## 3.API

### 3.1 快速了解

class i2cInterface_t;//i2c接口
struct config_t;//设置量程和数字低通滤波器

class imu_t;	//imu接口类，抽象出初始化/数据转换/SelfTest/Detect/IO的欸皮埃

class mpuSeries_t : public imu_t ;//基类，抽象出invensense的mpu系列以及部分icm系列imu的软复位/中断/温度转换/WhoAmI的api，实现初始化/数据转换/Detect/IO的方法

class mpu6050_t : public mpuSeries_t;//mpu6050驱动
class mpu6500Series_t : public mpuSeries_t;//基类，实现mpu6500系列以及部分icm系列imu的自检方法

class icm20602_t : public mpu6500Series_t;//icm20602驱动
class mpu9250_t : public mpu6500Series_t;//mpu9250驱动

class imuPtr_t : public std::shared_ptr<imu_t>;//imu_t的智能指针类，用于实例化imu_t对象



### 3.2 class i2cInterface_t;//i2c接口

```c++
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
                                                const unsigned char *val, unsigned int len));

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
                         unsigned char *val, unsigned int len) ;

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
                          const unsigned char *val, unsigned int len) ;

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
                            unsigned char *val, unsigned int len) ;

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
                             const unsigned char *val, unsigned int len) ;
    };
```

构造时需传入4个函数指针和一个用户指针（void*)，之后的imu驱动将通过i2cInterface_t对象访问i2c上的传感器，更多信息查看移植部分

### 3.3 struct config_t;//设置量程和数字低通滤波器

一个很普通的结构体

```c++
struct config_t {
        enum mpu_accel_fs {    // In the ACCEL_CONFIG (0x1C) register, the full scale select  bits are :
            MPU_FS_2G = 0,    // 00 = 2G
            MPU_FS_4G,        // 01 = 4
            MPU_FS_8G,        // 10 = 8
            MPU_FS_16G,        // 11 = 16
            NUM_MPU_AFS
        } accelFullScale;

        /** @brief Allowed value for accel DLPF bandwidth (ACCEL_CONFIG2 (0x1D) register) */
        enum mpu_accel_bw {        // In the ACCEL_CONFIG2 (0x1D) register, the BW setting bits are :
            MPU_ABW_218 = 1,    ///< 001 = 218 Hz
            MPU_ABW_99,            ///< 010 = 99 Hz
            MPU_ABW_45,            ///< 011 = 45 Hz
            MPU_ABW_21,            ///< 100 = 21 Hz
            MPU_ABW_10,            ///< 101 = 10 Hz
            MPU_ABW_5,            ///< 110 = 5 Hz
            MPU_ABW_420,        ///< 111 = 420 Hz
            NUM_MPU_ABW
        } accelBandwidth;

        enum mpu_gyro_fs {        // In the GYRO_CONFIG register, the fS_SEL bits are :
            MPU_FS_250dps = 0,    // 00 = 250
            MPU_FS_500dps,        // 01 = 500
            MPU_FS_1000dps,        // 10 = 1000
            MPU_FS_2000dps,        // 11 = 2000
            NUM_MPU_GFS
        } gyroFullScale;

        /** @brief Allowed value for gyro DLPF bandwidth (CONFIG (0x1A) register) */
        enum mpu_gyro_bw {   // In the CONFIG register, the  BW setting bits are :
            MPU_GBW_250 = 0, ///< 000 = 250 Hz
            MPU_GBW_176 = 1, ///< 001 = 176 Hz
            MPU_GBW_92,         ///< 010 = 92 Hz
            MPU_GBW_41,         ///< 011 = 41 Hz
            MPU_GBW_20,         ///< 100 = 20 Hz
            MPU_GBW_10,         ///< 101 = 10 Hz
            MPU_GBW_5,         ///< 110 = 5 Hz
            NUM_MPU_GBW
        } gyroBandwidth;

        config_t(mpu_accel_fs _accel_fs = MPU_FS_8G, mpu_accel_bw _accel_bw = MPU_ABW_99,
                 mpu_gyro_fs _gyro_gs = MPU_FS_2000dps, mpu_gyro_bw _gyro_bw = MPU_GBW_92) :
                accelFullScale(_accel_fs), accelBandwidth(_accel_bw),
                gyroFullScale(_gyro_gs), gyroBandwidth(_gyro_bw) {}
    };
```

### 3.4 驱动类

> 和代码中相比删除了变量和非pubulic方法

#### 3.4.1 class imu_t

```c++
    class imu_t {
    public:
        /**
         * @brief   初始化imu，初始化之后才能使用其他方法
         * @param  {config_t} _cfg : 量程等配置信息
         * @return {int}           : 错误码
         */
        virtual int Init(config_t _cfg = config_t()) = 0;

        /**
         * @brief   可以在初始化之前执行，检查是否有imu，并且设置正确的iic地址
         * @return {bool}  : true为成功
         */
        virtual bool Detect() = 0;

        /**
         * @brief 自检，自检过程中保持传感器静止
         * @return {int}  : 错误码
         */
        virtual int SelfTest() = 0;

        /**
         * @brief   转换！！！缓存！！!中加速度和陀螺仪的数据到指定的地方，单位如下
         * @param  {float*} acc_x  :可以等于NULL，米每平方妙
         * @param  {float*} acc_y  :可以等于NULL，米每平方妙
         * @param  {float*} acc_z  :可以等于NULL，米每平方妙
         * @param  {float*} gyro_x :可以等于NULL， dps(degree per second)
         * @param  {float*} gyro_y :可以等于NULL， dps(degree per second)
         * @param  {float*} gyro_z :可以等于NULL， dps(degree per second)
         * @return {int}           :错误码
         */
        virtual int Converter(float *acc_x, float *acc_y, float *acc_z,
                              float *gyro_x, float *gyro_y, float *gyro_z) = 0;

        /**
         * @brief   转换！！！缓存！！!中加速度和陀螺仪的数据到指定的地方，单位为LSB
         * @param  {int16_t*} acc_x  : 可以等于NULL
         * @param  {int16_t*} acc_y  : 可以等于NULL
         * @param  {int16_t*} acc_z  : 可以等于NULL
         * @param  {int16_t*} gyro_x : 可以等于NULL
         * @param  {int16_t*} gyro_y : 可以等于NULL
         * @param  {int16_t*} gyro_z : 可以等于NULL
         * @return {int}             : 错误码
         */
        virtual int Converter(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z,
                              int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) = 0;
        /**
         * @brief   转换！！！缓存！！!中磁力计的数据到指定的地方，单位为uT
         * @param  {float*} mag_x : 可以等于NULL
         * @param  {float*} mag_y : 可以等于NULL
         * @param  {float*} mag_z : 可以等于NULL
         * @return {int}          : 错误码
         */
        virtual int Converter(float *mag_x, float *mag_y, float *mag_z) = 0;
        /**
         * @brief   转换！！！缓存！！!中磁力计的数据到指定的地方，单位为LSB
         * @param  {int16_t*} mag_x : 可以等于NULL
         * @param  {int16_t*} mag_y : 可以等于NULL
         * @param  {int16_t*} mag_z : 可以等于NULL
         * @return {int}            : 错误码
         */
        virtual int Converter(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) = 0;

        /**
         * @brief   调用阻塞IO读取传感器数据到缓存，当读取完成或者发送错误时返回
         * @return {int}  : 错误码
         */
        virtual int ReadSensorBlocking() = 0;

        /**
         * @brief   调用非阻塞IO读取传感器数据到缓存，立即返回，请自行确定非阻塞读取是否完成
         * @return {int}  : 错误码
         */
        virtual int ReadSensorNonBlocking() = 0;

        /**
         * @brief   返回字符串告诉用户传感器的模组信息
         * @return {std::string}  : 字符串
         */
        virtual std::string Report() = 0;

    public:

        /**
         * @brief   返回是否初始化
         * @return {bool}  : true为是
         */
        bool IsOpen() { return isOpen; };

        /**
         * @brief   设置内存中的量程等配置信息，不会对传感器生效
         * @param  {config_t} _cfg : 
         */
        void SetConfig(config_t _cfg) { cfg = _cfg; }

        /**
         * @return {config_t}  :量程等配置信息
         */
        constexpr const config_t &GetConfig() { return cfg; }

        /**
         * @brief   设置通讯中的iic从机地址
         * @param  {uint8_t} _addr : 从机地址
         */
        void SetI2cAddr(uint8_t _addr) { addr = _addr; }

        /**
         * @return {uint8_t}  : 从机地址
         */
        constexpr const uint8_t &GetI2cAddr() { return addr; }
	protected:
        /**
         * imu_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        imu_t(i2cInterface_t &_i2c);
    };
```

#### 3.4.2 class mpuSeries_t

```c++
    class mpuSeries_t : public imu_t {
    public:
        int Init(config_t _cfg = config_t()) override;
        bool Detect() override;
        int Converter(float *acc_x, float *acc_y, float *acc_z,
                      float *gyro_x, float *gyro_y, float *gyro_z) override;        
        int Converter(float *mag_x, float *mag_y, float *mag_z) override;
        int ReadSensorBlocking() override;
        int ReadSensorNonBlocking() override;
    public:
        /**
         * @brief   软复位，可以在初始化之前执行
         * @return {int}  : 错误码
         */
        virtual int SoftReset() = 0;

        /**
         * @brief   转换！！！缓存！！!中加速度和陀螺仪的数据到指定的地方，单位为LSB
         * @param  {int16_t*} acc_x  : 可以等于NULL
         * @param  {int16_t*} acc_y  : 可以等于NULL
         * @param  {int16_t*} acc_z  : 可以等于NULL
         * @param  {int16_t*} gyro_x : 可以等于NULL
         * @param  {int16_t*} gyro_y : 可以等于NULL
         * @param  {int16_t*} gyro_z : 可以等于NULL
         * @return {int}             : 错误码
         */
        virtual int Converter(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z,
                              int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) ;
        /**
         * @brief   转换！！！缓存！！!中磁力计的数据到指定的地方，单位为LSB
         * @param  {int16_t*} mag_x : 可以等于NULL
         * @param  {int16_t*} mag_y : 可以等于NULL
         * @param  {int16_t*} mag_z : 可以等于NULL
         * @return {int}            : 错误码
         */
        virtual int Converter(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z);
        
        /**
         * @brief   转换缓冲中的温度到其他地方，单位为摄氏度
         * @param  {float*} temp :可以等于NULL
         * @return {int}         :错误码
         */
        virtual int Converter(float *temp) = 0;

        /**
         * @brief   使能传感器的DataReady中断
         * @return {int}  : 
         */
        virtual int EnableDataReadyInt();

        /**
         * @brief   返回传感器的DataReady中断状态
         * @return {bool}  :
         */
        virtual bool DataReady();

        /**
         * @brief   返回传感器WHO_AM_I寄存器的出厂默认值
         * @return {uint8_t}  : 
         */
        virtual uint8_t WhoAmI() = 0;

        /**
         * @brief   返回传感器WHO_AM_I寄存器的寄存器地址
         * @return {uint8_t}           :
         */
        virtual uint8_t RegWhoAmI() { return (uint8_t) icm20602_RegMap::WHO_AM_I; }
    protected:
        /**
         * mpuSeries_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        mpuSeries_t(i2cInterface_t &_i2c);
    };

```

#### 3.4.3 class mpu6050_t

```c++
    class mpu6050_t : public mpuSeries_t {
    public:
        /**
         * mpu6050_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        mpu6050_t(i2cInterface_t &_i2c) : mpuSeries_t(_i2c) {}
        int SelfTest() override;
        int Converter(float *temp) override;
        std::string Report() override;
        int SoftReset(void) override;
        uint8_t WhoAmI() override { return 0x68; }
    };

```

#### 3.4.4 class mpu6500Series_t

```c++

    class mpu6500Series_t : public mpuSeries_t {
    protected:
        /**
         * mpu6500Series_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        mpu6500Series_t(i2cInterface_t &_i2c) : mpuSeries_t(_i2c) {}
    public:
        int SelfTest() override;

        /**
         * @brief   返回加速度自检出厂数据的寄存器的寄存器地址
         * @return {uint8_t}  : 
         */
        virtual uint8_t RegSelfTestXAccel() = 0;

        /**
         * @brief   返回陀螺仪自检出厂数据的寄存器的寄存器地址
         * @return {uint8_t}  : 
         */
        virtual uint8_t RegSelfTestXGyro() = 0;
    };

```

#### 3.4.5 class icm20602_t

```c++

    class icm20602_t : public mpu6500Series_t {
    public:
        /**
         * icm20602_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        icm20602_t(i2cInterface_t &_i2c) : mpu6500Series_t(_i2c) {}

        int SoftReset(void) override;
        int Converter(float *temp) override;
        std::string Report() override;
        uint8_t WhoAmI() override { return 0x12; }
        uint8_t RegSelfTestXAccel() override { return (uint8_t) icm20602_RegMap::SELF_TEST_X_ACCEL; }
        uint8_t RegSelfTestXGyro() override { return (uint8_t) icm20602_RegMap::SELF_TEST_X_GYRO; }
    };

```
#### 3.4.6 class mpu9250_t 

```c++

    class mpu9250_t : public mpu6500Series_t {
    public:
        /**
         * mpu9250_t 
         * 
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         */
        mpu9250_t(i2cInterface_t &_i2c);
        int Init(config_t _cfg = config_t()) override;
        int Converter(float *acc_x, float *acc_y, float *acc_z,
                      float *gyro_x, float *gyro_y, float *gyro_z) override;
        int Converter(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z,
                      int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z) override;
        int Converter(float *mag_x, float *mag_y, float *mag_z) override;
        int Converter(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z) override;
        int Converter(float *temp) override;
        int ReadSensorBlocking() override;
        int ReadSensorNonBlocking() override;
        std::string Report() override;
        int SoftReset(void) override;
        uint8_t WhoAmI() override { return 0x71; }
        uint8_t RegSelfTestXAccel() override { return (uint8_t) mpu9250_RegMap::SELF_TEST_X_ACCEL; }
        uint8_t RegSelfTestXGyro() override { return (uint8_t) mpu9250_RegMap::SELF_TEST_X_GYRO; }
    public:
        /**
         * @brief   使用mpu9250的片上iic主机控制器读写挂载在mpu9250 iic bus上的iic从机
         * @param  {unsigned} char  : iic从机地址
         * @param  {unsigned} char  : 从机寄存器地址
         * @param  {unsigned*} char : 缓存地址
         * @param  {unsigned} int   : 数据长度
         * @return {int}            : 错误码
         */
        int SubI2cRead(unsigned char addr,
                       unsigned char reg,
                       unsigned char *val,
                       unsigned int len = 1);
        /**
         * @brief   使用mpu9250的片上iic主机控制器读写挂载在mpu9250 iic bus上的iic从机
         * @param  {unsigned} char        : iic从机地址
         * @param  {unsigned} char        : 从机寄存器地址
         * @param  {const unsigned*} char : 缓存地址
         * @param  {unsigned} int         : 数据长度
         * @return {int}                  : 错误码
         */
        int SubI2cWrite(unsigned char addr,
                        unsigned char reg,
                        const unsigned char *val,
                        unsigned int len = 1);
    };

```

### 3.5 class imuPtr_t

```c++
class imuPtr_t : public std::shared_ptr<imu_t> {
    public:
        /**
         * @brief   在堆上创建并接管mpu6050,mpu9250,icm20602对象
         * @param  {i2cInterface_t &} _i2c : 用哪个iic和传感器通讯啊？
         * @return {int}                   : 错误码
         */
        int Load(i2cInterface_t &_i2c);
    };
```



## 4.如何使用

#### 4.1 步骤

1. 实例化一个class i2cInterface_t对象，更多这部分的信息查看移植部分

2. 声明一个struct config_t变量并按需修改量程，3db带宽

3. 构建imu类并初始化，这个时候，可能不知道将使用的是哪种陀螺仪，可能是mpu6050，可能是9250，那么实例化一个class imuPtr_t对象并调用其Load方法，例如

   > ```c++
   > inv::config_t cfg;
   > inv::imuPtr_t my_imu;
   > if (0 == my_imu.Load(my_i2c)){
   >     my_imu->Init(cfg);
   > }
   > ```

   如果确定使用哪种陀螺仪，确定用到轮胎磨平，赛道报废永远都不会换其他型号，那么实例化一个具体传感器的驱动比如class mpu6050_t，class icm20602_t，class mpu9250_t ，例如只使用mpu6050,则

   > ```c++
   > inv::config_t cfg;
   > inv::mpu6050_t my_imu(my_i2c);
   > my_imu.Init(cfg);
   > ```
   > 
   
4. 自检，调用SelfTest()方法自检，自检时保持传感器静止。如果自检多次不通过说明传感器的内部微机械结构已经损坏。如果觉得自检不靠谱也可以跳过自检

5. 调用ReadSensorBlocking()方法读取传感器数据并调用Converter()方法转换数据，或者，调用ReadSensorNonBlocking() 方法，等待数据传输完成之后再调用Converter()方法转换数据。

#### 4.2 示例

这个例子中使用inv::imuPtr_t来创建imu对象

```c++
int remote_i2c_read(void *context,
                    unsigned char addr, unsigned char reg,
                    unsigned char *val, unsigned int len) {
    return static_cast<remote_i2c *>(context)->Read(addr, reg, val, len);
}

int remote_i2c_write(void *context,
                     unsigned char addr, unsigned char reg,
                     const unsigned char *val, unsigned int len) {
    return static_cast<remote_i2c *>(context)->Write(addr, reg, val, len);
}

remote_i2c iic("/dev/i2c-1");
inv::i2cInterface_t my_i2c(&iic, remote_i2c_read, remote_i2c_write,
                           remote_i2c_read, remote_i2c_write);
inv::imuPtr_t my_imu;
uint8_t val;

float acc[3] = {0, 0, 0};
float gyro[3] = {0, 0, 0};
float mag[3] = {0, 0, 0};
//float temp = 0;

int main(int argc, const char **argv) {
    if (0 == my_imu.Load(my_i2c)) {
        if (my_imu->Init() == 0) {
            //自检时保持静止，否则会直接失败
            if (my_imu->SelfTest() == 0) {
                usleep(10000);//等待10ms
                my_imu->ReadSensorBlocking();
                my_imu->Converter(acc, acc + 1, acc + 2, gyro, gyro + 1, gyro + 2);
//                static_cast<inv::mpuSeries_t*>(my_imu.get())->Converter(&temp);
                my_imu->Converter(mag, mag + 1, mag + 2);
                printf("%s\r\n", my_imu->Report().c_str());
                printf("accel \t%.3f \t%.3f \t%.3f m/s^2\r\n", acc[0], acc[1], acc[2]);
                printf("gyro \t%.3f \t%.3f \t%.3f dps \r\n", gyro[0], gyro[1], gyro[2]);
                printf("mag \t%.1f \t%.1f \t%.1f uT \r\n", mag[0], mag[1], mag[2]);
//                printf("temp \t%.3f C \r\n", temp);
            } else {
                printf("自检未通过\r\n");
            }
        } else {
            printf("初始化未通过\r\n");
        }
    } else {
        printf("没有imu\r\n");
    }
    printf("Hello\r\n");
    return 0;
}

```

## 5. 如何移植

实例化一个class i2cInterface_t需要传入1个用户参数，2个i2c阻塞读写函数指针和2个i2c非阻塞读写函数执政

移植需要实现4个i2c读写函数，至少实现其中i2c阻塞读写函数

这俩个i2c阻塞读写函数的参数列表和返回值定义以及示例如下，

```c++
/**
 * 
 * @param  {void*} context  : 用户参数
 * @param  {unsigned} char  : iic从机地址
 * @param  {unsigned} char  : 从机寄存器地址
 * @param  {unsigned*} char : 缓存地址
 * @param  {unsigned} int   : 数据长度
 * @return {int}            : 错误码
 */
int remote_i2c_read(void *context,
                    unsigned char addr, unsigned char reg,
                    unsigned char *val, unsigned int len) {
    return static_cast<remote_i2c *>(context)->Read(addr, reg, val, len);
}
/**
 * 
 * @param  {void*} context        : 用户参数
 * @param  {unsigned} char        : iic从机地址
 * @param  {unsigned} char        : 从机寄存器地址
 * @param  {const unsigned*} char : 缓存地址
 * @param  {unsigned} int         : 数据长度
 * @return {int}                  : 错误码
 */
int remote_i2c_write(void *context,
                     unsigned char addr, unsigned char reg,
                     const unsigned char *val, unsigned int len) {
    return static_cast<remote_i2c *>(context)->Write(addr, reg, val, len);
}
```

另外俩个i2c非阻塞读写函数的参数列表和返回值定义和上面俩个完全一样，只不过时前者是阻塞IO，调用后会等传输完成或者发生错误返回，后者时非阻塞IO，调用后会立刻返回，完成数据传输需要另外的方式确认。

如果不打算支撑非阻塞读写i2c，可以不用实现i2c非阻塞读写函数，并在构造时传递相同的函数指针如

```c++
remote_i2c iic("/dev/i2c-1");
inv::i2cInterface_t my_i2c(&iic, remote_i2c_read, remote_i2c_write,
                           remote_i2c_read, remote_i2c_write);
```

关于调试部分参阅预处理器定义部分

#### 5.1 如何使用SPI接口

确保清楚如何移植驱动，并且知道传感器是否支持SPI接口，之后如移植部分实现那四个函数，并在函数中忽略i2c从机地址，并且按照手册中对SPI传输的规定实现对传感器内部寄存器的读写。

## 6. 如何添加其他型号的驱动

当前支持的有三款，mpu6050,mpu9250,icm20602，如果要支持其他imu：，步骤如下

1. 继承class imu_t或者class imu_t的子类，然后override基类的方法
2. 如果要使用自动创建已连接传感器的的imu_t对象的话，继承class imuPtr_t并重写Load()方法。

## 7. 预处理器定义

定义以下宏以获得相关功能

```C
#define INV_IMU_DEBUG //此宏切换debug总开关，默认关闭
#define INV_PRINTF printf //定义printf函数
#define INV_YES_TRACE //打开代码追踪输出，默认关闭
#define INV_NO_DEBUG  //关闭代码调试输出，默认打开
```

## 8. 更新记录

- 2020-10-16 v1.0

  > 1. 本驱动重构了在k66工程上的c语言版本的imu驱动，同样支持了3款mpu6050,mpu9250,icm20602，把i2c作为主要接口，并且添加了自检函数，增加使用的可靠性。
  >
  > author：网瘾少年





