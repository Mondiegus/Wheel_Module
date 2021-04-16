
/* LICENSE */

#ifndef DRV_MLX90614_H_
#define DRV_MLX90614_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_config.h"
#include "inttypes.h"
#include <stdbool.h>

typedef struct
{
    uint8_t address;
    void    *drv_instance;
}drv_mlx90614_t;

retcode_t drv_mlx90614_init(drv_mlx90614_t* handler);
retcode_t drv_mlx90614_read_ambient_temp(drv_mlx90614_t* handler, uint16_t * ambient_temp);
retcode_t drv_mlx90614_read_object_temp(drv_mlx90614_t* handler, uint8_t object_number, uint16_t * object_temp);
retcode_t drv_mlx90614_read_ke(drv_mlx90614_t* handler, uint16_t * ke);
retcode_t drv_mlx90614_set_ke(drv_mlx90614_t* handler, uint16_t ke);

#ifdef __cplusplus
}
#endif
#endif /* DRV_MLX90614_H_ */
