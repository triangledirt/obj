#ifndef bit_h
#define bit_h

#include <stdlib.h>

typedef char case_bit_t;

case_bit_t case_bit_random();
void case_bit_randomize(case_bit_t *bit);
char case_bit_char(case_bit_t bit);

#endif
