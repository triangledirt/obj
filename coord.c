#include <stdio.h>
#include "coord.h"

void obj_coord_print(obj_coord_t *obj_coord)
{
  printf("coord::%ld,%ld,%ld\n", coord->x, coord->y, coord->z);
}
