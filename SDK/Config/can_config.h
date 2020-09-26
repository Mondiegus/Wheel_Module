
/* LICENSE */

#ifndef CONFIG_CAN_H_
#define CONFIG_CAN_H_

#if defined(STM32F042x6)
#include "stm32f0xx_hal.h"
#elif defined(STM32F303xC)
#include "stm32f3xx_hal.h"
#elif defined(STM32F767xI)
#include "stm32f7xx_hal.h"
#endif

/* GMU DATA: GEAR, CLUTCH */
#define CAN_GMU_ID				0x080
#define CAN_GMU_PERIOD			10
#define CAN_GMU_DLC				2

#define CAN_STOP_PMU_ID			0x710
#define CAN_STOP_PMU_PERIOD		10
#define CAN_STOP_PMU_DLC		8

#define CAN_STOP_ID				0x102
#define CAN_STOP_PERIOD			20
#define CAN_STOP_DLC			1

#define CAN_FACC_ID				0x100
#define CAN_FACC_PERIOD			10
#define CAN_FACC_DLC			6

#define CAN_FGYR_ID				0x101
#define CAN_FGYR_PERIOD			10
#define CAN_FGYR_DLC			6

#define CAN_RACC_ID				0x180
#define CAN_RACC_PERIOD			10
#define CAN_RACC_DLC			6

#define CAN_RGYR_ID				0x181
#define CAN_RGYR_PERIOD			10
#define CAN_RGYR_DLC			6

//---------------------Front Left------------------------

#define CAN_FL_SHOCK_ABS_ID						0x200
#define CAN_FL_SHOCK_ABS_PERIOD					30
#define CAN_FL_SHOCK_ABS_DLC					6
#define CAN_FL_SHOCK_ABS_BYTE_OFS				0
#define CAN_FL_SHOCK_ABS_MULT					1
#define CAN_FL_SHOCK_ABS_DIV					1
#define CAN_FL_SHOCK_ABS_OFS					0
#define CAN_FL_SHOCK_ABS_VALUE_TYPE				_int16_t

#define CAN_FL_BRAKE_DISC_TEMP_ID				0x200
#define CAN_FL_BRAKE_DISC_TEMP_PERIOD			30
#define CAN_FL_BRAKE_DISC_TEMP_DLC				6
#define CAN_FL_BRAKE_DISC_TEMP_BYTE_OFS			2
#define CAN_FL_BRAKE_DISC_TEMP_MULT				1
#define CAN_FL_BRAKE_DISC_TEMP_DIV				1
#define CAN_FL_BRAKE_DISC_TEMP_OFS				0
#define CAN_FL_BRAKE_DISC_TEMP_VALUE_TYPE		_int16_t

#define CAN_FL_WHEEL_SPEED_ID					0x200
#define CAN_FL_WHEEL_SPEED_PERIOD				30
#define CAN_FL_WHEEL_SPEED_DLC					6
#define CAN_FL_WHEEL_SPEED_BYTE_OFS				4
#define CAN_FL_WHEEL_SPEED_MULT					1
#define CAN_FL_WHEEL_SPEED_DIV					1
#define CAN_FL_WHEEL_SPEED_OFS					0
#define CAN_FL_WHEEL_SPEED_VALUE_TYPE			_int16_t

#define CAN_FL_TIRE_TEMP_1_ID					0x201
#define CAN_FL_TIRE_TEMP_DLC					8
#define CAN_FL_TIRE_TEMP1_1_BYTE_OFS			0
#define CAN_FL_TIRE_TEMP2_1_BYTE_OFS			1
#define CAN_FL_TIRE_TEMP3_1_BYTE_OFS			2
#define CAN_FL_TIRE_TEMP4_1_BYTE_OFS			3
#define CAN_FL_TIRE_TEMP5_1_BYTE_OFS			4
#define CAN_FL_TIRE_TEMP6_1_BYTE_OFS			5
#define CAN_FL_TIRE_TEMP7_1_BYTE_OFS			6
#define CAN_FL_TIRE_TEMP8_1_BYTE_OFS			7
#define CAN_FL_TIRE_TEMP_2_ID					0x202
#define CAN_FL_TIRE_TEMP1_2_BYTE_OFS			0
#define CAN_FL_TIRE_TEMP2_2_BYTE_OFS			1
#define CAN_FL_TIRE_TEMP3_2_BYTE_OFS			2
#define CAN_FL_TIRE_TEMP4_2_BYTE_OFS			3
#define CAN_FL_TIRE_TEMP5_2_BYTE_OFS			4
#define CAN_FL_TIRE_TEMP6_2_BYTE_OFS			5
#define CAN_FL_TIRE_TEMP7_2_BYTE_OFS			6
#define CAN_FL_TIRE_TEMP8_2_BYTE_OFS			7

#define CAN_FL_TIRE_TEMP_MULT					1
#define CAN_FL_TIRE_TEMP_DIV					1
#define CAN_FL_TIRE_TEMP_OFS					0
#define CAN_FL_TIRE_TEMP_VALUE_TYPE				_int8_t
#define CAN_FL_TIRE_TEMP_PERIOD					30

//---------------------Front Right------------------------

#define CAN_FR_SHOCK_ABS_ID						0x280
#define CAN_FR_SHOCK_ABS_PERIOD					30
#define CAN_FR_SHOCK_ABS_DLC					6
#define CAN_FR_SHOCK_ABS_BYTE_OFS				0
#define CAN_FR_SHOCK_ABS_MULT					1
#define CAN_FR_SHOCK_ABS_DIV					1
#define CAN_FR_SHOCK_ABS_OFS					0
#define CAN_FR_SHOCK_ABS_VALUE_TYPE				_int16_t

#define CAN_FR_BRAKE_DISC_TEMP_ID				0x280
#define CAN_FR_BRAKE_DISC_TEMP_PERIOD			30
#define CAN_FR_BRAKE_DISC_TEMP_DLC				6
#define CAN_FR_BRAKE_DISC_TEMP_BYTE_OFS			2
#define CAN_FR_BRAKE_DISC_TEMP_MULT				1
#define CAN_FR_BRAKE_DISC_TEMP_DIV				1
#define CAN_FR_BRAKE_DISC_TEMP_OFS				0
#define CAN_FR_BRAKE_DISC_TEMP_VALUE_TYPE		_int16_t

#define CAN_FR_WHEEL_SPEED_ID					0x280
#define CAN_FR_WHEEL_SPEED_PERIOD				30
#define CAN_FR_WHEEL_SPEED_DLC					6
#define CAN_FR_WHEEL_SPEED_BYTE_OFS				4
#define CAN_FR_WHEEL_SPEED_MULT					1
#define CAN_FR_WHEEL_SPEED_DIV					1
#define CAN_FR_WHEEL_SPEED_OFS					0
#define CAN_FR_WHEEL_SPEED_VALUE_TYPE			_int16_t

#define CAN_FR_TIRE_TEMP_1_ID					0x281
#define CAN_FR_TIRE_TEMP_DLC					8
#define CAN_FR_TIRE_TEMP1_1_BYTE_OFS			0
#define CAN_FR_TIRE_TEMP2_1_BYTE_OFS			1
#define CAN_FR_TIRE_TEMP3_1_BYTE_OFS			2
#define CAN_FR_TIRE_TEMP4_1_BYTE_OFS			3
#define CAN_FR_TIRE_TEMP5_1_BYTE_OFS			4
#define CAN_FR_TIRE_TEMP6_1_BYTE_OFS			5
#define CAN_FR_TIRE_TEMP7_1_BYTE_OFS			6
#define CAN_FR_TIRE_TEMP8_1_BYTE_OFS			7
#define CAN_FR_TIRE_TEMP_2_ID					0x282
#define CAN_FR_TIRE_TEMP1_2_BYTE_OFS			0
#define CAN_FR_TIRE_TEMP2_2_BYTE_OFS			1
#define CAN_FR_TIRE_TEMP3_2_BYTE_OFS			2
#define CAN_FR_TIRE_TEMP4_2_BYTE_OFS			3
#define CAN_FR_TIRE_TEMP5_2_BYTE_OFS			4
#define CAN_FR_TIRE_TEMP6_2_BYTE_OFS			5
#define CAN_FR_TIRE_TEMP7_2_BYTE_OFS			6
#define CAN_FR_TIRE_TEMP8_2_BYTE_OFS			7

#define CAN_FR_TIRE_TEMP_MULT					1
#define CAN_FR_TIRE_TEMP_DIV					1
#define CAN_FR_TIRE_TEMP_OFS					0
#define CAN_FR_TIRE_TEMP_VALUE_TYPE				_int8_t
#define CAN_FR_TIRE_TEMP_PERIOD					30

//---------------------Rear left------------------------

#define CAN_RL_SHOCK_ABS_ID						0x300
#define CAN_RL_SHOCK_ABS_PERIOD					30
#define CAN_RL_SHOCK_ABS_DLC					6
#define CAN_RL_SHOCK_ABS_BYTE_OFS				0
#define CAN_RL_SHOCK_ABS_MULT					1
#define CAN_RL_SHOCK_ABS_DIV					1
#define CAN_RL_SHOCK_ABS_OFS					0
#define CAN_RL_SHOCK_ABS_VALUE_TYPE				_int16_t

#define CAN_RL_BRAKE_DISC_TEMP_ID				0x300
#define CAN_RL_BRAKE_DISC_TEMP_PERIOD			30
#define CAN_RL_BRAKE_DISC_TEMP_DLC				6
#define CAN_RL_BRAKE_DISC_TEMP_BYTE_OFS			2
#define CAN_RL_BRAKE_DISC_TEMP_MULT				1
#define CAN_RL_BRAKE_DISC_TEMP_DIV				1
#define CAN_RL_BRAKE_DISC_TEMP_OFS				0
#define CAN_RL_BRAKE_DISC_TEMP_VALUE_TYPE		_int16_t

#define CAN_RL_WHEEL_SPEED_ID					0x300
#define CAN_RL_WHEEL_SPEED_PERIOD				30
#define CAN_RL_WHEEL_SPEED_DLC					6
#define CAN_RL_WHEEL_SPEED_BYTE_OFS				4
#define CAN_RL_WHEEL_SPEED_MULT					1
#define CAN_RL_WHEEL_SPEED_DIV					1
#define CAN_RL_WHEEL_SPEED_OFS					0
#define CAN_RL_WHEEL_SPEED_VALUE_TYPE			_int16_t

#define CAN_RL_TIRE_TEMP_1_ID					0x301
#define CAN_RL_TIRE_TEMP_DLC					8
#define CAN_RL_TIRE_TEMP1_1_BYTE_OFS			0
#define CAN_RL_TIRE_TEMP2_1_BYTE_OFS			1
#define CAN_RL_TIRE_TEMP3_1_BYTE_OFS			2
#define CAN_RL_TIRE_TEMP4_1_BYTE_OFS			3
#define CAN_RL_TIRE_TEMP5_1_BYTE_OFS			4
#define CAN_RL_TIRE_TEMP6_1_BYTE_OFS			5
#define CAN_RL_TIRE_TEMP7_1_BYTE_OFS			6
#define CAN_RL_TIRE_TEMP8_1_BYTE_OFS			7
#define CAN_RL_TIRE_TEMP_2_ID					0x302
#define CAN_RL_TIRE_TEMP1_2_BYTE_OFS			0
#define CAN_RL_TIRE_TEMP2_2_BYTE_OFS			1
#define CAN_RL_TIRE_TEMP3_2_BYTE_OFS			2
#define CAN_RL_TIRE_TEMP4_2_BYTE_OFS			3
#define CAN_RL_TIRE_TEMP5_2_BYTE_OFS			4
#define CAN_RL_TIRE_TEMP6_2_BYTE_OFS			5
#define CAN_RL_TIRE_TEMP7_2_BYTE_OFS			6
#define CAN_RL_TIRE_TEMP8_2_BYTE_OFS			7

#define CAN_RL_TIRE_TEMP_MULT					1
#define CAN_RL_TIRE_TEMP_DIV					1
#define CAN_RL_TIRE_TEMP_OFS					0
#define CAN_RL_TIRE_TEMP_VALUE_TYPE				_int8_t
#define CAN_RL_TIRE_TEMP_PERIOD					30

//---------------------Rear Right------------------------

#define CAN_RR_SHOCK_ABS_ID						0x380
#define CAN_RR_SHOCK_ABS_PERIOD					30
#define CAN_RR_SHOCK_ABS_DLC					6
#define CAN_RR_SHOCK_ABS_BYTE_OFS				0
#define CAN_RR_SHOCK_ABS_MULT					1
#define CAN_RR_SHOCK_ABS_DIV					1
#define CAN_RR_SHOCK_ABS_OFS					0
#define CAN_RR_SHOCK_ABS_VALUE_TYPE				_int16_t

#define CAN_RR_BRAKE_DISC_TEMP_ID				0x380
#define CAN_RR_BRAKE_DISC_TEMP_PERIOD			30
#define CAN_RR_BRAKE_DISC_TEMP_DLC				6
#define CAN_RR_BRAKE_DISC_TEMP_BYTE_OFS			2
#define CAN_RR_BRAKE_DISC_TEMP_MULT				1
#define CAN_RR_BRAKE_DISC_TEMP_DIV				1
#define CAN_RR_BRAKE_DISC_TEMP_OFS				0
#define CAN_RR_BRAKE_DISC_TEMP_VALUE_TYPE		_int16_t

#define CAN_RR_WHEEL_SPEED_ID					0x380
#define CAN_RR_WHEEL_SPEED_PERIOD				30
#define CAN_RR_WHEEL_SPEED_DLC					6
#define CAN_RR_WHEEL_SPEED_BYTE_OFS				4
#define CAN_RR_WHEEL_SPEED_MULT					1
#define CAN_RR_WHEEL_SPEED_DIV					1
#define CAN_RR_WHEEL_SPEED_OFS					0
#define CAN_RR_WHEEL_SPEED_VALUE_TYPE			_int16_t

#define CAN_RR_TIRE_TEMP_1_ID					0x381
#define CAN_RR_TIRE_TEMP_DLC					8
#define CAN_RR_TIRE_TEMP1_1_BYTE_OFS			0
#define CAN_RR_TIRE_TEMP2_1_BYTE_OFS			1
#define CAN_RR_TIRE_TEMP3_1_BYTE_OFS			2
#define CAN_RR_TIRE_TEMP4_1_BYTE_OFS			3
#define CAN_RR_TIRE_TEMP5_1_BYTE_OFS			4
#define CAN_RR_TIRE_TEMP6_1_BYTE_OFS			5
#define CAN_RR_TIRE_TEMP7_1_BYTE_OFS			6
#define CAN_RR_TIRE_TEMP8_1_BYTE_OFS			7
#define CAN_RR_TIRE_TEMP_2_ID					0x382
#define CAN_RR_TIRE_TEMP1_2_BYTE_OFS			0
#define CAN_RR_TIRE_TEMP2_2_BYTE_OFS			1
#define CAN_RR_TIRE_TEMP3_2_BYTE_OFS			2
#define CAN_RR_TIRE_TEMP4_2_BYTE_OFS			3
#define CAN_RR_TIRE_TEMP5_2_BYTE_OFS			4
#define CAN_RR_TIRE_TEMP6_2_BYTE_OFS			5
#define CAN_RR_TIRE_TEMP7_2_BYTE_OFS			6
#define CAN_RR_TIRE_TEMP8_2_BYTE_OFS			7

#define CAN_RR_TIRE_TEMP_MULT					1
#define CAN_RR_TIRE_TEMP_DIV					1
#define CAN_RR_TIRE_TEMP_OFS					0
#define CAN_RR_TIRE_TEMP_VALUE_TYPE				_int8_t
#define CAN_RR_TIRE_TEMP_PERIOD					30

/* EMU0 DATA: RPM, TPS, IAT, MAP, INJPW */
#define CAN_EMU0_ID						0x600
#define CAN_EMU0_PERIOD					50
#define CAN_EMU0_DLC					8

/* EMU2 DATA: VSPD, BARO, OILT, OILP, PUELP, CLT */
#define CAN_EMU2_ID						0x602
#define CAN_EMU2_PERIOD					50
#define CAN_EMU2_DLC					8

/* EMU2 DATA: AIN1, AIN2, AIN3, AIN4 */
#define CAN_EMU1_ID						0x601
#define CAN_EMU1_PERIOD					50
#define CAN_EMU1_DLC					8

/* PMU0 DATA: PMU Stat, Total Curr, Batt Vltg, Board Temp L, Board Temp R, Flash Temp*/
#define CAN_PMU0_ID						0x700
#define CAN_PMU0_PERIOD					50
#define CAN_PMU0_DLC					8

#define CAN_GEAR_BYTE_OFS				0
#define CAN_GEAR_MULT					1
#define CAN_GEAR_DIV					1
#define CAN_GEAR_OFS					0
#define CAN_GEAR_VALUE_TYPE				_uint8_t

#define CAN_CLUTCH_BYTE_OFS				1
#define CAN_CLUTCH_MULT					1
#define CAN_CLUTCH_DIV					1
#define CAN_CLUTCH_OFS					0
#define CAN_CLUTCH_VALUE_TYPE			_uint8_t

#define CAN_STOP_PEDAL_BYTE_OFS			0
#define CAN_STOP_PEDAL_MULT				1
#define CAN_STOP_PEDAL_DIV				1
#define CAN_STOP_PEDAL_OFS				0
#define CAN_STOP_PEDAL_VALUE_TYPE		_uint8_t

#define CAN_FACC_X_BYTE_OFS		0
#define CAN_FACC_X_MULT			1
#define CAN_FACC_X_DIV			100
#define CAN_FACC_X_OFS			0
#define CAN_FACC_X_VALUE_TYPE	_int16_t

#define CAN_FACC_Y_BYTE_OFS		2
#define CAN_FACC_Y_MULT			1
#define CAN_FACC_Y_DIV			100
#define CAN_FACC_Y_OFS			0
#define CAN_FACC_Y_VALUE_TYPE	_int16_t

#define CAN_FACC_Z_BYTE_OFS		4
#define CAN_FACC_Z_MULT			1
#define CAN_FACC_Z_DIV			100
#define CAN_FACC_Z_OFS			0
#define CAN_FACC_Z_VALUE_TYPE	_int16_t

#define CAN_FGYR_X_BYTE_OFS		0
#define CAN_FGYR_X_MULT			1
#define CAN_FGYR_X_DIV			10
#define CAN_FGYR_X_OFS			0
#define CAN_FGYR_X_VALUE_TYPE	_int16_t

#define CAN_FGYR_Y_BYTE_OFS		2
#define CAN_FGYR_Y_MULT			1
#define CAN_FGYR_Y_DIV			10
#define CAN_FGYR_Y_OFS			0
#define CAN_FGYR_Y_VALUE_TYPE	_int16_t

#define CAN_FGYR_Z_BYTE_OFS		4
#define CAN_FGYR_Z_MULT			1
#define CAN_FGYR_Z_DIV			10
#define CAN_FGYR_Z_OFS			0
#define CAN_FGYR_Z_VALUE_TYPE	_int16_t

#define CAN_RACC_X_BYTE_OFS		0
#define CAN_RACC_X_MULT			1
#define CAN_RACC_X_DIV			100
#define CAN_RACC_X_OFS			0
#define CAN_RACC_X_VALUE_TYPE	_int16_t

#define CAN_RACC_Y_BYTE_OFS		2
#define CAN_RACC_Y_MULT			1
#define CAN_RACC_Y_DIV			100
#define CAN_RACC_Y_OFS			0
#define CAN_RACC_Y_VALUE_TYPE	_int16_t

#define CAN_RACC_Z_BYTE_OFS		4
#define CAN_RACC_Z_MULT			1
#define CAN_RACC_Z_DIV			100
#define CAN_RACC_Z_OFS			0
#define CAN_RACC_Z_VALUE_TYPE	_int16_t

#define CAN_RGYR_X_BYTE_OFS		0
#define CAN_RGYR_X_MULT			1
#define CAN_RGYR_X_DIV			10
#define CAN_RGYR_X_OFS			0
#define CAN_RGYR_X_VALUE_TYPE	_int16_t

#define CAN_RGYR_Y_BYTE_OFS		2
#define CAN_RGYR_Y_MULT			1
#define CAN_RGYR_Y_DIV			10
#define CAN_RGYR_Y_OFS			0
#define CAN_RGYR_Y_VALUE_TYPE	_int16_t

#define CAN_RGYR_Z_BYTE_OFS		4
#define CAN_RGYR_Z_MULT			1
#define CAN_RGYR_Z_DIV			10
#define CAN_RGYR_Z_OFS			0
#define CAN_RGYR_Z_VALUE_TYPE	_int16_t

/*
#define CAN_RR_TIRE_TEMP_BYTE_OFS		4
#define CAN_RR_TIRE_TEMP_MULT			1
#define CAN_RR_TIRE_TEMP_DIV			1
#define CAN_RR_TIRE_TEMP_OFS			0
#define CAN_RR_TIRE_TEMP_VALUE_TYPE	_int16_t

#define CAN_RL_TIRE_TEMP_BYTE_OFS		6
#define CAN_RL_TIRE_TEMP_MULT			1
#define CAN_RL_TIRE_TEMP_DIV			1
#define CAN_RL_TIRE_TEMP_OFS			0
#define CAN_RL_TIRE_TEMP_VALUE_TYPE	_int16_t
*/

#define CAN_RPM_BYTE_OFS				0
#define CAN_RPM_MULT					1
#define CAN_RPM_DIV						1
#define CAN_RPM_OFS						0
#define CAN_RPM_VALUE_TYPE				_uint16_t

#define CAN_AIN4_BYTE_OFS				6
#define CAN_AIN4_MULT					5
#define CAN_AIN4_DIV					1024
#define CAN_AIN4_OFS					0
#define CAN_AIN4_VALUE_TYPE				_uint16_t

#define CAN_OILP_BYTE_OFS				4
#define CAN_OILP_MULT					1
#define CAN_OILP_DIV					16
#define CAN_OILP_OFS					0
#define CAN_OILP_VALUE_TYPE				_uint8_t

#define CAN_CLT_BYTE_OFS				4
#define CAN_CLT_MULT					1
#define CAN_CLT_DIV						1
#define CAN_CLT_OFS						0
#define CAN_CLT_VALUE_TYPE				_int16_t

#define CAN_BATV_BYTE_OFS				2
#define CAN_BATV_MULT					1000
#define CAN_BATV_DIV					9189
#define CAN_BATV_OFS					0
#define CAN_BATV_VALUE_TYPE				_uint8_t

#define CAN_STOP_BYTE_OFS				2
#define CAN_STOP_MULT					1
#define CAN_STOP_DIV					1
#define CAN_STOP_OFS					0
#define CAN_STOP_VALUE_TYPE				_uint8_t

#endif /* CONFIG_CAN_H_ */
