#include "bit.h"

void case_bit_randomize(case_bit_t *bit)
{
  *bit = random() % 2;
}
