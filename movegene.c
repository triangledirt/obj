#include "movegene.h"

void obj_movegene_parse(struct obj_movegene_t *movegene, obj_t obj, long start)
{
  long posindx;
  posindx = obj_num(obj, start, 3);
  switch (posindx) {
  case 0:
    movegene->xoffset = 0;
    movegene->yoffset = 1;
    break;
  case 1:
    movegene->xoffset = 1;
    movegene->yoffset = 1;
    break;
  case 2:
    movegene->xoffset = 1;
    movegene->yoffset = 0;
    break;
  case 3:
    movegene->xoffset = 1;
    movegene->yoffset = -1;
    break;
  case 4:
    movegene->xoffset = 0;
    movegene->yoffset = -1;
    break;
  case 5:
    movegene->xoffset = -1;
    movegene->yoffset = -1;
    break;
  case 6:
    movegene->xoffset = -1;
    movegene->yoffset = 0;
    break;
  case 7:
    movegene->xoffset = -1;
    movegene->yoffset = 1;
    break;
  }
}
