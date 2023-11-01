#include <stdlib.h>
#include "binop.h"

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

obj_bit_t obj_binop_calc(obj_binop_t binop, obj_bit_t bit1, obj_bit_t bit2)
{
  return value[binop][bit1][bit2];
}

void obj_binop_randomize(obj_binop_t *binop)
{
  *binop = random() % 16;
}
