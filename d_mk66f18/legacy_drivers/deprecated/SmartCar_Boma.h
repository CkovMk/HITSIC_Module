#ifndef _SMARTCAR_BOMA_H
#define _SMARTCAR_BOMA_H

#include "fsl_gpio.h"
#include "board.h"

typedef struct _Boma
{
  GPIO_Type* GPIO;
  PORT_Type* PORT;
  uint8_t    PIN;
} Boma_Switch;

void Boma_Init(void);

uint8_t Boma_Check(uint8_t key);
#endif
