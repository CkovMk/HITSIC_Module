# LIB_ADRCCTRL

[TOC]

## 版本

针对

## 简介

adrc控制器，库

**ADRC**是介于**PID**和**控制系统状态反馈的极点配置控制器**之间的一种控制器,结合了二者的优点。简单来说，ADRC就是假设被控系统为二阶系统的PID控制器。想了解更多内容请阅读附录中的资料。

> LQR是极点配置控制器的子集

ADRC控制器包括三个组件：跟踪微分器(TD)、非线性状态反馈（非线性组合）、扩张观测器(ESO)。在本ADRC控制器库中，跟踪微分器TD作为单独了模块提供，可做滤波器使用。

## 符号定义

本库关于ADRC的符号定义同附录1

## 示例

### TD

td作为滤波器使用的例子

```c
adrc_td_t filter;
#define period 5 //ms
void Init(){
    ADRCCTRL_TDInit(&filter,0.001*period,1.3456789);
    reg2YourMenu(&filter.r);
}
void loop5ms(){
    ADRCCTRL_TD(&filter,sensorGet());
    sensorData = filter.v1;
}
```

### ADRC

```c
adrc_t ctrl;
#define period 5 //ms
void Init(){
    ADRCCTRL_Init(&ctrl,0.001*period);
    reg2YourMenu(&ctrl.r);
    reg2YourMenu(&ctrl.u_max);
    reg2YourMenu(&ctrl.u_min);
    reg2YourMenu(&ctrl.b);
    reg2YourMenu(&ctrl.w0);
    reg2YourMenu(&ctrl.kp);
    reg2YourMenu(&ctrl.kd);
    reg2YourMenu(&ctrl.v);
}
void loop5ms(){
    ctrl.y = sensorGet();
    ADRCCTRL_Update(&ctrl);
    pwm = ctrl.u;
}
```

## 调参经验

控制器库里，一个控制环里有8个需要整定的参数，我在代码中注释了这些变量的作用。

**u_min u_max**

> 输出限幅，如果不限幅的话u会发散



**h** 

> 控制环周期， 2ms的控制周期的话h就是0.002，



**r**

> 跟随器的参数，含义是将v1的对时间二阶导数限制在-r,r之间，如果不需要滤波的话给个超级大的值就行



**kp kd b w0**

> 怎么调这4个看附录1，要手动调参的就这四个了，下面是我的经验：
>
> kp kd的含义和pid一致，如果从kd控制器过渡到ADRC的话，将原来的kp kd削减一半就差不多（前提单位一致）。
>
> b 和w0的话没有什么太大的经验，，，，，，，，，，，，，
>
> 4个参数还是比较难调的，很容易就震荡了，如果不是调完参数后一会就能看到效果的话，可能会调得很崩溃，可以使用上位机看波形帮助调参。



## 附录

1.  [关于ADRC算法以及参数整定（调参）的一些心得体会 - 西涯先生的博客.pdf](lib_adrcctrl.assets\关于ADRC算法以及参数整定（调参）的一些心得体会 - 西涯先生的博客.pdf) 
2.  [从PID技术到“自抗扰控制”技术.pdf](lib_adrcctrl.assets\从PID技术到"自抗扰控制"技术.pdf) 

## 极点配置

最后再扯一扯怎么用这个课本上的控制器。

此控制器适用的系统必须是线性时不变系统，而且实际应用时大概率会出现稳态误差。首先需要知道系统的状态空间模型（传递函数），可通过建模来求得，但是能建模的估计也不会困扰于这个比赛（C.M.注：指“全国大学生智能汽车竞赛”）。另外一种方法就是通过系统辨识的方法。MATLAB内置了一个系统辨识工具箱，只要把系统的输入信号和输出响应信号扔进去就能把传递函数求出来。输入信号推荐扫频信号、方波、三角波、随机信号等包含了大部分频率成分的信号。

之后先验证可控可观的程度，然后就是怎么把频率响应曲线拉平：

一般零点数量少于极点数量，把等量的极点设置到零点位置（虚数取abs）使之相互抵消，然后把剩下的极点尽量往频率高的位置设置（取决于可控程度）

最后就是s域到z域的转换，实际计算过程是离散的所以是z域，这部分MATLAB也可直接转换。



