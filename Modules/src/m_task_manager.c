
/* LICENSE */

#include "app_config.h"
#include "lib_can.h"
#include <stdbool.h>
#include "tim.h"
#include "m_status_led.h"
#include "m_shock_abs.h"
#include "m_brake_disc_temp.h"
#include "m_wheel_speed.h"
#include "m_tire_temp.h"
#include "m_adc_readings.h"
#include "adc.h"

static void (*can_handler)(void);

static volatile bool m_1ms_event;
static volatile bool m_10ms_event;
static retcode_t m_init_status;

void m_task_manager_loop(void);

void m_task_manager_init(void)
{
	m_init_status = retcode_ok;
#if CONFIG_CAN_LIB_ENABLED
	HAL_CAN_Init(&hcan);
	can_handler = lib_can_init();
#else
	UNUSED(can_handler);
#endif /* CONFIG_CAN_LIB_ENABLED */

	HAL_TIM_Base_Start_IT(&htim16);
 	HAL_TIM_Base_Start_IT(&htim17);

//#if CONFIG_STATUS_LED_ENABLED
//	m_status_led_init(m_init_status);
//#endif /* CONFIG_STATUS_LED_ENABLED */

#if CONFIG_BRAKE_DISC_TEMP_ENABLED
	m_brake_disc_temp_init();
#endif

#if CONFIG_TIRE_TEMP_ENABLED
	m_tire_temp_init();
#endif

#if CONFIG_SHOCK_ABS_ENABLED || CONFIG_WHEEL_SPEED_ENABLED
	m_adc_readings_init();
#endif

#if CONFIG_SHOCK_ABS_ENABLED
	m_shock_abs_init();
#endif

#if CONFIG_WHEEL_SPEED_ENABLED
	m_WHEEL_SPEED_init();
#endif

	m_task_manager_loop();
}

void m_task_manager_loop(void)
{
	while(1)
	{
		if (m_1ms_event)
		{
#if CONFIG_CAN_LIB_ENABLED
			(*can_handler)();
#endif /* CONFIG_CAN_LIB_ENABLED */
			m_1ms_event = false;
		}
		if (m_10ms_event)
		{
//#if CONFIG_STATUS_LED_ENABLED
//			m_status_led_update();
//#endif /* CONFIG_STATUS_LED_ENABLED */

#if CONFIG_BRAKE_DISC_TEMP_ENABLED
			m_brake_disc_temp_update();
#endif
#if CONFIG_TIRE_TEMP_ENABLED
			m_tire_temp_update();
#endif
			m_10ms_event = false;
#if CONFIG_SHOCK_ABS_ENABLED
			m_shock_abs_update();
#endif

#if CONFIG_WHEEL_SPEED_ENABLED
			m_WHEEL_SPEED_update();
#endif
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// 1ms
	if (htim == &htim16)
	{
	}
	// 10ms
	else if (htim == &htim17)
	{
		m_1ms_event = true;

		m_10ms_event = true;
	}
}
