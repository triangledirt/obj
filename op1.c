#include <stdlib.h>
#include "op1.h"

static obj_bit_t value[4][2] = {
  { 0, 0 },
  { 0, 1 },
  { 1, 0 },
  { 1, 1 }
};

obj_bit_t obj_op1_calc(obj_op1_t op1, obj_bit_t bit)
{
  return value[op1][bit];
}

void obj_op1_randomize(obj_op1_t *op1)
{
  *op1 = random() % 4;
}
