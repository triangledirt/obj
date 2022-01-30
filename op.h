#ifndef op_h
#define op_h

#include "bit.h"

typedef char op_t;

case_bit_t op_calc(op_t op, case_bit_t bit1, case_bit_t bit2);

void op_randomize(op_t *op);

#endif
