#ifndef obj_op3_h
#define obj_op3_h

#include "bit.h"

typedef char obj_op3_t;

obj_bit_t obj_op3_calc(obj_op3_t op3, obj_bit_t bit1, obj_bit_t bit2, obj_bit_t bit3);

void obj_op3_randomize(obj_op3_t *op3);

#endif
