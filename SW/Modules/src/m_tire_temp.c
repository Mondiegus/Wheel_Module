
/* LICENSE */

#include "app_config.h"
#if CONFIG_TIRE_TEMP_ENABLED
#include "can_config.h"
#include "i2c.h"
#include "m_tire_temp.h"
#include "lib_can.h"
#include "mlx90614.h"
#include "IRTempSens.h"
#include "IRTempSens.cpp"

//#if WHEEL == FRONT_LEFT
///* FRAME PARAETERS */
// #define CAN_TIRE_TEMP_1_ID				CAN_FL_TIRE_TEMP_1_ID
// #define CAN_TIRE_TEMP_2_ID				CAN_FL_TIRE_TEMP_2_ID
// #define CAN_TIRE_TEMP_PERIOD				CAN_FL_TIRE_TEMP_PERIOD
// #define CAN_TIRE_TEMP_DLC				CAN_FL_TIRE_TEMP_DLC
///* DATA PARAMETERS */
// #define CAN_TIRE_TEMP1_1_BYTE_OFS		CAN_FL_TIRE_TEMP1_1_BYTE_OFS
// #define CAN_TIRE_TEMP2_1_BYTE_OFS		CAN_FL_TIRE_TEMP2_1_BYTE_OFS
// #define CAN_TIRE_TEMP3_1_BYTE_OFS		CAN_FL_TIRE_TEMP3_1_BYTE_OFS
// #define CAN_TIRE_TEMP4_1_BYTE_OFS		CAN_FL_TIRE_TEMP4_1_BYTE_OFS
// #define CAN_TIRE_TEMP5_1_BYTE_OFS		CAN_FL_TIRE_TEMP5_1_BYTE_OFS
// #define CAN_TIRE_TEMP6_1_BYTE_OFS		CAN_FL_TIRE_TEMP6_1_BYTE_OFS
// #define CAN_TIRE_TEMP7_1_BYTE_OFS		CAN_FL_TIRE_TEMP7_1_BYTE_OFS
// #define CAN_TIRE_TEMP8_1_BYTE_OFS		CAN_FL_TIRE_TEMP8_1_BYTE_OFS
//
// #define CAN_TIRE_TEMP1_2_BYTE_OFS		CAN_FL_TIRE_TEMP1_2_BYTE_OFS
// #define CAN_TIRE_TEMP2_2_BYTE_OFS		CAN_FL_TIRE_TEMP2_2_BYTE_OFS
// #define CAN_TIRE_TEMP3_2_BYTE_OFS		CAN_FL_TIRE_TEMP3_2_BYTE_OFS
// #define CAN_TIRE_TEMP4_2_BYTE_OFS		CAN_FL_TIRE_TEMP4_2_BYTE_OFS
// #define CAN_TIRE_TEMP5_2_BYTE_OFS		CAN_FL_TIRE_TEMP5_2_BYTE_OFS
// #define CAN_TIRE_TEMP6_2_BYTE_OFS		CAN_FL_TIRE_TEMP6_2_BYTE_OFS
// #define CAN_TIRE_TEMP7_2_BYTE_OFS		CAN_FL_TIRE_TEMP7_2_BYTE_OFS
// #define CAN_TIRE_TEMP8_2_BYTE_OFS		CAN_FL_TIRE_TEMP8_2_BYTE_OFS
//
// #define CAN_TIRE_TEMP_MULT				CAN_FL_TIRE_TEMP_MULT
// #define CAN_TIRE_TEMP_DIV				CAN_FL_TIRE_TEMP_DIV
// #define CAN_TIRE_TEMP_OFS				CAN_FL_TIRE_TEMP_OFS
// #define CAN_TIRE_TEMP_VALUE_TYPE			CAN_FL_TIRE_TEMP_VALUE_TYPE
//#el
//#if WHEEL == FRONT_RIGHT
///* FRAME PARAETERS */
// #define CAN_TIRE_TEMP_1_ID				CAN_FR_TIRE_TEMP_1_ID
// #define CAN_TIRE_TEMP_2_ID				CAN_FR_TIRE_TEMP_2_ID
// #define CAN_TIRE_TEMP_PERIOD			CAN_FR_TIRE_TEMP_PERIOD
// #define CAN_TIRE_TEMP_DLC				CAN_FR_TIRE_TEMP_DLC
///* DATA PARAMETERS */
// #define CAN_TIRE_TEMP1_1_BYTE_OFS		CAN_FR_TIRE_TEMP1_1_BYTE_OFS
// #define CAN_TIRE_TEMP2_1_BYTE_OFS		CAN_FR_TIRE_TEMP2_1_BYTE_OFS
// #define CAN_TIRE_TEMP3_1_BYTE_OFS		CAN_FR_TIRE_TEMP3_1_BYTE_OFS
// #define CAN_TIRE_TEMP4_1_BYTE_OFS		CAN_FR_TIRE_TEMP4_1_BYTE_OFS
// #define CAN_TIRE_TEMP5_1_BYTE_OFS		CAN_FR_TIRE_TEMP5_1_BYTE_OFS
// #define CAN_TIRE_TEMP6_1_BYTE_OFS		CAN_FR_TIRE_TEMP6_1_BYTE_OFS
// #define CAN_TIRE_TEMP7_1_BYTE_OFS		CAN_FR_TIRE_TEMP7_1_BYTE_OFS
// #define CAN_TIRE_TEMP8_1_BYTE_OFS		CAN_FR_TIRE_TEMP8_1_BYTE_OFS
//
// #define CAN_TIRE_TEMP1_2_BYTE_OFS		CAN_FR_TIRE_TEMP1_2_BYTE_OFS
// #define CAN_TIRE_TEMP2_2_BYTE_OFS		CAN_FR_TIRE_TEMP2_2_BYTE_OFS
// #define CAN_TIRE_TEMP3_2_BYTE_OFS		CAN_FR_TIRE_TEMP3_2_BYTE_OFS
// #define CAN_TIRE_TEMP4_2_BYTE_OFS		CAN_FR_TIRE_TEMP4_2_BYTE_OFS
// #define CAN_TIRE_TEMP5_2_BYTE_OFS		CAN_FR_TIRE_TEMP5_2_BYTE_OFS
// #define CAN_TIRE_TEMP6_2_BYTE_OFS		CAN_FR_TIRE_TEMP6_2_BYTE_OFS
// #define CAN_TIRE_TEMP7_2_BYTE_OFS		CAN_FR_TIRE_TEMP7_2_BYTE_OFS
// #define CAN_TIRE_TEMP8_2_BYTE_OFS		CAN_FR_TIRE_TEMP8_2_BYTE_OFS
//
// #define CAN_TIRE_TEMP_MULT				CAN_FR_TIRE_TEMP_MULT
// #define CAN_TIRE_TEMP_DIV				CAN_FR_TIRE_TEMP_DIV
// #define CAN_TIRE_TEMP_OFS				CAN_FR_TIRE_TEMP_OFS
// #define CAN_TIRE_TEMP_VALUE_TYPE		CAN_FR_TIRE_TEMP_VALUE_TYPE

//#el
//#if WHEEL == REAR_LEFT
///* FRAME PARAETERS */
// #define CAN_TIRE_TEMP_1_ID				CAN_RL_TIRE_TEMP_1_ID
// #define CAN_TIRE_TEMP_2_ID				CAN_RL_TIRE_TEMP_2_ID
// #define CAN_TIRE_TEMP_PERIOD			CAN_RL_TIRE_TEMP_PERIOD
// #define CAN_TIRE_TEMP_DLC				CAN_RL_TIRE_TEMP_DLC
///* DATA PARAMETERS */
// #define CAN_TIRE_TEMP1_1_BYTE_OFS		CAN_RL_TIRE_TEMP1_1_BYTE_OFS
// #define CAN_TIRE_TEMP2_1_BYTE_OFS		CAN_RL_TIRE_TEMP2_1_BYTE_OFS
// #define CAN_TIRE_TEMP3_1_BYTE_OFS		CAN_RL_TIRE_TEMP3_1_BYTE_OFS
// #define CAN_TIRE_TEMP4_1_BYTE_OFS		CAN_RL_TIRE_TEMP4_1_BYTE_OFS
// #define CAN_TIRE_TEMP5_1_BYTE_OFS		CAN_RL_TIRE_TEMP5_1_BYTE_OFS
// #define CAN_TIRE_TEMP6_1_BYTE_OFS		CAN_RL_TIRE_TEMP6_1_BYTE_OFS
// #define CAN_TIRE_TEMP7_1_BYTE_OFS		CAN_RL_TIRE_TEMP7_1_BYTE_OFS
// #define CAN_TIRE_TEMP8_1_BYTE_OFS		CAN_RL_TIRE_TEMP8_1_BYTE_OFS
//
// #define CAN_TIRE_TEMP1_2_BYTE_OFS		CAN_RL_TIRE_TEMP1_2_BYTE_OFS
// #define CAN_TIRE_TEMP2_2_BYTE_OFS		CAN_RL_TIRE_TEMP2_2_BYTE_OFS
// #define CAN_TIRE_TEMP3_2_BYTE_OFS		CAN_RL_TIRE_TEMP3_2_BYTE_OFS
// #define CAN_TIRE_TEMP4_2_BYTE_OFS		CAN_RL_TIRE_TEMP4_2_BYTE_OFS
// #define CAN_TIRE_TEMP5_2_BYTE_OFS		CAN_RL_TIRE_TEMP5_2_BYTE_OFS
// #define CAN_TIRE_TEMP6_2_BYTE_OFS		CAN_RL_TIRE_TEMP6_2_BYTE_OFS
// #define CAN_TIRE_TEMP7_2_BYTE_OFS		CAN_RL_TIRE_TEMP7_2_BYTE_OFS
// #define CAN_TIRE_TEMP8_2_BYTE_OFS		CAN_RL_TIRE_TEMP8_2_BYTE_OFS
//
// #define CAN_TIRE_TEMP_MULT				CAN_RL_TIRE_TEMP_MULT
// #define CAN_TIRE_TEMP_DIV				CAN_RL_TIRE_TEMP_DIV
// #define CAN_TIRE_TEMP_OFS				CAN_RL_TIRE_TEMP_OFS
// #define CAN_TIRE_TEMP_VALUE_TYPE		CAN_RL_TIRE_TEMP_VALUE_TYPE
//
//#el
#if WHEEL == REAR_RIGHT
/* FRAME PARAETERS */
 #define CAN_TIRE_TEMP_1_ID				CAN_RR_TIRE_TEMP_1_ID
 #define CAN_TIRE_TEMP_2_ID				CAN_RR_TIRE_TEMP_2_ID
 #define CAN_TIRE_TEMP_PERIOD			CAN_RR_TIRE_TEMP_PERIOD
 #define CAN_TIRE_TEMP_DLC				CAN_RR_TIRE_TEMP_DLC
/* DATA PARAMETERS */
 #define CAN_TIRE_TEMP1_1_BYTE_OFS		CAN_RR_TIRE_TEMP1_1_BYTE_OFS
 #define CAN_TIRE_TEMP2_1_BYTE_OFS		CAN_RR_TIRE_TEMP2_1_BYTE_OFS
 #define CAN_TIRE_TEMP3_1_BYTE_OFS		CAN_RR_TIRE_TEMP3_1_BYTE_OFS
 #define CAN_TIRE_TEMP4_1_BYTE_OFS		CAN_RR_TIRE_TEMP4_1_BYTE_OFS
 #define CAN_TIRE_TEMP5_1_BYTE_OFS		CAN_RR_TIRE_TEMP5_1_BYTE_OFS
 #define CAN_TIRE_TEMP6_1_BYTE_OFS		CAN_RR_TIRE_TEMP6_1_BYTE_OFS
 #define CAN_TIRE_TEMP7_1_BYTE_OFS		CAN_RR_TIRE_TEMP7_1_BYTE_OFS
 #define CAN_TIRE_TEMP8_1_BYTE_OFS		CAN_RR_TIRE_TEMP8_1_BYTE_OFS

 #define CAN_TIRE_TEMP1_2_BYTE_OFS		CAN_RR_TIRE_TEMP1_2_BYTE_OFS
 #define CAN_TIRE_TEMP2_2_BYTE_OFS		CAN_RR_TIRE_TEMP2_2_BYTE_OFS
 #define CAN_TIRE_TEMP3_2_BYTE_OFS		CAN_RR_TIRE_TEMP3_2_BYTE_OFS
 #define CAN_TIRE_TEMP4_2_BYTE_OFS		CAN_RR_TIRE_TEMP4_2_BYTE_OFS
 #define CAN_TIRE_TEMP5_2_BYTE_OFS		CAN_RR_TIRE_TEMP5_2_BYTE_OFS
 #define CAN_TIRE_TEMP6_2_BYTE_OFS		CAN_RR_TIRE_TEMP6_2_BYTE_OFS
 #define CAN_TIRE_TEMP7_2_BYTE_OFS		CAN_RR_TIRE_TEMP7_2_BYTE_OFS
 #define CAN_TIRE_TEMP8_2_BYTE_OFS		CAN_RR_TIRE_TEMP8_2_BYTE_OFS

 #define CAN_TIRE_TEMP_MULT				CAN_RR_TIRE_TEMP_MULT
 #define CAN_TIRE_TEMP_DIV				CAN_RR_TIRE_TEMP_DIV
 #define CAN_TIRE_TEMP_OFS				CAN_RR_TIRE_TEMP_OFS
 #define CAN_TIRE_TEMP_VALUE_TYPE		CAN_RR_TIRE_TEMP_VALUE_TYPE

#endif

static lib_can_frame_t *f_tire_temp1;
static lib_can_data_t  *d_tire_temp1;

static lib_can_frame_t *f_tire_temp2;
static lib_can_data_t  *d_tire_temp2;

static lib_can_frame_t *f_tire_temp3;
static lib_can_data_t  *d_tire_temp3;

static lib_can_frame_t *f_tire_temp4;
static lib_can_data_t  *d_tire_temp4;

static lib_can_frame_t *f_tire_temp5;
static lib_can_data_t  *d_tire_temp5;

static lib_can_frame_t *f_tire_temp6;
static lib_can_data_t  *d_tire_temp6;

static lib_can_frame_t *f_tire_temp7;
static lib_can_data_t  *d_tire_temp7;

static lib_can_frame_t *f_tire_temp8;
static lib_can_data_t  *d_tire_temp8;

static lib_can_frame_t *f_tire_temp9;
static lib_can_data_t  *d_tire_temp9;

static lib_can_frame_t *f_tire_temp10;
static lib_can_data_t  *d_tire_temp10;

static lib_can_frame_t *f_tire_temp11;
static lib_can_data_t  *d_tire_temp11;

static lib_can_frame_t *f_tire_temp12;
static lib_can_data_t  *d_tire_temp12;

static lib_can_frame_t *f_tire_temp13;
static lib_can_data_t  *d_tire_temp13;

static lib_can_frame_t *f_tire_temp14;
static lib_can_data_t  *d_tire_temp14;

static lib_can_frame_t *f_tire_temp15;
static lib_can_data_t  *d_tire_temp15;

static lib_can_frame_t *f_tire_temp16;
static lib_can_data_t  *d_tire_temp16;

uint32_t compensatedVal_m_tire_temp;
//static uint16_t m_tire_temp;
uint16_t TireObjectTemperature;
uint16_t TireAmbientTemperature;
uint16_t TireEmissivity;
static uint8_t isExist;
volatile static float _temp_frame[4][16];
static uint8_t pixel_temp[16];

void calcAverageTemps(void)
{
	for(uint8_t j = 0; j < 16; ++j) {
		float avg_f = 0;
		for(uint8_t i = 0; i < 4; ++i) {
			avg_f += _temp_frame[i][j];
		}

		int32_t avg = (int32_t)(avg_f + 0.5f);
		avg = clamp_s(avg, 0, UINT8_MAX * 4);
		pixel_temp[j] = (uint8_t)divRound((uint32_t)avg, 4);
	}
}

retcode_t m_tire_temp_init(void)
{
    MLX90614_SetAddress(MLX90614_DEFAULT_ADDRESS,MLX90614_SECOND_ADDRESS);
	if(init())
	{
		isExist = 1;
		f_tire_temp1 = lib_can_register_frame_tx(CAN_TIRE_TEMP_1_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp1 = lib_can_register_data(f_tire_temp1, CAN_TIRE_TEMP1_1_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp2 = lib_can_register_frame_tx(CAN_TIRE_TEMP_1_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp2 = lib_can_register_data(f_tire_temp2, CAN_TIRE_TEMP2_1_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp3 = lib_can_register_frame_tx(CAN_TIRE_TEMP_1_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp3 = lib_can_register_data(f_tire_temp3, CAN_TIRE_TEMP3_1_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp4 = lib_can_register_frame_tx(CAN_TIRE_TEMP_1_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp4 = lib_can_register_data(f_tire_temp4, CAN_TIRE_TEMP4_1_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp5 = lib_can_register_frame_tx(CAN_TIRE_TEMP_1_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp5 = lib_can_register_data(f_tire_temp5, CAN_TIRE_TEMP5_1_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp6 = lib_can_register_frame_tx(CAN_TIRE_TEMP_1_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp6 = lib_can_register_data(f_tire_temp6, CAN_TIRE_TEMP6_1_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp7 = lib_can_register_frame_tx(CAN_TIRE_TEMP_1_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp7 = lib_can_register_data(f_tire_temp7, CAN_TIRE_TEMP7_1_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp8 = lib_can_register_frame_tx(CAN_TIRE_TEMP_1_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp8 = lib_can_register_data(f_tire_temp8, CAN_TIRE_TEMP8_1_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp9 = lib_can_register_frame_tx(CAN_TIRE_TEMP_2_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp9 = lib_can_register_data(f_tire_temp9, CAN_TIRE_TEMP1_2_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp10 = lib_can_register_frame_tx(CAN_TIRE_TEMP_2_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp10 = lib_can_register_data(f_tire_temp10, CAN_TIRE_TEMP2_2_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp11 = lib_can_register_frame_tx(CAN_TIRE_TEMP_2_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp11 = lib_can_register_data(f_tire_temp11, CAN_TIRE_TEMP3_2_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp12 = lib_can_register_frame_tx(CAN_TIRE_TEMP_2_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp12 = lib_can_register_data(f_tire_temp12, CAN_TIRE_TEMP4_2_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp13 = lib_can_register_frame_tx(CAN_TIRE_TEMP_2_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp13 = lib_can_register_data(f_tire_temp13, CAN_TIRE_TEMP5_2_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp14 = lib_can_register_frame_tx(CAN_TIRE_TEMP_2_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp14 = lib_can_register_data(f_tire_temp14, CAN_TIRE_TEMP6_2_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp15 = lib_can_register_frame_tx(CAN_TIRE_TEMP_2_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp15 = lib_can_register_data(f_tire_temp15, CAN_TIRE_TEMP7_2_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);

		f_tire_temp16 = lib_can_register_frame_tx(CAN_TIRE_TEMP_2_ID, CAN_TIRE_TEMP_PERIOD , CAN_TIRE_TEMP_DLC);
		d_tire_temp16 = lib_can_register_data(f_tire_temp16, CAN_TIRE_TEMP8_2_BYTE_OFS, CAN_TIRE_TEMP_MULT, CAN_TIRE_TEMP_DIV, CAN_TIRE_TEMP_OFS, CAN_TIRE_TEMP_VALUE_TYPE, _uint16_t);
	} else
		isExist = 0;

    return retcode_ok;
}

retcode_t m_tire_temp_deinit(void)
{
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp1), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp1), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp2), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp2), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp3), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp3), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp4), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp4), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp5), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp5), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp6), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp6), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp7), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp7), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp8), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp8), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp9), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp9), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp10), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp10), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp11), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp11), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp12), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp12), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp13), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp13), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp14), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp14), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp15), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp15), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_data(d_tire_temp16), retcode_ok);
	APP_RETURN_ERR(lib_can_unregister_frame(f_tire_temp16), retcode_ok);
	return retcode_ok;
}

void m_tire_temp_update(void)
{
	if(isExist) {
	//	MLX90614_ReadAmbientTemperature(&TireAmbientTemperature,MLX90614_SECOND_ADDRESS);
	//    MLX90614_ReadObjectTemperature(&TireObjectTemperature,MLX90614_SECOND_ADDRESS);
	//    MLX90614_GetEmissivity(&TireEmissivity,MLX90614_SECOND_ADDRESS);
		//m_tire_temp = TireObjectTemperature;
		getFullFrame((float *)_temp_frame);
		//startMeasurment();
		calcAverageTemps();
		lib_can_data_set(d_tire_temp1, &pixel_temp[0]);
		lib_can_data_set(d_tire_temp2, &pixel_temp[1]);
		lib_can_data_set(d_tire_temp3, &pixel_temp[2]);
		lib_can_data_set(d_tire_temp4, &pixel_temp[3]);
		 		lib_can_data_set(d_tire_temp5, &pixel_temp[4]);
		lib_can_data_set(d_tire_temp6, &pixel_temp[5]);
		lib_can_data_set(d_tire_temp7, &pixel_temp[6]);
		lib_can_data_set(d_tire_temp8, &pixel_temp[7]);
		lib_can_data_set(d_tire_temp9, &pixel_temp[8]);
		lib_can_data_set(d_tire_temp10, &pixel_temp[9]);
		lib_can_data_set(d_tire_temp11, &pixel_temp[10]);
		lib_can_data_set(d_tire_temp12, &pixel_temp[11]);
		lib_can_data_set(d_tire_temp13, &pixel_temp[12]);
		lib_can_data_set(d_tire_temp14, &pixel_temp[13]);
		lib_can_data_set(d_tire_temp15, &pixel_temp[14]);
		lib_can_data_set(d_tire_temp16, &pixel_temp[15]);
	}
}


#endif /* CONFIG_TIRE_TEMP_ENABLED */
