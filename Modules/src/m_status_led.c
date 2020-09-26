
/* LICENSE */

#include "app_config.h"
#include "main.h"
#if CONFIG_STATUS_LED_ENABLED
#include <stdbool.h>

#define ON_TIME_MS			100
#define ERROR_ON_TIME_MS	1100
#define OFF_TIME_MS			900

static void m_status_led_set(void);
static void m_status_led_clr(void);

static bool m_state;
static retcode_t m_error_status;

//void m_status_led_init(retcode_t error_status)
//{
////	m_status_led_clr();
//	m_state = false;
//
//	m_error_status = error_status;
//}

//void m_status_led_update(void)
//{
//	static uint32_t on_counter;
//	static uint32_t off_counter;
//
//	if (m_state)
//	{
//		m_status_led_set();
//		if (m_error_status == retcode_ok)
//		{
//			if ((HAL_GetTick() - on_counter) > ON_TIME_MS)
//			{
//				off_counter = HAL_GetTick();
//				m_state = false;
//			}
//		}
//		else
//		{
//			if ((HAL_GetTick() - on_counter) > ERROR_ON_TIME_MS)
//			{
//				off_counter = HAL_GetTick();
//				m_state = false;
//			}
//		}
//	}
//	else
//	{
//		m_status_led_clr();
//		if ((HAL_GetTick() - off_counter) > OFF_TIME_MS)
//		{
//			on_counter = HAL_GetTick();
//			m_state = true;
//		}
//	}
//}

//static void m_status_led_set(void)
//{
//	HAL_GPIO_WritePin(INFO_GPIO_Port, INFO_Pin, GPIO_PIN_SET);
//}
//
//static void m_status_led_clr(void)
//{
//	HAL_GPIO_WritePin(INFO_GPIO_Port, INFO_Pin, GPIO_PIN_RESET);
//}

#endif /* CONFIG_STATUS_LED_ENABLED */
