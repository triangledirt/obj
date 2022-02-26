#include <string.h>
#include "val.h"

void val_copy(val_t *source, val_t *dest)
{
  dest->num = source->num;
  memcpy(dest->str, source->str, VAL_STRSZ);
}

void val_init(val_t *val)
{
  val->num = 0.0;
  memset(val->str, ' ', VAL_STRSZ);
}
