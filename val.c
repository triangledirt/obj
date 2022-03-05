#include <string.h>
#include "val.h"

static long cmpnum(double val1, double val2);
static long cmpstr(char val1[VAL_STRSZ], char val2[VAL_STRSZ]);

long cmpnum(double val1, double val2)
{
  long cmp;
  if (val1 < val2) {
    cmp = -1;
  } else if (val1 > val2) {
    cmp = 1;
  } else {
    cmp = 0;
  }
  return cmp;
}

long cmpstr(char val1[VAL_STRSZ], char val2[VAL_STRSZ])
{
  return strncmp(val1, val2, VAL_STRSZ);
}

long val_cmp(val_t *val1, val_t *val2, type_t type)
{
  long cmp;
  switch (type) {
    case type_num:
      cmp = cmpnum(val1->num, val2->num);
      break;
    case type_str:
      cmp = cmpstr(val1->str, val2->str);
      break;
  };
  return cmp;
}

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
