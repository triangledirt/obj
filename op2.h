#ifndef obj_op2_h
#define obj_op2_h

#include "bit.h"

typedef char obj_op2_t;

obj_bit_t obj_op2_calc(obj_op2_t op2, obj_bit_t bit1, obj_bit_t bit2);

void obj_op2_randomize(obj_op2_t *op2);

#endif
