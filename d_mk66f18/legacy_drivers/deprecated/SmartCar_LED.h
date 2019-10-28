#ifndef  __SMARTCAR_LED_H
#define  __SMARTCAR_LED_H

#include "fsl_gpio.h"
#include "board.h"

void LED_Init(PORT_Type *port, GPIO_Type *gpio,uint32_t pin);
void LED_On(GPIO_Type *gpio,uint32_t pin);
void LED_Off(GPIO_Type *gpio,uint32_t pin);
void LED_Toggle(GPIO_Type *gpio,uint32_t pin);

#endif