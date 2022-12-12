#include <math.h>
#include <stdio.h>
#include "modelstat.h"

/*
void obj_modelstat_note(struct obj_modelstat_t *stat, obj_bit_t guess, obj_bit_t actual)
{
}
*/

void obj_modelstat_print(struct obj_modelstat_t *stat, long type)
{
  printf("ticks=%lu\n", stat->ticks);
  printf("talks=%lu\n", stat->talks);
  printf("swaps=%lu\n", stat->swaps);
}

void obj_modelstat_reset(struct obj_modelstat_t *stat)
{
  stat->ticks = 0;
  stat->talks = 0;
  stat->swaps = 0;
}
