#ifndef __MASK_UTIL_H
#define __MASK_UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mask_rand.h"
#include "uint128.h"
#ifndef MAXSHARES
#define MAXSHARES 10
#endif

void SecAnd_128_nshares(UINT128* x, UINT128* y, UINT128* result, int n);
void SecAnd_64_nshares(uint64_t* x, uint64_t* y, uint64_t* result, int n);
void SecAnd_32_nshares(uint32_t* x, uint32_t* y, uint32_t* result, int n);
void SecAnd_16_nshares(uint16_t* x, uint16_t* y, uint16_t* result, int n);
void SecAnd_8_nshares(uint8_t* x, uint8_t* y, uint8_t* result, int n);

void Refresh_128_nshares(UINT128* x, UINT128* result, int n);
void Refresh_64_nshares(uint64_t* x, uint64_t* result, int n);
void Refresh_32_nshares(uint32_t* x, uint32_t* result, int n);
void Refresh_16_nshares(uint16_t* x, uint16_t* result, int n);
void Refresh_8_nshares(uint8_t* x, uint8_t* result, int n);

void RefreshMasks_64_nshares(uint64_t* x, uint64_t* result, int n);
void RefreshMasks_16_nshares(uint16_t* x, uint16_t* result, int n);

void SecAdd_128_nshares(UINT128* x, UINT128* y, UINT128* result, int n);
void SecAdd_64_nshares(uint64_t* x, uint64_t* y, uint64_t* result, int n);
void SecAdd_32_nshares(uint32_t* x, uint32_t* y, uint32_t* result, int n);
void SecAdd_16_nshares(uint16_t* x, uint16_t* y, uint16_t* result, int n);
void SecAdd_8_nshares(uint8_t* x, uint8_t* y, uint8_t* result, int n);

void SecOr_64_nshares(uint64_t* x, uint64_t* y, uint64_t* result, int n);
void SecOr_32_nshares(uint32_t* x, uint32_t* y, uint32_t* result, int n);
void SecOr_16_nshares(uint16_t* x, uint16_t* y, uint16_t* result, int n);
void SecOr_8_nshares(uint8_t* x, uint8_t* y, uint8_t* result, int n);

void A2B_128_nshares(UINT128* x, UINT128* result, int n);
void A2B_64_nshares(uint64_t* x, uint64_t* result, int n);
void A2B_16_nshares(uint16_t* x, uint16_t* result, int n);

uint16_t FullXor_16_nshares(uint16_t* x, int n);
uint16_t FullXor_8_nshares(uint8_t* x, int n);

void B2A_BitAdd_32_nshares(uint32_t* A, uint32_t x, int n);
void B2A_Bit_32_nshares(uint32_t* x, uint32_t* result, int n);
void B2A_BitAdd_16_nshares(uint16_t* A, uint16_t x, int n);
void B2A_Bit_16_nshares(uint16_t* x, uint16_t* result, int n);
void B2A_BitAdd_8_nshares(uint8_t* A, uint8_t x, int n);
void B2A_Bit_8_nshares(uint8_t* x, uint8_t* result, int n);

void B2A_16_nshares(uint16_t* x, uint16_t* result, int n);

void SecNonzeroBool_64_nshares(uint64_t* x, uint64_t* result, int n);
void SecNonzeroBool_32_nshares(uint32_t* x, uint32_t* result, int n);
void SecNonzeroBool_16_nshares(uint16_t* x, uint16_t* result, int n);
void SecNonzeroBool_8_nshares(uint8_t* x, uint8_t* result, int n);

void SecNonzeroArith_16_nshares(uint16_t* x, uint16_t* result, int n);

#endif
