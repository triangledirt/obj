#include <stdio.h>
#include "meshlong.h"

void obj_meshlong_print(struct obj_meshlong_t *meshlong)
{
  printf("game %d\n", meshlong->game);
  printf("inaddr1 %d\n", meshlong->inaddr1);
  printf("inaddr2 %d\n", meshlong->inaddr2);
  printf("outaddr %d\n", meshlong->outaddr);
}
