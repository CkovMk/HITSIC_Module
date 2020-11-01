# DRV_IMU_INVENSENSE

[TOC]

## 简介

InvenSense惯性导航驱动库（DRV_IMU_INVENSENSE，简称DRV_IMU_INV），是给MPU6050、MPU9250、ICM20602等InvenSense公司开发的的惯性导航传感器（IMU）设计的驱动。

考虑到该传感器数据量不大，且I2C使用简便可靠，本驱动设计上主要考虑I2C接口。实际应用中如要使用SPI接口，请参见“移植指南 - 移植通信接口 - 使用SPI通信 ”。

**注意：自检的时候保持传感器静止，自检不通过说明传感器内部微机械结构已经出问题了，建议更换传感器**

关于返回值：

返回类型为`int`的函数返回错误码，为0代表正常，其余全是错误。

返回类型为`bool`的函数返回`true`代表成功，`false`失败，和返回`int`的函数刚好相反

**override方法的注释请参看它在基类中的注释**

命名空间：`namespace inv`

## 版本说明

### v1.0-beta.0

by [beforelight](https://github.com/beforelight/RemoteIIC)（网瘾少年） @hitsic 2020-10-16 
**改动说明**

- 本驱动重构了在k66工程上的c语言版本的IMU驱动，同样支持了3款MPU6050,MPU9250,ICM20602，把I2C作为主要接口，并且添加了自检函数，增加使用的可靠性。

**开发计划**

- 进行进一步测试，验证其稳定性。
- 提供对ICM20948的支持。

**已知问题**
- 暂无




## API文档

![类图](drv_imu_invensense.assets/ClassDiagram.png)

### 类型简介

`class i2cInterface_t;`//i2c接口
`struct config_t;`//设置量程和数字低通滤波器

`class imu_t;`	//imu接口类，抽象出初始化/数据转换/SelfTest/Detect/IO的API

`class mpuSeries_t : public imu_t ;`//基类，抽象出invensense的mpu系列以及部分icm系列imu的软复位/中断/温度转换/WhoAmI的api，实现初始化/数据转换/Detect/IO的方法

`class mpu6050_t : public mpuSeries_t;`//mpu6050驱动
`class mpu6500Series_t : public mpuSeries_t;`//基类，实现mpu6500系列以及部分icm系列imu的自检方法

`class icm20602_t : public mpu6500Series_t;`//icm20602驱动
`class mpu9250_t : public mpu6500Series_t;`//mpu9250驱动

`class imuPtr_t : public std::shared_ptr<imu_t>;`//imu_t的智能指针类，用于实例化imu_t对象



详细API文档请参考**头文件**中的注释

override方法的注释请参看它在基类中的注释



## 应用指南

#### 使用步骤

##### 不确定使用的IMU型号

1. 声明一个`i2cInterface_t`对象，这个对象负责i2c通讯

2. 声明一个`struct config_t`变量，并按需修改量程，3db带宽

3. 声明`imuPtr_t`指针类，调用`Load`方法检测i2c上的有没有imu，初始化该imu

   ```c++
   inv::imuPtr_t my_imu;
   if (0 == my_imu.Load(my_i2c)){
    my_imu->Init(cfg);
   }
   ```

4. 自检，调用`SelfTest()`方法自检，自检时保持传感器静止。如果自检多次不通过说明传感器的内部微机械结构已经损坏。如果觉得自检不靠谱也可以跳过自检

5. 调用`ReadSensorBlocking()`方法读取传感器数据并调用`Convert()`方法转换数据，或者，调用`ReadSensorNonBlocking()` 方法，等待数据传输完成之后再调用`Convert()`方法转换数据。

6. 示例见示例代码中的**example1**

##### 确定使用什么型号的IMU

确定比如确定是用mpu6050

1. 声明一个`i2cInterface_t`对象，这个对象负责i2c通讯

2. 声明一个`struct config_t`变量，并按需修改量程，3db带宽

3. 声明一个`mpu6050_t`对象，在构建传入`i2cInterface_t`对象。

4. 调用`Detect`检测是否有mpu6050，并初始化

   ```c++
   inv::mpu6050_t my_imu(my_i2c);
   if (true == my_imu.Detect()) {
       my_imu.Init();
       }
   ```

5. 自检，调用`SelfTest()`方法自检，自检时保持传感器静止。如果自检多次不通过说明传感器的内部微机械结构已经损坏。如果觉得自检不靠谱也可以跳过自检

6. 调用`ReadSensorBlocking()`方法读取传感器数据并调用`Convert()`方法转换数据，或者，调用`ReadSensorNonBlocking()` 方法，等待数据传输完成之后再调用`Convert()`方法转换数据。

7. 示例见示例代码中的**example2**

#### 示例代码

> 注意，本示例有些函数在单片机工程上不存在

```c++
#include <iostream>
#include "remote_i2c.h"
#include "drv_imu_invensense.hpp"

int remote_i2c_read(void *context,
                    uint8_t addr, uint8_t reg,
                    uint8_t *val, unsigned int len) {
    return static_cast<remote_i2c *>(context)->Read(addr, reg, val, len);
}

int remote_i2c_write(void *context,
                     uint8_t addr, uint8_t reg,
                     const uint8_t *val, unsigned int len) {
    return static_cast<remote_i2c *>(context)->Write(addr, reg, val, len);
}

remote_i2c iic("/dev/i2c-1");
inv::i2cInterface_t my_i2c(&iic, remote_i2c_read, remote_i2c_write);

float acc[3] = {0, 0, 0};
float gyro[3] = {0, 0, 0};
float mag[3] = {0, 0, 0};
float temp = 0;



int example1(int argc, const char **argv) {
    inv::imuPtr_t my_imu;
    if (0 == my_imu.Load(my_i2c)) {
        if (my_imu->Init() == 0) {
            //自检时保持静止，否则会直接失败
            if (my_imu->SelfTest() == 0) {
                usleep(10000);//等待10ms
                my_imu->ReadSensorBlocking();
                my_imu->Convert(acc, acc + 1, acc + 2, gyro, gyro + 1, gyro + 2);
//                static_cast<inv::mpuSeries_t*>(my_imu.get())->Convert(&temp);
                my_imu->Convert(mag, mag + 1, mag + 2);
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
    return 0;
}

int example2(int argc, const char **argv) {
    inv::mpu6050_t my_imu(my_i2c);
    if (true == my_imu.Detect()) {
        if (my_imu.Init() == 0) {
            //自检时保持静止，否则会直接失败
            if (my_imu.SelfTest() == 0) {
                usleep(10000);//等待10ms
                my_imu.ReadSensorBlocking();
                my_imu.Convert(acc, acc + 1, acc + 2, gyro, gyro + 1, gyro + 2);
                my_imu.Convert(&temp);
                my_imu.Convert(mag, mag + 1, mag + 2);
                printf("%s\r\n", my_imu.Report().c_str());
                printf("accel \t%.3f \t%.3f \t%.3f m/s^2\r\n", acc[0], acc[1], acc[2]);
                printf("gyro \t%.3f \t%.3f \t%.3f dps \r\n", gyro[0], gyro[1], gyro[2]);
                printf("mag \t%.1f \t%.1f \t%.1f uT \r\n", mag[0], mag[1], mag[2]);
                printf("temp \t%.3f C \r\n", temp);
            } else {
                printf("自检未通过\r\n");
            }
        } else {
            printf("初始化未通过\r\n");
        }
    } else {
        printf("没有mpu6050\r\n");
    }
    return 0;
}



int main(int argc, const char **argv){
    printf("\r\n*****************example1*****************\r\n");
    example1(argc,argv);
    printf("\t\n*****************example2*****************\r\n");
    example2(argc,argv);
    printf("Hello\r\n");

    return 0;
}

```

##### 运行结果（MPU6050)

> ```
> /tmp/tmp.5gHXYJ77gB/cmake-build-debug-/RemoteIIC
> 
> *****************example1*****************
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:809:trace: mpu6050 detected
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:559:trace: 0x40 at PWR_MGMT_1,wait it get 0x40
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:559:trace: 0x40 at PWR_MGMT_1,wait it get 0x40
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:510:trace: 6050 accel[0] self test result = 0.038087,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:510:trace: 6050 accel[1] self test result = 0.031343,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:510:trace: 6050 accel[2] self test result = 0.020956,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:523:trace: 6050 gryo[0] self test result = 0.040242,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:523:trace: 6050 gryo[1] self test result = 0.016939,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:523:trace: 6050 gryo[2] self test result = -0.132162,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:559:trace: 0x40 at PWR_MGMT_1,wait it get 0x40
> model:mpu6050	addr:104	
> accel 	-0.847 	2.716 	-9.936 m/s^2
> gyro 	-0.427 	20.630 	28.259 dps 
> mag 	0.0 	0.0 	0.0 uT 
> 	
> *****************example2*****************
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:559:trace: 0x40 at PWR_MGMT_1,wait it get 0x40
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:559:trace: 0x40 at PWR_MGMT_1,wait it get 0x40
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:510:trace: 6050 accel[0] self test result = 0.035131,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:510:trace: 6050 accel[1] self test result = 0.029827,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:510:trace: 6050 accel[2] self test result = 0.025306,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:523:trace: 6050 gryo[0] self test result = 0.034490,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:523:trace: 6050 gryo[1] self test result = 0.017075,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:523:trace: 6050 gryo[2] self test result = -0.050849,it demands >-0.14 && <0.14
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:559:trace: 0x40 at PWR_MGMT_1,wait it get 0x40
> model:mpu6050	addr:104	
> accel 	-0.785 	2.725 	-9.948 m/s^2
> gyro 	0.549 	20.874 	28.198 dps 
> mag 	0.0 	0.0 	0.0 uT 
> temp 	34.059 C 
> Hello
> 
> Process finished with exit code 0
> 
> ```
>
> 

##### 运行结果（MPU9250)

> ```
> /tmp/tmp.5gHXYJ77gB/cmake-build-debug-/RemoteIIC
> 
> *****************example1*****************
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:813:trace: mpu9250 detected
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:794:trace: 0x1 at PWR_MGMT_1,wait it get 0x1
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:595:trace: 0xaf 0xaf 0xa4 at ak8963_RegMap::ASAX
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:603:trace: 1.183594 1.183594 1.140625 at ak8963Asa
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:794:trace: 0x1 at PWR_MGMT_1,wait it get 0x1
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:595:trace: 0xaf 0xaf 0xa4 at ak8963_RegMap::ASAX
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:603:trace: 1.183594 1.183594 1.140625 at ak8963Asa
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:113:trace: accel[0] st result = 5,it demands less than 500
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:113:trace: accel[1] st result = 0,it demands less than 500
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:113:trace: accel[2] st result = 14,it demands less than 500
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:155:trace: gyro[0] st result = 17089500,it demands greater than 8763000
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:155:trace: gyro[1] st result = 19301500,it demands greater than 9583500
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:155:trace: gyro[2] st result = 24152300,it demands greater than 12048500
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:182:trace: gyro[0] st result = 80500,it demands less than 2620000
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:182:trace: gyro[1] st result = 188100,it demands less than 2620000
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:182:trace: gyro[2] st result = 50650,it demands less than 2620000
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:794:trace: 0x1 at PWR_MGMT_1,wait it get 0x1
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:595:trace: 0xaf 0xaf 0xa4 at ak8963_RegMap::ASAX
> /tmp/tmp.5gHXYJ77gB/drv_imu_invensense.cpp:603:trace: 1.183594 1.183594 1.140625 at ak8963Asa
> model:mpu9250	addr:104	magnetometer:ak8963	ID:72	INF:154	
> accel 	-4.768 	0.833 	-8.996 m/s^2
> gyro 	0.549 	-1.465 	0.549 dps 
> mag 	-89.7 	62.0 	-29.3 uT 
> 	
> *****************example2*****************
> 没有mpu6050
> Hello
> 
> Process finished with exit code 0
> 
> ```



### 调试功能

定义以下宏以获得相关功能
所有这些控制宏，都在port文件中定义，且采用`#if (defined(YOUR_MACRO) && (YOUR_MACRO >0U)) `
的形式

```C
#define HITSIC_INV_IMU_DEBUG 0 //此宏切换debug总开关
#define INV_PRINTF printf //定义printf函数
#define HITSIC_INV_YES_TRACE 0 //打开代码追踪输出
#define HITSIC_INV_NO_DEBUG 1  //关闭代码调试输出
```

举例：（注意，具体宏以代码为准，实际定义可能有些许出入）

使能驱动出错时输出消息

```c++
#define HITSIC_INV_IMU_DEBUG 1
#define HITSIC_INV_YES_TRACE 0
#define HITSIC_INV_NO_DEBUG 0
#include <cstdio>
#define INV_PRINTF printf
```

使能驱动出错时输出消息+输出其他调试消息

```c++
#define HITSIC_INV_IMU_DEBUG 1
#define HITSIC_INV_YES_TRACE 1
#define HITSIC_INV_NO_DEBUG 0
#include <cstdio>
#define INV_PRINTF printf
```



## 移植指南

### 移植通信接口

#### 使用I2C通信

实例化一个`class i2cInterface_t`需要传入1个用户参数，2个i2c阻塞读写函数指针和i2c非阻塞读函数。

**移植需要实现这几个函数，至少实现其中的i2c阻塞读写函数**

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
                    uint8_t addr, uint8_t reg,
                    uint8_t *val, unsigned int len) {
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
                     uint8_t addr, uint8_t reg,
                     const uint8_t *val, unsigned int len) {
    return static_cast<remote_i2c *>(context)->Write(addr, reg, val, len);
}
```

另外非阻塞读函数的参数列表和返回值定义和上面完全一样，只不过时一个是阻塞IO，调用后会等传输完成或者发生错误返回，另一个是非阻塞IO，调用后会立刻返回，完成数据传输需要另外的方式确认。

如果不打算支撑非阻塞读写i2c，可以不用实现i2c非阻塞读写函数，并在构造时省略后者，见示例代码或下文

```c++
remote_i2c iic("/dev/i2c-1");
inv::i2cInterface_t my_i2c(&iic, remote_i2c_read, remote_i2c_write);
```

关于调试部分请参阅预处理器定义部分

#### 使用SPI通信

确保清楚如何移植驱动，并且知道传感器是否支持SPI接口，之后如移植部分实现那四个函数，并在函数中忽略i2c从机地址，并且按照手册中对SPI传输的规定实现对传感器内部寄存器的读写。

###  添加其他型号的驱动

当前支持的有三款，MPU6050,MPU9250,ICM20602，如果要支持其他寄存器结构类似的imu：，步骤如下

1. 继承class imu_t或者class imu_t的子类，然后override基类的方法
2. 如果要使用自动创建已连接传感器的的imu_t对象的话，继承class imuPtr_t并重写Load()方法。





