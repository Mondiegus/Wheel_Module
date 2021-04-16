
/* LICENSE */

#ifndef M_BRAKE_DISC_TEMP_H_
#define M_BRAKE_DISC_TEMP_H_
#include "drv_mlx90614.h"
#include "app_config.h"

retcode_t m_brake_disc_temp_init(void);
retcode_t m_brake_disc_temp_deinit(void);
void m_brake_disc_temp_update(void);

#endif /* M_BRAKE_DISC_TEMP_H_ */
