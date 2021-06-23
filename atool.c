#include "atool.h"

unsigned long atool_wrapidx(long idx, unsigned long range)
{
  unsigned long wrap;
  if (idx >= (long) range) {
    wrap= idx % range;
  } else if (idx < 0) {
    if (1 == range) {
      wrap = 0;
    } else {
      wrap = range - ((-1 * idx) % range);
    }
  } else {
    wrap = idx;
  }
  return wrap;
}
