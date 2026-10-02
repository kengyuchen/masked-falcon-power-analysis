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

#define NOTRACES 200 // Make sure this matches the number in the python script
#define SHAREN 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elmoasmfunctionsdef.h"

#include "include/mask_falcon.h"

#define ELMO_READER ((volatile uint32_t *) 0xE1000000)

uint64_t get_fpr_elmo(void);

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
	uint64_t f_re, f_im, c_re, c_im, res_re;		
	uint64_t result[2];
	f_re = get_fpr_elmo();
	f_im = get_fpr_elmo();
    
    for(i=0;i<NOTRACES;i++){
        
        // Set up inputs and key.
		c_re = get_fpr_elmo();
		c_im = get_fpr_elmo();
		mask_prng_is_init = 1;
		init_prng_table(i);
        
		starttrigger();
        
		FPC_MUL_MASKED(f_re, f_im, c_re, c_im, result);
		res_re = result[0];

		endtrigger();
        
		for (int j = 0; j < 8; j++){
			uint8_t b = (res_re >> (56 - j * 8)) & 0xFF;
			printbyte(&b);
		}
    }
    
    endprogram();

    return 0;
}
