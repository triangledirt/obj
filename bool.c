#include "bool.h"

void case_bool_invert(case_bool_t *bool)
{
  *bool = bool ? case_bool_false : case_bool_true;
}
