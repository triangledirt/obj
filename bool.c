#include "bool.h"

void case_bool_invert(case_bool_t *bool)
{
  *bool = bool ? false : true;
}
