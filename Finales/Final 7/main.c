#include "dna.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef TEST

int main()
{
    
   dna_t dna1 = dna_T();
   dna_t dna2 = dna_A();
   dna_t dna3 = dna_join (dna1, dna2);
    dna_print(dna3);
    printf("DNA1: ");
    

    
    dna_destroy(dna1);
    dna_destroy(dna2);
}

#endif
