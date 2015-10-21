#include "stm8s.h"
#include "stm8s_tim1.h"
#include "stm8s_tim2.h"

#include "timer.h"
#include "button.h"

#define LEDS_PORT (GPIOB)

#define BUTTON_PORT (GPIOB)
#define BUTTON_PIN1  (GPIO_PIN_6)
#define BUTTON_PIN2  (GPIO_PIN_7)

typedef enum button_timer_status_e
{
	BUTTON_STATUS_INIT = 0,
	BUTTON_STATUS_LESS_2S,
	BUTTON_STATUS_MORE_2S,
	BUTTON_STATUS_MORE_5S
}button_timer_status_t;

typedef enum button_event_e
{
	BUTTON_INVALID = 0,
	BUTTON1_SHORT_PRESS,
	BUTTON1_DOUBLE_PRESS,
	BUTTON1_LONG_HOLD,
	BUTTON1_LONG_PRESS,
	BUTTON1_VERY_LONG_HOLD,
	BUTTON1_VERY_LONG_PRESS,
	BUTTON2_SHORT_PRESS,
	BUTTON2_DOUBLE_PRESS,
	BUTTON2_LONG_HOLD,
	BUTTON2_LONG_PRESS,
	BUTTON2_VERY_LONG_HOLD,
	BUTTON2_VERY_LONG_PRESS
} button_event_t;

static button_timer_status_t  m_button1_timer_status = BUTTON_STATUS_INIT;
static button_timer_status_t  m_button2_timer_status = BUTTON_STATUS_INIT;

static bool detect_double_button1_press = FALSE;
static bool detect_double_button2_press = FALSE;

static u8   button_status = 0xFF;
static u8   button_first_detect_status = 0xFF;

static u8   m_timer_id_button1_detet;
static u8   m_timer_id_double_btn1_detet;
static u8   m_timer_id_button2_detet;
static u8   m_timer_id_double_btn2_detet;

static u8   m_timer_id_debonce_detet;

void Delay (u16 nCount);

extern u32 int_timer1;
extern u32 int_timer2;

void app_button_event_handler(button_event_t button_event);
void button1_push(void);
void button1_release(void);
void button2_push(void);
void button2_release(void);


static void button1_duration_timeout_handler(void)
{
	button_event_t button_event = BUTTON_INVALID;
	switch (m_button1_timer_status)
	{
		case BUTTON_STATUS_INIT:
			{
				break;
			}
		case BUTTON_STATUS_LESS_2S:
			{
				button_event = BUTTON1_LONG_HOLD;
				timer_start(m_timer_id_button1_detet, 300);
				m_button1_timer_status = BUTTON_STATUS_MORE_2S;
				break;
			}
		case BUTTON_STATUS_MORE_2S:
			{
				button_event = BUTTON1_VERY_LONG_HOLD;
				m_button1_timer_status = BUTTON_STATUS_MORE_5S;
				break;
			}
		default:
			{
				break;
			}
	}
	if (button_event != BUTTON_INVALID)
	{
		app_button_event_handler(button_event);
	}
}

static void button2_duration_timeout_handler(void)
{
	button_event_t button_event = BUTTON_INVALID;
	switch (m_button2_timer_status)
	{
		case BUTTON_STATUS_INIT:
			{
				break;
			}
		case BUTTON_STATUS_LESS_2S:
			{
				button_event = BUTTON2_LONG_HOLD;
				timer_start(m_timer_id_button2_detet, 300);
				m_button2_timer_status = BUTTON_STATUS_MORE_2S;
				break;
			}
		case BUTTON_STATUS_MORE_2S:
			{
				button_event = BUTTON2_VERY_LONG_HOLD;
				m_button2_timer_status = BUTTON_STATUS_MORE_5S;
				break;
			}
		default:
			{
				break;
			}
	}
	if (button_event != BUTTON_INVALID)
	{
		app_button_event_handler(button_event);
	}
}

void double_btn1_timeout_handler(void)
{
	button_event_t button_event = BUTTON1_SHORT_PRESS;
	detect_double_button1_press = FALSE;
	m_button1_timer_status = BUTTON_STATUS_INIT;
	timer_stop(m_timer_id_double_btn1_detet);
	app_button_event_handler(button_event);
}

void double_btn2_timeout_handler(void)
{
	button_event_t button_event = BUTTON2_SHORT_PRESS;
	detect_double_button2_press = FALSE;
	m_button2_timer_status = BUTTON_STATUS_INIT;
	timer_stop(m_timer_id_double_btn2_detet);
	app_button_event_handler(button_event);
}

void btn_debonce_timeout_handler(void)
{
	u8 valid_button;
	u8 current_button;
	u8 changed_button;

	current_button = GPIO_ReadInputData(BUTTON_PORT);


	valid_button = ~(current_button ^ button_first_detect_status);


	changed_button = ((current_button^button_status) & valid_button);
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


void button_init()
{
	timer_create(&m_timer_id_button1_detet, button1_duration_timeout_handler);
	timer_create(&m_timer_id_double_btn1_detet, double_btn1_timeout_handler);
	timer_create(&m_timer_id_button2_detet, button2_duration_timeout_handler);
	timer_create(&m_timer_id_double_btn2_detet, double_btn2_timeout_handler);
	timer_create(&m_timer_id_debonce_detet, btn_debonce_timeout_handler);
}


void btn_short_button1_press(void)
{
		u8 Led = GPIO_PIN_1;
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
}

void btn_double_button1_press(void)
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

void btn_long_hold_button1_press(void)
{
		u8 Led = GPIO_PIN_1;
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
}

void btn_long_button1_press(void)
{
}

void btn_very_long_hold_button1_press(void)
{
		u8 Led = GPIO_PIN_1;
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
}

void btn_very_long_button1_press(void)
{
}

void btn_short_button2_press(void)
{
		u8 Led = GPIO_PIN_1;
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
}

void btn_double_button2_press(void)
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

void btn_long_hold_button2_press(void)
{
		u8 Led = GPIO_PIN_1;
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
}

void btn_long_button2_press(void)
{
}

void btn_very_long_hold_button2_press(void)
{
		u8 Led = GPIO_PIN_1;
		GPIO_WriteHigh(LEDS_PORT, Led);
		Delay(10000);
		GPIO_WriteLow(LEDS_PORT, Led);
}

void btn_very_long_button2_press(void)
{
}

void app_button_event_handler(button_event_t button_event)
{
	switch (button_event)
	{
		case BUTTON_INVALID:
		{
			break;
		}
		case BUTTON1_SHORT_PRESS:
		{
			btn_short_button1_press();
			break;
		}
		case BUTTON1_DOUBLE_PRESS:
		{
			btn_double_button1_press();
			break;
		}
		case BUTTON1_LONG_HOLD:
		{
			btn_long_hold_button1_press();
			break;
		}
		case BUTTON1_LONG_PRESS:
		{
			btn_long_button1_press();
			break;
		}
		case BUTTON1_VERY_LONG_HOLD:
		{
			btn_very_long_hold_button1_press();
			break;
		}
		case BUTTON1_VERY_LONG_PRESS:
		{
			btn_very_long_button1_press();
			break;
		}
		case BUTTON2_SHORT_PRESS:
		{
			btn_short_button2_press();
			break;
		}
		case BUTTON2_DOUBLE_PRESS:
		{
			btn_double_button2_press();
			break;
		}
		case BUTTON2_LONG_HOLD:
		{
			btn_long_hold_button2_press();
			break;
		}
		case BUTTON2_LONG_PRESS:
		{
			btn_long_button2_press();
			break;
		}
		case BUTTON2_VERY_LONG_HOLD:
		{
			btn_very_long_hold_button2_press();
			break;
		}
		case BUTTON2_VERY_LONG_PRESS:
		{
			btn_very_long_button2_press();
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
	timer_stop(m_timer_id_button1_detet);
	if (m_button1_timer_status != BUTTON_STATUS_LESS_2S)
	{
		m_button1_timer_status = BUTTON_STATUS_LESS_2S;
		timer_start(m_timer_id_button1_detet, 200);
	}
}

void button1_release(void)
{
	button_event_t button_event = BUTTON_INVALID;
	timer_stop(m_timer_id_button1_detet);
	switch (m_button1_timer_status)
	{
		case BUTTON_STATUS_INIT:
			{
				break;
			}
		case BUTTON_STATUS_LESS_2S:
			{
				if (detect_double_button1_press == FALSE)
				{
					detect_double_button1_press = TRUE;
					timer_start(m_timer_id_double_btn1_detet,100);  //500ms
				}
				else
				{
					button_event = BUTTON1_DOUBLE_PRESS;
					detect_double_button1_press = FALSE;
					timer_stop(m_timer_id_double_btn1_detet);
				}
				break;
			}
		case BUTTON_STATUS_MORE_2S:
			{
				button_event = BUTTON1_LONG_PRESS;
				break;
			}
		case BUTTON_STATUS_MORE_5S:
			{
				button_event = BUTTON1_VERY_LONG_PRESS;
				break;
			}
		default:
			{
				break;
			}
	}
	m_button1_timer_status = BUTTON_STATUS_INIT;
	if (button_event != BUTTON_INVALID)
	{
		app_button_event_handler(button_event);
	}
}

void button2_push(void)
{
	timer_stop(m_timer_id_button2_detet);
	if (m_button2_timer_status != BUTTON_STATUS_LESS_2S)
	{
		m_button2_timer_status = BUTTON_STATUS_LESS_2S;
		timer_start(m_timer_id_button2_detet, 200);
	}
}

void button2_release(void)
{
	button_event_t button_event = BUTTON_INVALID;
	timer_stop(m_timer_id_button2_detet);
	switch (m_button2_timer_status)
	{
		case BUTTON_STATUS_INIT:
			{
				break;
			}
		case BUTTON_STATUS_LESS_2S:
			{
				if (detect_double_button2_press == FALSE)
				{
					detect_double_button2_press = TRUE;
					timer_start(m_timer_id_double_btn2_detet,100);  //500ms
				}
				else
				{
					button_event = BUTTON2_DOUBLE_PRESS;
					detect_double_button2_press = FALSE;
					timer_stop(m_timer_id_double_btn2_detet);
				}
				break;
			}
		case BUTTON_STATUS_MORE_2S:
			{
				button_event = BUTTON2_LONG_PRESS;
				break;
			}
		case BUTTON_STATUS_MORE_5S:
			{
				button_event = BUTTON2_VERY_LONG_PRESS;
				break;
			}
		default:
			{
				break;
			}
	}
	m_button2_timer_status = BUTTON_STATUS_INIT;
	if (button_event != BUTTON_INVALID)
	{
		app_button_event_handler(button_event);
	}
}

void button_event_handler(void)
{
	button_first_detect_status = GPIO_ReadInputData(BUTTON_PORT);
	timer_start(m_timer_id_debonce_detet, 3);
}
