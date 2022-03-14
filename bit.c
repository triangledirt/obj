#include "bit.h"

case_bit_t case_bit_random()
{
  return random() % 2;
}

void case_bit_randomize(case_bit_t *bit)
{
  *bit = random() % 2;
}
