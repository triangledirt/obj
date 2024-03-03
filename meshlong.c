#include <stdio.h>
#include "meshlong.h"

char obj_meshlong_face(struct obj_meshlong_t *meshlong)
{
  char c = 97;
  c += 0;
  ;;
  return c;
}

void obj_meshlong_print(struct obj_meshlong_t *meshlong)
{
  printf("game %d\n", meshlong->game);
  printf("inaddr1 %d\n", meshlong->inaddr1);
  printf("inaddr2 %d\n", meshlong->inaddr2);
  printf("outaddr %d\n", meshlong->outaddr);
}
