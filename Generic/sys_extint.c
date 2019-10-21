#include "sys_extint.h"

#ifdef __cplusplus
extern "C" {
#endif

list_t PORTA_irq_list = { NULL,NULL,0,0 };
list_t PORTB_irq_list = { NULL,NULL,0,0 };
list_t PORTC_irq_list = { NULL,NULL,0,0 };
list_t PORTD_irq_list = { NULL,NULL,0,0 };
list_t PORTE_irq_list = { NULL,NULL,0,0 };
#define ErrorCheck(result,expression)  result = expression;if(0!= result)return result
status_t EXTINT_Init()
{
	status_t result;
	List_Constructor(&PORTA_irq_list, sizeof(extint_irqNode_t));
	List_Constructor(&PORTB_irq_list, sizeof(extint_irqNode_t));
	List_Constructor(&PORTC_irq_list, sizeof(extint_irqNode_t));
	List_Constructor(&PORTD_irq_list, sizeof(extint_irqNode_t));
	List_Constructor(&PORTE_irq_list, sizeof(extint_irqNode_t));
	return result;
}

status_t EXTINT_PortSetup(PORT_Type* PORTx, uint32_t pin, port_interrupt_t config, uint32_t priority_, void(*gpio_interrupt_callback)(void))
{
	assert(PORTx);
	uint32_t pin_mask_b = 1 << pin;
	list_t* ItList = EXTINT_ListGet(PORTx);
	assert(ItList);
	status_t result;
	//先遍历链表删除相同项
	for (list_node_t* j = List_ItBegin(ItList); j != List_ItEnd(ItList); ) 
        {
		if (((extint_irqNode_t*)j->data)->pin_mask == pin_mask_b) 
                {
			j = j->next;
			List_Erase(ItList, j->prev);
		}
		else 
                {
			j = j->next;
		}
	}
	//插入
	extint_irqNode_t buf;
	buf.callback = gpio_interrupt_callback;
	buf.pin_mask = pin_mask_b;
	buf.priority = priority_;
	list_node_t* ij;
	for (ij = List_ItBegin(ItList); ij != List_ItEnd(ItList); ij = ij->next) 
        {
		if (((extint_irqNode_t*)ij->data)->priority > priority_) //往前插入
                {
			break;
		}
	}
	List_Insert(ItList, ij, &buf);
	//配置中断
	PORT_SetPinInterruptConfig(PORTx, pin, config);
	return 0;
}

status_t EXTINT_PortRemove(PORT_Type* PORTx, uint32_t pin){
	assert(PORTx);
	//取消中断
	PORT_SetPinInterruptConfig(PORTx, pin, kPORT_InterruptOrDMADisabled);
	uint32_t pin_mask_b = 1 << pin;
	list_t* ItList = EXTINT_ListGet(PORTx);
	assert(ItList);
	//遍历链表删除相同项
	for (list_node_t* j = List_ItBegin(ItList); j != List_ItEnd(ItList); ) 
        {
		if (((extint_irqNode_t*)j->data)->pin_mask == pin_mask_b) 
                {
			j = j->next;
			List_Erase(ItList, j->prev);
		}
		else 
                {
			j = j->next;
		}
	}
	return 0;
}

list_t* EXTINT_ListGet(PORT_Type* PORTx){
	if (PORTx == PORTA){
		return &PORTA_irq_list;
	}
	else if (PORTx == PORTB){
		return &PORTB_irq_list;
	}
	else if (PORTx == PORTC){
		return &PORTC_irq_list;
	}
	else if (PORTx == PORTD){
		return &PORTD_irq_list;
	}
	else if (PORTx == PORTE){
		return &PORTE_irq_list;
	}
	else{
		return NULL;
	}
}

  
void PORTX_IRQHandler(PORT_Type* _port, list_t* _list)
{
	uint32_t flag = _port->ISFR;
	_port->ISFR = flag;
	for (list_node_t* j = List_ItBegin(_list); j != List_ItEnd(_list); j = j->next) 
        {
		if (((extint_irqNode_t*)j->data)->pin_mask&flag) 
                {
			(*((extint_irqNode_t*)j->data)->callback)();
		}
	}
}

void PORTA_IRQHandler(void){PORTX_IRQHandler(PORTA, &PORTA_irq_list);}
void PORTB_IRQHandler(void){PORTX_IRQHandler(PORTB, &PORTB_irq_list);}
void PORTC_IRQHandler(void){PORTX_IRQHandler(PORTC, &PORTC_irq_list);}
void PORTD_IRQHandler(void){PORTX_IRQHandler(PORTD, &PORTD_irq_list);}
void PORTE_IRQHandler(void){PORTX_IRQHandler(PORTE, &PORTE_irq_list);}

#ifdef __cplusplus
}
#endif