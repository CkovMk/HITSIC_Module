#include "sys_extint.hpp"

#if defined(HITSIC_USE_EXTINT) && (HITSIC_USE_EXTINT > 0)

/*!
 * @addtogroup extint
 * @{
 */

std::map<INTC_Type*, std::map<uint32_t, extInt_t>> extInt_t::isrSet;

//std::map<uint32_t, extInt_t> extInt_t::isrSet[FSL_FEATURE_SOC_PORT_COUNT]; // TODO: this is not portable.

status_t extInt_t::init(void)
{
    status_t result = kStatus_Success;
    isrSet.clear();
#if defined(HITSIC_EXTMGR_INITLIZE) && (HITSIC_EXTMGR_INITLIZE > 0)
    EXTINT_PlatformInit();
#endif // ! HITSIC_EXTMGR_INITLIZE
    return result;
}

extInt_t* extInt_t::insert(INTC_Type *_gpio, uint32_t _pin, handler_t _handler)
{
    isrSet[_gpio][_pin] = extInt_t(_gpio, _pin, _handler);
    return &isrSet[_gpio][_pin];
}

status_t extInt_t::remove(INTC_Type *_gpio, uint32_t _pin)
{
    HAL_EnterCritical();
    auto set_it = isrSet.find(_gpio);
    if (set_it == isrSet.end())
    {
        HAL_ExitCritical();
        return kStatus_Fail;
    }
    auto it = set_it->second.find(_pin);
    if (it != set_it->second.end())
    {
        set_it->second.erase(it);

    }
    HAL_ExitCritical();
    return kStatus_Success;
}

status_t extInt_t::remove(extInt_t *_inst)
{
    return remove(_inst->gpio, _inst->pin);
}

void extInt_t::isr(INTC_Type *_gpio)
{
    uint32_t flag = EXTINT_GetInterruptFlags(_gpio);

    for (auto it : isrSet[_gpio])
    {
        if (flag & (1 << it.first))
        {
            (*it.second.handler)(it.second.userData);
        }
    }
    EXTINT_ClearInterruptFlags(_gpio, 0xffff);
}

void extInt_t::setup(INTC_Type *_gpio, uint32_t _pin, handler_t _handler)
{
#ifdef DEBUG
    if (_handler == NULL)
    {
        throw std::invalid_argument(std::string("extInt_t setup failure.\n"));
        return;
    }
#else

#endif // ! DEBUG
    HAL_EnterCritical();
    gpio = _gpio;
    pin = _pin;
    handler = _handler;
    HAL_ExitCritical();
}


/* @} */

#endif // ! HITSIC_USE_EXTINT
