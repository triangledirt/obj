#include <stdio.h>
#include "coord.h"

void coord_print(coord_t *coord)
{
  printf("coord::%ld,%ld,%ld\n", coord->x, coord->y, coord->z);
}
