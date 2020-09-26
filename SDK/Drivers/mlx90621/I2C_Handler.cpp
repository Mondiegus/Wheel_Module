#include "I2C_Handler.h"
#include "i2c.h"


char i2c_write_with_size(uint8_t address, uint8_t *data, uint16_t size, uint32_t timeout /*= 100*/)
{
	if(data == NULL)
		return false;

	return HAL_I2C_Master_Transmit(&hi2c1, address, data, size, timeout) == HAL_OK;
}

char i2c_write(uint8_t address, uint8_t value, uint32_t timeout /*= 100*/)
{
	volatile HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, address, &value, 1, timeout);
	assert_param(status == HAL_OK);
	return status == HAL_OK;
}

char i2c_read(uint8_t address, uint8_t *data, uint16_t size, uint32_t timeout /*= 100*/)
{
	if(data == NULL)
		return false;

	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Receive(&hi2c1, address, data, size, timeout);
	return status == HAL_OK;
}

char i2c_writeThenRead(uint8_t address, uint8_t *tx_data, uint16_t tx_size, uint8_t *rx_data, uint16_t rx_size)
{
	if(tx_data == NULL || rx_data == NULL)
		return false;

	char success = true;
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Sequential_Transmit_IT(&hi2c1,address, tx_data, tx_size,I2C_FIRST_FRAME);
	if(status != HAL_OK)
		success = false;

	if(success)
	{
		i2c_waitForXfer();
		status = HAL_I2C_Master_Sequential_Receive_IT(&hi2c1,address, rx_data, rx_size,I2C_LAST_FRAME);
		if(status == HAL_OK) {
			i2c_waitForXfer();
		} else {
			success = false;
		}
	}

	return success;
}

void i2c_waitForXfer(void)
{
	while(hi2c1.State != HAL_I2C_STATE_READY)
		__NOP();
}
