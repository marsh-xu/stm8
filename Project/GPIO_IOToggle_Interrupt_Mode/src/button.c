#include "stm8s.h"
#include "stm8s_tim1.h"

#include "button.h"

#define LEDS_PORT (GPIOB)

#define BUTTON_PORT (GPIOB)
#define BUTTON_PIN1  (GPIO_PIN_6)
#define BUTTON_PIN2  (GPIO_PIN_7)

typedef enum button_timer_status_e
{
    BUTTON_STATUS_INIT = 0,
		BUTTON_STATUS_2s,
		BUTTON_STATUS_5s,
} button_timer_status_t;
static button_timer_status_t  m_button_timer_status = BUTTON_STATUS_INIT;

static bool led_is_on = FALSE;
static u8   button_status = 0;
static bool timer_is_started = FALSE;

void Delay (u16 nCount);
extern u32 int_timers;

void start_timer1(void)
{
    TIM1_DeInit();
    TIM1_TimeBaseInit(0x8, TIM1_COUNTERMODE_UP, 125, 3); // (1/4MHz)*32*125 = 1mS
    TIM1_SetCounter(0); // T = n * 1mS
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); //Enable TIM4 IT UPDATE
    TIM1_Cmd(ENABLE); //Enable TIM4
}

void stop_timer1(void)
{
    TIM1_Cmd(DISABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE, DISABLE);
}

void button1_push(void)
{
	switch (m_button_timer_status)
	{
		case 
	}
}

void button1_release(void)
{
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
	u8 current_button = GPIO_ReadInputData(BUTTON_PORT);
	changed_button = current_button^button_status;
	if ((changed_button & BUTTON_PIN1)!= 0)
	{
		if ((current_button & BUTTON_PIN1) != 0)
		{
			button1_push();
		}
		else
		{
			button1_relese();
		}
	}
	
	if ((changed_button & BUTTON_PIN2)!= 0)
	{
		if ((current_button & BUTTON_PIN2) != 0)
		{
			button2_push();
		}
		else
		{
			button2_relese();
		}
	}
}


void timer1_timeout_handler(void)
{
    u8 Led = GPIO_PIN_1;

    if (led_is_on == FALSE)
    {
        GPIO_WriteHigh(LEDS_PORT, Led);
        led_is_on = TRUE;
    }
    else
    {
        led_is_on = FALSE;
        GPIO_WriteLow(LEDS_PORT, Led);
    }
}
