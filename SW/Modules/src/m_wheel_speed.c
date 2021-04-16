
/* LICENSE */

#include "app_config.h"
#if CONFIG_WHEEL_SPEED_ENABLED
#include "can_config.h"
#include "adc.h"
#include "m_adc_readings.h"
#include "m_wheel_speed.h"
#include "lib_can.h"

//#if WHEEL == FRONT_LEFT
// #define CAN_WHEEL_SPEED_ID				CAN_FL_WHEEL_SPEED_ID
// #define CAN_WHEEL_SPEED_PERIOD			CAN_FL_WHEEL_SPEED_PERIOD
// #define CAN_WHEEL_SPEED_DLC			CAN_FL_WHEEL_SPEED_DLC
//
// #define CAN_WHEEL_SPEED_BYTE_OFS		CAN_FL_WHEEL_SPEED_BYTE_OFS
// #define CAN_WHEEL_SPEED_MULT			CAN_FL_WHEEL_SPEED_MULT
// #define CAN_WHEEL_SPEED_DIV			CAN_FL_WHEEL_SPEED_DIV
// #define CAN_WHEEL_SPEED_OFS			CAN_FL_WHEEL_SPEED_OFS
// #define CAN_WHEEL_SPEED_VALUE_TYPE	CAN_FL_WHEEL_SPEED_VALUE_TYPE
//#el
//#if WHEEL == FRONT_RIGHT
// #define CAN_WHEEL_SPEED_ID				CAN_FR_WHEEL_SPEED_ID
// #define CAN_WHEEL_SPEED_PERIOD			CAN_FR_WHEEL_SPEED_PERIOD
// #define CAN_WHEEL_SPEED_DLC			CAN_FR_WHEEL_SPEED_DLC
//
// #define CAN_WHEEL_SPEED_BYTE_OFS		CAN_FR_WHEEL_SPEED_BYTE_OFS
// #define CAN_WHEEL_SPEED_MULT			CAN_FR_WHEEL_SPEED_MULT
// #define CAN_WHEEL_SPEED_DIV			CAN_FR_WHEEL_SPEED_DIV
// #define CAN_WHEEL_SPEED_OFS			CAN_FR_WHEEL_SPEED_OFS
// #define CAN_WHEEL_SPEED_VALUE_TYPE		CAN_FR_WHEEL_SPEED_VALUE_TYPE
//#el
//#if WHEEL == REAR_LEFT
// #define CAN_WHEEL_SPEED_ID				CAN_RL_WHEEL_SPEED_ID
// #define CAN_WHEEL_SPEED_PERIOD			CAN_RL_WHEEL_SPEED_PERIOD
// #define CAN_WHEEL_SPEED_DLC			CAN_RL_WHEEL_SPEED_DLC
//
// #define CAN_WHEEL_SPEED_BYTE_OFS		CAN_RL_WHEEL_SPEED_BYTE_OFS
// #define CAN_WHEEL_SPEED_MULT			CAN_RL_WHEEL_SPEED_MULT
// #define CAN_WHEEL_SPEED_DIV			CAN_RL_WHEEL_SPEED_DIV
// #define CAN_WHEEL_SPEED_OFS			CAN_RL_WHEEL_SPEED_OFS
// #define CAN_WHEEL_SPEED_VALUE_TYPE		CAN_RL_WHEEL_SPEED_VALUE_TYPE
//#el
#if WHEEL == REAR_RIGHT
 #define CAN_WHEEL_SPEED_ID				CAN_RR_WHEEL_SPEED_ID
 #define CAN_WHEEL_SPEED_PERIOD			CAN_RR_WHEEL_SPEED_PERIOD
 #define CAN_WHEEL_SPEED_DLC			CAN_RR_WHEEL_SPEED_DLC

 #define CAN_WHEEL_SPEED_BYTE_OFS		CAN_RR_WHEEL_SPEED_BYTE_OFS
 #define CAN_WHEEL_SPEED_MULT			CAN_RR_WHEEL_SPEED_MULT
 #define CAN_WHEEL_SPEED_DIV			CAN_RR_WHEEL_SPEED_DIV
 #define CAN_WHEEL_SPEED_OFS			CAN_RR_WHEEL_SPEED_OFS
 #define CAN_WHEEL_SPEED_VALUE_TYPE		CAN_RR_WHEEL_SPEED_VALUE_TYPE
#endif

//TODO: Depend of which sensor we'll use
#define MAX_WHEEL_DEFL_VAL	450
#define MIN_WHEEL_DEFL_VAL	250

uint32_t m_WHEEL_SPEED_compensate(uint32_t *reading);

static uint16_t compensate_div;
static uint16_t compensatedVal_wheel_speed;
uint16_t NonCompensatedVal_wheel_speed;

static lib_can_frame_t *f_wheel_speed;
static lib_can_data_t  *d_wheel_speed;

retcode_t m_WHEEL_SPEED_init(void)
{
	f_wheel_speed = lib_can_register_frame_tx(CAN_WHEEL_SPEED_ID, CAN_WHEEL_SPEED_PERIOD , CAN_WHEEL_SPEED_DLC);
	d_wheel_speed = lib_can_register_data(f_wheel_speed, CAN_WHEEL_SPEED_BYTE_OFS, CAN_WHEEL_SPEED_MULT, CAN_WHEEL_SPEED_DIV, CAN_WHEEL_SPEED_OFS, CAN_WHEEL_SPEED_VALUE_TYPE, _uint16_t);

	//TODO: Depend of which sensor we'll use
	compensate_div = (MAX_WHEEL_DEFL_VAL - MIN_WHEEL_DEFL_VAL)/100;
    return retcode_ok;
}

void m_WHEEL_SPEED_update(void)
{
	NonCompensatedVal_wheel_speed = * wheel_speed_reading;
	uint32_t compensated_reading = m_WHEEL_SPEED_compensate(wheel_speed_reading);
	compensatedVal_wheel_speed = compensated_reading;
	lib_can_data_set(d_wheel_speed, &compensatedVal_wheel_speed);
}

uint32_t m_WHEEL_SPEED_compensate(uint32_t *reading)
{


	return ((uint16_t)*reading - MIN_WHEEL_DEFL_VAL)/compensate_div;
}

#endif /* CONFIG_WHEEL_SPEED_ENABLED */
