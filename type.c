#include "type.h"

static char *numname = "num";
static char *strname = "str";

char *type_name(type_t type)
{
  char *str;
  switch (type) {
    case type_num:
      str = numname;
      break;
    case type_str:
      str = strname;
      break;
  }
  return str;
}
