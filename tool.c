#include "tool.h"

unsigned long tool_wrapidx(long idx, unsigned long range)
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
