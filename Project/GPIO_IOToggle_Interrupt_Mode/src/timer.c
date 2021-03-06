#include "stm8s.h"
#include "stm8s_tim4.h"

#include "timer.h"

#define MAX_TIMER_NUMBER    6

typedef struct timer_manager_s
{
    u8   timer_index;
    bool timer_started;
    s32  timer_left;
    app_timer_timeout_handler_t handler;
} timer_manager_t;

static timer_manager_t m_timer_manager[MAX_TIMER_NUMBER] = {0};
static u8 m_current_timer_num = 0;
static u8 m_running_timer_num = 0;
static s32 m_wait_timer_tick = 0;

static s32 m_past_tick = 0;

s32 system_past_tick = 0;

void timer_init(void)
{
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_32, 250); // (1/4MHz)*32*125 = 1mS
    TIM4_SetCounter(0); // T = n * 1mS
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE); //Enable TIM4 IT UPDATE
    TIM4_Cmd(ENABLE);
}

void timer_create(u8 *timer_index, app_timer_timeout_handler_t timerout_hander)
{
	if (m_current_timer_num >= MAX_TIMER_NUMBER)
	{
		// No space for the new timer.
		return;
	}
	else
	{
		*timer_index = m_current_timer_num;
		m_timer_manager[m_current_timer_num].timer_index = m_current_timer_num;
		m_timer_manager[m_current_timer_num].timer_started = FALSE;
		m_timer_manager[m_current_timer_num].handler = timerout_hander;
		m_current_timer_num ++;
	}
}

void timer_schedule(bool is_new)
{
	u8 index = 0;
	m_wait_timer_tick = 0x7FFFFFFF;
	m_running_timer_num = 0;

	for (index = 0; index < m_current_timer_num; index ++)
	{
		if (m_timer_manager[index].timer_started == TRUE)
		{
			if (is_new == FALSE)
			{
				m_timer_manager[index].timer_left -= system_past_tick;
			}
			if (m_timer_manager[index].timer_left > 0)
			{
				if (m_wait_timer_tick >= m_timer_manager[index].timer_left)
				{
					m_running_timer_num ++;
					m_wait_timer_tick = m_timer_manager[index].timer_left;
				}
			}
			else
			{
				m_timer_manager[index].timer_left = 0;
				m_timer_manager[index].timer_started == FALSE;
				m_timer_manager[index].handler();
			}
		}
	}
	system_past_tick = 0;
	if (m_wait_timer_tick == 0xFFFFFFFF)
	{
		m_wait_timer_tick = 0;
	}
}


void timer_start(u8 timer_index, u32 duration)
{
	m_timer_manager[timer_index].timer_started = TRUE;
	m_timer_manager[timer_index].timer_left = duration;

	timer_schedule(TRUE);
}

void timer_stop(u8 timer_index)
{
	m_timer_manager[timer_index].timer_started = FALSE;
}


void tick_timeout_handler(void)
{
	if (m_running_timer_num > 0)
	{
		system_past_tick ++;
		if (system_past_tick >= m_wait_timer_tick)
		{
			timer_schedule(FALSE);
		}
	}
}
