#include "IRTempSens.h"
#include "main.h"
#include "I2C_Handler.h"
#include "I2C_Handler.cpp"
#include "Utilities.h"

//using namespace Utilities;

#define		EEPROM	 0x50
#define		SENSOR	 0x60
#define		StartMeasur	 0x0001
#define		ReadEEPROM	 0x0000
#define		Read		 0x0002
#define		WriteConf	 0x0003
#define		WriteTrim	 0x0004
#define		dAi_0_0		 0x00
#define		Bi_0_0		 0x40
#define		dAlpha_0_0	 0x80
#define		Ks_scales	 0xC0
#define		Ks4			 0xC4
#define		Acommon_L	 0xD0
#define		Acommon_H	 0xD1
#define		KT_scales	 0xD2
#define		A_CP_L		 0xD3
#define		A_CP_H		 0xD4

#define		B_CP		 0xD5
#define		Alpha_CP_L	 0xD6
#define		Alpha_CP_H	 0xD7

#define		TGC			 0xD8
#define		Off_scales	 0xD9
#define		Vth_L		 0xDA
#define		Vth_H		 0xDB
#define		K_T1_L		 0xDC
#define		K_T1_H		 0xDD
#define		K_T2_L		 0xDE
#define		K_T2_H		 0xDF
#define		Alpha0_L	 0xE0
#define		Alpha0_H	 0xE1
#define		Alpha0_scale 0xE2
#define		dAlpha_scale 0xE3
#define		Epsilon_L	 0xE4
#define		Epsilon_H	 0xE5
#define		Ks_Ta_L		 0xE6
#define		Ks_Ta_H		 0xE7

#define		CFG_L		 0xF5
#define		CFG_H		 0xF6
#define		OSC_trim	 0xF7
#define		IR_Sens_0_0		 0x00
#define		PTAT_Sens		 0x40
#define		CompPixel		 0x41

#define		RR_512Hz 	 0x0
#define		RR_256Hz 	 0x6
#define		RR_128Hz 	 0x7
#define		RR_64Hz		 0x8
#define		RR_32Hz		 0x39
#define		RR_16Hz		 0xA
#define		RR_8Hz		 0xB
#define		RR_4Hz		 0xC
#define		RR_2Hz		 0xD
#define		RR_1Hz		 0x3E
#define		RR_0Hz5		 0xF

#define		AR_15bit 	 0x0
#define		AR_16bit 	 0x1
#define		AR_17bit 	 0x2
#define		AR_18bit	 0x3

struct value{
	uint8_t value_L;
	uint8_t value_H;
};
struct settings{
	uint16_t ref_rate;
	uint16_t adc_res;
	uint16_t step_mode;
	uint16_t sleep_mode;
	uint16_t reserved;
	uint16_t measure_running;
	uint16_t por_occured;
	uint16_t i2c_fmp_disable;
	uint16_t eeprom_disable;
	uint16_t reserved2;
	uint16_t adc_low_ref_enable;
	uint16_t reserved3;
};

union ConfigReg {
	uint16_t value;
	struct value values;
	struct settings settingss;
};

int init()
{
	// Power On Reset (POR) delay
	HAL_Delay(5);
	if(readEEPROM()==1)
	{
		assignCoefficients();
		writeOscTrimValue();
		writeConfigReg();
		return 1;
	} else
		return 0;
}

void startMeasurment()
{
	uint16_t tx_data = StartMeasur;
	i2c_write_with_size((uint8_t)SENSOR << 1, (uint8_t *)&tx_data, 2,100);
}

void getFullFrame(float *temp_frame)
{
	//if(temp_frame == NULL) return;

	readRAM();

	if(_calculation_test == true) {
		_ram_data[IR_Sens_0_0]	= 0x01B7;
		_ram_data[PTAT_Sens]	= 0x67DE;
		_ram_data[CompPixel]	= 0xFFDC;
	}

	_ambient_temp = calcAmbientTemp(_ram_data[PTAT_Sens]);
	_ambient_temp_m25 = _ambient_temp - 25.0f;

	for(uint8_t row = 0; row < ROWS; ++row) {
		for(uint8_t col = 0; col < COLUMNS; ++col) {
			temp_frame[COLUMNS * row + col] = calcPixelTemp(row, col, _ram_data[row + ROWS * col]);
		}
	}
}

void calculation_test()
{
	_eeprom_data[ Vth_L]		= 0x20;
	_eeprom_data[ Vth_H]		= 0x64;
	_eeprom_data[ K_T1_L]		= 0x89;
	_eeprom_data[ K_T1_H]		= 0x55;
	_eeprom_data[ K_T2_L]		= 0x7E;
	_eeprom_data[ K_T2_H]		= 0x5E;
	_eeprom_data[ KT_scales]	= 0x8B;

	_eeprom_data[ dAi_0_0]		= 0x21;
	_eeprom_data[ Bi_0_0]			= 0xBC;
	_eeprom_data[ dAlpha_0_0]		= 0xCD;
	_eeprom_data[ Ks_scales]		= 0x99;
	_eeprom_data[ Ks4]			= 0x9E;
	_eeprom_data[ Acommon_L]		= 0x8A;
	_eeprom_data[ Acommon_H]		= 0xFF;
	_eeprom_data[ A_CP_L]			= 0x9D;
	_eeprom_data[ A_CP_H]			= 0xFF;
	_eeprom_data[ B_CP]			= 0xA2;
	_eeprom_data[ Alpha_CP_L]		= 0xA8;
	_eeprom_data[ Alpha_CP_H]		= 0x0F;
	_eeprom_data[ TGC]			= 0x18;
	_eeprom_data[ Off_scales]		= 0x07;
	_eeprom_data[ Alpha0_L]		= 0xAE;
	_eeprom_data[ Alpha0_H]		= 0x4E;
	_eeprom_data[ Alpha0_scale]	= 0x26;
	_eeprom_data[ dAlpha_scale]	= 0x1F;
	_eeprom_data[ Epsilon_L]		= 0x00;
	_eeprom_data[ Epsilon_H]		= 0x80;
	_eeprom_data[ Ks_Ta_L]		= 0x0C;
	_eeprom_data[ Ks_Ta_H]		= 0x02;

	_calculation_test = true;

	assignCoefficients();
}

int readEEPROM()
{
	if(!i2c_write(EEPROM << 1, ReadEEPROM,100))
		return 0;

	if(!i2c_read(EEPROM << 1, _eeprom_data, _EEPROM_BYTE_SIZE,100))
		return 0;
	return 1;
}

void readRAM()
{
	uint8_t tx_data[4] = {
			Read,
			IR_Sens_0_0,
			1, /* address step */
			_RAM_WORD_SIZE
	};
	hi2c1.State;

	i2c_writeThenRead(SENSOR << 1, tx_data, sizeof(tx_data), (uint8_t *)_ram_data, _RAM_BYTE_SIZE);
}

void writeOscTrimValue()
{
	uint8_t tx_data[5] = {
			WriteTrim,
			(uint8_t)(_eeprom_data[ OSC_trim] - 0xAA),
			_eeprom_data[OSC_trim],
			0x00 - 0xAA, /* fixed value */
			0x00  /* fixed value */
	};
	i2c_write_with_size(SENSOR << 1, tx_data, sizeof(tx_data),100);
}

void writeConfigReg()
{
	union ConfigReg cfg_reg;

	cfg_reg.values.value_L = _eeprom_data[ CFG_L];
	cfg_reg.values.value_H = _eeprom_data[ CFG_H];

	cfg_reg.settingss.ref_rate = RR_32Hz;
	cfg_reg.settingss.adc_res =  AR_15bit;
	cfg_reg.settingss.step_mode = 0;
	cfg_reg.settingss.por_occured = 1;

	uint8_t tx_data[5] = {
			WriteConf,
			(uint8_t)(cfg_reg.values.value_L - 0x55),
			cfg_reg.settingss.ref_rate,
			(uint8_t)(cfg_reg.values.value_H - 0x55),
			cfg_reg.values.value_H
	};

	i2c_write_with_size(SENSOR << 1, tx_data, sizeof(tx_data),100);
}

void assignCoefficients()
{

	int16_t Vth = (int16_t)((_eeprom_data[ Vth_H] << 8) | _eeprom_data[ Vth_L]);

	int16_t Kt1_int		= (int16_t)((_eeprom_data[ K_T1_H] << 8) | _eeprom_data[ K_T1_L]);
	int16_t Kt2_int		= (int16_t)((_eeprom_data[ K_T2_H] << 8) | _eeprom_data[ K_T2_L]);

	uint8_t Kt1_scale	= (uint8_t)(_eeprom_data[ KT_scales] >> 4);			/* [0;	15] */
	uint8_t Kt2_scale	= (uint8_t)((_eeprom_data[ KT_scales] & 0x0F) + 10);	/* [10; 25] */

	float Kt1			= Kt1_int / (float)pow2i(Kt1_scale);
	float Kt2			= Kt2_int / (float)pow2i(Kt2_scale);

	_COEF.MINUS_KT1		= -Kt1;
	_COEF.TWO_KT2		= 2 * Kt2;
	_COEF.FOUR_KT2		= 2 * _COEF.TWO_KT2;
	_COEF.KT1_KT2_VTH	= (Kt1 * Kt1) - (_COEF.FOUR_KT2 * Vth);

	float Acommon		= (int16_t)((_eeprom_data[ Acommon_H] << 8) | _eeprom_data[ Acommon_L]);
	uint8_t dAi_scale	= (uint8_t)(_eeprom_data[ Off_scales] >> 4);			/* [0;	15] */
	uint8_t Bi_scale	= (uint8_t)(_eeprom_data[ Off_scales] & 0x0F);		/* [0;	15] */

	for(uint8_t row = 0; row < ROWS; ++row) {
		for(uint8_t col = 0; col < COLUMNS; ++col) {
			_COEF.AI[row][col] = Acommon + (float)mul_pow2i(_eeprom_data[getCoefAddress( dAi_0_0, row, col)], dAi_scale);
			_COEF.BI[row][col] = (int8_t)_eeprom_data[getCoefAddress( Bi_0_0, row, col)] / (float)pow2i(Bi_scale);
		}
	}

	_COEF.A_CP_		= (int16_t)((_eeprom_data[ A_CP_H] << 8) | _eeprom_data[ A_CP_L]);
	_COEF.B_CP_		= ((int8_t)_eeprom_data[ B_CP]) / (float)pow2i(Bi_scale);

	_COEF.TGC_		= ((int8_t)_eeprom_data[ TGC]) / 32.0f;
	_COEF.EPSILON	= ((uint16_t)((_eeprom_data[ Epsilon_H] << 8) | _eeprom_data[ Epsilon_L])) / 32768.0f;
	uint8_t dAlpha_scale_;
	dAlpha_scale_	= _eeprom_data[dAlpha_scale];
	uint8_t Alpha0_scale_;
	Alpha0_scale_	= _eeprom_data[Alpha0_scale];
	float Alpha0			= ((uint16_t)((_eeprom_data[ Alpha0_H] << 8) | _eeprom_data[ Alpha0_L])) / pow2(Alpha0_scale_);
	float AlphaCP			= ((uint16_t)((_eeprom_data[ Alpha_CP_H] << 8) | _eeprom_data[ Alpha_CP_L])) / pow2(Alpha0_scale_);
	float KsTa				= ((int16_t)((_eeprom_data[ Ks_Ta_H] << 8) | _eeprom_data[ Ks_Ta_L])) / (float)pow2i(20);

	for(uint8_t row = 0; row < ROWS; ++row) {
		for(uint8_t col = 0; col < COLUMNS; ++col) {

			float dAlpha	= _eeprom_data[getCoefAddress( dAlpha_0_0, row, col)] / pow2(dAlpha_scale_);
			float Alpha	= Alpha0 + dAlpha;

			_COEF.ALPHA_TGC[row][col]		= Alpha - _COEF.TGC_ * AlphaCP;
			_COEF.KS_ALPHA_TGC[row][col]	= KsTa * _COEF.ALPHA_TGC[row][col];
		}
	}

	uint8_t Ks4_scale	= (uint8_t)((_eeprom_data[ Ks_scales] & 0x0F) + 8);			/* [8;	23] */
	_COEF.KS4			= ((int8_t)_eeprom_data[ Ks4]) / (float)pow2i(Ks4_scale);
	_COEF.ONE_MINUS_KS4	= 1 - _COEF.KS4 * 273.15f;
}

uint8_t getCoefAddress(uint8_t start_adr, uint8_t row, uint8_t col)
{
	if(row > 3) row = 3;
	if(col > 15) col = 15;

	return (uint8_t)(start_adr + row + ((col % 2) * 4) + ((col / 2) * 8));
}

float calcAmbientTemp(uint16_t ptat_val)
{
	volatile float result = _COEF.FOUR_KT2 * (float)ptat_val;
	result = result + _COEF.KT1_KT2_VTH;
	result = sqrt(result);
	result = _COEF.MINUS_KT1 + result;
	result = result / _COEF.TWO_KT2;

	return result + 25.0f;
}

float calcPixelTemp(uint8_t row, uint8_t col, int16_t vir_value)
{
	float pixel_temp = 0;

	float vir_comp = calcVIRCompensated(row, col, vir_value);

	float alpha_comp = calcAlphaCompensated(row, col);

	float tak4 = (_ambient_temp + 273.15f);
	tak4 = tak4 * tak4 * tak4 * tak4;

	float sx = calcSx(tak4, alpha_comp, vir_comp);

	float under_root = (vir_comp / (alpha_comp * _COEF.ONE_MINUS_KS4 + sx)) + tak4;

	float root_result = sqrt(under_root);
	root_result = sqrt(root_result);

	pixel_temp = root_result - 273.15f;

	return pixel_temp;
}

float calcVIRCompensated(uint8_t row, uint8_t col, int16_t vir_value)
{
	float vir_offset_comp = calcVIROffsetComp(row, col, vir_value);
	float vircp_offset_comp = calcVIRCPOffsetComp(_ram_data[65]);
	float vir_tgc_comp = vir_offset_comp - _COEF.TGC_ * vircp_offset_comp;

	return vir_tgc_comp / _COEF.EPSILON;
}

float calcVIROffsetComp(uint8_t row, uint8_t col, int16_t vir_value)
{
	return vir_value - (_COEF.AI[row][col] + _COEF.BI[row][col] * _ambient_temp_m25);
}

float calcVIRCPOffsetComp(int16_t vircp_value)
{
	return vircp_value - (_COEF.A_CP_ + _COEF.B_CP_ * _ambient_temp_m25);
}

float calcAlphaCompensated(uint8_t row, uint8_t col)
{
	return _COEF.ALPHA_TGC[row][col] + _COEF.KS_ALPHA_TGC[row][col] * _ambient_temp_m25;
}

float calcSx(float tak4, float alpha_comp, float vir_comp)
{
	float alpha_3 = alpha_comp * alpha_comp * alpha_comp;
	float alpha_4 = alpha_3 * alpha_comp;
	float under_root = alpha_3 * vir_comp + alpha_4 * tak4;
	float root_result = sqrtf(under_root);
	root_result = sqrtf(root_result);

	return _COEF.KS4 * root_result;
}
