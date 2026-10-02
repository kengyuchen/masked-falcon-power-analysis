/*
 * University of Bristol – Open Access Software Licence
 * Copyright (c) 2016, The University of Bristol, a chartered
 * corporation having Royal Charter number RC000648 and a charity
 * (number X1121) and its place of administration being at Senate
 * House, Tyndall Avenue, Bristol, BS8 1TH, United Kingdom.
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Any use of the software for scientific publications or commercial
 * purposes should be reported to the University of Bristol
 * (OSI-notifications@bristol.ac.uk and quote reference 2668). This is
 * for impact and usage monitoring purposes only.
 *
 * Enquiries about further applications and development opportunities
 * are welcome. Please contact elisabeth.oswald@bristol.ac.uk
 */

#define NOTRACES 5000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elmoasmfunctionsdef.h"

#include "include/mask_falcon.h"

typedef struct {
    uint8_t s;
    uint16_t e;
    uint64_t m;
} Fpu;

Fpu fpuSplit(uint64_t x) {
    Fpu fpu;
    fpu.s = (uint8_t)((x >> 63) & 0x1);
    fpu.e = (uint16_t)((x >> 52) & 0x7FF);
    fpu.m = x & 0xFFFFFFFFFFFFFULL;
    return fpu;
}

uint64_t fpuMerge(Fpu fpu) {
    uint64_t result = 0;
    result |= ((uint64_t)(fpu.s & 0x1) << 63);
    result |= ((uint64_t)(fpu.e & 0x7FF) << 52);
    result |= (fpu.m & 0xFFFFFFFFFFFFFULL);
    return result;
}

#define ELMO_READER ((volatile uint32_t *) 0xE1000000)

uint64_t get_fpr_elmo(void) {
	uint32_t high, low;
	uint64_t c;

    high = *ELMO_READER; 
    low  = *ELMO_READER;
    c = ((uint64_t)high << 32) | low;
	return c;
}

int main(void) {
    
    int i;

	/*uint8_t sx_mask[SHAREN], sy_mask[SHAREN];*/
	/*uint16_t ex_mask[SHAREN], ey_mask[SHAREN];*/
	/*uint64_t result[SHAREN];*/
	/*UINT128 xu_mask[SHAREN], yu_mask[SHAREN];*/

	uint64_t f_re, f_im, c_re, c_im, res_re;
	f_re = 4633974435773956355ULL; // 62.3687303719362 
	f_im = 4625802639892893324ULL; // 18.15231417894988
    
    for(i=0;i<NOTRACES;i++){
        
        // Set up inputs and key.
		c_re = get_fpr_elmo();
		c_im = get_fpr_elmo();

		/*init_prng_table(i);*/
		/*init_nshares(x_re, sx_mask, ex_mask, xu_mask, SHAREN);*/
		/*init_nshares(y_re, sy_mask, ey_mask, yu_mask, SHAREN);*/
        
        starttrigger();
        
			res_re = (uint64_t)fpr_mul((fpr)f_re, (fpr)c_re);
			/*SecFprMul_nshares(sx_mask, ex_mask, xu_mask, sy_mask, ey_mask, yu_mask, result, SHAREN);*/
        
        endtrigger();
        
		/*uint8_t b = prng_pt & 0XFF;*/
		for (int j = 0; j < 8; j++){
	        uint8_t b = (res_re >> (56 - j * 8)) & 0xFF;
	        printbyte(&b);
		}
    }
    
    endprogram();

    return 0;
}
