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
        //printf("%x ", X[i]);
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
        if (i != 34) {
            for (int k = 0; k < 16; k++){
                X[k] = Xpp[k];
            }
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

int main(void){
    //uint64_t a = 0x0000000000000000ULL;
    //uint64_t b = 0x123456789abcdef1ULL;
    //uint64_t c = 0xb4329ed38453aac8ULL;
    unsigned long long a = 0x0000000000000000ULL;
    unsigned long long b = 0x123456789abcdef1ULL;
    unsigned long long c = 0xb4329ed38453aac8ULL;
    //printf("1<<31: %"PRIx64"\n",(uint64_t) (1<<31));
    //printf("1<<31: %jx\n", (((uint64_t) 1)<<32));
    printf("t(%jx) = %jx\n", a, twine_perm_z(a));
    printf("t(%jx) = %jx\n", b, twine_perm_z(b));
    printf("t(%jx) = %jx\n", c, twine_perm_z(c));
    return 0; 
}

