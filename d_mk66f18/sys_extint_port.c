#include <sys_extint.h>

#if defined(HITSIC_USE_EXTINT) && (HITSIC_USE_EXTINT > 0)

#ifdef __cplusplus
extern "C"{
#endif

extern extint_t extint_porta, extint_portb, extint_portc, extint_portd, extint_porte;

extint_t *EXTINT_GetInst(INTC_Type *base)
{
    switch((uint32_t)base)
    {
    case PORTA_BASE:
        return &extint_porta;
    case PORTB_BASE:
            return &extint_portb;
    case PORTC_BASE:
            return &extint_portc;
    case PORTD_BASE:
            return &extint_portd;
    case PORTE_BASE:
            return &extint_porte;
    default:
        return NULL;
    }
}

#if defined(HTISIC_EXTINT_DEFAULT_IRQ) && (HTISIC_EXTINT_DEFAULT_IRQ > 0)

void PORTA_IRQHandler(void)
{
    uint32_t flag = PORT_GetPinsInterruptFlags(PORTA);
    EXTINT_Isr(&extint_porta, flag);
    PORT_ClearPinsInterruptFlags(PORTA, 0xffff);
}

void PORTB_IRQHandler(void)
{
    uint32_t flag = PORT_GetPinsInterruptFlags(PORTB);
    EXTINT_Isr(&extint_portb, flag);
    PORT_ClearPinsInterruptFlags(PORTB, 0xffff);
}

void PORTC_IRQHandler(void)
{
    uint32_t flag = PORT_GetPinsInterruptFlags(PORTC);
    EXTINT_Isr(&extint_portc, flag);
    PORT_ClearPinsInterruptFlags(PORTC, 0xffff);
}

void PORTD_IRQHandler(void)
{
    uint32_t flag = PORT_GetPinsInterruptFlags(PORTD);
    EXTINT_Isr(&extint_portd, flag);
    PORT_ClearPinsInterruptFlags(PORTD, 0xffff);
}

void PORTE_IRQHandler(void)
{
    uint32_t flag = PORT_GetPinsInterruptFlags(PORTE);
    EXTINT_Isr(&extint_porte, flag);
    PORT_ClearPinsInterruptFlags(PORTE, 0xffff);
}
#endif // ! HTISIC_EXTMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

#endif // ! HITSIC_USE_EXTINT
