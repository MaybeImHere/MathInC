#ifndef _BIGINT_GUARD
#define _BIGINT_GUARD

#include <stdint.h>
#include <stdbool.h>

typedef struct BigInt {
	uint32_t* buffer;
	// buffer_capacity should always equal digits_length + scratch_space
	// length
	uint64_t buffer_capacity;
	
	uint32_t* digits;
	uint64_t digits_length;

	uint32_t* scratch_space;
	uint64_t scratch_space_length;

	// is_negative == True => the BigInt is negative	
	bool is_negative;
} BigInt;

/* Initializes a BigInt with a small amount of space. Initializes to a value of
 * 0. bigint is modified even if allocation fails.
 * ErrNullPtr: if bigint is NULL.
 * ErrAllocFailed: if unable to allocate memory.
 */
Err bigint_init(BigInt* bigint);

/* Sets the value of the BigInt to the value defined by simple_value.
 * Essentially just a shortcut to setting the value of BigInt.
 * ErrNullPtr: if bigint is NULL.
 * ErrAllocFailed: if unable to allocate memory.
 */
Err bigint_set_value_simple(BigInt* bigint, int32_t simple_value);

/* Converts a BigInt to a base 10 string representation.
 * ErrNullPtr: if bigint, out is NULL.
 * ErrInvalidParameter: if max_out_len < 2.
 */
Err bigint_to_base10_string(
		const* BigInt* bigint, 
		char* out, 
		uint64_t max_out_len);

#endif
