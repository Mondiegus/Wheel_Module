#ifndef I2C_HANDLER_H_
#define I2C_HANDLER_H_


char i2c_write_with_size(uint8_t address, uint8_t *data, uint16_t size, uint32_t timeout);
char i2c_write(uint8_t address, uint8_t value, uint32_t timeout);
char i2c_read(uint8_t address, uint8_t *data, uint16_t size, uint32_t timeout);
char i2c_writeThenRead(uint8_t address, uint8_t *tx_data, uint16_t tx_size, uint8_t *rx_data, uint16_t rx_size);

volatile bool _isTxPending = false;
volatile bool _isRxPending = false;

void i2c_waitForXfer(void);

#endif /* I2C_HANDLER_H_ */
