#include "stm8s.h"
#include "stm8s_tim1.h"

#include "button.h"

#define LEDS_PORT (GPIOB)

static bool led_is_on = FALSE;

extern u32 int_timers;

void start_timer1(void)
{
    TIM1_DeInit();
    TIM1_TimeBaseInit(0x8, TIM1_COUNTERMODE_UP, 125, 3); // (1/4MHz)*32*125 = 1mS
    TIM1_SetCounter(0); // T = n * 1mS
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); //Enable TIM4 IT UPDATE
    TIM1_Cmd(ENABLE); //Enable TIM4
}

void button_event_handler(void)
{
    start_timer1();
}


void timer1_timeout_handler(void)
{
    u8 Led = GPIO_PIN_1;

    TIM1_Cmd(DISABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE, DISABLE);

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
