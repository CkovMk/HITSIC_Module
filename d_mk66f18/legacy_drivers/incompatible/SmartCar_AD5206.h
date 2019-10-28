#ifndef _SMARTCAR_AD5206_H
#define _SMARTCAR_AD5206_h

#include "math.h"
#include "include.h"

#define CS_GPIO   GPIOB
#define CS_PORT   PORTB
#define CS_PIN    20U
#define DIN_GPIO  GPIOB
#define DIN_PORT  PORTB
#define DIN_PIN   21U
#define CLK_GPIO  GPIOB
#define CLK_PORT  PORTB
#define CLK_PIN   22U

typedef enum
{
    R0,         //R1 
    R1,         //R2        
    R2,         //R3
    R3,         //R4
    R4,         //R5
    R5,         //R6
} Rn_n;
#define R0 0X00
#define R1 0X20
#define R2 0X40 // 0X40
#define R3 0X60
#define R4 0X80
#define R5 0XA0  //0XA0
void WriteAD5206(unsigned char address,unsigned char dat); 
void AD5206_init();
#endif
