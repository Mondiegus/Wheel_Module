
/* LICENSE */

#include "app_config.h"
#if (CONFIG_SHOCK_ABS_ENABLED || CONFIG_WHEEL_SPEED_ENABLED)
#include "adc.h"
#include <stdbool.h>

uint16_t *shock_abs_reading;
uint16_t *wheel_speed_reading;
static uint16_t m_read_values[2];

void m_adc_readings_init(void)
{
	HAL_ADC_Start_DMA(&hadc1, m_read_values, 2);

	shock_abs_reading = &m_read_values[0];
	wheel_speed_reading = &m_read_values[1];
}

#endif /* CONFIG_SHOCK_ABS_ENABLED || CONFIG_WHEEL_SPEED_ENABLED */
