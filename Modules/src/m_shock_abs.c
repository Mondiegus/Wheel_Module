
/* LICENSE */

#include "app_config.h"
#if CONFIG_SHOCK_ABS_ENABLED
#include "can_config.h"
#include "adc.h"
#include "m_adc_readings.h"
#include "m_shock_abs.h"
#include "lib_can.h"

//#if WHEEL == FRONT_LEFT
// #define CAN_SHOCK_ABS_ID			CAN_FL_SHOCK_ABS_ID
// #define CAN_SHOCK_ABS_PERIOD		CAN_FL_SHOCK_ABS_PERIOD
// #define CAN_SHOCK_ABS_DLC			CAN_FL_SHOCK_ABS_DLC
//
// #define CAN_SHOCK_ABS_BYTE_OFS		CAN_FL_SHOCK_ABS_BYTE_OFS
// #define CAN_SHOCK_ABS_MULT			CAN_FL_SHOCK_ABS_MULT
// #define CAN_SHOCK_ABS_DIV			CAN_FL_SHOCK_ABS_DIV
// #define CAN_SHOCK_ABS_OFS			CAN_FL_SHOCK_ABS_OFS
// #define CAN_SHOCK_ABS_VALUE_TYPE	CAN_FL_SHOCK_ABS_VALUE_TYPE
//#el
//#if WHEEL == FRONT_RIGHT
// #define CAN_SHOCK_ABS_ID			CAN_FR_SHOCK_ABS_ID
// #define CAN_SHOCK_ABS_PERIOD		CAN_FR_SHOCK_ABS_PERIOD
// #define CAN_SHOCK_ABS_DLC			CAN_FR_SHOCK_ABS_DLC
//
// #define CAN_SHOCK_ABS_BYTE_OFS		CAN_FR_SHOCK_ABS_BYTE_OFS
// #define CAN_SHOCK_ABS_MULT			CAN_FR_SHOCK_ABS_MULT
// #define CAN_SHOCK_ABS_DIV			CAN_FR_SHOCK_ABS_DIV
// #define CAN_SHOCK_ABS_OFS			CAN_FR_SHOCK_ABS_OFS
// #define CAN_SHOCK_ABS_VALUE_TYPE	CAN_FR_SHOCK_ABS_VALUE_TYPE
//#el
//#if WHEEL == REAR_LEFT
// #define CAN_SHOCK_ABS_ID			CAN_RL_SHOCK_ABS_ID
// #define CAN_SHOCK_ABS_PERIOD		CAN_RL_SHOCK_ABS_PERIOD
// #define CAN_SHOCK_ABS_DLC			CAN_RL_SHOCK_ABS_DLC
//
// #define CAN_SHOCK_ABS_BYTE_OFS		CAN_RL_SHOCK_ABS_BYTE_OFS
// #define CAN_SHOCK_ABS_MULT			CAN_RL_SHOCK_ABS_MULT
// #define CAN_SHOCK_ABS_DIV			CAN_RL_SHOCK_ABS_DIV
// #define CAN_SHOCK_ABS_OFS			CAN_RL_SHOCK_ABS_OFS
// #define CAN_SHOCK_ABS_VALUE_TYPE	CAN_RL_SHOCK_ABS_VALUE_TYPE
//#el
#if WHEEL == REAR_RIGHT
 #define CAN_SHOCK_ABS_ID			CAN_RR_SHOCK_ABS_ID
 #define CAN_SHOCK_ABS_PERIOD		CAN_RR_SHOCK_ABS_PERIOD
 #define CAN_SHOCK_ABS_DLC			CAN_RR_SHOCK_ABS_DLC

 #define CAN_SHOCK_ABS_BYTE_OFS		CAN_RR_SHOCK_ABS_BYTE_OFS
 #define CAN_SHOCK_ABS_MULT			CAN_RR_SHOCK_ABS_MULT
 #define CAN_SHOCK_ABS_DIV			CAN_RR_SHOCK_ABS_DIV
 #define CAN_SHOCK_ABS_OFS			CAN_RR_SHOCK_ABS_OFS
 #define CAN_SHOCK_ABS_VALUE_TYPE	CAN_RR_SHOCK_ABS_VALUE_TYPE
#endif

#define MAX_SHOCK_DEFL_VAL	4000
#define MIN_SHOCK_DEFL_VAL	0

uint32_t m_shock_abs_compensate(uint32_t *reading);

static uint16_t compensate_div;
static uint16_t compensatedVal_shock_abs;
static uint16_t NonCompensatedVal_shock_abs;

static lib_can_frame_t *f_shock_abs;
static lib_can_data_t  *d_shock_abs;

retcode_t m_shock_abs_init(void)
{
	f_shock_abs = lib_can_register_frame_tx(CAN_SHOCK_ABS_ID, CAN_SHOCK_ABS_PERIOD , CAN_SHOCK_ABS_DLC);
	d_shock_abs = lib_can_register_data(f_shock_abs, CAN_SHOCK_ABS_BYTE_OFS, CAN_SHOCK_ABS_MULT, CAN_SHOCK_ABS_DIV, CAN_SHOCK_ABS_OFS, CAN_SHOCK_ABS_VALUE_TYPE, _uint16_t);

	compensate_div =(MAX_SHOCK_DEFL_VAL - MIN_SHOCK_DEFL_VAL)/100;

    return retcode_ok;
}

void m_shock_abs_update(void)
{
	NonCompensatedVal_shock_abs = *shock_abs_reading;
	uint32_t compensated_reading = m_shock_abs_compensate(shock_abs_reading);
	compensatedVal_shock_abs = compensated_reading;
	lib_can_data_set(d_shock_abs, &compensatedVal_shock_abs);
}

uint32_t m_shock_abs_compensate(uint32_t *reading)
{
	return ((uint16_t)*reading);
/*
	return ((uint16_t)*reading - MIN_SHOCK_DEFL_VAL)/compensate_div;
*/
	}

#endif /* CONFIG_SHOCK_ABS_ENABLED */
