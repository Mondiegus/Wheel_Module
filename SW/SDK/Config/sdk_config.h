
/* LICENSE */

#ifndef SDK_CONFIG_H_
#define SDK_CONFIG_H_

#if defined(STM32F042x6)
#include "stm32f0xx_hal.h"
#elif defined(STM32F303xC)
#include "stm32f3xx_hal.h"
#elif defined(STM32F767xI)
#include "stm32f7xx_hal.h"
#endif

#define ASSERT(expr) ((void)0U)

#define CONFIG_MPU6050_ENABLED 	(CONFIG_ACC_ENABLED || CONFIG_GYR_ENABLED || CONFIG_TEMP_ENABLED)
#define CONFIG_MLX90614_ENABLED (CONFIG_BRAKE_DISC_TEMP_ENABLED)

typedef enum
{
    retcode_ok       = HAL_OK,
    retcode_error    = HAL_ERROR,
    retcode_busy     = HAL_BUSY,
    retcode_timeout  = HAL_TIMEOUT
}retcode_t;

#define APP_RETURN_ERR(code, ok_code) if (code != ok_code) return code

#endif /* SDK_CONFIG_H_ */
