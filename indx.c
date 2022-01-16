#include "indx.h"

unsigned long indx_wrap(long i, unsigned long range)
{
  unsigned long wrap;
  if (i >= (long) range) {
    wrap = i % range;
  } else if (i < 0) {
    wrap = (1 == range) ? 0 : range - (-i % range);
  } else {
    wrap = i;
  }
  return wrap;
}
