
/* LICENSE */

#include "app_config.h"
#if CONFIG_MLX90614_ENABLED
#include "drv_mlx90614.h"
#include "drv_mlx90614_reg.h"
#include "drv_mlx90614_hal.h"
#include <stdbool.h>

#define I2C_TIMEOUT_MS  	1000

retcode_t drv_mlx90614_read_ram(drv_mlx90614_t * handler, uint16_t address, uint16_t * read_value);
retcode_t drv_mlx90614_read_eeprom(drv_mlx90614_t * handler, uint16_t address, uint16_t * read_value);
retcode_t drv_mlx90614_write_eeprom(drv_mlx90614_t * handler, uint16_t address, uint16_t write_value);

retcode_t drv_mlx90614_init(drv_mlx90614_t* handler)
{
	handler->drv_instance = drv_hal_i2c_init();
	/* Format I2C address */
	handler->address = DRV_MLX90614_ADDR << 1;
	/* Check if device is connected */
	return drv_hal_i2c_is_dev_ready(handler->drv_instance, handler->address, I2C_TIMEOUT_MS);
}


retcode_t drv_mlx90614_read_ram(drv_mlx90614_t * handler, uint16_t address, uint16_t * read_value)
{
	return drv_hal_i2c_mem_read(handler->drv_instance, handler->address, address | DRV_MLX90614_RAM_ACCESS, 1, (uint8_t *)read_value, 2, I2C_TIMEOUT_MS);
}


retcode_t drv_mlx90614_read_eeprom(drv_mlx90614_t * handler, uint16_t address, uint16_t * read_value)
{
	return drv_hal_i2c_mem_read(handler->drv_instance, handler->address, address | DRV_MLX90614_EEPROM_ACCESS, 1, (uint8_t *)read_value, 2, I2C_TIMEOUT_MS);
}


retcode_t drv_mlx90614_write_eeprom(drv_mlx90614_t * handler, uint16_t address, uint16_t  write_value)
{
	return drv_hal_i2c_mem_write(handler->drv_instance, handler->address, address | DRV_MLX90614_EEPROM_ACCESS, 1, (uint8_t *) &write_value, 2, I2C_TIMEOUT_MS);
}


retcode_t drv_mlx90614_read_object_temp(drv_mlx90614_t* handler, uint8_t object_number, uint16_t * object_temp)
{
    /* Read object temperature */
    if (object_number == 1)
    {
    	return drv_mlx90614_read_ram(handler, DRV_MLX90614_OBJECT1, object_temp);
    }
    else if (object_number == 2)
    {
    	return drv_mlx90614_read_ram(handler, DRV_MLX90614_OBJECT2, object_temp);
    }
    return retcode_error;
}


retcode_t drv_mlx90614_read_ambient_temp(drv_mlx90614_t* handler, uint16_t * ambient_temp)
{
	return drv_mlx90614_read_ram(handler, DRV_MLX90614_AMBIENT, ambient_temp);
}


retcode_t drv_mlx90614_read_ke(drv_mlx90614_t* handler, uint16_t * ke)
{
	return drv_mlx90614_read_eeprom(handler, DRV_MLX90614_KE, ke);
}

/*
drv_hal_status_t drv_mlx90614_set_ke(drv_mlx90614_t* handler, uint16_t ke)
{
	return drv_mlx90614_write_eeprom(handler, DRV_MLX90614_KE, &ke);
}
*/

#endif /* CONFIG_MLX90614_ENABLED */
