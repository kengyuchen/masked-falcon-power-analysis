#ifndef __MASK_RAND_H
#define __MASK_RAND_H

#include <stdlib.h>
#include <stdint.h>
#include "inner.h"
#include "uint128.h"

#define RANDBYTE 3072
#define RANDBYTEMASK 0x7ff

extern uint32_t randcount;
extern uint8_t prng_table[RANDBYTE];
extern size_t prng_pt;

void init_prng_table(uint64_t SEED);

unsigned mask_randbyte(void);

UINT128 rand128(void);

uint64_t rand64(void);

uint32_t rand32(void);

uint16_t rand16(void);

uint8_t rand8(void);

#endif