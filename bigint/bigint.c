#include "bigint.h"

// 2^32 congruent 6 mod 10.
const uint32_t TWO_POW_32_MOD_10 = 6;

// Returns (2^32)^n mod 10
static uint32_t bigint_2_pow_32_pow_n_mod_10(uint32_t n) {
	if (n == 0) return 1;
	if (n == 1) return TWO_POW_32_MOD_10;
	
	uint32_t ret = TWO_POW_32_MOD_10;
	// first, we notice that (2^32) ^ n % 10 = 6 ^ n % 10
	// Next, we note that 6 % 10 = 6, and that 6^2 % 10 = 6.
	// Next, notice that we can break down (2^32) ^ n into
	// (2^32) ^ a32 * (2^32) ^ a31 * ... (2^32) ^ a0
	// where a_n is 2^n if the nth bit is 1, and 0 otherwise.
	// For example, (2^32)^0b10010 is equal to
	// (2^32) ^ 0 * 
	// (2^32) ^ (2^1) * 
	// (2^32) ^ 0 * 
	// (2^32) ^ 0 * 
	// (2^32) ^ (2^4)
	// 
	// Therefore, to compute (2^32)^n % 10 is equivalent to computing
	// (6 ^ 0 *
	// 6 ^ 2 *
	// 6 ^ 0 *
	// 6 ^ 0 *
	// 6 ^ 16) mod 10
	// By substituting in 6 = (2^32) % 10
	// Now, notice that 6 % 10 = 6, and 6^2 % 10 = 6.
	// That means that for any k > 0, 6^k = 1, since multiplying 6 by itself
	// always results in 6. If k = 0, 6^0 = 1, so that just drops out of the
	// above expression. That means we can simplify the above expression to
	// to
	// 6 * 6 % 10 = 6
	//
	
}

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
	uint32_t most_sig_quotient;
	uint32_t least_sig_quotient;
	uint32_t remainder;
} DivisionResult2Digit;

/* Divides the number most_sig . least_sig (where . is concatination) by 10 and
 * returns the quotient and remainder.
 */
static DivisionResult2Digit bigint_2_digit_div_10(uint32_t most_sig, uint32_t least_sig) {
	DivisionResult2Digit ret = { 0 };
	
	ret.most_sig_quotient = most_sig / 10;
	uint32_t first_remainder = most_sig % 10;

	// how do you divide digit_a digit_b by 10?

	return ret;
}

Err bigint_to_base10_string(
		const* BigInt* bigint, 
		char* out, 
		uint64_t max_out_len) {
	if (!bigint || !out) return ErrNullPtr;
	
	// implements long division.
	// if a is the value of the bigint, 10x+m = 0 mod 10, and 10x+m=a

	// we start at the end, since bigint->digits[0] is the least
	// significant digit.
	uint64_t digit_pos = bigint->digits_length - 1;
	
	uint32_t current_digit = bigint->digits[digit_pos];
	uint32_t scratch_pos = 0
	if (current_digit >= 10) {
		// we don't have to carry over anything. just divide and 
		// subtract the remainder.
		uint32_t quotient = current_digit / 10;
		uint32_t remainder = current_digit % 10;

		bigint->scratch_space[scratch_pos] = quotient;

	}

	return ErrGood;
}
