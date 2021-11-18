#include <stdlib.h>
#include "atrac.h"

unsigned char trac[256];

unsigned char atrac_calc(unsigned char idx)
{
  return trac[idx];
}

unsigned char atrac_prog(unsigned char idx, unsigned char val)
{
  trac[idx] = val;
}

unsigned char atrac_randomize()
{
  unsigned char idx;
  for (idx = 0; idx < 256; idx++) {
    trac[idx] = random() % 256;
  }
}
