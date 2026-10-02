#include "mask_fpr_nshares.h"


void SecFPR_nshares(uint8_t* s_mask, uint16_t* e_mask, uint64_t* m_mask, uint64_t* result, int n){
/* 
Input
	s_mask: Boolean
	e_mask: Arithmetic
	m_mask: Boolean
Output
	result: Boolean
*/
	uint64_t tmp64[MAXSHARES];
	uint16_t tmp16[MAXSHARES], negtmp16[MAXSHARES];
	int i;

/*
	e += 1076;
	t = (uint32_t)e >> 31;
	m &= (uint64_t)t - 1;
*/
	e_mask[0] += 1076;
	A2B_16_nshares(e_mask, e_mask, n);
	for (i = 0; i < n; i++){
		tmp64[i] = -(uint64_t)(e_mask[i] >> 15);
	}
	tmp64[0] = ~tmp64[0];
	SecAnd_64_nshares(m_mask, tmp64, m_mask, n);

/*
	t = (uint32_t)(m >> 54);
	e &= -(int)t;	
	x = (((uint64_t)s << 63) | (m >> 2)) + ((uint64_t)(uint32_t)e << 52);
*/
	uint64_t x_mask[MAXSHARES];
	for (i = 0; i < n; i++){
		tmp16[i] = ((m_mask[i] >> 54) & 1);
		negtmp16[i] =  -tmp16[i];
	}
	SecAnd_16_nshares(e_mask, negtmp16, e_mask, n);
	SecAdd_16_nshares(e_mask, tmp16, e_mask, n);
	Refresh_16_nshares(e_mask, e_mask, n);
	Refresh_8_nshares(s_mask, s_mask, n);
	for (i = 0; i < n; i++){
		x_mask[i] = (((uint64_t)s_mask[i] << 63) | (uint64_t)(e_mask[i] & 0X7FF) << 52 | (((m_mask[i] & (((uint64_t)1 << 54) - 1))) >> 2));
	}

/*
	f = (unsigned)m & 7U;
	x += (0xC8U >> f) & 1;
*/
	uint8_t f0[MAXSHARES], f1[MAXSHARES], f2[MAXSHARES];
	for (i = 0; i < n; i++){
		f0[i] = (uint8_t)(m_mask[i]);
		f1[i] = (uint8_t)((m_mask[i] >> 1));
		f2[i] = (uint8_t)((m_mask[i] >> 2));
	}
	Refresh_8_nshares(f0, f0, n);
	uint8_t f[MAXSHARES];
	SecOr_8_nshares(f0, f2, f, n);
	SecAnd_8_nshares(f, f1, f, n);
	for (i = 0; i < n; i++){
		tmp64[i] = (f[i] & 1);
	}
	SecAdd_64_nshares(x_mask, tmp64, x_mask, n);
	for (i = 0; i < n; i++){
		result[i] = x_mask[i];
	}
	return;
}

void SecFprMul_nshares(uint8_t* sx_mask, uint16_t* ex_mask, UINT128* xu_mask, \
					   uint8_t* sy_mask, uint16_t* ey_mask, UINT128* yu_mask, \
					   uint64_t* result, int n){
/* 
Input
	sx_mask, sy_mask: Boolean
	ex_mask, ex_mask: Arithmetic
	xu_mask, yu_mask: 128-bit Arithmetic
Output
	result: Boolean
*/
	int i, j;
	uint64_t tmp64[MAXSHARES];

	uint8_t s[MAXSHARES];
	for (i = 0; i < n; i++){
		s[i] = sx_mask[i] ^ sy_mask[i];
	}
	uint16_t e[MAXSHARES];
	for (i = 0; i < n; i++){
		e[i] = ex_mask[i] + ey_mask[i];
	}
	e[0] -= 2100;
	UINT128 zu_mask[MAXSHARES];
	for (i = 0; i < n; i++){
		zu_mask[i] = UINT128Mul(xu_mask[i], yu_mask[i]);
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			UINT128 r, t;
			r = rand128();
			zu_mask[i] = UINT128Sub(zu_mask[i], r);
			t = UINT128Mul(xu_mask[i], yu_mask[j]);
			r = UINT128Add(r, t);
			t = UINT128Mul(xu_mask[j], yu_mask[i]);
			r = UINT128Add(r, t);
			zu_mask[j] = UINT128Add(zu_mask[j], r);
		}
	}
	A2B_128_nshares(zu_mask, zu_mask, n);
	uint64_t zu[MAXSHARES];
	for (i = 0; i < n; i++){
		zu[i] = (uint64_t)zu_mask[i].value[0] << 46 | \
				(uint64_t)zu_mask[i].value[1] << 14 | \
				(uint64_t)zu_mask[i].value[2] >> 18;
	}
/*
	zu |= ((z0 | z1) + 0x01FFFFFF) >> 25;
	zv = (zu >> 1) | (zu & 1);
	w = zu >> 55;
	zu ^= (zu ^ zv) & -w;
*/
	uint64_t sticky[MAXSHARES];
	for (i = 0; i < n; i++){
		sticky[i] = (uint64_t)(zu_mask[i].value[2] & (((uint64_t)1 << 19) - 1)) << 32 | zu_mask[i].value[3];
	}
	SecNonzeroBool_64_nshares(sticky, sticky, n);

	uint64_t zv[MAXSHARES], w[MAXSHARES], neg_w[MAXSHARES];
	for (i = 0; i < n; i++){
		zv[i] = (zu[i] >> 1);
		w[i] = (zu[i] >> 55);
		neg_w[i] = -(w[i] & 1);
	}
	for (i = 0; i < n; i++){
		tmp64[i] = (zu[i] ^ zv[i]);
	}
	Refresh_64_nshares(neg_w, neg_w, n);
	SecAnd_64_nshares(tmp64, neg_w, tmp64, n);
	for (i = 0; i < n; i++){
		zu[i] ^= tmp64[i];
	}
	SecOr_64_nshares(zu, sticky, zu, n);

/*
	e = ex + ey - 2100 + (int)w;
*/
	uint16_t w_16[MAXSHARES];
	for (i = 0; i < n; i++){
		w_16[i] = (uint16_t)w[i];
	}
	B2A_Bit_16_nshares(w_16, w_16, n);
	for (i = 0; i < n; i++){
		e[i] += w_16[i];
	}

/*
	d = ((ex + 0x7FF) & (ey + 0x7FF)) >> 11;
	zu &= -(uint64_t)d;
*/
	uint16_t is_nonzero_ex_16[MAXSHARES], is_nonzero_ey_16[MAXSHARES];
	uint8_t is_nonzero_ex[MAXSHARES], is_nonzero_ey[MAXSHARES], d[MAXSHARES];
	SecNonzeroArith_16_nshares(ex_mask, is_nonzero_ex_16, n);
	SecNonzeroArith_16_nshares(ey_mask, is_nonzero_ey_16, n);
	for (i = 0; i < n; i++){
		is_nonzero_ex[i] = is_nonzero_ex_16[i];
		is_nonzero_ey[i] = is_nonzero_ey_16[i];
	}
	SecAnd_8_nshares(is_nonzero_ex, is_nonzero_ey, d, n);
	for (i = 0; i < n; i++){
		tmp64[i] = -(uint64_t)(d[i] & 1);
	}
	SecAnd_64_nshares(zu, tmp64, zu, n);
	SecFPR_nshares(s, e, zu, result, n);

	return;
}

void SecFprNeg_nshares(uint64_t* x, uint64_t* result, int n){
	int i;
	result[0] = x[0] ^ ((uint64_t)1 << 63);
	for (i = 1; i < n; i++){
		result[i] = x[i];
	}
	return;
}

uint64_t fpr_lrotate(uint64_t x, int n){
	uint64_t ulshx, urshx;
	ulshx = x ^ ((x ^ (x << 32)) & -(uint64_t)(n >> 5));
	ulshx = ulshx << (n & 31);
	int urshn = 64 - n;
	urshx = x ^ ((x ^ (x >> 32)) & -(uint64_t)(urshn >> 5));
	urshx = urshx ^ ((x ^ urshx) & -(uint64_t)(urshn >> 6));
	urshx = urshx >> (urshn & 31);
	return ulshx | urshx;
}

uint64_t fpr_rrotate(uint64_t x, int n){
	uint64_t ulshx, urshx;
	int ulshn = 64 - n;
	ulshx = x ^ ((x ^ (x << 32)) & -(uint64_t)(ulshn >> 5));
	ulshx = ulshx ^ ((x ^ ulshx) & -(uint64_t)(ulshn >> 6));
	ulshx = ulshx << (ulshn & 31);
	urshx = x ^ ((x ^ (x >> 32)) & -(uint64_t)(n >> 5));
	urshx = urshx >> (n & 31);
	return urshx | ulshx;
}

void SecFprUrsh_nshares(uint64_t* x, uint16_t* c, uint64_t* result, int n){
/*
Input
	x: Boolean
	c: Arithmetic, between 0 to 63
	result: Boolean
*/
	uint64_t urshx[MAXSHARES], mask[MAXSHARES];
	int i, j;
	mask[0] = ((uint64_t)1 << 63);
	for (j = 1; j < n; j++){
		mask[j] = 0;
	}
	for (j = 0; j < n; j++){
		urshx[j] = fpr_rrotate(x[j], c[0]);
		mask[j] = fpr_rrotate(mask[j], c[0]);
	}
	for (i = 1; i < n; i++){
		RefreshMasks_64_nshares(urshx, urshx, n);
		RefreshMasks_64_nshares(mask, mask, n);
		for (j = 0; j < n; j++){
			urshx[j] = fpr_rrotate(urshx[j], c[i]);
			mask[j] = fpr_rrotate(mask[j], c[i]);
		}
	}

	for (i = 1; i <= 32; i <<= 1){
		for (j = 0; j < n; j++){
			mask[j] ^= (mask[j] >> i);
		}
	}
	SecAnd_64_nshares(urshx, mask, result, n);
	for (j = 0; j < n; j++){
		urshx[j] ^= result[j];
		urshx[j] ^= (result[j] & 1);
	}
	SecNonzeroBool_64_nshares(urshx, urshx, n);
	for (j = 0; j < n; j++){
		result[j] = (result[j] & (uint64_t)(-2)) ^ urshx[j];
	}
	return;
}

void SecFprNorm64_nshares(uint64_t* x, uint16_t* e, int n){
	uint64_t tmp64_1[MAXSHARES], tmp64_2[MAXSHARES];
	uint32_t nt32[MAXSHARES];
	uint16_t nt16[MAXSHARES];
	uint8_t nt8[MAXSHARES];
	int i;
	e[0] -= 63;

	for (i = 0; i < n; i++){
		tmp64_1[i] = x[i] ^ (x[i] << 32);
		nt32[i] = (x[i] >> 32);
	}
	SecNonzeroBool_32_nshares(nt32, nt32, n);
	for (i = 0; i < n; i++){
		tmp64_2[i] = -(uint64_t)nt32[i];
	}
	tmp64_2[0] = ~tmp64_2[0];
	SecAnd_64_nshares(tmp64_1, tmp64_2, tmp64_1, n);
	for (i = 0; i < n; i++){
		x[i] ^= tmp64_1[i];
	}
	B2A_Bit_32_nshares(nt32, nt32, n);
	for (i = 0; i < n; i++){
		e[i] += (nt32[i] << 5);
	}

	for (i = 0; i < n; i++){
		tmp64_1[i] = x[i] ^ (x[i] << 16);
		nt16[i] = (x[i] >> 48);
	}
	SecNonzeroBool_16_nshares(nt16, nt16, n);
	for (i = 0; i < n; i++){
		tmp64_2[i] = -(uint64_t)nt16[i];
	}
	tmp64_2[0] = ~tmp64_2[0];
	SecAnd_64_nshares(tmp64_1, tmp64_2, tmp64_1, n);
	for (i = 0; i < n; i++){
		x[i] ^= tmp64_1[i];
	}
	B2A_Bit_16_nshares(nt16, nt16, n);
	for (i = 0; i < n; i++){
		e[i] += (nt16[i] << 4);
	}

	for (i = 0; i < n; i++){
		tmp64_1[i] = x[i] ^ (x[i] << 8);
		nt8[i] = (x[i] >> 56);
	}
	SecNonzeroBool_8_nshares(nt8, nt8, n);
	for (i = 0; i < n; i++){
		tmp64_2[i] = -(uint64_t)nt8[i];
	}
	tmp64_2[0] = ~tmp64_2[0];
	SecAnd_64_nshares(tmp64_1, tmp64_2, tmp64_1, n);
	for (i = 0; i < n; i++){
		x[i] ^= tmp64_1[i];
		nt16[i] = nt8[i];
	}
	B2A_Bit_16_nshares(nt16, nt16, n);
	for (i = 0; i < n; i++){
		e[i] += (nt16[i] << 3);
	}

	for (i = 0; i < n; i++){
		tmp64_1[i] = x[i] ^ (x[i] << 4);
		nt8[i] = (x[i] >> 60);
	}
	SecNonzeroBool_8_nshares(nt8, nt8, n);
	for (i = 0; i < n; i++){
		tmp64_2[i] = -(uint64_t)nt8[i];
	}
	tmp64_2[0] = ~tmp64_2[0];
	SecAnd_64_nshares(tmp64_1, tmp64_2, tmp64_1, n);
	for (i = 0; i < n; i++){
		x[i] ^= tmp64_1[i];
		nt16[i] = nt8[i];
	}
	B2A_Bit_16_nshares(nt16, nt16, n);
	for (i = 0; i < n; i++){
		e[i] += (nt16[i] << 2);
	}

	for (i = 0; i < n; i++){
		tmp64_1[i] = x[i] ^ (x[i] << 2);
		nt8[i] = (x[i] >> 62);
	}
	SecNonzeroBool_8_nshares(nt8, nt8, n);
	for (i = 0; i < n; i++){
		tmp64_2[i] = -(uint64_t)nt8[i];
	}
	tmp64_2[0] = ~tmp64_2[0];
	SecAnd_64_nshares(tmp64_1, tmp64_2, tmp64_1, n);
	for (i = 0; i < n; i++){
		x[i] ^= tmp64_1[i];
		nt16[i] = nt8[i];
	}
	B2A_Bit_16_nshares(nt16, nt16, n);
	for (i = 0; i < n; i++){
		e[i] += (nt16[i] << 1);
	}

	for (i = 0; i < n; i++){
		tmp64_1[i] = x[i] ^ (x[i] << 1);
		nt16[i] = (x[i] >> 63);
	}
	for (i = 0; i < n; i++){
		tmp64_2[i] = -(uint64_t)nt16[i];
	}
	tmp64_2[0] = ~tmp64_2[0];
	SecAnd_64_nshares(tmp64_1, tmp64_2, tmp64_1, n);
	for (i = 0; i < n; i++){
		x[i] ^= tmp64_1[i];
	}
	B2A_Bit_16_nshares(nt16, nt16, n);
	for (i = 0; i < n; i++){
		e[i] += nt16[i];
	}

	return;
}

void SecFprAdd_nshares(uint64_t* x_mask, uint64_t* y_mask, uint64_t* result, int n){
/*
Input
	x_mask: Boolean
	y_mask: Boolean
Output
	result: Boolean
*/
	int i;
	uint64_t tmp64[MAXSHARES], one[MAXSHARES];
	uint16_t tmp16[MAXSHARES];
	for (i = 1; i < n; i++){
		one[i] = 0;
	}
	one[0] = 1;

/*
	m = ((uint64_t)1 << 63) - 1;
	za = (x & m) - (y & m);
	cs = (uint32_t)(za >> 63)
		| ((1U - (uint32_t)(-za >> 63)) & (uint32_t)(x >> 63));
	m = (x ^ y) & -(uint64_t)cs;
	x ^= m;
	y ^= m;
*/
	uint64_t xm[MAXSHARES], ym[MAXSHARES];
	uint64_t m = ((uint64_t)1 << 63) - 1;

	for (i = 0; i < n; i++){
		xm[i] = x_mask[i] & m;
		ym[i] = y_mask[i] & m;
	}
	ym[0] = ~ym[0];


	uint64_t d[MAXSHARES];
	uint64_t b64[MAXSHARES], cs64[MAXSHARES], xy[MAXSHARES];
	uint8_t sd[MAXSHARES], b8[MAXSHARES], sx[MAXSHARES], cs8[MAXSHARES];

	SecAdd_64_nshares(xm, ym, d, n);

	for (i = 0; i < n; i++){
		sd[i] = d[i] >> 63; // x - y - 1 < 0
	}
	for (i = 0; i < n; i++){
		tmp64[i] = d[i];
	}
	tmp64[0] ^= ((uint64_t)1 << 63);
	tmp64[0] = ~tmp64[0];
	SecNonzeroBool_64_nshares(tmp64, b64, n); // x - y - 1 != 01...1
	for (i = 0; i < n; i++){
		sd[i] ^= b64[i];
	}
	d[0] = ~d[0];
	SecNonzeroBool_64_nshares(d, b64, n); // x - y - 1 != 11...1
	for (i = 0; i < n; i++){
		sd[i] ^= b64[i];
	}

	for (i = 0; i < n; i++){
		b8[i] = (b64[i] & 1);
		sx[i] = (x_mask[i] >> 63);
	}
	b8[0] = ~b8[0];
	SecAnd_8_nshares(b8, sx, cs8, n);
	SecOr_8_nshares(cs8, sd, cs8, n);
	for (i = 0; i < n; i++){
		cs64[i] = -(uint64_t)(cs8[i] & 1);
	}

	for (i = 0; i < n; i++){
		xy[i] = x_mask[i] ^ y_mask[i];
	}
	SecAnd_64_nshares(xy, cs64, xy, n);

	uint64_t zx_mask[MAXSHARES], zy_mask[MAXSHARES];
	for (i = 0; i < n; i++){
		zx_mask[i] = x_mask[i] ^ xy[i];
		zy_mask[i] = y_mask[i] ^ xy[i];
	}
	
/*
	ex = (int)(x >> 52);
	sx = ex >> 11;
	ex &= 0x7FF;
	m = (uint64_t)(uint32_t)((ex + 0x7FF) >> 11) << 52;
	xu = ((x & (((uint64_t)1 << 52) - 1)) | m) << 3;
	ex -= 1078;
	ey = (int)(y >> 52);
	sy = ey >> 11;
	ey &= 0x7FF;
	m = (uint64_t)(uint32_t)((ey + 0x7FF) >> 11) << 52;
	yu = ((y & (((uint64_t)1 << 52) - 1)) | m) << 3;
	ey -= 1078;
*/
	m = (((uint64_t)1 << 52) - 1);

	uint16_t ex_mask[MAXSHARES], ey_mask[MAXSHARES];
	uint8_t sx_mask[MAXSHARES], sy_mask[MAXSHARES];
	uint64_t xu[MAXSHARES], yu[MAXSHARES];
	for (i = 0; i < n; i++){
		ex_mask[i] = ((zx_mask[i] >> 52) & 0x7FF);
		sx_mask[i] = (zx_mask[i] >> 63);
		ey_mask[i] = ((zy_mask[i] >> 52) & 0x7FF);
		sy_mask[i] = (zy_mask[i] >> 63);
	}
	SecNonzeroBool_16_nshares(ex_mask, tmp16, n);
	for (i = 0; i < n; i++){
		xu[i] = (((zx_mask[i] & m) | ((uint64_t)tmp16[i] << 52)) << 3);
	}
	B2A_16_nshares(ex_mask, ex_mask, n);
	ex_mask[0] -= 1078;
	SecNonzeroBool_16_nshares(ey_mask, tmp16, n);
	for (i = 0; i < n; i++){
		yu[i] = (((zy_mask[i] & m) | ((uint64_t)tmp16[i] << 52)) << 3);
	}
	B2A_16_nshares(ey_mask, ey_mask, n);
	ey_mask[0] -= 1078;

/*
	cc = ex - ey;
	yu &= -(uint64_t)((uint32_t)(cc - 60) >> 31);
	cc &= 63;

	m = fpr_ulsh(1, cc) - 1;
	yu |= (yu & m) + m;
	yu = fpr_ursh(yu, cc);
*/
	uint16_t cc[MAXSHARES], cc60[MAXSHARES];
	for (i = 0; i < n; i++){
		cc60[i] = ex_mask[i] - ey_mask[i];
		cc[i] = cc60[i] & 63;
	}
	cc60[0] -= 60;
	A2B_16_nshares(cc60, cc60, n);
	for (i = 0; i < n; i++){
		tmp64[i] = -(uint64_t)(cc60[i] >> 15);
	}
	SecAnd_64_nshares(yu, tmp64, yu, n);
	SecFprUrsh_nshares(yu, cc, yu, n);
	
/*
	xu += yu - ((yu << 1) & -(uint64_t)(sx ^ sy));
	FPR_NORM64(xu, ex);
*/
	uint64_t myu[MAXSHARES], sxy[MAXSHARES];
	for (i = 0; i < n; i++){
		myu[i] = yu[i];
	}
	myu[0] = ~myu[0];
	SecAdd_64_nshares(myu, one, myu, n);
	Refresh_64_nshares(yu, yu, n);
	for (i = 0; i < n; i++){
		tmp64[i] = yu[i] ^ myu[i];
		sxy[i] = -(uint64_t)(sx_mask[i] ^ sy_mask[i]);
	}
	SecAnd_64_nshares(tmp64, sxy, tmp64, n);
	for (i = 0; i < n; i++){
		yu[i] ^= tmp64[i];
	}
	SecAdd_64_nshares(xu, yu, xu, n);
	SecFprNorm64_nshares(xu, ex_mask, n);

/*
	xu |= ((uint32_t)xu & 0x1FF) + 0x1FF;
	xu >>= 9;
	ex += 9;
*/
	for (i = 0; i < n; i++){
		tmp16[i] = xu[i] & 0X3FF;
	}
	SecNonzeroBool_16_nshares(tmp16, tmp16, n);
	for (i = 0; i < n; i++){
		xu[i] = ((xu[i] >> 9) & (uint64_t)(-2)) ^ tmp16[i];
	}
	ex_mask[0] += 9;
	Refresh_8_nshares(sx_mask, sx_mask, n);
	SecFPR_nshares(sx_mask, ex_mask, xu, result, n);
	return;
}


