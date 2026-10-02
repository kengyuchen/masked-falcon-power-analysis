#ifndef __UINT128
#define __UINT128

#include <stdint.h>
#include <stdio.h>

typedef struct uint128 {
	uint32_t value[4];
} UINT128;

void print128(UINT128 x);

#define ZERO128(x) { \
	x.value[0] = 0; \
	x.value[1] = 0; \
	x.value[2] = 0; \
	x.value[3] = 0; \
}

UINT128 UINT128Add(UINT128 x, UINT128 y);

UINT128 UINT128Add_t(UINT128 x, UINT128 y);

UINT128 UINT128Neg(UINT128 x);

UINT128 UINT128Sub(UINT128 x, UINT128 y);

UINT128 UINT128Mul(UINT128 x, UINT128 y);

UINT128 UINT128Xor(UINT128 x, UINT128 y);

UINT128 UINT128And(UINT128 x, UINT128 y);

UINT128 UINT128LSH(UINT128 x, int n);

UINT128 UINT128LSH32(UINT128 x);

UINT128 UINT128LSH64(UINT128 x);

#endif