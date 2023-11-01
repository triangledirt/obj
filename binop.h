#ifndef obj_binop_h
#define obj_binop_h

#include "bit.h"

typedef char obj_binop_t;

obj_bit_t obj_binop_calc(obj_binop_t binop, obj_bit_t bit1, obj_bit_t bit2);

void obj_binop_randomize(obj_binop_t *binop);

#endif
