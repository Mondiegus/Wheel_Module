
#include "main.h"
#include "i2c.h"

#include "mlx90614.h"
#include "math.h"

I2C_HandleTypeDef *mlx90614_i2c;

uint8_t ObjectTemperature;
uint8_t AmbientTemperature;

MLX90614_STATUS MLX90614_Read16(uint8_t Register, uint16_t *Result, uint16_t DevAddress)
{
	uint8_t tmp[2];

	if(HAL_OK != HAL_I2C_Mem_Read(mlx90614_i2c, DevAddress, Register, 1, tmp, 2, 10))
	{
		return MLX90614_ERROR;
	}

	*Result = (uint16_t)tmp[1]<<8 | tmp[0];

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_Read16Eeprom(uint8_t Address, uint16_t *Result, uint16_t DevAddress)
{
	uint8_t tmp[2];

	Address &= 0x1F;
	Address |= 0x20;

	if(HAL_OK != HAL_I2C_Mem_Read(mlx90614_i2c, DevAddress, Address, 1, tmp, 2, 10))
	{
		return MLX90614_ERROR;
	}

	*Result = (uint16_t)tmp[1]<<8 | tmp[0];

	return MLX90614_OK;
}

uint8_t crc8(uint8_t InCrc, uint8_t InData)
{
	uint8_t i;
	uint8_t Data;
	Data = InCrc ^= InData;
	for ( i = 0; i < 8; i++ )
	{
		if (( Data & 0x80 ) != 0 )
		{
			Data <<= 1;
			Data ^= 0x07;
		}
		else
		{
			Data <<= 1;
		}
	}
	return Data;
}

MLX90614_STATUS MLX90614_Write16Crc(uint8_t Address, uint16_t Value, uint16_t DevAddress)
{
	uint8_t Crc;
	uint8_t tmp[3];

	Address &= 0x1F;
	Address |= 0x20;

	Crc = crc8(0, DevAddress);
	Crc = crc8(Crc, Address);
	Crc = crc8(Crc, Value & 0xFF);
	Crc = crc8(Crc, Value>>8);

	tmp[0] = Value & 0xFF;
	tmp[1] = Value>>8;
	tmp[2] = Crc;
	if(HAL_OK != HAL_I2C_Mem_Write(mlx90614_i2c, DevAddress, Address, 1, tmp, 3, 10))
	{
		return MLX90614_ERROR;
	}

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_Write16Eeprom(uint8_t Address, uint16_t Value, uint16_t DevAddress)
{

	uint16_t Verify;

	if(MLX90614_OK != MLX90614_Write16Crc(Address, 0, DevAddress))
	{
		return MLX90614_ERROR;
	}

	HAL_Delay(10); // Erasing time ~5ms;

	if(MLX90614_OK != MLX90614_Write16Crc(Address, Value, DevAddress))
	{
		return MLX90614_ERROR;
	}

	HAL_Delay(10); // Writing time ~5ms;

	MLX90614_Read16Eeprom(Address, &Verify, DevAddress);
	if(Verify != Value)
	{
		return MLX90614_ERROR;
	}

	return MLX90614_OK;
}

uint16_t MLX90614_KelvinToCelsius(uint16_t Temperature)
{
	uint16_t Result;

	Result = (uint16_t)Temperature * 0.02;
	Result = Result - 273.15;

	return Result;
}

MLX90614_STATUS MLX90614_ReadAmbientTemperature(uint16_t *Temperature, uint16_t DevAddress)
{
	uint16_t tmp;

	if(MLX90614_OK != MLX90614_Read16(MLX90614_TA, &tmp, DevAddress))
	{
		return MLX90614_ERROR;
	}

	*Temperature = MLX90614_KelvinToCelsius(tmp);

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_ReadObjectTemperature(uint16_t *Temperature, uint16_t DevAddress)
{
	uint16_t tmp;

	if(MLX90614_OK != MLX90614_Read16(MLX90614_TOBJ1, &tmp, DevAddress))
	{
		return MLX90614_ERROR;
	}

	*Temperature = MLX90614_KelvinToCelsius(tmp);

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_GetId(uint32_t *Id, uint16_t DevAddress)
{
	uint16_t id[2];

	if(MLX90614_OK != MLX90614_Read16Eeprom(MLX90614_ID1, id, DevAddress))
	{
		return MLX90614_ERROR;
	}

	if(MLX90614_OK != MLX90614_Read16Eeprom(MLX90614_ID3, id+1, DevAddress))
	{
		return MLX90614_ERROR;
	}

	*Id = (uint32_t)id[0]<<16 | id[1];

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_GetEmissivity(uint16_t *Emissivity, uint16_t DevAddress)
{
	uint16_t tmp;

	if(MLX90614_OK != MLX90614_Read16Eeprom(MLX90614_EMISS, &tmp, DevAddress))
	{
		return MLX90614_ERROR;
	}

	*Emissivity = (uint16_t)tmp/65535.0;

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_SetEmissivity(uint16_t Emissivity, uint16_t DevAddress)
{
	if (Emissivity < 0.1 || Emissivity > 1)
		return MLX90614_ERROR;

	uint16_t current_emissivity;
	uint16_t current_calibration;
	uint16_t new_emissivity;
	uint16_t new_calibration;

	MLX90614_Read16Eeprom(MLX90614_EMISS, &current_emissivity, DevAddress);
	MLX90614_Read16Eeprom(MLX90614_EMISS_CALIBRATION, &current_calibration, DevAddress);

	new_emissivity = (uint16_t)round(65535.0 * Emissivity);

	if(new_emissivity == current_emissivity)
	{
		return MLX90614_OK;
	}

	new_calibration = (uint16_t)(((uint16_t)current_emissivity / new_emissivity) * current_calibration);

	if(MLX90614_OK != MLX90614_Write16Eeprom(MLX90614_EMISS, new_emissivity, DevAddress))
	{
		return MLX90614_ERROR;
	}

	if(MLX90614_OK != MLX90614_Write16Eeprom(MLX90614_EMISS_CALIBRATION, new_calibration, DevAddress))
	{
		return MLX90614_ERROR;
	}

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_ResetEmissivity(uint16_t DefaultCalibrationy, uint16_t DevAddress)
{

	if(MLX90614_OK != MLX90614_Write16Eeprom(MLX90614_EMISS, 0xFFFF, DevAddress))
	{
		return MLX90614_ERROR;
	}

	if(MLX90614_OK != MLX90614_Write16Eeprom(MLX90614_EMISS_CALIBRATION, DefaultCalibrationy, DevAddress))
	{
		return MLX90614_ERROR;
	}

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_SetAddress(uint8_t Address, uint16_t DevAddress)
{
	if(Address > 0x7F) return MLX90614_ERROR;

	if(MLX90614_OK != MLX90614_Write16Eeprom(MLX90614_ADDR, Address, DevAddress))
	{
		return MLX90614_ERROR;
	}

	return MLX90614_OK;
}

MLX90614_STATUS MLX90614_Init(I2C_HandleTypeDef *hi2c)
{
	mlx90614_i2c = hi2c;

	return MLX90614_OK;
}

