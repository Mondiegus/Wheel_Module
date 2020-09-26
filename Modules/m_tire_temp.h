
/* LICENSE */

#ifndef M_TIRE_TEMP_H_
#define M_TIRE_TEMP_H_
#include "drv_mlx90614.h"
#include "app_config.h"

retcode_t m_tire_temp_init(void);
retcode_t m_tire_temp_deinit(void);
void m_tire_temp_update(void);

#endif /* M_TIRE_TEMP_H_ */
