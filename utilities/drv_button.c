/**
 * Copyright 2018 - 2020 HITSIC
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

#include <drv_button.h>

#if defined(HITSIC_USE_DRV_BUTTON) && (HITSIC_USE_DRV_BUTTON > 0)

 /*!
  * @addtogroup button
  * @{
  */

#ifdef __cplusplus
extern "C"{
#endif

void BUTTON_Setup(button_t *_inst, button_config_t *_cfg)
{
    _inst->config = _cfg;
    while (_inst->config->read() != _inst->config->logic)
    {
    }
    _inst->msCnt = 0UL;
    _inst->status = BUTTON_STAT_NONE;

    button_interrupt_t interrupt_release = (_inst->config->logic == button_logic_default) ? button_interrupt_fall : button_interrupt_rise;
    button_interrupt_t interrupt_pressdn = (_inst->config->logic == button_logic_default) ? button_interrupt_rise : button_interrupt_fall;
    _inst->config->setInterrupt(_inst->intCfg = interrupt_pressdn);
}

button_t *BUTTON_Construct(button_config_t *_cfg)
{
    button_t *inst = (button_t*)malloc(sizeof(button_t));
    if (inst == NULL)
    {
        return NULL;
    }
    BUTTON_Setup(inst, _cfg);
    return inst;
}

void BUTTON_Destruct(button_t *_inst)
{
    assert(_inst);
    free(_inst);
    _inst = NULL;
}

void BUTTON_ExtIsr(button_t *_inst)
{
    button_interrupt_t interrupt_release = (_inst->config->logic == button_logic_default) ? button_interrupt_fall : button_interrupt_rise;
    button_interrupt_t interrupt_pressdn = (_inst->config->logic == button_logic_default) ? button_interrupt_rise : button_interrupt_fall;

    if (_inst->intCfg == interrupt_release)
    { //should be a HIGH IRQ
        _inst->config->setInterrupt(_inst->intCfg = interrupt_pressdn);
        unsigned long long t = BUTTON_TIMER_MS - _inst->msCnt;
        _inst->msCnt = BUTTON_TIMER_MS;
        if (BUTTON_TIME_SHRT <= t && t < BUTTON_SHRT_TOUT && _inst->status == BUTTON_STAT_NONE)
        { //short press
            _inst->status = BUTTON_SHRT_PRES;
            if (_inst->config->handler != NULL)
            {
                _inst->config->handler(_inst->status, _inst->config->userData);
            }
        }
        else
        {
            _inst->status = BUTTON_STAT_NONE;
        }
    }
    else if (_inst->intCfg == interrupt_pressdn)
    { //should be a LOW IRQ
        uint64_t t = BUTTON_TIMER_MS - _inst->msCnt;
        if (t < BUTTON_TIME_INTV)
        {
            return;
        }
        _inst->config->setInterrupt(_inst->intCfg = interrupt_release);
        if (!((_inst->status == BUTTON_LONG_PRES || _inst->status == BUTTON_LRPT_PRES) && (t < BUTTON_TIME_INTV)))
	    {
		    _inst->status = BUTTON_STAT_NONE;
	    }
        _inst->msCnt = BUTTON_TIMER_MS;
    }
}

void BUTTON_PitIsr(button_t *_inst)
{
    button_interrupt_t interrupt_release = (_inst->config->logic == button_logic_default) ? button_interrupt_fall : button_interrupt_rise;
    button_interrupt_t interrupt_pressdn = (_inst->config->logic == button_logic_default) ? button_interrupt_rise : button_interrupt_fall;

	if (_inst->config->read() == _inst->config->logic)
	{
		_inst->config->setInterrupt(_inst->intCfg = interrupt_pressdn);
		return;
	}
	uint64_t t = BUTTON_TIMER_MS - _inst->msCnt;
	if (_inst->intCfg == interrupt_release)
	{
		if (BUTTON_TIME_LONG <= t && t < BUTTON_REPT_TOUT)
		{ //long press
			_inst->status = BUTTON_LONG_PRES;
			_inst->msCnt = BUTTON_TIMER_MS;
			_inst->config->setInterrupt(_inst->intCfg = interrupt_pressdn); 
            if (_inst->config->handler != NULL)
            {
                _inst->config->handler(_inst->status, _inst->config->userData);
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
			_inst->config->setInterrupt(_inst->intCfg = interrupt_pressdn);
            if (_inst->config->handler != NULL)
            {
                _inst->config->handler(_inst->status, _inst->config->userData);
            }
		}
	}
}

#ifdef __cplusplus
}
#endif

/* @} */

#endif // ! HITSIC_USE_DRV_BUTTON
