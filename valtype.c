#include "valtype.h"

static char *numname = "num";
static char *strname = "str";

char *valtype_name(valtype_t valtype)
{
  char *str;
  switch (valtype) {
    case valtype_num:
      str = numname;
      break;
    case valtype_str:
      str = strname;
      break;
  }
  return str;
}
