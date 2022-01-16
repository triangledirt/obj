#include <stdlib.h>
#include "trace.h"

static unsigned char trace[256];

unsigned char trace_calc(unsigned char indx)
{
  return trace[indx];
}

unsigned char trace_prog(unsigned char indx, unsigned char val)
{
  trace[indx] = val;
}

unsigned char trace_randomize()
{
  unsigned char i;
  for (i = 0; i < 256; i++)
    trace[i] = random() % 256;
}
