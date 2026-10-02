#include "mask_rand.h"
#include "inner.h"

uint32_t randcount;
uint8_t prng_table[RANDBYTE];
size_t prng_pt;

inner_shake256_context mask_sc;
prng mask_p;

void init_prng_table(uint64_t SEED){

#ifdef MASK_FALCON_PRNG
    uint8_t in[8];
    for (int i = 0; i < 8; i++){
        in[i] = (uint8_t)SEED;
        SEED >>= 8;
    }
    inner_shake256_init(&mask_sc);
    inner_shake256_inject(&mask_sc, (const uint8_t *)in, 8);
    inner_shake256_flip(&mask_sc);
    Zf(prng_init)(&mask_p, &mask_sc);
#else
    srand(SEED);
    prng_pt = 0;
    randcount = 0;
    for (int i = 0; i < RANDBYTE; i++){
        prng_table[i] = (uint8_t)rand();
    }
#endif

    return;
}

unsigned mask_randbyte(void){
# ifdef MASK_FALCON_PRNG
    return prng_get_u8(&mask_p);
# else
    return prng_table[(prng_pt++) & RANDBYTEMASK];
#endif
}

UINT128 rand128(void){
    UINT128 result;
#ifdef MASK_FALCON_PRNG
    uint64_t u = prng_get_u64(&mask_p);
    result.value[0] = (u & 0XFFFFFFFF);
    result.value[1] = (u >> 32);
    u = prng_get_u64(&mask_p);
    result.value[2] = (u & 0XFFFFFFFF);
    result.value[3] = (u >> 32);
#else
    result.value[0] = rand32();
    result.value[1] = rand32();
    result.value[2] = rand32();
    result.value[3] = rand32();
#endif
    return result;
}

uint64_t rand64(void){

#ifdef MASK_FALCON_PRNG
    return prng_get_u64(&mask_p);
#else
    uint64_t result = 0;
    for (int count = 8; count > 0; count--) {
        result = 256U * result + mask_randbyte();
    }
    return result;
#endif
}

uint32_t rand32(void){
    uint32_t result = 0;
    for (int count = 4; count > 0; count--) {
        result = 256U * result + mask_randbyte();
    }
    return result;
}

uint16_t rand16(void){
    uint16_t result = 0;
    for (int count = 2; count > 0; count--) {
        result = 256U * result + mask_randbyte();
    }
    return result;
}

uint8_t rand8(void){
    uint8_t result = mask_randbyte();
    return result;
}