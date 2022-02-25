#include <string.h>
#include "val.h"

void val_copy(val_t *dest, val_t *src)
{
  dest->num = src->num;
  memcpy(dest->str, src->str, VAL_STR_SZ);
}

void val_init(val_t *val)
{
  val->num = 0.0;
  memset(val->str, ' ', VAL_STR_SZ);
}
