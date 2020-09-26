#ifndef MLX90614_H_
#define MLX90614_H_


#define MLX90614_DEFAULT_ADDRESS ((0x5A)<<1)
#define MLX90614_SECOND_ADDRESS ((0x00)<<1)

// RAM
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07
#define MLX90614_TOBJ2 0x08
// EEPROM
#define MLX90614_TOMAX 0x00
#define MLX90614_TOMIN 0x01
#define MLX90614_PWMCTRL 0x02
#define MLX90614_TARANGE 0x03
#define MLX90614_EMISS 0x04
#define MLX90614_CONFIG 0x05
#define MLX90614_ADDR 0x0E
#define MLX90614_EMISS_CALIBRATION 0x0F
#define MLX90614_ID1 0x1C
#define MLX90614_ID2 0x1D
#define MLX90614_ID3 0x1E
#define MLX90614_ID4 0x1F

#define MLX90614CMD_UNLOCK_EMISS_CALIBRATION 0x60
#define MLX90614_SLEEP_MODE 0xFF

typedef enum
{
	MLX90614_OK 	= 0,
	MLX90614_ERROR	= 1
}MLX90614_STATUS;

MLX90614_STATUS MLX90614_Init(I2C_HandleTypeDef *hi2c);
MLX90614_STATUS MLX90614_SetAddress(uint8_t Address, uint16_t DevAddress);
MLX90614_STATUS MLX90614_GetId(uint32_t *Id, uint16_t DevAddress);

MLX90614_STATUS MLX90614_ReadObjectTemperature(uint16_t *Temperature, uint16_t DevAddress);
MLX90614_STATUS MLX90614_ReadAmbientTemperature(uint16_t *Temperature, uint16_t DevAddress);

//
//	IR Emissivity Table - https://msalamon.pl/download/691/
//
MLX90614_STATUS MLX90614_GetEmissivity(uint16_t *Emissivity, uint16_t DevAddress);
MLX90614_STATUS MLX90614_SetEmissivity(uint16_t Emissivity, uint16_t DevAddress);
MLX90614_STATUS MLX90614_ResetEmissivity(uint16_t DefaultEmissivity, uint16_t DevAddress);

#endif /* MLX90614_H_ */
