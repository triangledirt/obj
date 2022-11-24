#include "valtype.h"

static char *numname = "num";
static char *strname = "str";

char *obj_valtype_name(enum obj_valtype_t valtype)
{
  char *str;
  switch (valtype) {
  case obj_valtype_num:
    str = numname;
    break;
  case obj_valtype_str:
    str = strname;
    break;
  }
  return str;
}
