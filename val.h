#ifndef obj_val_h
#define obj_val_h

#include "class.h"
#include "valtype.h"

union obj_val_t {
  double num;
  char str[OBJ_CLASS_STR];
};
typedef union obj_val_t obj_val_t;

void obj_val_init(obj_val_t *val, obj_valtype_t valtype);
long obj_val_compare(obj_val_t *val1, obj_val_t *val2, obj_valtype_t valtype);
void obj_val_copy(obj_val_t *source, obj_val_t *dest, obj_valtype_t valtype);
void obj_val_print(obj_val_t *val, obj_valtype_t valtype);

#endif
