#include <stdio.h>

#include "../bigint/bigint.h"

const char* err_to_string(Err err) {	
	switch(err) {
	case ErrGood:			return "No error";
	case ErrNullPtr:		return "Null Pointer";
	case ErrAllocFailed:		return "Alloc Failed";
	case ErrInvalidParameter:	return "Invalid Parameter";
	case ErrInternal:		return "Internal Error";
	default:			return "Unknown Error";
	}
}

void test_bigint_init() {
	BigInt bigint = { 0 };
	Err err = bigint_init(&bigint);	

	if (err != ErrGood) {
		printf("XXX bigint_init(): %s\n", err_to_string(err));
		return;
	} else {
		printf("    bigint_init()\n");
	}
	return;
}

void bigint_test() {
	// first test bigint_init
	BigInt bigint = { 0 };
	Err err = bigint_init(&bigint);	

	if (err != ErrGood) {
		printf("XXX bigint_init(): %s\n", err_to_string(err));
		return;
	} else {
		printf("    bigint_init()\n");
	}
	
	// now test bigint_set_value_simple
	err = bigint_set_value_simple(&bigint, 72946);
	if (err != ErrGood) {
		printf("XXX bigint_set_value_simple(): %s\n", err_to_string(err));
		return;
	} else {
		print("    bigint_set_value_simple()\n");
	}

	return;
}

int main() {
	return 0;
}
