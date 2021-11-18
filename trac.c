#include <stdlib.h>
#include "trac.h"

unsigned char trac[256];

unsigned char trac_calc(unsigned char idx)
{
  return trac[idx];
}

unsigned char trac_prog(unsigned char idx, unsigned char val)
{
  trac[idx] = val;
}

unsigned char trac_randomize()
{
  unsigned char idx;
  for (idx = 0; idx < 256; idx++) {
    trac[idx] = random() % 256;
  }
}
