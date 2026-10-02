#include "uint128.h"

void print128(UINT128 x){
	printf("print128 [%lu, %lu, %lu, %lu,]\n", x.value[0], x.value[1], x.value[2], x.value[3]);
}

UINT128 UINT128Add(UINT128 x, UINT128 y){
	uint64_t xbuf, ybuf, sum;
	uint64_t carry;
	UINT128 result;
	xbuf = x.value[3], ybuf = y.value[3];
	sum = xbuf + ybuf;
	result.value[3] = sum & 0xFFFFFFFF;
	carry = (sum >> 32);
	xbuf = x.value[2], ybuf = y.value[2];
	sum = xbuf + ybuf + carry;
	result.value[2] = sum & 0xFFFFFFFF;
	carry = (sum >> 32);
	xbuf = x.value[1], ybuf = y.value[1];
	sum = xbuf + ybuf + carry;
	result.value[1] = sum & 0xFFFFFFFF;
	carry = (sum >> 32);
	xbuf = x.value[0], ybuf = y.value[0];
	sum = xbuf + ybuf + carry;
	result.value[0] = sum & 0xFFFFFFFF;
	return result;
}

UINT128 UINT128Neg(UINT128 x){
	UINT128 result;

	uint32_t value3nonzero = (x.value[3] != 0);
	uint32_t value2nonzero = (x.value[2] != 0);
	uint32_t value1nonzero = (x.value[1] != 0);
	result.value[3] = -(x.value[3]);
	result.value[2] = -(x.value[2]) - value3nonzero;
	result.value[1] = -(x.value[1]) - (value3nonzero & value2nonzero);
	result.value[0] = -(x.value[0]) - (value3nonzero & value2nonzero & value1nonzero);
	return result;
}

UINT128 UINT128Sub(UINT128 x, UINT128 y){
	UINT128 ny = UINT128Neg(y);
	return UINT128Add(x, ny);
}

// Helper to perform 32x32 -> 64 multiplication without calling __aeabi_lmul
// This uses schoolbook math at the 16-bit level to avoid M0 library branches.
static inline uint64_t umul32_ct(uint32_t a, uint32_t b) {
    uint32_t a_lo = a & 0xFFFF;
    uint32_t a_hi = a >> 16;
    uint32_t b_lo = b & 0xFFFF;
    uint32_t b_hi = b >> 16;

    uint32_t p0 = a_lo * b_lo;
    uint32_t p1 = a_lo * b_hi;
    uint32_t p2 = a_hi * b_lo;
    uint32_t p3 = a_hi * b_hi;

    uint64_t mid = (uint64_t)(p0 >> 16) + p1 + p2;
    uint64_t lo = ((uint64_t)(p0 & 0xFFFF)) | ((mid & 0xFFFF) << 16);
    uint64_t hi = p3 + (mid >> 16);
	return lo | (hi << 32);
}


UINT128 UINT128Mul(UINT128 x, UINT128 y){
	UINT128 result;
	uint64_t buf, sum, carry, carry2;
	/*buf = (uint64_t)x.value[3] * (uint64_t)y.value[3];*/
	buf = umul32_ct(x.value[3], y.value[3]);
	result.value[3] = (uint32_t)buf;
	carry = buf >> 32;

	/*buf = (uint64_t)x.value[3] * (uint64_t)y.value[2];*/
	buf = umul32_ct(x.value[3], y.value[2]);
	carry2 = buf >> 32;
	sum = (uint32_t)buf + carry;
	/*buf = (uint64_t)x.value[2] * (uint64_t)y.value[3];*/
	buf = umul32_ct(x.value[2], y.value[3]);
	carry2 += (buf >> 32);
	sum += (uint32_t)buf;
	carry2 += (sum >> 32);
	result.value[2] = (uint32_t)sum;
	carry = carry2;

	/*buf = (uint64_t)x.value[3] * (uint64_t)y.value[1];*/
	buf = umul32_ct(x.value[3], y.value[1]);
	carry2 = (buf >> 32);
	sum = (uint32_t)buf + carry;
	/*buf = (uint64_t)x.value[2] * (uint64_t)y.value[2];*/
	buf = umul32_ct(x.value[2], y.value[2]);
	carry2 += (buf >> 32);
	sum += (uint32_t)buf;
	/*buf = (uint64_t)x.value[1] * (uint64_t)y.value[3];*/
	buf = umul32_ct(x.value[1], y.value[3]);
	carry2 += (buf >> 32);
	sum += (uint32_t)buf;
	carry2 += (sum >> 32);
	result.value[1] = (uint32_t)sum;
	carry = carry2;

	/*buf = (uint64_t)x.value[3] * (uint64_t)y.value[0];*/
	buf = umul32_ct(x.value[3], y.value[0]);
	sum = buf + carry;
	/*buf = (uint64_t)x.value[2] * (uint64_t)y.value[1];*/
	buf = umul32_ct(x.value[2], y.value[1]);
	sum += buf;
	/*buf = (uint64_t)x.value[1] * (uint64_t)y.value[2];*/
	buf = umul32_ct(x.value[1], y.value[2]);
	sum += buf;
	/*buf = (uint64_t)x.value[0] * (uint64_t)y.value[3];*/
	buf = umul32_ct(x.value[0], y.value[3]);
	sum += buf;
	result.value[0] = (uint32_t)(sum);

	return result;

}

UINT128 UINT128Xor(UINT128 x, UINT128 y){
	UINT128 result;
	result.value[0] = x.value[0] ^ y.value[0];
	result.value[1] = x.value[1] ^ y.value[1];
	result.value[2] = x.value[2] ^ y.value[2];
	result.value[3] = x.value[3] ^ y.value[3];
	return result;
}

UINT128 UINT128And(UINT128 x, UINT128 y){
	UINT128 result;
	result.value[0] = x.value[0] & y.value[0];
	result.value[1] = x.value[1] & y.value[1];
	result.value[2] = x.value[2] & y.value[2];
	result.value[3] = x.value[3] & y.value[3];
	return result;
}

UINT128 UINT128LSH(UINT128 x, int n){
/*
	0 <= n < 32
*/
	UINT128 result;
	int rsh = 32 - n;
	result.value[0] = (x.value[0] << n) | (x.value[1] >> rsh);
	result.value[1] = (x.value[1] << n) | (x.value[2] >> rsh);
	result.value[2] = (x.value[2] << n) | (x.value[3] >> rsh);
	result.value[3] = x.value[3] << n;
	return result;
}

UINT128 UINT128LSH32(UINT128 x){
	UINT128 result;
	result.value[0] = x.value[1];
	result.value[1] = x.value[2];
	result.value[2] = x.value[3];
	result.value[3] = 0;
	return result;
}

UINT128 UINT128LSH64(UINT128 x){
	UINT128 result;
	result.value[0] = x.value[2];
	result.value[1] = x.value[3];
	result.value[2] = 0;
	result.value[3] = 0;
	return result;
}
