
/* LICENSE */

#ifndef DRV_MLX90614_REG_H_
#define DRV_MLX90614_REG_H_
#ifdef __cplusplus
extern "C" {
#endif

// Default I2C address
#define DRV_MLX90614_ADDR			((uint8_t)0x5A)

/* EEPROM ADDRESSES */
#define DRV_MLX90614_TO_MAX			((uint8_t)0x00)
#define DRV_MLX90614_TO_MIN			((uint8_t)0x01)
#define DRV_MLX90614_PWMCTRL		((uint8_t)0x02)
#define DRV_MLX90614_TA_RANGE		((uint8_t)0x03)
#define DRV_MLX90614_KE				((uint8_t)0x04)
#define DRV_MLX90614_CONFIG1		((uint8_t)0x05)
#define DRV_MLX90614_SMBUS_ADDR		((uint8_t)0x0E)
#define DRV_MLX90614_ID_NUMB1		((uint8_t)0x1C)
#define DRV_MLX90614_ID_NUMB2		((uint8_t)0x1D)
#define DRV_MLX90614_ID_NUMB3		((uint8_t)0x1E)
#define DRV_MLX90614_ID_NUMB4		((uint8_t)0x1F)

/* RAM ADDRESSES */
#define DRV_MLX90614_AMBIENT		((uint8_t)0x06)
#define DRV_MLX90614_OBJECT1		((uint8_t)0x07)
#define DRV_MLX90614_OBJECT2		((uint8_t)0x08)

/* COMMANDS */
#define DRV_MLX90614_RAM_ACCESS		((uint8_t)0x00)
#define DRV_MLX90614_EEPROM_ACCESS	((uint8_t)0x20)
#define DRV_MLX90614_READ_FLAGS		((uint8_t)0xF0)
#define DRV_MLX90614_ENTER_SLEEP	((uint8_t)0xFF)

/* Control bits in configuration register */
#define DRV_MLX90614_CONFIG_IIR_SHIFT 	0 /* IIR coefficient */
#define DRV_MLX90614_CONFIG_IIR_MASK 	(0x7 << DRV_MLX90614_CONFIG_IIR_SHIFT)
#define DRV_MLX90614_CONFIG_DUAL_SHIFT 	6 /* single (0) or dual (1) IR sensor */
#define DRV_MLX90614_CONFIG_DUAL_MASK 	(1 << DRV_MLX90614_CONFIG_DUAL_SHIFT)
#define DRV_MLX90614_CONFIG_FIR_SHIFT 	8 /* FIR coefficient */
#define DRV_MLX90614_CONFIG_FIR_MASK 	(0x7 << DRV_MLX90614_CONFIG_FIR_SHIFT)
#define DRV_MLX90614_CONFIG_GAIN_SHIFT 	11 /* gain */
#define DRV_MLX90614_CONFIG_GAIN_MASK 	(0x7 << DRV_MLX90614_CONFIG_GAIN_SHIFT)

/* Timings (in ms) */
#define DRV_MLX90614_TIMING_EEPROM 	20 /* time for EEPROM write/erase to complete */
#define DRV_MLX90614_TIMING_WAKEUP 	34 /* time to hold SDA low for wake-up */
#define DRV_MLX90614_TIMING_STARTUP 250 /* time before first data after wake-up */

#define DRV_MLX90614_AUTOSLEEP_DELAY 5000 /* default autosleep delay */

/* Magic constants */
#define DRV_MLX90614_CONST_OFFSET_DEC 				-13657 /* decimal part of the Kelvin offset */
#define DRV_MLX90614_CONST_OFFSET_REM 				500000 /* remainder of offset (273.15*50) */
#define DRV_MLX90614_CONST_SCALE 					20 /* Scale in milliKelvin (0.02 * 1000) */
#define DRV_MLX90614_CONST_RAW_EMISSIVITY_MAX 		65535 /* max value for emissivity */
#define DRV_MLX90614_CONST_EMISSIVITY_RESOLUTION 	15259 /* 1/65535 ~ 0.000015259 */
#define DRV_MLX90614_CONST_FIR 						0x7 /* Fixed value for FIR part of low pass filter */


#ifdef __cplusplus
}
#endif
#endif /* DRV_MLX90614_REG_H_ */
