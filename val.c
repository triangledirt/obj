#include <stdio.h>
#include <string.h>
#include "val.h"

static long comparenum(double val1, double val2);
static long comparestr(char val1[VAL_STRSZ], char val2[VAL_STRSZ]);

long comparenum(double val1, double val2)
{
  long compare;
  if (val1 < val2) {
    compare = -1;
  } else if (val1 > val2) {
    compare = 1;
  } else {
    compare = 0;
  }
  return compare;
}

long comparestr(char val1[VAL_STRSZ], char val2[VAL_STRSZ])
{
  return strncmp(val1, val2, VAL_STRSZ - 1);
}

long val_compare(val_t *val1, val_t *val2, type_t type)
{
  long compare;
  switch (type) {
    case type_num:
      compare = comparenum(val1->num, val2->num);
      break;
    case type_str:
      compare = comparestr(val1->str, val2->str);
      printf(":%s:%s:\n", val1->str, val2->str);
      break;
  };
  return compare;
}

void val_copy(val_t *source, val_t *dest, type_t type)
{
  switch (type) {
    case type_num:
      dest->num = source->num;
      break;
    case type_str:
      strncpy(dest->str, source->str, VAL_STRSZ - 1);
      break;
  };
}

void val_init(val_t *val, type_t type)
{
  switch (type) {
    case type_num:
      val->num = 0.0;
      break;
    case type_str:
      val->str[0] = '\0';
      break;
  };
}
