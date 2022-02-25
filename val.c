#include <string.h>
#include "val.h"

void val_init(val_t *val)
{
  val->num = 0.0;
  memset(val->str, ' ', VAL_STR_SZ);
}
