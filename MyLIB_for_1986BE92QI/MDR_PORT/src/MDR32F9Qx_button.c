#include "MDR32F9Qx_button.h"									// Работа с кнопками.

#include "MDR32F9Qx_port.h"										// Описание регистров. 
#include "MDR32F9Qx_rst_clk.h"

// Инициализация пинов на портах B, C, E для работы с кнопками навигации, 
// установленными на плате. 
// Подключение кнопок описано в inc файле.
void initPinForButton (void) 
{	
	MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTB|RST_CLK_PCLK_PORTC|RST_CLK_PCLK_PORTE;			// Включаем тактирование портов B, C, E.
	
  MDR_PORTB->OE 		&= ~((uint32_t)(UP_MSK|RIGHT_MSK));																// Входы.
	MDR_PORTB->FUNC 	&= ~((uint32_t)(UP_MSK|RIGHT_MSK));																// Режим работы - порт.
	MDR_PORTB->ANALOG |= UP_MSK|RIGHT_MSK;																							// Цифровые.
	MDR_PORTB->PULL 	&= ~((uint32_t)(UP_MSK|RIGHT_MSK|UP_MSK<<16|RIGHT_MSK<<16));			// Подтяжка отключена.
	MDR_PORTB->PD 		&= ~((uint32_t)(UP_MSK|RIGHT_MSK|UP_MSK<<16|RIGHT_MSK<<16));			// Триггер Шмитта выключен гистерезис 200 мВ.
																																											// Управляемый драйвер.
	MDR_PORTB->PWR		|= PWRMAX_UP_MSK|PWRMAX_RIGHT_MSK;																// Максимальная скорость обоих выводов.
	MDR_PORTB->GFEN		|= UP_MSK|RIGHT_MSK;																							// Фильтр импульсов включен (фильтрация импульсов до 10 нс).

  MDR_PORTC->OE 		&= ~((uint32_t)(SELECT_MSK));																			// Вход.
	MDR_PORTC->FUNC 	&= ~((uint32_t)(SELECT_MSK));																			// Режим работы - порт.
	MDR_PORTC->ANALOG |= SELECT_MSK;																										// Цифровой.
	MDR_PORTC->PULL 	&= ~((uint32_t)(SELECT_MSK|SELECT_MSK<<16));											// Подтяжка отключена.
	MDR_PORTC->PD 		&= ~((uint32_t)(SELECT_MSK|SELECT_MSK<<16));											// Триггер Шмитта выключен гистерезис 200 мВ.
																																											// Управляемый драйвер.
	MDR_PORTC->PWR		|= PWRMAX_SELECT_MSK;																							// Максимальная скорость вывода.
	MDR_PORTC->GFEN		|= SELECT_MSK;																										// Фильтр импульсов включен (фильтрация импульсов до 10 нс).

  MDR_PORTE->OE 		&= ~((uint32_t)(DOWN_MSK|LEFT_MSK));															// Входы.
	MDR_PORTE->FUNC 	&= ~((uint32_t)(DOWN_MSK|LEFT_MSK));															// Режим работы - порт.
	MDR_PORTE->ANALOG |= DOWN_MSK|LEFT_MSK;																							// Цифровые.
	MDR_PORTE->PULL 	&= ~((uint32_t)(DOWN_MSK|LEFT_MSK|DOWN_MSK<<16|LEFT_MSK<<16));		// Подтяжка отключена.
	MDR_PORTE->PD 		&= ~((uint32_t)(DOWN_MSK|LEFT_MSK|DOWN_MSK<<16|LEFT_MSK<<16));		// Триггер Шмитта выключен гистерезис 200 мВ.
																																											// Управляемый драйвер.
	MDR_PORTE->PWR		|= PWRMAX_DOWN_MSK|PWRMAX_LEFT_MSK;																// Максимальная скорость обоих выводов.
	MDR_PORTE->GFEN		|= DOWN_MSK|LEFT_MSK;																							// Фильтр импульсов включен (фильтрация импульсов до 10 нс).
}
