#ifndef IRTEMPSENS_H_
#define IRTEMPSENS_H_

#define ROWS 4
#define COLUMNS 16
#define _EEPROM_BYTE_SIZE 256
#define _RAM_WORD_SIZE 66
#define _RAM_BYTE_SIZE _RAM_WORD_SIZE * 2
uint8_t  _eeprom_data[_EEPROM_BYTE_SIZE];

int init();

void startMeasurment();
void getFullFrame(float *temp_frame);

void calculation_test();

struct Coefficients {
	float		MINUS_KT1;
	float		TWO_KT2;
	float		FOUR_KT2;
	float		KT1_KT2_VTH;

	float		AI[ROWS][COLUMNS];
	float		BI[ROWS][COLUMNS];

	float		A_CP_;
	float		B_CP_;

	float		TGC_;
	float		EPSILON;

	float		ALPHA_TGC[ROWS][COLUMNS];
	float		KS_ALPHA_TGC[ROWS][COLUMNS];

	float		KS4;
	float		ONE_MINUS_KS4;
};
uint16_t _ram_data[_RAM_BYTE_SIZE / 2];

struct Coefficients _COEF;
float _ambient_temp;
float _ambient_temp_m25;
bool _calculation_test = false;

int readEEPROM();
void readRAM();
void writeOscTrimValue();
void writeConfigReg();
void assignCoefficients();
uint8_t getCoefAddress(uint8_t start_adr, uint8_t row, uint8_t col);

float calcAmbientTemp(uint16_t ptat_val);
float calcPixelTemp(uint8_t row, uint8_t col, int16_t vir_value);
float calcVIRCompensated(uint8_t row, uint8_t col, int16_t vir_value);
float calcVIROffsetComp(uint8_t row, uint8_t col, int16_t vir_value);
float calcVIRCPOffsetComp(int16_t vircp_value);
float calcAlphaCompensated(uint8_t row, uint8_t col);
float calcSx(float tak4, float alpha_comp, float vir_comp);

#endif /* IRTEMPSENS_H_ */
