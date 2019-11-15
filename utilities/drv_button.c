/**
 * Copyright 2018 - 2019 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "drv_button.h"

#ifdef __cplusplus
extern "C"
{
#endif

    //#ifdef CPU_MK66FX1M0VLQ18
    PORT_Type *BUTTON_GetPortInst(GPIO_Type *gpio)
    {
        static PORT_Type *lut[5] = PORT_BASE_PTRS;
        return lut[((uint32_t)gpio - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)];
    }
    //#endif // ! CPU_MK66FX1M0VLQ18

    void BUTTON_Setup(button_t *_inst, GPIO_Type *_gpio, uint32_t _pin)
    {
        _inst->gpio = _gpio;
        _inst->pin = _pin;
        _inst->handler = NULL;
        while (BUTTON_ReadPin(_inst) == BUTTON_PRESSDN_LOGIC)
        {
        }
        BUTTON_SetInterrupt(_inst, BUTTON_PRESSDN_EXTINT);
    }

    button_t *BUTTON_Construct(GPIO_Type *_gpio, uint32_t _pin)
    {
        button_t *inst = (button_t*)malloc(sizeof(button_t));
        if (inst == NULL)
        {
            return inst;
        }
        BUTTON_Setup(inst, _gpio, _pin);
        return inst;
    }

    void BUTTON_InstallHandler(button_t *_inst, button_handler_t _handler)
    {
        _inst->handler = _handler;
    }

    void BUTTON_UninstallHandler(button_t *_inst)
    {
        _inst->handler = NULL;
    }

    void BUTTON_SetInterrupt(button_t *_inst, port_interrupt_t _int)
    {
        assert(_inst);
        _inst->intCfg = _int;
        PORT_SetPinInterruptConfig(BUTTON_GetPortInst(_inst->gpio), _inst->pin, _inst->intCfg);
    }

    uint32_t BUTTON_ReadPin(button_t *_inst)
    {
        assert(_inst);
        return GPIO_PinRead(_inst->gpio, _inst->pin);
    }

    void BUTTON_ExtIsr(button_t *_inst)
    {
        if (_inst->intCfg == BUTTON_RELEASE_EXTINT)
        { //should be a HIGH IRQ
            BUTTON_SetInterrupt(_inst, BUTTON_PRESSDN_EXTINT);
            unsigned long long t = BUTTON_TIMER_MS - _inst->msCnt;
            _inst->msCnt = BUTTON_TIMER_MS;
            if (BUTTON_TIME_SHRT <= t && t < BUTTON_SHRT_TOUT && _inst->status == BUTTON_STAT_NONE)
            { //short press
                _inst->status = BUTTON_SHRT_PRES;
                if (_inst->handler != NULL)
                {
                    _inst->handler(_inst);
                }
            }
            else
            {
                _inst->status = BUTTON_STAT_NONE;
            }
        }
        else if (_inst->intCfg == BUTTON_PRESSDN_EXTINT)
        { //should be a LOW IRQ
            uint64_t t = BUTTON_TIMER_MS - _inst->msCnt;
            if (t < BUTTON_TIME_INTV)
            {
                return;
            }
            BUTTON_SetInterrupt(_inst, BUTTON_RELEASE_EXTINT);
            _inst->status = BUTTON_STAT_NONE;
            _inst->msCnt = BUTTON_TIMER_MS;
        }
    }

    void BUTTON_PitIsr(button_t *_inst)
    {
		if (BUTTON_ReadPin(_inst) == 1)
		{
			BUTTON_SetInterrupt(_inst, BUTTON_PRESSDN_EXTINT);
			return;
		}
		uint64_t t = BUTTON_TIMER_MS - _inst->msCnt;
		if (_inst->intCfg == BUTTON_RELEASE_EXTINT)
		{
			if (BUTTON_TIME_LONG <= t && t < BUTTON_REPT_TOUT)
			{ //long press
				_inst->status = BUTTON_LONG_PRES;
				_inst->msCnt = BUTTON_TIMER_MS;
				BUTTON_SetInterrupt(_inst, BUTTON_PRESSDN_EXTINT); 
                if (_inst->handler != NULL)
                {
                    _inst->handler(_inst);
                }
			}
		}
		else if (_inst->status == BUTTON_LONG_PRES || _inst->status == BUTTON_LRPT_PRES)
		{
			if (t >= BUTTON_REPT_TOUT)
			{
				_inst->status = BUTTON_STAT_NONE;
			}
			else if (t >= BUTTON_TIME_LRPT)
			{ //long press
				_inst->status = BUTTON_LRPT_PRES;
				_inst->msCnt = BUTTON_TIMER_MS;
				BUTTON_SetInterrupt(_inst, BUTTON_PRESSDN_EXTINT);
                if (_inst->handler != NULL)
                {
                    _inst->handler(_inst);
                }
			}
		}
    }

#ifdef __cplusplus
}
#endif