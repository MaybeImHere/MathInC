#ifndef _ERR_GUARD
#define _ERR_GUARD

typedef enum Err {
	ErrGood,
	ErrNullPtr,
	ErrAllocFailed,
	ErrInvalidParameter,
	ErrInternal
};

#endif
