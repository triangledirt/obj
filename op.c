#include "op.h"

static obj_bit_t value[16][2][2] = {
  { { 0, 0 }, { 0, 0 } },
  { { 0, 0 }, { 0, 1 } },
  { { 0, 0 }, { 1, 0 } },
  { { 0, 0 }, { 1, 1 } },
  { { 0, 1 }, { 0, 0 } },
  { { 0, 1 }, { 0, 1 } },
  { { 0, 1 }, { 1, 0 } },
  { { 0, 1 }, { 1, 1 } },
  { { 1, 0 }, { 0, 0 } },
  { { 1, 0 }, { 0, 1 } },
  { { 1, 0 }, { 1, 0 } },
  { { 1, 0 }, { 1, 1 } },
  { { 1, 1 }, { 0, 0 } },
  { { 1, 1 }, { 0, 1 } },
  { { 1, 1 }, { 1, 0 } },
  { { 1, 1 }, { 1, 1 } }
};

obj_bit_t obj_op_calc(obj_op_t op, obj_bit_t bit1, obj_bit_t bit2)
{
  return value[op][bit1][bit2];
}

void obj_op_randomize(obj_op_t *op)
{
  *op = random() % 16;
}
