#include "MDR32F9Qx_2led.h"									// Работа с кнопками.

#include "MDR32F9Qx_port.h"									// Описание регистров. 
#include "MDR32F9Qx_rst_clk.h"

// Инициализация порта C для работы с двумя светодиодами.
void initPinPortCForLed (void) 
{
	MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTC;												// Включаем тактирование портов C.
	MDR_PORTC->OE 		|= LED0|LED1;																			// Выход.
	MDR_PORTC->FUNC 	&= ~((uint32_t)(LED0|LED1));											// Режим работы - порт.
	MDR_PORTC->ANALOG |= LED0|LED1;																			// Цифровые.
	MDR_PORTC->PULL 	&= ~((uint32_t)(LED0|LED1|LED0<<16|LED1<<16));		// Подтяжка отключена.
	MDR_PORTC->PD 		&= ~((uint32_t)(LED0|LED1|LED0<<16|LED1<<16));		// Триггер Шмитта выключен гистерезис 200 мВ.
																																			// Управляемый драйвер.
	MDR_PORTC->PWR		|= PWRMAX_LED0|PWRMAX_LED1;												// Максимальная скорость пин обоих светодиодов.
	MDR_PORTC->GFEN		&= ~((uint32_t)(LED0|LED1));											// Фильтрация импульсов отключена.
}
