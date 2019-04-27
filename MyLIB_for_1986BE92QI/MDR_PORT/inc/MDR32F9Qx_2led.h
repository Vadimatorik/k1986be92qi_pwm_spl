#ifndef LED_H
#define LED_H

// Подключение светодиодов.	
#define LED0						(1<<0)			// PORTC.
#define LED1						(1<<1)			// PORTC.

#define PWRMAX_LED0			 (3<<2*0)		// Максимальная скорость работы порта.
#define PWRMAX_LED1			 (3<<2*1)	

#define LED0_ON()						*(uint32_t*)0x43700000 = 1
#define LED0_OFF()					*(uint32_t*)0x43700000 = 0
#define LED1_ON()						*(uint32_t*)0x43700004 = 1
#define LED1_OFF()					*(uint32_t*)0x43700004 = 0
#define LED0_FLAG						*(uint32_t*)0x43700000
#define LED1_FLAG						*(uint32_t*)0x43700004 

// Функции.
void initPinPortCForLed (void);			// Настройка пин светодиодов.
#endif
