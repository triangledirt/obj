#include <stdio.h>
#include <string.h>
#include "val.h"

static long comparenum(double val1, double val2);
static long comparestr(char val1[OBJ_CLASS_STR], char val2[OBJ_CLASS_STR]);

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

long comparestr(char val1[OBJ_CLASS_STR], char val2[OBJ_CLASS_STR])
{
  return strncmp(val1, val2, OBJ_CLASS_STR - 1);
}

long obj_val_compare(obj_val_t *val1, obj_val_t *val2, obj_valtype_t valtype)
{
  long compare;
  switch (valtype) {
  case obj_valtype_num:
    compare = comparenum(val1->num, val2->num);
    break;
  case obj_valtype_str:
    compare = comparestr(val1->str, val2->str);
    break;
  }
  return compare;
}

void obj_val_copy(obj_val_t *source, obj_val_t *dest, obj_valtype_t valtype)
{
  switch (valtype) {
  case obj_valtype_num:
    dest->num = source->num;
    break;
  case obj_valtype_str:
    strncpy(dest->str, source->str, OBJ_CLASS_STR - 1);
    break;
  }
}

void obj_val_init(obj_val_t *val, obj_valtype_t valtype)
{
  switch (valtype) {
  case obj_valtype_num:
    val->num = 0.0;
    break;
  case obj_valtype_str:
    val->str[0] = '\0';
    break;
  }
}

void obj_val_print(obj_val_t *val, obj_valtype_t valtype)
{
  switch (valtype) {
  case obj_valtype_num:
    printf("%f", val->num);
    break;
  case obj_valtype_str:
    printf("%s", val->str);
    break;
  }
}
