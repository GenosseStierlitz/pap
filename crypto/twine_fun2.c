#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

//uint64_t S[16] = {0xc, 0x0, 0xf, 0xa, 0x2, 0xb, 0x9, 0x5, 0x8, 0x3, 0xd, 0x7, 0x1, 0xe, 0x6, 0x4};
//uint64_t Pi[16] = {5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};
unsigned S[16] = {0xc, 0x0, 0xf, 0xa, 0x2, 0xb, 0x9, 0x5, 0x8, 0x3, 0xd, 0x7, 0x1, 0xe, 0x6, 0x4};
unsigned Pi[16] = {5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};

uint64_t twine_perm_z(uint64_t input){
    unsigned X[16];
    unsigned Xpp[16];

    for (int i = 0; i < 16; i++){
        X[i] = (input >> (4*i)) & 0xF;
        printf("%x ", X[i]);
    }
    printf("\n");
    for (int i = 0; i < 35; i++){
        
        // definition of X^{i+1}
        //for (int k = 0: k < 16; k++){
        //    Xpp[k] = X[k]
        //}
        for (int l = 0; l< 15; l++){
            printf("%x", X[l]);
        }
        printf("\n");


        for (int j = 0; j < 8; j++){
            X[2*j + 1] = S[X[2*j]] ^ X[2*j + 1];
        }
        for (int h = 0; h < 16; h++){
           Xpp[Pi[h]] = X[h];
        }

        // X <- X++
        for (int k = 0; k < 16; k++){
            X[k] = Xpp[k];
        }

    }

    for (int j = 0; j < 8; j++){
        Xpp[2*j + 1] ^= S[X[2*j]];
    }

    uint64_t c = 0;
    for (int i = 0; i < 16; i++){
        //printf("XPP : %x ", Xpp[i]);
        c = c ^ (((uint64_t) Xpp[i]) << (4*i));
        //printf("  c: %jx\n", c);
        //c += (Xpp[i] << (60 - 4*i));
    }
    return c;;
}

uint32_t twine_fun1(uint32_t k, uint32_t m){
    uint64_t km = m + (((uint64_t) k)<<32);
    return twine_perm_z(km);

}

uint32_t twine_fun2(uint32_t k, uint16_t *m, unsigned mlen){
    if (mlen == 0){
        return 0xffff; 
    }
    else {
        return twine_fun1(k, (twine_fun2(k, m, mlen-1))<<16 ^ m[mlen - 1]);
    }
}

uint32_t twine_fun2_fix32(uint32_t k, uint32_t m){
    return twine_fun1(k, (twine_fun1(k, 0xffff0000 ^ (m >> 16))<<16) ^ ((uint16_t) m));
    //return twine_fun1(k, (twine_fun1(k, 0xffff0000 ^ (m >> 16))<<16) ^ (0xffff & m));
}

uint32_t twine_fun2_fix16(uint32_t k, uint32_t m){
    return twine_fun1(k, 0xffff0000 ^ ((uint16_t) m));
}

int main(void){
    //uint64_t a = 0x0000000000000000ULL;
    //uint64_t b = 0x123456789abcdef1ULL;
    //uint64_t c = 0xb4329ed38453aac8ULL;
    //unsigned long long a = 0x0000000000000000ULL;
    //unsigned long long b = 0x123456789abcdef1ULL;
    //unsigned long long c = 0xb4329ed38453aac8ULL;
    //printf("1<<31: %"PRIx64"\n",(uint64_t) (1<<31));
    //printf("1<<31: %jx\n", (((uint64_t) 1)<<32));
    //printf("t(%jx) = %jx\n", a, twine_perm_z(a));
    //printf("t(%jx) = %jx\n", b, twine_perm_z(b));
    //printf("t(%jx) = %jx\n", c, twine_perm_z(c));
    //
    //uint32_t k1 = 0x0;
    //uint32_t m1 = 0x0;
    //uint32_t k2 = 0xcdef1234;
    //uint32_t m2 = 0xab123478;
    //printf("f(%"PRIx32", %"PRIx32") = %"PRIx32"\n", k1, m1, twine_fun1(k1, m1));
    //printf("f(%"PRIx32", %"PRIx32") = %"PRIx32"\n", k2, m2, twine_fun1(k2, m2));
    //
    uint16_t m1[] = {0x67fc};
    uint16_t m2[] = {0xef12, 0x5678};
    uint16_t m3[] = {0xef12, 0x5678, 0x31aa, 0x7123};
    uint32_t k1 = 0x0;
    uint32_t k2 = 0x23ae90ff;
    uint32_t k3 = 0xeeeeeeee;
    printf("f2(%"PRIx32", m%d, %d) = %"PRIx32"\n", k1, 1, 1, twine_fun2(k1, m1, 1));
    printf("f2_16(%"PRIx32", m%d) = %"PRIx32"\n", k1, 1, twine_fun2_fix16(k1, 0x67fc));
    printf("f2(%"PRIx32", m%d, %d) = %"PRIx32"\n", k1, 2, 2, twine_fun2(k2, m2, 2));
    printf("f2_32(%"PRIx32", m%d) = %"PRIx32"\n", k2, 2, twine_fun2_fix32(k2, 0xef125678));
    printf("f2(%"PRIx32", m%d, %d) = %"PRIx32"\n", k1, 3, 4, twine_fun2(k3, m3, 4));
    return 0; 
}

