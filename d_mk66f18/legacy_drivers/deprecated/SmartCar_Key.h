#ifndef _SMARTCAR_KEY_H
#define _SMARTCAR_KEY_H

#include "fsl_common.h"
#include "SmartCar_Systick.h"




//-------------------------------------------------------------------------------------------------------------------
//  @brief      按键定义，可根据需要调整顺序
//-------------------------------------------------------------------------------------------------------------------
typedef enum
{
    KEY_ENTER,//0 	中心键
    KEY_UP,   //1	上键

    KEY_DOWN, //2 	下键
    KEY_LEFT, //3	左键

    KEY_RIGHT,//4	右键

    KEY_MAX,  //5   初始化用
}KEY_e;

typedef enum
{
    KEY_P_DOWN,
    KEY_P_UP,
}KEY_STATUS;

typedef struct _Key
{
  GPIO_Type* GPIO;
  PORT_Type* PORT;
  uint8_t    PIN;
} KEY;


void Key_Init();
KEY_STATUS Key_Check(KEY_e key);

#endif // __KEY_H

