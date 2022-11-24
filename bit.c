#include <stdlib.h>
#include "bit.h"

char obj_bit_char(obj_bit_t bit)
{
  return (bit != 0) ? '1' : '0';
}

obj_bit_t obj_bit_random()
{
  return random() % 2;
}

void obj_bit_randomize(obj_bit_t *bit)
{
  *bit = random() % 2;
}
