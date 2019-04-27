#ifndef PWM_H
#define PWM_H

#include "MDR32F9Qx_port.h"											// Описание регистров. 
#include "MDR32F9Qx_rst_clk.h"

// Функции.
void initTimerPWMled (uint32_t PWM_speed);								// Инициализация таймера в режими ШИМ для работы со светодиодом. 
void initTimerPWMledSPL (uint32_t PWM_speed);							// Тоже самое, но на SPL.
void initTimerButtonCheck (void); 												// Инициализация таймера для вызова прерывания опроса клавиш 5 раз в секунду.
void initTimerButtonCheckSPL (void);											// Тоже самое, но на SPL.
void initTimerPWMconstPeriod (uint16_t timeEnabledState); // Инициализация таймера в режиме ШИМ с переменной скважностью. Параметр - 0...FFFF. - время действия импульса.
#endif
