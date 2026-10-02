The code implements the masking countermeasure on [Falcon](https://falcon-sign.info/), a post-quantum digital signature scheme planned to be standardized in [NIST Post-Quantum Cryptography Standardization Process](https://csrc.nist.gov/projects/post-quantum-cryptography).

The code is referenced from Falcon's [Round-3 Submission](https://csrc.nist.gov/CSRC/media/Projects/post-quantum-cryptography/documents/round-3/submissions/Falcon-Round3.zip); specifically, the code under the directory  `falcon-round3/Extra/c/`.



## Basic Usage

1. In `Makefile`,
    ```makefile
    MASK_FLAGS := -DMASK_FALCON -DSHAREN=2
    ```
    Change `-DSHAREN` to use other masking order.

2. In `Makefile`,
	```makefile
	# MASK_FLAGS += -DMASK_FALCON_PRNG
	```
	Uncomment this to enable Falcon built-in PRNG for the randomness in masking.
3. Use `make all` to recompile all the object files without masking.

4. Use `make mask` to recompile all the object files with masking countermeasure.

5. `./test_falcon` will test whether the program pass the benchmark test.

6. `./speed` will measure the speed of the program.



## Remarks about Changes in Reference Code

### `inner.h`

```c
void Zf(poly_mul_fft)(fpr *restrict a, const fpr *restrict b, unsigned logn);
#ifdef MASK_FALCON
void Zf(poly_mul_fft_mask)(fpr *restrict a, const fpr *restrict b, unsigned logn);
#endif
```



### `sign.c`

Two polynomial multiplications in function `do_sign_dyn`  is changed to the masked version.

```c
/* In function do_sign_dyn() and do_sign_tree() */

/*
 * Apply the lattice basis to obtain the real target
 * vector (after normalization with regards to modulus).
 */
Zf(FFT)(t0, logn);
ni = fpr_inverse_of_q;
memcpy(t1, t0, n * sizeof *t0);
#ifdef MASK_FALCON
Zf(poly_mul_fft_mask)(t1, b01, logn);
Zf(poly_mulconst)(t1, fpr_neg(ni), logn);
Zf(poly_mul_fft_mask)(t0, b11, logn);
Zf(poly_mulconst)(t0, ni, logn);
#else
Zf(poly_mul_fft)(t1, b01, logn);
Zf(poly_mulconst)(t1, fpr_neg(ni), logn);
Zf(poly_mul_fft)(t0, b11, logn);
Zf(poly_mulconst)(t0, ni, logn);
#endif
```

### `fft.c`

Header files and functions for masking are included here:

```c
#ifdef MASK_FALCON
#include "mask_falcon.h"

void
Zf(poly_mul_fft_mask)(
	fpr *restrict a, const fpr *restrict b, unsigned logn)
{
	size_t n, hn, u;

	n = (size_t)1 << logn;
	hn = n >> 1;
#if FALCON_AVX2 // yyyAVX2+1
	if (n >= 8) {
		for (u = 0; u < hn; u += 4) {
			__m256d a_re, a_im, b_re, b_im, c_re, c_im;

			a_re = _mm256_loadu_pd(&a[u].v);
			a_im = _mm256_loadu_pd(&a[u + hn].v);
			b_re = _mm256_loadu_pd(&b[u].v);
			b_im = _mm256_loadu_pd(&b[u + hn].v);
			c_re = FMSUB(
				a_re, b_re, _mm256_mul_pd(a_im, b_im));
			c_im = FMADD(
				a_re, b_im, _mm256_mul_pd(a_im, b_re));
			_mm256_storeu_pd(&a[u].v, c_re);
			_mm256_storeu_pd(&a[u + hn].v, c_im);
		}
	} else {
		for (u = 0; u < hn; u ++) {
			uint64_t x_re = ((uint64_t*)a)[u];
			uint64_t x_im = ((uint64_t*)a)[u + hn];
			uint64_t y_re = ((uint64_t*)b)[u];
			uint64_t y_im = ((uint64_t*)b)[u + hn];
			uint64_t z[2];
			FPC_MUL_MASKED(x_re, x_im, y_re, y_im, z);
			a[u] = *(fpr*)&z[0];
			a[u + hn] = *(fpr*)&z[1];
		}
	}
#else // yyyAVX2+0
	for (u = 0; u < hn; u ++) {
		uint64_t x_re = ((uint64_t*)a)[u];
		uint64_t x_im = ((uint64_t*)a)[u + hn];
		uint64_t y_re = ((uint64_t*)b)[u];
		uint64_t y_im = ((uint64_t*)b)[u + hn];
		uint64_t z[2];
		FPC_MUL_MASKED(x_re, x_im, y_re, y_im, z);
		a[u] = *(fpr*)&z[0];
		a[u + hn] = *(fpr*)&z[1];
	}
#endif // yyyAVX2-
}

#endif
```

