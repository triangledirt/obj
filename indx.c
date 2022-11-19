#include "indx.h"

unsigned long obj_indx_wrap(long indx, unsigned long range)
{
  unsigned long wrap;
  if (indx >= (long) range) {
    wrap = indx % range;
  } else if (indx < 0) {
    wrap = (1 == range) ? 0 : range - (-indx % range);
  } else {
    wrap = indx;
  }
  return wrap;
}
