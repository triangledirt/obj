#include <stdio.h>
#include <string.h>
#include "val.h"

static long comparenum(double val1, double val2);
static long comparestr(char val1[CASE_STR], char val2[CASE_STR]);

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

long comparestr(char val1[CASE_STR], char val2[CASE_STR])
{
  return strncmp(val1, val2, CASE_STR - 1);
}

long val_compare(val_t *val1, val_t *val2, valtype_t valtype)
{
  long compare;
  switch (valtype) {
    case valtype_num:
      compare = comparenum(val1->num, val2->num);
      break;
    case valtype_str:
      compare = comparestr(val1->str, val2->str);
      break;
  }
  return compare;
}

void val_copy(val_t *source, val_t *dest, valtype_t valtype)
{
  switch (valtype) {
    case valtype_num:
      dest->num = source->num;
      break;
    case valtype_str:
      strncpy(dest->str, source->str, CASE_STR - 1);
      break;
  }
}

void val_init(val_t *val, valtype_t valtype)
{
  switch (valtype) {
    case valtype_num:
      val->num = 0.0;
      break;
    case valtype_str:
      val->str[0] = '\0';
      break;
  }
}

void val_print(val_t *val, valtype_t valtype)
{
  switch (valtype) {
    case valtype_num:
      printf("%f", val->num);
      break;
    case valtype_str:
      printf("%s", val->str);
      break;
  }
}
