#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>


typedef struct cell_ts {
    uint32_t preimage;
    uint32_t value;
    struct cell_ts *next;
} cell_t;
//uint64_t S[16] = {0xc, 0x0, 0xf, 0xa, 0x2, 0xb, 0x9, 0x5, 0x8, 0x3, 0xd, 0x7, 0x1, 0xe, 0x6, 0x4};
//uint64_t Pi[16] = {5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};
unsigned S[16] = {0xc, 0x0, 0xf, 0xa, 0x2, 0xb, 0x9, 0x5, 0x8, 0x3, 0xd, 0x7, 0x1, 0xe, 0x6, 0x4};
unsigned Pi[16] = {5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};














//void add_list(cell_t **cell, uint32_t val, uint32_t preimage){
void add_list(cell_t **cell, uint32_t val){
    cell_t *newcell = malloc(sizeof(cell_t));
    newcell->value = val;
    newcell->next = *cell;
    //newcell->preimage = preimage;
    *cell = newcell;
}

//uint8_t is_in_list(cell_t *cell, uint32_t val, preimage){
uint8_t is_in_list(cell_t *cell, uint32_t val){
    uint8_t found = 0;
    while(cell != NULL){
        if (cell->value == val){
            found = 1;
            //printf("collision found: val=0x%x pre1=%d pre2=%d\n", val, cell->preimage, preimage);
            break;
        }
        cell = cell->next;
    }
    return found;
}

//uint8_t in_hashtable(cell_t **h, uint32_t val, uint32_t preimage){
uint8_t in_hashtable(cell_t **h, uint32_t val){
    //if (is_in_list(h[val>>16], val, preimage)){
    if (is_in_list(h[val>>16], val)){
        return 1;
    }
    //add_list(&(h[val>>16]), val, preimage);
    add_list(&(h[val>>16]), val);
    return 0;
}



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


uint64_t coll_search(uint32_t k, uint32_t (*fun)(uint32_t, uint32_t)){
    cell_t **h = malloc((1<<16) * sizeof(cell_t *));
    for (uint64_t i = 0; i <= 0xffffffff; i++){
        //if (in_hashtable(h, fun(k, i), i)){
        if (in_hashtable(h, fun(k, i))){
            return i;
        }
    }
    return 0;
}



uint64_t  rand32(){
  uint32_t x=0x0;
  x |= (rand() & 0xff) << 8;
  x = rand() & 0xff;
  x |= (rand() & 0xff) << 16;
  x |= (rand() & 0xff) << 24;

  //printf("%"PRIx32"\n",x);
  return x;
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
    uint32_t k1 = 0x0;
    uint32_t m1 = 0x0;
    uint32_t k2 = 0xcdef1234;
    uint32_t m2 = 0xab123478;
    FILE *fp;
  //  printf("f(%"PRIx32", %"PRIx32") = %"PRIx32"\n", k1, m1, twine_fun1(k1, m1));

    //printf("f(%"PRIx32", %"PRIx32") = %"PRIx32"\n", k2, m2, twine_fun1(k2, m2));
    fp = fopen( "out_file.txt", "w+" );

    for(int i=1;i<=1000;i++){

    uint32_t k = rand32();
    int res=0;
    res = coll_search(k, twine_fun1);
    if (res == 0){
        printf("collision not found for some reason\n");
    }
    else {
        printf("Collsion found after %d attempts.\n", res);
        fprintf(fp, " %d %d\n",i, res);
    }
  }
    fclose(fp);
    return 0;
}
