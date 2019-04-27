#include "MDR32F9Qx_PWM.h"									// Работа с таймером.

#include "MDR32F9Qx_port.h"									// Описание регистров. 
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_timer.h"

// Инициализация таймера в режиме ШИМ для работы со светодиодом. 
void initTimerPWMled (uint32_t PWM_speed) 
{
	MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER1|RST_CLK_PCLK_PORTA;	// Включаем таймер и тактирование порта A.
	MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM1_CLK_EN;					// Подаем тактирование без предделителя. 
	
	MDR_PORTA->OE 		|= (1<<1);																			// Выход.
	MDR_PORTA->FUNC 	|= (2<<(1*2));																	// Режим работы - альтернативная функция.
	MDR_PORTA->ANALOG |= (1<<1);																			// Цифровые.
	MDR_PORTA->PWR		|= (3<<(1*2));															  	// Максимальная скорость пин обоих светодиодов.
	
	MDR_TIMER1->CNT = 0;																							// Считай с 0.
	MDR_TIMER1->PSG = 32000-1;																 				// На таймер идет частота TIM_CLK/32000.
	MDR_TIMER1->ARR = PWM_speed;																			// 1 секунда 250 тиков. У нас частота 2 герца. 
	MDR_TIMER1->CH1_CNTRL = 3<<TIMER_CH_CNTRL_OCCM_Pos;								// Переключение REF, если CNT = CCR;
	MDR_TIMER1->CH1_CNTRL1 = (2<<TIMER_CH_CNTRL1_SELO_Pos)						// На выход выдается сигнал c REF.
	| (1<<TIMER_CH_CNTRL1_SELOE_Pos);																	// Канал всегда работает на выход.
	MDR_TIMER1->CNTRL = TIMER_CNTRL_CNT_EN;														// Включаем таймер.
}

// Инициализация таймера в режиме ШИМ для работы со светодиодом в режиме SPL.
void initTimerPWMledSPL (uint32_t PWM_speed)
{
	PORT_InitTypeDef PWMPortA_structInit;												// Структура для инициализации вывода таймера в режиме ШИМ.
	TIMER_CntInitTypeDef timerPWM_structInit;										// Структура для настройки основного таймера (без каналов).
	TIMER_ChnInitTypeDef timerPWM_channelStructInit;						// Структура канала ШИМ.
	TIMER_ChnOutInitTypeDef timerPWM_channelOUTPWMStructInit;		// Структура настройки выхода канала ШИМ.
	
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);								// Включаем тактирование порта A.
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER1, ENABLE);								// Включаем тактирование таймера 1.
	
	PWMPortA_structInit.PORT_FUNC 			= PORT_FUNC_ALTER;  		// Вывод работают в режиме альтернативной функции.
	PWMPortA_structInit.PORT_GFEN 			= PORT_GFEN_OFF;				// Входной фильтр отключен.
	PWMPortA_structInit.PORT_MODE 			= PORT_MODE_DIGITAL;		// Вывод цифровой.
	PWMPortA_structInit.PORT_OE	 				= PORT_OE_OUT;					// Вывод работает на выход.
	PWMPortA_structInit.PORT_PD	 				= PORT_PD_DRIVER;				// Управляемый драйвер.
	PWMPortA_structInit.PORT_PD_SHM 		= PORT_PD_SHM_OFF;			// Триггер Шмитта выключен.
	PWMPortA_structInit.PORT_Pin				= PORT_Pin_1;						// Пин нашего светодиода.
	PWMPortA_structInit.PORT_PULL_DOWN 	= PORT_PULL_DOWN_OFF;		// Подтяжка в 0 отключена.
	PWMPortA_structInit.PORT_PULL_UP 		= PORT_PULL_UP_OFF;			// Подтяжка в 1 отключена. 
	PWMPortA_structInit.PORT_SPEED			= PORT_SPEED_MAXFAST;		// Работа вывода с максимальной скоростью.

	PORT_Init(MDR_PORTA, &PWMPortA_structInit);									// Инициализируем порт.

	// Заполняем структуру таймера.
	timerPWM_structInit.TIMER_ARR_UpdateMode		= TIMER_ARR_Update_Immediately;					// Регистр ARR можно обновлять в любое время.
	timerPWM_structInit.TIMER_BRK_Polarity			= TIMER_BRKPolarity_NonInverted;				// BRK сигнал не инвертируется (нас этот параметр не касается).
	timerPWM_structInit.TIMER_CounterDirection	= TIMER_CntDir_Up;											// Считаем "вверх". CNT инкрементируется (CNT++). 
	timerPWM_structInit.TIMER_CounterMode				= TIMER_CntMode_ClkFixedDir;						// Считаем в одну сторону, вверх.
	timerPWM_structInit.TIMER_ETR_FilterConf		= TIMER_Filter_1FF_at_TIMER_CLK;				// Сигнал зафиксирован в 1-м триггере на частоте TIM_CLK (В нашем случае оставляем по-умолчанию).
	timerPWM_structInit.TIMER_ETR_Polarity			= TIMER_ETRPolarity_NonInverted;				// ETR на входе не инвертируется (мы его и не используем).
	timerPWM_structInit.TIMER_ETR_Prescaler			= TIMER_ETR_Prescaler_None;							// Частота ETR на входе не делится (ETR не используем.).
	timerPWM_structInit.TIMER_EventSource				= TIMER_EvSrc_None;											// Таймер не вызывает прерываний.
	timerPWM_structInit.TIMER_FilterSampling		= TIMER_FDTS_TIMER_CLK_div_1;						// FDTS = TIMER_CLK. (Так не используем.).
	timerPWM_structInit.TIMER_IniCounter				= 0;																		// Считаем с 0. Начальное значение счетчика. (CNT = 0.).
	timerPWM_structInit.TIMER_Period						= PWM_speed;														// Считаем до указанного в параметрах функции значения (ARR = PWM_speed).
	timerPWM_structInit.TIMER_Prescaler					= 32000 - 1;														// Делитель входного сигнала. PSG регистр. 
	
	TIMER_CntInit(MDR_TIMER1, &timerPWM_structInit);																		// Инициализируем основной таймер.
	
	// Заполняем структуру PWM канала.
	timerPWM_channelStructInit.TIMER_CH_BRK_Reset					= TIMER_CH_BRK_RESET_Disable;				// Сброс канала BRK не производится (BRK не используем).
	timerPWM_channelStructInit.TIMER_CH_CCR1_Ena					=	DISABLE;													// CCR1 не используем.
	timerPWM_channelStructInit.TIMER_CH_CCR1_EventSource	=	TIMER_CH_CCR1EvSrc_PE;						// Выбор события по входному каналу для CAP1: положительный фронт по Chi. (По умолчанию, мы не используем).
	timerPWM_channelStructInit.TIMER_CH_CCR_UpdateMode		= TIMER_CH_CCR_Update_Immediately;	// Регистр CCR можно обновлять в любое время (CCR не используем).
	timerPWM_channelStructInit.TIMER_CH_ETR_Ena						= DISABLE;													// ETR не используется.
	timerPWM_channelStructInit.TIMER_CH_ETR_Reset					= TIMER_CH_ETR_RESET_Disable;				// Сброс ETR не производится.
	timerPWM_channelStructInit.TIMER_CH_EventSource				= TIMER_CH_EvSrc_PE;								// Выбор события по входному каналу: положительный фронт. (Так же не используется).
	timerPWM_channelStructInit.TIMER_CH_FilterConf				= TIMER_Filter_1FF_at_TIMER_CLK;		// Входной сигнал от TIMER_CLK фиксируется одним триггером.
	timerPWM_channelStructInit.TIMER_CH_Mode							= TIMER_CH_MODE_PWM;								// Канал в ШИМ режиме.
	timerPWM_channelStructInit.TIMER_CH_Number						=	TIMER_CHANNEL1;										// Первый канал. 
	timerPWM_channelStructInit.TIMER_CH_Prescaler					= TIMER_CH_Prescaler_None;					// В канале частота не делится.
	timerPWM_channelStructInit.TIMER_CH_REF_Format				= TIMER_CH_REF_Format3;							// Сигнал REF меняется при CNT == ARR.
	
	TIMER_ChnInit(MDR_TIMER1, &timerPWM_channelStructInit);																		// Инициализируем канал.
	
	// Параметры выхода.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_DirOut_Mode 			= TIMER_CH_OutMode_Output;				// Всегда выход.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_DirOut_Polarity 	= TIMER_CHOPolarity_NonInverted;	// Неинвертированный.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_DirOut_Source	 		= TIMER_CH_OutSrc_REF;						// На выход REF сигнал.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_DTG_AuxPrescaler 	= 0;															// Делителя не стоит.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_DTG_ClockSource 	= TIMER_CH_DTG_ClkSrc_TIMER_CLK;	// Источник тактового сигнала для DTG - TIMER_CLK. Но DTG мы все равно не используем.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_DTG_MainPrescaler = 0;															// Делитель сигнала на DTG.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_NegOut_Mode 			= TIMER_CH_OutMode_Input;					// Инвертный канал на вход. Все остальные его параметр берем по умолчанию, т.к. они не важны.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_NegOut_Polarity 	= TIMER_CHOPolarity_NonInverted;	// Без инвертирования инвертированного канала.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_NegOut_Source 		= TIMER_CH_DTG_ClkSrc_TIMER_CLK;	// Источник тактового сигнала для DTG - TIMER_CLK.
	timerPWM_channelOUTPWMStructInit.TIMER_CH_Number 						= TIMER_CHANNEL1;									// Первый канал.
	
	TIMER_ChnOutInit(MDR_TIMER1, &timerPWM_channelOUTPWMStructInit);															// Настраиваем канал на выход.
	
	TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv1);																										// Подаем источник сигнала для счета (частота процессора без предделителя).
																																																// В этой функции выбор делителя (у нас "1") и включение подачи такта.
	TIMER_Cmd(MDR_TIMER1, ENABLE);																																// Включаем таймер.
}

// Инициализация таймера в режиме ШИМ с постоянным периодом но плавающим временем работы.
void initTimerPWMconstPeriod (uint16_t timeEnabledState) 
{
	MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER1|RST_CLK_PCLK_PORTA;	// Включаем таймер и тактирование порта A.
	MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM1_CLK_EN;					// Подаем тактирование без предделителя. 
	
	MDR_PORTA->OE 		|= PORT_Pin_1;																	// Выход.
	MDR_PORTA->FUNC 	|= (2<<(1*2));																	// Режим работы - альтернативная функция.
	MDR_PORTA->ANALOG |= (1<<1);																			// Цифровые.
	MDR_PORTA->PWR		|= (3<<(1*2));															  	// Максимальная скорость пин обоих светодиодов.
	
	MDR_TIMER1->CNT = 0;																							// Считай с 0.
	MDR_TIMER1->PSG = 0;																 							// Делитель тактовой частоты отсутствует. 
	MDR_TIMER1->ARR = 0xFFFF;																					// Период постоянный. И дает возможнсоть выбирать период заполнения от 0 до 0xFFFF.
	
	MDR_TIMER1->CCR1 = timeEnabledState;															// Канал будет держать 1 до этого значения и 0 - после.
	MDR_TIMER1->CH1_CNTRL = 6<<TIMER_CH_CNTRL_OCCM_Pos;					  		// REF = 1, когда CNT < CCR1, 0 - CNT >= CCR1;
	MDR_TIMER1->CH1_CNTRL1 = (2<<TIMER_CH_CNTRL1_SELO_Pos)						// На выход выдается сигнал c REF.
	| (1<<TIMER_CH_CNTRL1_SELOE_Pos);																	// Канал всегда работает на выход.
	MDR_TIMER1->CH1_CNTRL2 = TIMER_CH_CNTRL2_CCRRLD;									// CCR1 обновляется лишь при CNT = 0. Чтобы не было глюков.
	MDR_TIMER1->CNTRL = TIMER_CNTRL_CNT_EN;														// Включаем таймер.
}

// Настройка таймера для генерации прерываний 25 раз в секунду.
void initTimerButtonCheck (void) 
{
	MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER2;										// Включаем тактирование таймера 2.
	MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM2_CLK_EN;					// Подаем тактирование без предделителя. 
	MDR_TIMER2->CNT = 0;																							// Считай с 0.
	MDR_TIMER2->PSG = 32000-1;																 				// На таймер идет частота TIM_CLK/32000.
	MDR_TIMER2->ARR = 10;																							// 1 секунда 250 тиков. У нас 25 опросов в секунду => 250/25=10.
	MDR_TIMER2->IE 	= TIMER_IE_CNT_ARR_EVENT_IE;											// Разрешаем прерывание по совподению CNT и ARR.
	NVIC_EnableIRQ(Timer2_IRQn);																			// Разрешаем прерывание от таймера в целом.
	MDR_TIMER2->CNTRL = TIMER_CNTRL_CNT_EN;														// Включаем таймер.
}

// Настройка таймера для генерации прерываний 25 раз в секунду при помощи SPL.
void initTimerButtonCheckSPL (void) 
{
	TIMER_CntInitTypeDef timerButtonCheck_structInit;																						// Структура для настройки основного таймера вызова прерывания для опроса клавиш.
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER2, ENABLE);																								// Включаем тактирование таймера 1.
	TIMER_BRGInit(MDR_TIMER2, TIMER_HCLKdiv1);																									// Подаем источник сигнала для счета (частота процессора без предделителя).
	
	// Заполняем структуру основного таймера.
	timerButtonCheck_structInit.TIMER_ARR_UpdateMode		= TIMER_ARR_Update_Immediately;					// Регистр ARR можно обновлять в любое время.
	timerButtonCheck_structInit.TIMER_BRK_Polarity			= TIMER_BRKPolarity_NonInverted;				// BRK сигнал не инвертируется (нас этот параметр не касается).
	timerButtonCheck_structInit.TIMER_CounterDirection	= TIMER_CntDir_Up;											// Считаем "вверх". CNT инкрементируется (CNT++). 
	timerButtonCheck_structInit.TIMER_CounterMode				= TIMER_CntMode_ClkFixedDir;						// Считаем в одну сторону, вверх.
	timerButtonCheck_structInit.TIMER_ETR_FilterConf		= TIMER_Filter_1FF_at_TIMER_CLK;				// Сигнал зафиксирован в 1-м триггере на частоте TIM_CLK (В нашем случае оставляем по-умолчанию).
	timerButtonCheck_structInit.TIMER_ETR_Polarity			= TIMER_ETRPolarity_NonInverted;				// ETR на входе не инвертируется (мы его и не используем).
	timerButtonCheck_structInit.TIMER_ETR_Prescaler			= TIMER_ETR_Prescaler_None;							// Частота ETR на входе не делится (ETR не используем.).
	timerButtonCheck_structInit.TIMER_EventSource				= TIMER_EvSrc_TM2;											// Таймер вызывает прерывание при CNT = ARR.
	timerButtonCheck_structInit.TIMER_FilterSampling		= TIMER_FDTS_TIMER_CLK_div_1;						// FDTS = TIMER_CLK. (Так не используем.).
	timerButtonCheck_structInit.TIMER_IniCounter				= 0;																		// Считаем с 0. Начальное значение счетчика. (CNT = 0.).
	timerButtonCheck_structInit.TIMER_Period						= 250/25;																// Считаем до указанного в параметрах функции значения (ARR = PWM_speed).
	timerButtonCheck_structInit.TIMER_Prescaler					= 32000 - 1;														// Делитель входного сигнала. PSG регистр. 
	
	TIMER_CntInit(MDR_TIMER2, &timerButtonCheck_structInit);																		// Инициализируем основной таймер.
	
	TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);																		// Разрешаем прерывание по CNT = ARR.	
	NVIC_EnableIRQ(Timer2_IRQn);																																// Разрешаем прерывание от таймера в целом.
	
	TIMER_Cmd(MDR_TIMER2, ENABLE);																															// Включаем таймер.
}
