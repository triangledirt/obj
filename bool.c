#include "bool.h"

void obj_bool_invert(obj_bool_t *bool)
{
  *bool = bool ? obj_bool_false : obj_bool_true;
}
