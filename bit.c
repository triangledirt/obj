#include "bit.h"

char case_bit_char(case_bit_t bit)
{
  return (bit != 0) ? '1' : '0';
}

case_bit_t case_bit_random()
{
  return random() % 2;
}

void case_bit_randomize(case_bit_t *bit)
{
  *bit = random() % 2;
}
