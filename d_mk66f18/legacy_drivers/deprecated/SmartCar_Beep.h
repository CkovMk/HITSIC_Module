#ifndef  __SMARTCAR_BEEP_H
#define  __SMARTCAR_BEEP_H

#include "fsl_gpio.h"
#include "board.h"

void Beep_Init(PORT_Type *port, GPIO_Type *gpio,uint32_t pin);
void Beep_On(GPIO_Type *gpio,uint32_t pin);
void Beep_Off(GPIO_Type *gpio,uint32_t pin);
void Beep_Toggle(GPIO_Type *gpio,uint32_t pin);

#endif