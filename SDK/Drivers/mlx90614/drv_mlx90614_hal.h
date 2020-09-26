
/* LICENSE */

#ifndef DRV_HAL_H_
#define DRV_HAL_H_

#include "app_config.h"
#if CONFIG_MLX90614_ENABLED
#include "i2c.h"

#define I2C_INSTANCE &hi2c1

void *drv_hal_i2c_init(void);
retcode_t drv_hal_i2c_is_dev_ready(void *drv_instance, uint16_t dev_address, uint32_t timeout);
retcode_t drv_hal_i2c_mem_read(void *drv_instance, uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t *data, uint16_t size, uint32_t timeout);
retcode_t drv_hal_i2c_master_transmit(void *drv_instance, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t timeout);
retcode_t drv_hal_i2c_mem_write(void *drv_instance, uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t *data, uint16_t size, uint32_t timeout);

#endif /* CONFIG_MLX90614_ENABLED */
#endif /* DRV_HAL_H_ */
