#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>


uint64_t rot(uint64_t v, uint8_t sh){
    return (v << sh) ^ (v >> (64 - sh));
}


void sipround(uint64_t *v0, uint64_t *v1, uint64_t *v2, uint64_t *v3){
    uint64_t u0 = *v0;
    uint64_t u1 = *v1;
    uint64_t u2 = *v2;
    uint64_t u3 = *v3;

    u0 = u0 + u1;
    u2 = u2 + u3;
    u1 = rot(u1, 13);
    u3 = rot(u3, 16);
    u1 = u1 ^ u0;
    u3 = u3 ^ u2;
    u0 = rot(u0, 32);
    u2 = u2 + u1;
    u0 = u0 + u3;
    u1 = rot(u1, 17);
    u3 = rot(u3, 21);
    u1 = u1 ^ u2;
    u3 = u3 ^ u0;
    u2 = rot(u2, 32);

    *v0 = u0;
    *v1 = u1;
    *v2 = u2;
    *v3 = u3;
}

uint64_t sipshash_2_4(uint64_t k[2], uint8_t *m, unsigned mlen){
    uint64_t v0 = k[0] ^ ((uint64_t) 0x736f6d6570736575);
    uint64_t v1 = k[1] ^ ((uint64_t) 0x646f72616e646f6d); 
    uint64_t v2 = k[0] ^ ((uint64_t) 0x6c7967656e657261); 
    uint64_t v3 = k[1] ^ ((uint64_t) 0x7465646279746573); 
    
    unsigned w = 0;
    uint64_t mi;
    unsigned i;

    for(i = 0; i+(1<<3) <= mlen; i=i+(1<<3)){
        mi = m[i];
        mi = mi + ((uint64_t) (m[i + 1])<<8);
        mi = mi + ((uint64_t) (m[i + 2])<<16);
        mi = mi + ((uint64_t) (m[i + 3])<<24);
        mi = mi + ((uint64_t) (m[i + 4])<<32);
        mi = mi + ((uint64_t) (m[i + 5])<<40);
        mi = mi + ((uint64_t) (m[i + 6])<<48);
        mi = mi + ((uint64_t) (m[i + 7])<<56);
        w++;

        v3 = v3 ^ mi;

        sipround(&v0, &v1, &v2, &v3);
        sipround(&v0, &v1, &v2, &v3);

        v0 = v0 ^ mi;
    }
    if (i <= mlen) {
        mi = 0;
        uint8_t j = 0;
        for (;i < mlen; i++){
            mi = mi + (((uint64_t) m[i])<<(j<<3));
            j++;
        }
        mi = mi + (((uint64_t) (mlen % 256))<<56);

        v3 = v3 ^ mi;

        sipround(&v0, &v1, &v2, &v3);
        sipround(&v0, &v1, &v2, &v3);

        v0 = v0 ^ mi;
    }

    //Finalization
    v2 = v2 ^ 0xff;
    sipround(&v0, &v1, &v2, &v3);
    sipround(&v0, &v1, &v2, &v3);
    sipround(&v0, &v1, &v2, &v3);
    sipround(&v0, &v1, &v2, &v3);

    return (v0 ^ v1 ^ v2 ^ v3);

}

int main(void){
    uint64_t k[2] = {0x0706050403020100, 0x0f0e0d0c0b0a0908};
    uint64_t k2[2] = {0, 0};
    uint8_t m[15] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e};
    uint8_t m2[8] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};

    uint64_t kg[2] = {0,0};
    uint8_t found = 0;
    printf("Looking for a key for hash 0xa129ca6149be45e5 and message m:\n");
    while (found == 0) {
        kg[1] = 0;
        while(1){
            (kg[1])++;
            if(sipshash_2_4(k, m, 15) == 0xa129ca6149be45e5){
                printf("Key found: %jx%jx\n", k[0], k[1]);
                found = 1;
                break;
            }
        }
        (kg[0])++;
    } 

    printf("sipslash k m 15 = %jx\n", sipshash_2_4(k, m, 15));
    printf("sipslash k m2 8 = %jx\n", sipshash_2_4(k, m2, 8));
    printf("sipslash k NULL 0 = %jx\n", sipshash_2_4(k, NULL, 0));
    printf("sipslash k2 NULL 0 = %jx\n", sipshash_2_4(k2, NULL, 0));
    return 0;
}

