#include "tool.h"

unsigned long tool_wrapidx(long idx, unsigned long range)
{
  unsigned long wrap;
  if (idx >= (long) range) {
    wrap = idx % range;
  } else if (idx < 0) {
    if (1 == range) {
      wrap = 0;
    } else {
      wrap = range - (-idx % range);
    }
  } else {
    wrap = idx;
  }
  return wrap;
}
