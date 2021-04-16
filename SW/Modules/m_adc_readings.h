
/* LICENSE */

#ifndef M_ADC_READINGS_H_
#define M_ADC_READINGS_H_
#include "app_config.h"

extern uint32_t *shock_abs_reading;
extern uint32_t *wheel_speed_reading;

void m_adc_readings_update(void);
void m_adc_readings_init(void);

#endif /* M_ADC_READINGS_H_ */
