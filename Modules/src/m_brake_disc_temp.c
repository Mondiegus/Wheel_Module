
/* LICENSE */

#include "app_config.h"
#if CONFIG_BRAKE_DISC_TEMP_ENABLED
#include "can_config.h"
#include "i2c.h"
#include "m_brake_disc_temp.h"
#include "lib_can.h"
#include "mlx90614.h"

//#if WHEEL == FRONT_LEFT
///* FRAME PARAETERS */
// #define CAN_BRAKE_DISC_TEMP_ID			CAN_FL_BRAKE_DISC_TEMP_ID
// #define CAN_BRAKE_DISC_TEMP_PERIOD		CAN_FL_BRAKE_DISC_TEMP_PERIOD
// #define CAN_BRAKE_DISC_TEMP_DLC		CAN_FL_BRAKE_DISC_TEMP_DLC
///* DATA PARAMETERS */
// #define CAN_BRAKE_DISC_TEMP_BYTE_OFS	CAN_FL_BRAKE_DISC_TEMP_BYTE_OFS
// #define CAN_BRAKE_DISC_TEMP_MULT		CAN_FL_BRAKE_DISC_TEMP_MULT
// #define CAN_BRAKE_DISC_TEMP_DIV		CAN_FL_BRAKE_DISC_TEMP_DIV
// #define CAN_BRAKE_DISC_TEMP_OFS		CAN_FL_BRAKE_DISC_TEMP_OFS
// #define CAN_BRAKE_DISC_TEMP_VALUE_TYPE	CAN_FL_BRAKE_DISC_TEMP_VALUE_TYPE
//#el
//#if WHEEL == FRONT_RIGHT
///* FRAME PARAETERS */
// #define CAN_BRAKE_DISC_TEMP_ID			CAN_FR_BRAKE_DISC_TEMP_ID
// #define CAN_BRAKE_DISC_TEMP_PERIOD		CAN_FR_BRAKE_DISC_TEMP_PERIOD
// #define CAN_BRAKE_DISC_TEMP_DLC		CAN_FR_BRAKE_DISC_TEMP_DLC
///* DATA PARAMETERS */
// #define CAN_BRAKE_DISC_TEMP_BYTE_OFS	CAN_FR_BRAKE_DISC_TEMP_BYTE_OFS
// #define CAN_BRAKE_DISC_TEMP_MULT		CAN_FR_BRAKE_DISC_TEMP_MULT
// #define CAN_BRAKE_DISC_TEMP_DIV		CAN_FR_BRAKE_DISC_TEMP_DIV
// #define CAN_BRAKE_DISC_TEMP_OFS		CAN_FR_BRAKE_DISC_TEMP_OFS
// #define CAN_BRAKE_DISC_TEMP_VALUE_TYPE	CAN_FR_BRAKE_DISC_TEMP_VALUE_TYPE

//#el
//#if WHEEL == REAR_LEFT
///* FRAME PARAETERS */
// #define CAN_BRAKE_DISC_TEMP_ID			CAN_RL_BRAKE_DISC_TEMP_ID
// #define CAN_BRAKE_DISC_TEMP_PERIOD		CAN_RL_BRAKE_DISC_TEMP_PERIOD
// #define CAN_BRAKE_DISC_TEMP_DLC		CAN_RL_BRAKE_DISC_TEMP_DLC
///* DATA PARAMETERS */
// #define CAN_BRAKE_DISC_TEMP_BYTE_OFS	CAN_RL_BRAKE_DISC_TEMP_BYTE_OFS
// #define CAN_BRAKE_DISC_TEMP_MULT		CAN_RL_BRAKE_DISC_TEMP_MULT
// #define CAN_BRAKE_DISC_TEMP_DIV		CAN_RL_BRAKE_DISC_TEMP_DIV
// #define CAN_BRAKE_DISC_TEMP_OFS		CAN_RL_BRAKE_DISC_TEMP_OFS
// #define CAN_BRAKE_DISC_TEMP_VALUE_TYPE	CAN_RL_BRAKE_DISC_TEMP_VALUE_TYPE
//
//#el
#if WHEEL == REAR_RIGHT
/* FRAME PARAETERS */
 #define CAN_BRAKE_DISC_TEMP_ID			CAN_RR_BRAKE_DISC_TEMP_ID
 #define CAN_BRAKE_DISC_TEMP_PERIOD		CAN_RR_BRAKE_DISC_TEMP_PERIOD
 #define CAN_BRAKE_DISC_TEMP_DLC		CAN_RR_BRAKE_DISC_TEMP_DLC
/* DATA PARAMETERS */
 #define CAN_BRAKE_DISC_TEMP_BYTE_OFS	CAN_RR_BRAKE_DISC_TEMP_BYTE_OFS
 #define CAN_BRAKE_DISC_TEMP_MULT		CAN_RR_BRAKE_DISC_TEMP_MULT
 #define CAN_BRAKE_DISC_TEMP_DIV		CAN_RR_BRAKE_DISC_TEMP_DIV
 #define CAN_BRAKE_DISC_TEMP_OFS		CAN_RR_BRAKE_DISC_TEMP_OFS
 #define CAN_BRAKE_DISC_TEMP_VALUE_TYPE	CAN_RR_BRAKE_DISC_TEMP_VALUE_TYPE

#endif

//static drv_mlx90614_t m_handler;

static lib_can_frame_t *f_brake_disc_temp;
static lib_can_data_t  *d_brake_disc_temp;
uint32_t compensatedVal_m_brake_disc_temp;
static uint16_t m_brake_disc_temp;
uint16_t WheelObjectTemperature;
uint16_t WheelAmbientTemperature;
uint16_t WheelEmissivity;
retcode_t m_brake_disc_temp_init(void)
{

    MLX90614_Init(&hi2c1);
	//MLX90614_SetAddress(MLX90614_SECOND_ADDRESS,MLX90614_DEFAULT_ADDRESS);
	MLX90614_SetAddress(MLX90614_DEFAULT_ADDRESS,MLX90614_SECOND_ADDRESS);
//    MLX90614_SetEmissivity(0.985,MLX90614_DEFAULT_ADDRESS); // Human skin
    MLX90614_SetEmissivity(0.650,MLX90614_SECOND_ADDRESS); // break disc

	f_brake_disc_temp = lib_can_register_frame_tx(CAN_BRAKE_DISC_TEMP_ID, CAN_BRAKE_DISC_TEMP_PERIOD , CAN_BRAKE_DISC_TEMP_DLC);
	d_brake_disc_temp = lib_can_register_data(f_brake_disc_temp, CAN_BRAKE_DISC_TEMP_BYTE_OFS, CAN_BRAKE_DISC_TEMP_MULT, CAN_BRAKE_DISC_TEMP_DIV, CAN_BRAKE_DISC_TEMP_OFS, CAN_BRAKE_DISC_TEMP_VALUE_TYPE, _uint16_t);

    return retcode_ok;
}

retcode_t m_brake_disc_temp_deinit(void)
{
	APP_RETURN_ERR(lib_can_unregister_data(d_brake_disc_temp), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_brake_disc_temp), retcode_ok);
	return retcode_ok;
}

void m_brake_disc_temp_update(void)
{
	MLX90614_ReadAmbientTemperature(&WheelAmbientTemperature,MLX90614_SECOND_ADDRESS);
    MLX90614_ReadObjectTemperature(&WheelObjectTemperature,MLX90614_SECOND_ADDRESS);
    MLX90614_GetEmissivity(&WheelEmissivity,MLX90614_SECOND_ADDRESS);
    m_brake_disc_temp = WheelObjectTemperature;
	lib_can_data_set(d_brake_disc_temp, &m_brake_disc_temp);
}

#endif /* CONFIG_BRAKE_DISC_TEMP_ENABLED */
