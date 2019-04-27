#ifndef BUTTON_H
#define BUTTON_H
// Маски бит портов клавиш. 
#define DOWN_MSK					(1<<1)	// PORTE
#define SELECT_MSK				(1<<2)	// PORTC
#define LEFT_MSK					(1<<3)	// PORTE
#define UP_MSK						(1<<5)	// PORTB
#define RIGHT_MSK 				(1<<6)	// PORTB

#define PWRMAX_UP_MSK			(3<<2*5)// PORTB
#define PWRMAX_RIGHT_MSK	(3<<2*6)
#define PWRMAX_SELECT_MSK (3<<2*2)// PORTC.
#define PWRMAX_DOWN_MSK		(3<<2*1)// PORTE.
#define PWRMAX_LEFT_MSK		(3<<2*3)

// Читать состояние клавишы.
#define DOWN_FLAG					*(uint32_t*)0x43900004	
#define SELECT_FLAG				*(uint32_t*)0x43700008
#define LEFT_FLAG					*(uint32_t*)0x4390000c
#define UP_FLAG						*(uint32_t*)0x43600014
#define RIGHT_FLAG				*(uint32_t*)0x43600018

// Функции.
void initPinForButton (void);									// Инициализация пинов на портах B, C, E для работы с кнопками навигации, 
																							// установленными на плате. 

#endif
