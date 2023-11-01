#ifndef obj_op_h
#define obj_op_h

#include "bit.h"

typedef char obj_op_t;

obj_bit_t obj_op_calc(obj_op_t op, obj_bit_t bit1, obj_bit_t bit2);

void obj_op_randomize(obj_op_t *op);

#endif
