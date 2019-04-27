#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_ssp.h"

// Собственные библиотеки. 
#include "MDR32F9Qx_button.h"								// Работа с кнопками.
#include "MDR32F9Qx_2led.h"									// Работа с светодиодами.
#include "MDR32F9Qx_PWM.h"									// Работа с таймером.

int PWM_time = 0xFFFF;											// По началу горит полностью.
void Timer2_IRQHandler (void)
{
	MDR_TIMER2->STATUS  = 0;									// Сбрасываем флаг. Обязательно первой коммандой.	
	//LED1_FLAG = !LED1_FLAG;										// Показываем, что прерывание было обработано.
	// 65535 = 3·5·17·257. 257 раз пусть будет для нас диапазоном от 0 - не горит до 257 - горит на полную. => шаг 3*5*17 = 255.
	if (UP_FLAG == 0) PWM_time+=255;						// Проверияем, нажата ли какая-нибудь клавиша. Если нажата - что-то делаем с частотой.
	else if (DOWN_FLAG == 0) PWM_time-=255;			
		else if (LEFT_FLAG == 0) PWM_time-=255;
			else if (RIGHT_FLAG == 0) PWM_time+=255;
	if (PWM_time < 0) PWM_time = 0;						// Проверяем, чтобы время "единицы" было не меньше нуля и не больше периода.
		else if (PWM_time > 0xFFFF) PWM_time = 0xFFFF;
	MDR_TIMER1->CCR1 = PWM_time;								// Меняем частоту. 
}

int main (void)
{
	initTimerPWMconstPeriod(PWM_time);					// Запускаем ШИМ. Параметр - скорость ШИМ.
	initPinForButton();													// Настраиваем кнопки.
	initPinPortCForLed(); 											// Работа светодиода (клавиша считывается).
	initTimerButtonCheck();											// Инициализация таймера.
  while (1)			
  {
  }
}


