#include "mask_utils.h"
#ifndef MAXSHARES
#define MAXSHARES 10
#endif

void SecAnd_128_nshares(UINT128* x, UINT128* y, UINT128* result, int n){
	int i, j;
	UINT128 temp[MAXSHARES];
	for (i = 0; i < n; i++){
		temp[i] = UINT128And(x[i], y[i]);
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			UINT128 rij, rji;
			rij = rand128();
			rji = UINT128Xor(UINT128And(x[i], y[j]), rij);
			rji = UINT128Xor(rji, UINT128And(x[j], y[i]));
			temp[i] = UINT128Xor(temp[i], rij);
			temp[j] = UINT128Xor(temp[j], rji);
		}
	}
	for (i = 0; i < n; i++){
		result[i] = temp[i];
	}
	return;
}

void SecAnd_64_nshares(uint64_t* x, uint64_t* y, uint64_t* result, int n){
	int i, j;
	uint64_t temp[MAXSHARES];
	for (i = 0; i < n; i++){
		temp[i] = x[i] & y[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			uint64_t rij, rji;
			rij = rand64();
			rji = (x[i] & y[j]) ^ rij;
			rji ^= (x[j] & y[i]);
			temp[i] = temp[i] ^ rij;
			temp[j] = temp[j] ^ rji;
		}
	}
	for (i = 0; i < n; i++){
		result[i] = temp[i];
	}
	return;
}

void SecAnd_32_nshares(uint32_t* x, uint32_t* y, uint32_t* result, int n){
	int i, j;
	uint32_t temp[MAXSHARES];
	for (i = 0; i < n; i++){
		temp[i] = x[i] & y[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			uint32_t rij, rji;
			rij = rand32();
			rji = (x[i] & y[j]) ^ rij;
			rji ^= (x[j] & y[i]);
			temp[i] = temp[i] ^ rij;
			temp[j] = temp[j] ^ rji;
		}
	}
	for (i = 0; i < n; i++){
		result[i] = temp[i];
	}
	return;
}

void SecAnd_16_nshares(uint16_t* x, uint16_t* y, uint16_t* result, int n){
	int i, j;
	uint16_t temp[MAXSHARES];
	for (i = 0; i < n; i++){
		temp[i] = x[i] & y[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			uint16_t rij, rji;
			rij = rand16();
			rji = (x[i] & y[j]) ^ rij;
			rji ^= (x[j] & y[i]);
			temp[i] = temp[i] ^ rij;
			temp[j] = temp[j] ^ rji;
		}
	}
	for (i = 0; i < n; i++){
		result[i] = temp[i];
	}
	return;
}

void SecAnd_8_nshares(uint8_t* x, uint8_t* y, uint8_t* result, int n){
	int i, j;
	uint8_t temp[MAXSHARES];
	for (i = 0; i < n; i++){
		temp[i] = x[i] & y[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			uint8_t rij, rji;
			rij = rand8();
			rji = (x[i] & y[j]) ^ rij;
			rji ^= (x[j] & y[i]);
			temp[i] = temp[i] ^ rij;
			temp[j] = temp[j] ^ rji;
		}
	}
	for (i = 0; i < n; i++){
		result[i] = temp[i];
	}
	return;
}

void Refresh_128_nshares(UINT128* x, UINT128* result, int n){
	int i, j;
	UINT128 r;
	for (i = 0; i < n; i++){
		result[i] = x[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			r = rand128();
			result[i] = UINT128Xor(result[i], r);
			result[j] = UINT128Xor(result[j], r);
		}
	}
	return;
}

void Refresh_64_nshares(uint64_t* x, uint64_t* result, int n){
	int i, j;
	uint64_t r;
	for (i = 0; i < n; i++){
		result[i] = x[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			r = rand64();
			result[i] ^= r;
			result[j] ^= r;
		}
	}
	return;
}

void Refresh_32_nshares(uint32_t* x, uint32_t* result, int n){
	int i, j;
	uint32_t r;
	for (i = 0; i < n; i++){
		result[i] = x[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			r = rand32();
			result[i] ^= r;
			result[j] ^= r;
		}
	}
	return;
}

void Refresh_16_nshares(uint16_t* x, uint16_t* result, int n){
	int i, j;
	uint64_t r;
	for (i = 0; i < n; i++){
		result[i] = x[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			r = rand16();
			result[i] ^= r;
			result[j] ^= r;
		}
	}
	return;
}

void Refresh_8_nshares(uint8_t* x, uint8_t* result, int n){
	int i, j;
	uint8_t r;
	for (i = 0; i < n; i++){
		result[i] = x[i];
	}
	for (i = 0; i < n; i++){
		for (j = i+1; j < n; j++){
			r = rand8();
			result[i] ^= r;
			result[j] ^= r;
		}
	}
	return;
}

void RefreshMasks_64_nshares(uint64_t* x, uint64_t* result, int n){
	int i;
	uint64_t r;
	for (i = 0; i < n; i++){
		result[i] = x[i];
	}
	for (i = 1; i < n; i++){
		r = rand64();
		result[0] ^= r;
		result[i] ^= r;
	}
	return;
}

void RefreshMasks_16_nshares(uint16_t* x, uint16_t* result, int n){
	int i;
	uint16_t r;
	for (i = 0; i < n; i++){
		result[i] = x[i];
	}
	for (i = 1; i < n; i++){
		r = rand16();
		result[0] ^= r;
		result[i] ^= r;
	}
	return;
}

void SecAdd_128_nshares(UINT128* x, UINT128* y, UINT128* result, int n){
	UINT128 P[MAXSHARES], G[MAXSHARES], a[MAXSHARES];
	UINT128 tmp[MAXSHARES];
	int i, j;
	for (j = 0; j < n; j++){
		tmp[j] = P[j] = UINT128Xor(x[j], y[j]);
	}
	SecAnd_128_nshares(x, y, G, n);
	for (i = 0; i < 6; i++){
		int lsh = (1 << i);
		for (j = 0; j < n; j++){
			a[j] = UINT128LSH(G[j], lsh);
		}
		SecAnd_128_nshares(a, P, a, n);
		for (j = 0; j < n; j++){
			G[j] = UINT128Xor(G[j], a[j]);
		}
		for (j = 0; j < n; j++){
			a[j] = UINT128LSH(P[j], lsh);
		}
		Refresh_128_nshares(a, a, n);
		SecAnd_128_nshares(P, a, P, n);
	}
	for (j = 0; j < n; j++){
		a[j] = UINT128LSH64(G[j]);
	}
	SecAnd_128_nshares(a, P, a, n);
	for (j = 0; j < n; j++){
		G[j] = UINT128Xor(G[j], a[j]);
	}
	for (j = 0; j < n; j++){
		result[j] = UINT128Xor(tmp[j], UINT128LSH(G[j], 1));
	}
	return;
}

void SecAdd_64_nshares(uint64_t* x, uint64_t* y, uint64_t* result, int n){
	uint64_t P[MAXSHARES], G[MAXSHARES], a[MAXSHARES];
	uint64_t tmp[MAXSHARES];
	int i, j;
	for (j = 0; j < n; j++){
		tmp[j] = P[j] = x[j]^y[j];
	}
	SecAnd_64_nshares(x, y, G, n);
	for (i = 0; i < 5; i++){
		int lsh = (1 << i);
		for (j = 0; j < n; j++){
			a[j] = G[j] << lsh;
		}
		SecAnd_64_nshares(a, P, a, n);
		for (j = 0; j < n; j++){
			G[j] ^= a[j];
		}
		for (j = 0; j < n; j++){
			a[j] = P[j] << lsh;
		}
		Refresh_64_nshares(a, a, n);
		SecAnd_64_nshares(P, a, P, n);
	}
	for (j = 0; j < n; j++){
		a[j] = G[j] << 32;
	}
	SecAnd_64_nshares(a, P, a, n);
	for (j = 0; j < n; j++){
		G[j] ^= a[j];
	}
	for (j = 0; j < n; j++){
		result[j] = tmp[j] ^ (G[j] << 1);
	}
	return;
}

void SecAdd_32_nshares(uint32_t* x, uint32_t* y, uint32_t* result, int n){
	uint32_t P[MAXSHARES], G[MAXSHARES], a[MAXSHARES];
	uint32_t tmp[MAXSHARES];
	int i, j;
	for (j = 0; j < n; j++){
		tmp[j] = P[j] = x[j]^y[j];
	}
	SecAnd_32_nshares(x, y, G, n);
	for (i = 0; i < 4; i++){
		int lsh = (1 << i);
		for (j = 0; j < n; j++){
			a[j] = G[j] << lsh;
		}
		SecAnd_32_nshares(a, P, a, n);
		for (j = 0; j < n; j++){
			G[j] ^= a[j];
		}
		for (j = 0; j < n; j++){
			a[j] = P[j] << lsh;
		}
		Refresh_32_nshares(a, a, n);
		SecAnd_32_nshares(P, a, P, n);
	}
	for (j = 0; j < n; j++){
		a[j] = G[j] << 16;
	}
	SecAnd_32_nshares(a, P, a, n);
	for (j = 0; j < n; j++){
		G[j] ^= a[j];
	}
	for (j = 0; j < n; j++){
		result[j] = tmp[j] ^ (G[j] << 1);
	}
	return;
}

void SecAdd_16_nshares(uint16_t* x, uint16_t* y, uint16_t* result, int n){
	uint16_t P[MAXSHARES], G[MAXSHARES], a[MAXSHARES];
	uint16_t tmp[MAXSHARES];
	int i, j;
	for (j = 0; j < n; j++){
		tmp[j] = P[j] = x[j]^y[j];
	}
	SecAnd_16_nshares(x, y, G, n);
	for (i = 0; i < 3; i++){
		int lsh = (1 << i);
		for (j = 0; j < n; j++){
			a[j] = G[j] << lsh;
		}
		SecAnd_16_nshares(a, P, a, n);
		for (j = 0; j < n; j++){
			G[j] ^= a[j];
		}
		for (j = 0; j < n; j++){
			a[j] = P[j] << lsh;
		}
		Refresh_16_nshares(a, a, n);
		SecAnd_16_nshares(P, a, P, n);
	}
	for (j = 0; j < n; j++){
		a[j] = G[j] << 8;
	}
	SecAnd_16_nshares(a, P, a, n);
	for (j = 0; j < n; j++){
		G[j] ^= a[j];
	}
	for (j = 0; j < n; j++){
		result[j] = tmp[j] ^ (G[j] << 1);
	}
	return;
}

void SecAdd_8_nshares(uint8_t* x, uint8_t* y, uint8_t* result, int n){
	uint8_t P[MAXSHARES], G[MAXSHARES], a[MAXSHARES];
	uint8_t tmp[MAXSHARES];
	int i, j;
	for (j = 0; j < n; j++){
		tmp[j] = P[j] = x[j]^y[j];
	}
	SecAnd_8_nshares(x, y, G, n);
	for (i = 0; i < 2; i++){
		int lsh = (1 << i);
		for (j = 0; j < n; j++){
			a[j] = G[j] << lsh;
		}
		SecAnd_8_nshares(a, P, a, n);
		for (j = 0; j < n; j++){
			G[j] ^= a[j];
		}
		for (j = 0; j < n; j++){
			a[j] = P[j] << lsh;
		}
		Refresh_8_nshares(a, a, n);
		SecAnd_8_nshares(P, a, P, n);
	}
	for (j = 0; j < n; j++){
		a[j] = G[j] << 4;
	}
	SecAnd_8_nshares(a, P, a, n);
	for (j = 0; j < n; j++){
		G[j] ^= a[j];
	}
	for (j = 0; j < n; j++){
		result[j] = tmp[j] ^ (G[j] << 1);
	}
	return;
}

// static void SecAddOne_64_nshares(uint64_t* x, uint64_t* result, int n){
// 	uint8_t acc[MAXSHARES], b[MAXSHARES];
// 	uint64_t tmp[MAXSHARES];
// 	int i, j;
// 	for (i = 0; i < n; i++){
// 		tmp[i] = x[i];
// 	}
// 	tmp[0] ^= 1;
// 	for (i = 0; i < n; i++){
// 		acc[i] = (x[i] & 1);
// 		tmp[i] ^= (acc[i] << 1);
// 	}
// 	for (i = 1; i < 63; i++){
// 		for (j = 0; j < n; j++){
// 			b[j] = ((x[j] >> i) & 1);
// 		}
// 		SecAnd_8_nshares(b, acc, acc, n);
// 		for (j = 0; j < n; j++){
// 			tmp[j] ^= (acc[j] << (i+1));
// 		}
// 	}
// 	for (i = 0; i < n; i++){
// 		result[i] = tmp[i];
// 	}
// 	return;
// }

void SecOr_64_nshares(uint64_t* x, uint64_t* y, uint64_t* result, int n){
	uint64_t tmp1[MAXSHARES], tmp2[MAXSHARES];
	int i;
	for (i = 0; i < n; i++){
		tmp1[i] = x[i];
		tmp2[i] = y[i];
	}
	tmp1[0] = ~tmp1[0];
	tmp2[0] = ~tmp2[0];
	SecAnd_64_nshares(tmp1, tmp2, result, n);
	result[0] = ~result[0];
}

void SecOr_32_nshares(uint32_t* x, uint32_t* y, uint32_t* result, int n){
	uint32_t tmp1[MAXSHARES], tmp2[MAXSHARES];
	int i;
	for (i = 0; i < n; i++){
		tmp1[i] = x[i];
		tmp2[i] = y[i];
	}
	tmp1[0] = ~tmp1[0];
	tmp2[0] = ~tmp2[0];
	SecAnd_32_nshares(tmp1, tmp2, result, n);
	result[0] = ~result[0];
}

void SecOr_16_nshares(uint16_t* x, uint16_t* y, uint16_t* result, int n){
	uint16_t tmp1[MAXSHARES], tmp2[MAXSHARES];
	int i;
	for (i = 0; i < n; i++){
		tmp1[i] = x[i];
		tmp2[i] = y[i];
	}
	tmp1[0] = ~tmp1[0];
	tmp2[0] = ~tmp2[0];
	SecAnd_16_nshares(tmp1, tmp2, result, n);
	result[0] = ~result[0];
}

void SecOr_8_nshares(uint8_t* x, uint8_t* y, uint8_t* result, int n){
	uint8_t tmp1[MAXSHARES], tmp2[MAXSHARES];
	int i;
	for (i = 0; i < n; i++){
		tmp1[i] = x[i];
		tmp2[i] = y[i];
	}
	tmp1[0] = ~tmp1[0];
	tmp2[0] = ~tmp2[0];
	SecAnd_8_nshares(tmp1, tmp2, result, n);
	result[0] = ~result[0];
}

static void A2B_128_2shares(UINT128 x[2], UINT128 result[2]){
	UINT128 s, t, u, P, Pt, G, H, U;
	s = rand128();
	t = rand128();
	u = rand128();

	P = UINT128Xor(x[0], s);
	P = UINT128Xor(P, x[1]);
	G = UINT128Xor(s, UINT128And(UINT128Xor(x[0], t), x[1]));
	G = UINT128Xor(G, UINT128And(t, x[1]));
	for (int i = 0; i < 5; i++){
		int lsh = (1 << i);
		H = UINT128Xor( UINT128Xor(UINT128LSH(G, lsh), t), UINT128LSH(s, lsh));
		U = UINT128Xor(u, UINT128And(P, H));
		U = UINT128Xor(U, UINT128And(P, t));
		U = UINT128Xor(U, UINT128And(s, H));
		U = UINT128Xor(U, UINT128And(s, t));
		G = UINT128Xor(UINT128Xor(G, U), u);
		H = UINT128Xor( UINT128Xor(UINT128LSH(P, lsh), t), UINT128LSH(s, lsh));
		Pt = P;
		P = UINT128Xor(u, UINT128And(Pt, H));
		P = UINT128Xor(P, UINT128And(Pt, t));
		P = UINT128Xor(P, UINT128And(s, H));
		P = UINT128Xor(P, UINT128And(s, t));
		P = UINT128Xor(P, s);
		P = UINT128Xor(P, u);
	}
	H = UINT128Xor( UINT128Xor(UINT128LSH32(G), t), UINT128LSH32(s));
	U = UINT128Xor(u, UINT128And(P, H));
	U = UINT128Xor(U, UINT128And(P, t));
	U = UINT128Xor(U, UINT128And(s, H));
	U = UINT128Xor(U, UINT128And(s, t));
	G = UINT128Xor(UINT128Xor(G, U), u);
	H = UINT128Xor( UINT128Xor(UINT128LSH32(P), t), UINT128LSH32(s));
	Pt = P;
	P = UINT128Xor(u, UINT128And(Pt, H));
	P = UINT128Xor(P, UINT128And(Pt, t));
	P = UINT128Xor(P, UINT128And(s, H));
	P = UINT128Xor(P, UINT128And(s, t));
	P = UINT128Xor(P, s);
	P = UINT128Xor(P, u);

	H = UINT128Xor( UINT128Xor(UINT128LSH64(G), t), UINT128LSH64(s));
	U = UINT128Xor(u, UINT128And(P, H));
	U = UINT128Xor(U, UINT128And(P, t));
	U = UINT128Xor(U, UINT128And(s, H));
	U = UINT128Xor(U, UINT128And(s, t));
	G = UINT128Xor(UINT128Xor(G, U), u);	

	result[0] = UINT128Xor(x[0], UINT128LSH(G, 1));
	result[0] = UINT128Xor(result[0], UINT128LSH(s, 1));
	result[1] = x[1];
	return;
}

void A2B_128_nshares(UINT128* x, UINT128* result, int n){
	if (n == 1){
		result[0] = x[0];
		return;
	} 
	else if (n == 2){
		UINT128 y[2], r;
		r = rand128();
		y[0] = UINT128Add(x[0], r);
		y[1] = UINT128Sub(x[1], r);
		A2B_128_2shares(y, result);
		return;
	}
	UINT128 x_left[MAXSHARES], x_right[MAXSHARES];
	int i, n2 = n / 2;
	for (i = 0; i < n; i++){
		x_left[i].value[0] = x_left[i].value[1] = x_left[i].value[2] = x_left[i].value[3] = 0;
		x_right[i].value[0] = x_right[i].value[1] = x_right[i].value[2] = x_right[i].value[3] = 0;
	}
	for (i = 0; i < n2; i++){
		x_left[i] = x[i];
	}
	A2B_128_nshares(x_left, x_left, n2);
	Refresh_128_nshares(x_left, x_left, n);

	for (i = 0; i < n - n2; i++){
		x_right[i] = x[i + n2];
	}
	A2B_128_nshares(x_right, x_right, n - n2);
	Refresh_128_nshares(x_right, x_right, n);

	SecAdd_128_nshares(x_left, x_right, result, n);
}

static void A2B_64_2shares(uint64_t x[2], uint64_t result[2]){
	uint64_t s, t, u, P, G, H, U;
	s = rand64();
	t = rand64();
	u = rand64();

	P = x[0] ^ s;
	P = P ^ x[1];
	G = s ^ ((x[0] ^ t) & x[1]);
	G = G ^ (t & x[1]);
	for (int i = 0; i < 5; i++){
		int lsh = (1 << i);
		H = (G << lsh) ^ t ^ (s << lsh);
		U = (u ^ (P & H)) ^ (P & t) ^ (s & H) ^ (s & t);
		G = G ^ U ^ u;
		H = (P << lsh) ^ t ^ (s << lsh);
		P = (u ^ (P & H)) ^ (P & t) ^ (s & H) ^ (s & t);
		P = P ^ s;
		P = P ^ u;
	}
	H = (G << 32) ^ t ^ (s << 32);
	U = (u ^ (P & H)) ^ (P & t) ^ (s & H) ^ (s & t);
	G = G ^ U ^ u;
	result[0] = x[0] ^ (G << 1);
	result[0] = result[0] ^ (s << 1);
	result[1] = x[1];
	return;
}

void A2B_64_nshares(uint64_t* x, uint64_t* result, int n){
	if (n == 1){
		result[0] = x[0];
		return;
	} 
	else if (n == 2){
		uint64_t y[2], r;
		r = rand64();
		y[0] = x[0] + r;
		y[1] = x[1] - r;
		A2B_64_2shares(y, result);
		return;
	}
	uint64_t x_left[MAXSHARES], x_right[MAXSHARES];
	int i, n2 = n / 2;
	for (i = 0; i < n; i++){
		x_left[i] = 0;
		x_right[i] = 0;
	}
	for (i = 0; i < n2; i++){
		x_left[i] = x[i];
	}
	A2B_64_nshares(x_left, x_left, n2);
	Refresh_64_nshares(x_left, x_left, n);

	for (i = 0; i < n - n2; i++){
		x_right[i] = x[i + n2];
	}
	A2B_64_nshares(x_right, x_right, n - n2);
	Refresh_64_nshares(x_right, x_right, n);

	SecAdd_64_nshares(x_left, x_right, result, n);
}

static void A2B_16_2shares(uint16_t x[2], uint16_t result[2]){
	uint16_t s, t, u, P, G, H, U;
	s = rand16();
	t = rand16();
	u = rand16();

	P = x[0] ^ s;
	P = P ^ x[1];
	G = s ^ ((x[0] ^ t) & x[1]);
	G = G ^ (t & x[1]);
	for (int i = 0; i < 3; i++){
		int lsh = (1 << i);
		H = (G << lsh) ^ t ^ (s << lsh);
		U = (u ^ (P & H)) ^ (P & t) ^ (s & H) ^ (s & t);
		G = G ^ U ^ u;
		H = (P << lsh) ^ t ^ (s << lsh);
		P = (u ^ (P & H)) ^ (P & t) ^ (s & H) ^ (s & t);
		P = P ^ s;
		P = P ^ u;
	}
	H = (G << 8) ^ t ^ (s << 8);
	U = (u ^ (P & H)) ^ (P & t) ^ (s & H) ^ (s & t);
	G = G ^ U ^ u;
	result[0] = x[0] ^ (G << 1);
	result[0] = result[0] ^ (s << 1);
	result[1] = x[1];
	return;
}

void A2B_16_nshares(uint16_t* x, uint16_t* result, int n){
	if (n == 1){
		result[0] = x[0];
		return;
	} 
	else if (n == 2){
		uint16_t y[2], r;
		r = rand16();
		y[0] = x[0] + r;
		y[1] = x[1] - r;
		A2B_16_2shares(y, result);
		return;
	}
	uint16_t x_left[MAXSHARES], x_right[MAXSHARES];
	int i, n2 = n / 2;
	for (i = 0; i < n; i++){
		x_left[i] = 0;
		x_right[i] = 0;
	}
	for (i = 0; i < n2; i++){
		x_left[i] = x[i];
	}
	A2B_16_nshares(x_left, x_left, n2);
	Refresh_16_nshares(x_left, x_left, n);

	for (i = 0; i < n - n2; i++){
		x_right[i] = x[i + n2];
	}
	A2B_16_nshares(x_right, x_right, n - n2);
	Refresh_16_nshares(x_right, x_right, n);

	SecAdd_16_nshares(x_left, x_right, result, n);
}

uint16_t FullXor_16_nshares(uint16_t* x, int n){
	uint16_t tmp[MAXSHARES], res = 0;
	int i;
	for (i = 0; i < n; i++){
		tmp[i] = x[i];
	}
	Refresh_16_nshares(tmp, tmp, n);
	for (i = 0; i < n; i++){
		res ^= tmp[i];
	}
	return res;
}

uint16_t FullXor_8_nshares(uint8_t* x, int n){
	uint8_t tmp[MAXSHARES], res = 0;
	int i;
	for (i = 0; i < n; i++){
		tmp[i] = x[i];
	}
	Refresh_8_nshares(tmp, tmp, n);
	for (i = 0; i < n; i++){
		res ^= tmp[i];
	}
	return res;
}

void B2A_BitAdd_32_nshares(uint32_t* A, uint32_t x, int n){
	uint32_t B[MAXSHARES], r;
	int i;
	for (i = 1; i <= n-1; i++){
		B[i] = 0;
	}
	B[n] = rand32();
	B[0] = A[0] - B[n];
	for (i = 1; i <= n-1; i++){
		r = rand32();
		B[i] = A[i] - r;
		B[n] = B[n] + r;
	}
	for (i = 0; i <= n; i++){
		A[i] = B[i] - 2 * (B[i] * x);
	}
	A[0] += x;
	return;
}

void B2A_Bit_32_nshares(uint32_t* x, uint32_t* result, int n){
	uint32_t tmp[MAXSHARES];
	int i;
	for (i = 0; i < n; i++){
		tmp[i] = 0;
	}
	tmp[0] = x[0] & 1;
	for (i = 1; i < n; i++){
		B2A_BitAdd_32_nshares(tmp, x[i] & 1, i);
	}
	for (i = 0; i < n; i++){
		result[i] = tmp[i];
	}
}

void B2A_BitAdd_16_nshares(uint16_t* A, uint16_t x, int n){
	uint16_t B[MAXSHARES], r;
	int i;
	for (i = 1; i <= n-1; i++){
		B[i] = 0;
	}
	B[n] = rand16();
	B[0] = A[0] - B[n];
	for (i = 1; i <= n-1; i++){
		r = rand16();
		B[i] = A[i] - r;
		B[n] = B[n] + r;
	}
	for (i = 0; i <= n; i++){
		A[i] = B[i] - 2 * (B[i] * x);
	}
	A[0] += x;
	return;
}

void B2A_Bit_16_nshares(uint16_t* x, uint16_t* result, int n){
	uint16_t tmp[MAXSHARES];
	int i;
	for (i = 0; i < n; i++){
		tmp[i] = 0;
	}
	tmp[0] = x[0] & 1;
	for (i = 1; i < n; i++){
		B2A_BitAdd_16_nshares(tmp, x[i] & 1, i);
	}
	for (i = 0; i < n; i++){
		result[i] = tmp[i];
	}
}

void B2A_BitAdd_8_nshares(uint8_t* A, uint8_t x, int n){
	uint8_t B[MAXSHARES], r;
	int i;
	for (i = 1; i <= n-1; i++){
		B[i] = 0;
	}
	B[n] = rand8();
	B[0] = A[0] - B[n];
	for (i = 1; i <= n-1; i++){
		r = rand8();
		B[i] = A[i] - r;
		B[n] = B[n] + r;
	}
	for (i = 0; i <= n; i++){
		A[i] = B[i] - 2 * (B[i] * x);
	}
	A[0] += x;
	return;
}

void B2A_Bit_8_nshares(uint8_t* x, uint8_t* result, int n){
	uint8_t tmp[MAXSHARES];
	int i;
	for (i = 0; i < n; i++){
		tmp[i] = 0;
	}
	tmp[0] = x[0] & 1;
	for (i = 1; i < n; i++){
		B2A_BitAdd_8_nshares(tmp, x[i] & 1, i);
	}
	for (i = 0; i < n; i++){
		result[i] = tmp[i];
	}
}

static uint16_t Psi(uint16_t x, uint16_t y){
  return (x ^ y) - y;
}

static uint16_t Psi0(uint16_t x, uint16_t y, int n){
  return Psi(x, y) ^ ((~n & 1) * x);
}

static void B2A_16_nshares_rec(uint16_t *x, uint16_t *result, int n){  
	if (n == 2){
		uint16_t r1 = rand16();
		uint16_t r2 = rand16();
		uint32_t y0 = (x[0] ^ r1) ^ r2;
		uint32_t y1 = x[1] ^ r1;
		uint32_t y2 = x[2] ^ r2;

		uint32_t z0 = y0 ^ Psi(y0, y1);
		uint32_t z1 = Psi(y0, y2);

		result[0] = y1 ^ y2;
		result[1] = z0 ^ z1;
		return;
	}

	uint16_t y[MAXSHARES + 1], z[MAXSHARES + 1], A[MAXSHARES], B[MAXSHARES];;
	for (int i = 0; i <= n; i++){
		y[i] = x[i];
	}

	RefreshMasks_16_nshares(y, y, n+1);

	z[0] = Psi0(y[0], y[1], n);
	for (int i = 1; i < n; i++){
		z[i] = Psi(y[0], y[i+1]);
	}

	B2A_16_nshares_rec(y+1, A, n-1);
	B2A_16_nshares_rec(z, B, n-1);

	for (int i = 0; i < n-2; i++){
		result[i] = A[i] + B[i];
	}

	result[n-2] = A[n-2];
	result[n-1] = B[n-2];

	return;
}

void B2A_16_nshares(uint16_t *x, uint16_t *result, int n){
	uint16_t x_ext[MAXSHARES];
	for (int i = 0; i < n; i++){
		x_ext[i] = x[i];
	}
	x_ext[n] = 0;
	B2A_16_nshares_rec(x_ext, result, n);
	return;
}

void SecNonzeroBool_64_nshares(uint64_t* x, uint64_t* result, int n){
	uint32_t t32[MAXSHARES], left32[MAXSHARES], right32[MAXSHARES];
	uint16_t t16[MAXSHARES], left16[MAXSHARES], right16[MAXSHARES];
	uint8_t t8[MAXSHARES], left8[MAXSHARES], right8[MAXSHARES];
	int i;

	for (i = 0; i < n; i++){
		left32[i] = (x[i] >> 32);
		right32[i] = x[i] & (0Xffffffff);
	}
	Refresh_32_nshares(left32, left32, n);
	SecOr_32_nshares(left32, right32, t32, n);

	for (i = 0; i < n; i++){
		left16[i] = (t32[i] >> 16);
		right16[i] = t32[i] & (0Xffff);
	}
	Refresh_16_nshares(left16, left16, n);
	SecOr_16_nshares(left16, right16, t16, n);

	for (i = 0; i < n; i++){
		left8[i] = (t16[i] >> 8);
		right8[i] = t16[i] & (0Xff);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 4);
		right8[i] = t8[i] & (0xf);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 2);
		right8[i] = t8[i] & (0x3);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 1);
		right8[i] = t8[i] & (0x1);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		result[i] = t8[i] & 1;
	}
	return;
}

void SecNonzeroBool_32_nshares(uint32_t* x, uint32_t* result, int n){
	uint16_t t16[MAXSHARES], left16[MAXSHARES], right16[MAXSHARES];
	uint8_t t8[MAXSHARES], left8[MAXSHARES], right8[MAXSHARES];
	int i;

	for (i = 0; i < n; i++){
		left16[i] = (x[i] >> 16);
		right16[i] = x[i] & (0Xffff);
	}
	Refresh_16_nshares(left16, left16, n);
	SecOr_16_nshares(left16, right16, t16, n);

	for (i = 0; i < n; i++){
		left8[i] = (t16[i] >> 8);
		right8[i] = t16[i] & (0Xff);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 4);
		right8[i] = t8[i] & (0xf);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 2);
		right8[i] = t8[i] & (0x3);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 1);
		right8[i] = t8[i] & (0x1);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		result[i] = t8[i] & 1;
	}
	return;
}

void SecNonzeroBool_16_nshares(uint16_t* x, uint16_t* result, int n){
	uint8_t t8[MAXSHARES], left8[MAXSHARES], right8[MAXSHARES];
	int i;

	for (i = 0; i < n; i++){
		left8[i] = (x[i] >> 8);
		right8[i] = x[i] & (0Xff);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 4);
		right8[i] = t8[i] & (0xf);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 2);
		right8[i] = t8[i] & (0x3);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 1);
		right8[i] = t8[i] & (0x1);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		result[i] = t8[i] & 1;
	}
	return;
}

void SecNonzeroBool_8_nshares(uint8_t* x, uint8_t* result, int n){
	uint8_t t8[MAXSHARES], left8[MAXSHARES], right8[MAXSHARES];
	int i;

	for (i = 0; i < n; i++){
		left8[i] = (x[i] >> 4);
		right8[i] = x[i] & (0xf);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 2);
		right8[i] = t8[i] & (0x3);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		left8[i] = (t8[i] >> 1);
		right8[i] = t8[i] & (0x1);
	}
	Refresh_8_nshares(left8, left8, n);
	SecOr_8_nshares(left8, right8, t8, n);

	for (i = 0; i < n; i++){
		result[i] = t8[i] & 1;
	}
	return;
}

void SecNonzeroArith_16_nshares(uint16_t* x, uint16_t* result, int n){
	uint16_t t[MAXSHARES];
	int i, n2 = n / 2;
	for (i = 0; i < n2; i++){
		t[i] = x[i];
	}
	for (i = n2; i < n; i++){
		t[i] = -x[i];
	}
	A2B_16_nshares(t, t, n2);
	A2B_16_nshares(t+n2, t+n2, (n - n2));
	SecNonzeroBool_16_nshares(t, result, n);
	return;
}
