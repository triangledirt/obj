#include "index.h"

unsigned long index_wrap(long idx, unsigned long range)
{
  unsigned long wrap;
  if (idx >= (long) range) {
    wrap = idx % range;
  } else if (idx < 0) {
    wrap = (1 == range) ? 0 : range - (-idx % range);
  } else {
    wrap = idx;
  }
  return wrap;
}
