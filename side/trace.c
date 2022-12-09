#include <stdlib.h>
#include "trace.h"

static unsigned char trace[256];

unsigned char obj_trace_calc(unsigned char indx)
{
  return trace[indx];
}

unsigned char obj_trace_program(unsigned char indx, unsigned char val)
{
  trace[indx] = val;
}

unsigned char obj_trace_randomize()
{
  unsigned char i;
  for (i = 0; i < 256; i++)
    trace[i] = random() % 256;
}
