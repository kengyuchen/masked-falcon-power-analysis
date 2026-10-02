#include <stdio.h>
#include <time.h>
#include "mask_utils.h"
#include "mask_rand.h"
#include "uint128.h"
#include "mask_fpr_nshares.h"

#ifndef SHAREN
#define SHAREN 2
#endif

int mask_prng_is_init = 0;

static void init_nshares(uint64_t x, uint8_t* sx_mask, uint16_t* ex_mask, UINT128* xu_mask, int n){
	uint64_t xu = (x & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);
	ex_mask[0] = (uint16_t)((x >> 52) & 0x7FF);
	sx_mask[0] = (uint8_t)(x >> 63);
	xu_mask[0].value[0] = 0;
	xu_mask[0].value[1] = 0;
	xu_mask[0].value[2] = xu >> 32;
	xu_mask[0].value[3] = xu & (((uint64_t)1 << 32) - 1);
	for (int i = 1; i < n; i++){
		xu_mask[i] = rand128();
		xu_mask[0] = UINT128Sub(xu_mask[0], xu_mask[i]);
		ex_mask[i] = rand16();
		ex_mask[0] -= ex_mask[i];
		sx_mask[i] = rand8();
		sx_mask[0] ^= sx_mask[i]; 
	}
	return;
}

static void FPC_MUL_MASKED(uint64_t x_re, uint64_t x_im, uint64_t y_re, uint64_t y_im, uint64_t z[2]){
	if (mask_prng_is_init == 0){
		mask_prng_is_init = 1;
		uint64_t SEED = (uint64_t)time(NULL);
		init_prng_table(SEED);
	}
	int i;
	uint8_t x_re_s_mask[MAXSHARES], x_im_s_mask[MAXSHARES], y_re_s_mask[MAXSHARES], y_im_s_mask[MAXSHARES];
	uint16_t x_re_e_mask[MAXSHARES], x_im_e_mask[MAXSHARES], y_re_e_mask[MAXSHARES], y_im_e_mask[MAXSHARES];
	UINT128 x_re_u_mask[MAXSHARES], x_im_u_mask[MAXSHARES], y_re_u_mask[MAXSHARES], y_im_u_mask[MAXSHARES];
	uint64_t result_re_mask[MAXSHARES], result_im_mask[MAXSHARES];

	init_nshares(x_re, x_re_s_mask, x_re_e_mask, x_re_u_mask, SHAREN);
	init_nshares(x_im, x_im_s_mask, x_im_e_mask, x_im_u_mask, SHAREN);
	init_nshares(y_re, y_re_s_mask, y_re_e_mask, y_re_u_mask, SHAREN);
	init_nshares(y_im, y_im_s_mask, y_im_e_mask, y_im_u_mask, SHAREN);

	uint64_t tmp1[MAXSHARES], tmp2[MAXSHARES];
	SecFprMul_nshares(x_re_s_mask, x_re_e_mask, x_re_u_mask, \
	 			 	  y_re_s_mask, y_re_e_mask, y_re_u_mask, \
	 			 	  tmp1, SHAREN);
	SecFprMul_nshares(x_im_s_mask, x_im_e_mask, x_im_u_mask, \
	 			 	  y_im_s_mask, y_im_e_mask, y_im_u_mask, \
	 			 	  tmp2, SHAREN);
	SecFprNeg_nshares(tmp2, tmp2, SHAREN);
	SecFprAdd_nshares(tmp1, tmp2, result_re_mask, SHAREN);

	SecFprMul_nshares(x_re_s_mask, x_re_e_mask, x_re_u_mask, \
	 			 	  y_im_s_mask, y_im_e_mask, y_im_u_mask, \
	 			 	  tmp1, SHAREN);
	SecFprMul_nshares(x_im_s_mask, x_im_e_mask, x_im_u_mask, \
	 			 	  y_re_s_mask, y_re_e_mask, y_re_u_mask, \
	 			 	  tmp2, SHAREN);
	SecFprAdd_nshares(tmp1, tmp2, result_im_mask, SHAREN);


	uint64_t result_re = 0, result_im = 0;
	for (i = 0; i < SHAREN; i++){
		result_re ^= result_re_mask[i];
		result_im ^= result_im_mask[i];
	}
	z[0] = result_re;
	z[1] = result_im;
	return;
}
