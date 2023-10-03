#include <stdio.h>
#include "coord.h"

void obj_coord_print(struct obj_coord_t *coord)
{
  printf("coord::%ld,%ld,%ld\n", coord->x, coord->y, coord->z);
}
