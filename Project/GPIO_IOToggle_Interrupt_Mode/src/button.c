#include "stm8s.h"
#include "stm8s_tim1.h"
#include "stm8s_tim2.h"

#include "button.h"

#define LEDS_PORT (GPIOB)

#define BUTTON_PORT (GPIOB)
#define BUTTON_PIN1  (GPIO_PIN_6)
#define BUTTON_PIN2  (GPIO_PIN_7)

typedef enum button_timer_status_e
{
    BUTTON_STATUS_INIT = 0,
		BUTTON_STATUS_2s = 1,
		BUTTON_STATUS_5s = 2
}button_timer_status_t;

typedef enum button_event_e
{
	BUTTON_INVALID = 0,
	BUTTON_SHORT_PRESS,
	BUTTON_DOUBLE_PRESS,
	BUTTON_LONG_HOLD,
	BUTTON_LONG_PRESS,
	BUTTON_VERY_LONG_HOLD,
	BUTTON_VERY_LONG_PRESS
} button_event_t;

static button_timer_status_t  m_button_timer_status = BUTTON_STATUS_INIT;

static bool detect_double_button_press = FALSE;
static bool led_is_on = FALSE;
static u8   button_status = 0xFF;
static bool timer_is_started = FALSE;
static u8   timer1_index_s = 0;
static u8   timer2_index_50ms = 0;

void Delay (u16 nCount);
extern u32 int_timers;

void start_timer1(u8 duration) // ms
{
		timer1_index_s = duration;
    TIM1_DeInit();
    TIM1_TimeBaseInit(0x8, TIM1_COUNTERMODE_UP, 125, 3); // (1/4MHz)*32*125 = 1mS
    TIM1_SetCounter(0); // T = n * 1mS
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); //Enable TIM4 IT UPDATE
    TIM1_Cmd(ENABLE); //Enable TIM4
}

void start_timer2(u8 duration) // 50ms
{
		timer2_index_50ms = duration;
    TIM1_DeInit();
    TIM1_TimeBaseInit(0x8, TIM1_COUNTERMODE_UP, 125, 3); // (1/4MHz)*32*125 = 1mS
    TIM1_SetCounter(0); // T = n * 1mS
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); //Enable TIM4 IT UPDATE
    TIM1_Cmd(ENABLE); //Enable TIM4
}

void stop_timer1(void)
{
	  timer1_index_s = 0;
    TIM1_Cmd(DISABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE, DISABLE);
}

void stop_timer2(void)
{
	  timer2_index_50ms = 0;
    TIM2_Cmd(DISABLE);
    TIM2_ITConfig(TIM1_IT_UPDATE, DISABLE);
}

void btn_short_button_press(void)
{
		u8 Led = GPIO_PIN_1;
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
}

void btn_double_button_press(void)
{
		u8 Led = GPIO_PIN_1;
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
}

void app_button_event_handler(button_event_t button_event)
{
	switch (button_event)
	{
		case BUTTON_INVALID:
		{
			break;
		}
		case BUTTON_SHORT_PRESS:
		{
			btn_short_button_press();
			break;
		}
		case BUTTON_DOUBLE_PRESS:
		{
			btn_double_button_press();
			break;
		}
		case BUTTON_LONG_HOLD:
		{
			break;
		}
		case BUTTON_LONG_PRESS:
		{
			break;
		}
		case BUTTON_VERY_LONG_HOLD:
		{
			break;
		}
		case BUTTON_VERY_LONG_PRESS:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void button1_push(void)
{
	switch (m_button_timer_status)
	{
		case BUTTON_STATUS_INIT:
			{
				m_button_timer_status = BUTTON_STATUS_2s;
				start_timer1(2);
				break;
			}
		case BUTTON_STATUS_2s:
			{
				break;
			}
		case BUTTON_STATUS_5s:
			{
				break;
			}
		default:
			{
				break;
			}
	}
}

void button1_release(void)
{
	button_event_t button_event = BUTTON_INVALID;
	switch (m_button_timer_status)
	{
		case BUTTON_STATUS_INIT:
			{
				break;
			}
		case BUTTON_STATUS_2s:
			{
				if (detect_double_button_press == FALSE)
				{
					detect_double_button_press = TRUE;
					start_timer2(10);  //500ms
				}
				else
				{
					button_event = BUTTON_DOUBLE_PRESS;
					detect_double_button_press = FALSE;
				}
				break;
			}
		case BUTTON_STATUS_5s:
			{
				break;
			}
		default:
			{
				break;
			}
	}
}

void button2_push(void)
{
}

void button2_release(void)
{
}

void button_event_handler(void)
{
	u8 changed_button;
	u8 current_button;
	u8 delay_button;
	u8 valid_button;
	
	current_button = GPIO_ReadInputData(BUTTON_PORT);
	valid_button = ~(current_button ^ delay_button);
	
	
	changed_button = (current_button^button_status);
	button_status = current_button;
	if ((changed_button & BUTTON_PIN1)!= 0)
	{
		if ((current_button & BUTTON_PIN1) == 0)
		{
			button1_push();
		}
		else
		{
			button1_release();
		}
	}
	
	if ((changed_button & BUTTON_PIN2)!= 0)
	{
		if ((current_button & BUTTON_PIN2) == 0)
		{
			button2_push();
		}
		else
		{
			button2_release();
		}
	}
}


void timer1_event_handler(void)
{
	stop_timer1();
	switch (m_button_timer_status)
	{
		case BUTTON_STATUS_INIT:
			{
				start_timer1(3);
				m_button_timer_status = BUTTON_STATUS_2s;
				break;
			}
		case BUTTON_STATUS_2s:
			{
				m_button_timer_status = BUTTON_STATUS_5s;
				break;
			}
		case BUTTON_STATUS_5s:
			{
				m_button_timer_status = BUTTON_STATUS_5s;
				break;
			}
		default:
			{
				break;
			}
	}
}

void timer2_event_handler(void)
{
	button_event_t button_event = BUTTON_SHORT_PRESS;
	detect_double_button_press = FALSE;
	stop_timer2();
	app_button_event_handler(button_event);
}


void timer1_timeout_handler(void)
{
	  timer1_index_s = timer1_index_s - 1;
		
		if (timer1_index_s <= 0)
		{
			  stop_timer1();
				timer1_event_handler();
		}
}

void timer2_timeout_handler(void)
{
	  timer2_index_50ms = timer2_index_50ms - 1;
		
		if (timer2_index_50ms <= 0)
		{
			  stop_timer2();
				timer2_event_handler();
		}
}