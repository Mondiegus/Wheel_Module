#ifndef INC_UTILITIES_H_
#define INC_UTILITIES_H_

#include "main.h"
#include "math.h"

inline __attribute__((always_inline))
int32_t sign_extend(uint32_t value, const uint8_t bits) {
	uint32_t sbit = 1u << (bits - 1);
	return (value ^ sbit) - sbit;
}

inline __attribute__((always_inline))
uint32_t clamp(uint32_t value, uint32_t min, uint32_t max)
{
	return (value < min) ? min : (value > max ? max : value);
}

inline __attribute__((always_inline))
int32_t clamp_s(int32_t value, int32_t min, int32_t max)
{
	return (value < min) ? min : (value > max ? max : value);
}

inline __attribute__((always_inline))
uint32_t divRound(uint32_t value, uint32_t divisor)
{
	return (value + divisor / 2) / divisor;
}

inline __attribute__((always_inline))
uint32_t divUp(uint32_t value, uint32_t divisor)
{
	return (value + divisor - 1) / divisor;
}

//inline __attribute__((always_inline))
uint32_t log2_32(uint32_t value)
{
	return (31 - __builtin_clz(value));
}

inline __attribute__((always_inline))
uint8_t log2_8(uint8_t value)
{
	return (uint8_t)log2((uint32_t)value);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
inline __attribute__((always_inline))
float pow2(uint8_t value)
{
	uint32_t a = (127 + value) << 23;
	float result = *((float *)&a);
	return result;
}
#pragma GCC diagnostic pop

/**
 * @brief Rise 2 to power of @param value
 * @param value - exponent with allowed range = [0; 31]
 * @return Integer result of exponentiation
 */
inline __attribute__((always_inline))
uint32_t pow2i(uint8_t value)
{
	assert_param(value < 32);
	return 1 << value;
}

/**
 * @brief Multiply @param value by 2 raised to power of @param exp
 * @param value - number to multiply
 * @param exp - exponent with allowed range = [0; 24]
 * @return Integer result of multiplication by power of 2
 */
inline __attribute__((always_inline))
uint32_t mul_pow2i(uint8_t value, uint8_t exp)
{
	assert_param(exp < 25);
	return value << exp;
}

inline __attribute__((always_inline))
float sqrtt(float value)
{
	float result;
	result = sqrt(value);
	return result;
}

uint16_t getMinValIndex(uint8_t *array, uint16_t size);


#endif /* INC_UTILITIES_H_ */
