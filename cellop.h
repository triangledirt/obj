#ifndef obj_cellop_h
#define obj_cellop_h

#include "bit.h"

typedef char obj_cellop_t;

obj_bit_t obj_cellop_calc(obj_cellop_t cellop, obj_bit_t bit1, obj_bit_t bit2, obj_bit_t bit3);

void obj_cellop_randomize(obj_cellop_t *cellop);

#endif
