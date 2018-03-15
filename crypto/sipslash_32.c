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

uint32_t sip_hash_fix32(uint32_t k, uint32_t m){
    
    uint64_t v0 = (uint64_t) 0x736f6d6570736575;
    uint64_t v1 = k ^ ((uint64_t) 0x646f72616e646f6d); 
    uint64_t v2 = (uint64_t) 0x6c7967656e657261; 
    uint64_t v3 = k ^ ((uint64_t) 0x7465646279746573); 

    uint64_t mi = m;

    mi = mi + (((uint64_t) 4)<<56);
    v3 = v3 ^ mi;
    sipround(&v0, &v1, &v2, &v3);
    sipround(&v0, &v1, &v2, &v3);

    v0 = v0 ^ mi;
    
    //Finalization
    v2 = v2 ^ 0xff;
    sipround(&v0, &v1, &v2, &v3);
    sipround(&v0, &v1, &v2, &v3);
    sipround(&v0, &v1, &v2, &v3);
    sipround(&v0, &v1, &v2, &v3);

    return (v0 ^ v1 ^ v2 ^ v3);

}

int main(void){
    uint32_t k = 0x01234567;
    uint32_t m = 0x89abcdef;
        
    printf("calling sip_hash_fix32 : %x\n", sip_hash_fix32(k, m));

    return 0;
}

