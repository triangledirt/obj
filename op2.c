#include <stdlib.h>
#include "op2.h"

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

obj_bit_t obj_op2_calc(obj_op2_t op2, obj_bit_t bit1, obj_bit_t bit2)
{
  return value[op2][bit1][bit2];
}

void obj_op2_randomize(obj_op2_t *op2)
{
  *op2 = random() % 16;
}
