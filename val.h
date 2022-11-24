#ifndef obj_val_h
#define obj_val_h

#include "class.h"
#include "valtype.h"

union obj_val_t {
  double num;
  char str[OBJ_CLASS_STR];
};

void obj_val_init(union obj_val_t *val, enum obj_valtype_t valtype);
long obj_val_compare(union obj_val_t *val1, union obj_val_t *val2, enum obj_valtype_t valtype);
void obj_val_copy(union obj_val_t *source, union obj_val_t *dest, enum obj_valtype_t valtype);
void obj_val_print(union obj_val_t *val, enum obj_valtype_t valtype);

#endif
