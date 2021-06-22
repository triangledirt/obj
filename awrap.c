#include "awrap.h"

unsigned long awrap_index(long idx, unsigned long range)
{
  unsigned long wrapped;
  if (idx >= (long) range) {
    wrapped = idx % range;
  } else if (idx < 0) {
    if (1 == range) {
      wrapped = 0;
    } else {
      wrapped = range - ((-1 * idx) % range);
    }
  } else {
    wrapped= idx;
  }
  return wrapped;
}
