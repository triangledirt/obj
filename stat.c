#include "stat.h"

void obj_stat_reset(struct obj_stat_t *stat)
{
  stat->zeroes = 0;
  stat->ones = 0;
}
