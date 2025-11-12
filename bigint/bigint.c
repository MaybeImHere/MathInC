#include <stdlib.h>
#include <string.h>

#include "bigint.h"

/* If the length of the digits portion of the buffer was changed, use this
 * function to resize the scratch space to a length of digits_length + 1 or
 * greater.
 * ErrNullPtr: if bigint is NULL.
 * ErrAllocFailed: if unable to allocate more memory.
 */
static Err bigint_scratch_space_reset(BigInt* bigint) {
	if (!bigint) return ErrNullPtr;
	
	uint64_t new_buffer_capacity = bigint->digits_length * 2 + 1;
	if (bigint->buffer_capacity < new_buffer_capacity) {
		// we ran out of room in the buffer, as the digits array became
		// too long.
		uint32_t* new_buffer = realloc(bigint->buffer, 
				new_buffer_capacity);
		if (!new_buffer) return ErrAllocFailed;
		bigint->buffer = new_buffer;
		bigint->buffer_capacity = new_buffer_capacity;
		
		bigint->digits = bigint->buffer;
		
		bigint->scratch_space = 
			&(bigint->buffer[bigint->digits_length]);
		bigint->scratch_space_length = 
			bigint->buffer_capacity - bigint->digits_length;
		return ErrGood;
	} else {
		bigint->scratch_space = 
			&(bigint->buffer[bigint->digits_length]);	
		bigint->scratch_space_length =
			bigint->buffer_capacity - bigint->digits_length;
		return ErrGood;
	}
}

Err bigint_init(BigInt* bigint) {
	if (!bigint) return ErrNullPtr;
		
	bigint->buffer = calloc(1 + 2, sizeof(uint32_t));
	if (bigint->buffer == NULL) return ErrAllocFailed;
	bigint->buffer_capacity = 3;

	bigint->digits = buffer;
	bigint->digits_length = 1;

	bigint->scratch_space = &(buffer[1]);
	bigint->scratch_space_length = 2;

	bigint->is_negative = false;

	return ErrGood;
}

Err bigint_set_value_simple(BigInt* bigint, int32_t simple_value) {
	if (!bigint) return ErrNullPtr;
	
	bool is_negative = simple_value < 0;

	if (is_negative) {
		bigint->digits[0] = -simple_value;
		bigint->is_negative = true;
	} else {
		bigint->digits[0] = simple_value;
		bigint->is_negative = false;
	}

	bigint->digits_length = 1;
	
	Err err = bigint_scratch_space_reset(bigint);
	if (err == ErrAllocFailed) return ErrAllocFailed;
	if (err != ErrGood) return ErrInternal;

	return ErrGood;
}

typedef struct DivisionResult2Digit {
	uint32_t high;
	uint32_t low;
	uint32_t remainder;
} DivisionResult2Digit;

/* Divides the number most_sig . least_sig (where . is concatination) by 10 and
 * returns the quotient and remainder.
 */
static DivisionResult2Digit bigint_2_digit_div_10(uint32_t most_sig, uint32_t least_sig) {
	DivisionResult2Digit ret = { 0 };
	
	uint64_t converted = (((uint64_t)most_sig) << 32) + ((uint32_t)least_sig);
	ret.remainder = (uint32_t)(converted % 10);
	converted /= 10;
	
	ret.high = (uint32_t)(converted >> 32);
	ret.low = (uint32_t)(converted & 0xFFFFFFFF);

	return ret;
}

static uint32_t bigint_divide_scratch_space_10(Bigint* bigint, uint64_t* number_length) {
	uint64_t pos = 0;
	uint32_t remainder = 0;
	
	while (pos < *number_length) {
		DivisionResult2Digit result = bigint_2_digit_div_10(
				remainder, 
				bigint->scratch_space[pos]);
		remainder = result.remainder;
		bigint->scratch_space[pos] = result.low;
		pos++;
	}
	
	// check to see if the length of the number decreased.
	if (bigint->scratch_space[0] == 0) {
		(*number_length)--;
		// make sure to move the quotient so it starts at scratch_space[0]
		memmove(bigint->scratch_space, &(bigint->scratch_space[1]), output_length);
	}

	return remainder;
}

Err bigint_to_base10_string(
		const* BigInt* bigint, 
		char* out, 
		uint64_t max_out_len) {
	if (!bigint || !out) return ErrNullPtr;
	if (max_out_len < 2) return ErrInvalidParameter;

	// first we copy the number to the scratch space to prepare to call 
	// bigint_divide_scratch_space_10
	memcpy(bigint->scratch_space, bigint->digits, bigint->digits_length);

	uint64_t current_length = bigint->digits_length;
	// make sure to set the null byte.
	out[max_out_len - 1] = 0;
	// we need to go backwards since we get the least sig. digits first.
	uint64_t out_pos = max_out_len - 2;

	// we start with 1 since we include the null byte at the end.
	uint64_t characters_pushed = 1;

	// divide until the length of the number reaches 0, meaning we are done.
	while (current_length >= 1) {
		uint32_t digit = bigint_divide_scratch_space_10(bigint, &current_length);
		out[out_pos] = '0' + (char)(digit);
		characters_pushed++;

		// check if we ran out of space.
		if (out_pos == 0) break;
		out_pos--;
	}

	// make sure we move the string to the beginning, since we started at
	// the end first.
	memmove(out, &(out[out_pos]), (size_t)characters_pushed);

	return ErrGood;
}
