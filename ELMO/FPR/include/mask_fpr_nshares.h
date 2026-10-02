#ifndef __MASK_FPR_NSHARES_H
#define __MASK_FPR_NSHARES_H
#include "mask_utils.h"
#endif

void SecFPR_nshares(uint8_t* s_mask, uint16_t* e_mask, uint64_t* m_mask, uint64_t* result, int n);

void SecFprMul_nshares(uint8_t* sx_mask, uint16_t* ex_mask, UINT128* xu_mask, \
					   uint8_t* sy_mask, uint16_t* ey_mask, UINT128* yu_mask, \
					   uint64_t* result, int n);

void SecFprNeg_nshares(uint64_t* x, uint64_t* result, int n);

uint64_t fpr_lrotate(uint64_t x, int n);

uint64_t fpr_rrotate(uint64_t x, int n);

void SecFprUrsh_nshares(uint64_t* x, uint16_t* c, uint64_t* result, int n);

void SecFprNorm64_nshares(uint64_t* x, uint16_t* e, int n);

void SecFprAdd_nshares(uint64_t* x_mask, uint64_t* y_mask, uint64_t* result, int n);
