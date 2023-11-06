#include "index.h"

unsigned long obj_index_wrap(long index, unsigned long range)
{
  unsigned long wrap;
  if (index >= (long) range) {
    wrap = index % range;
  } else if (index < 0) {
    wrap = (1 == range) ? 0 : range - (-index % range);
  } else {
    wrap = index;
  }
  return wrap;
}
