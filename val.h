#ifndef val_h
#define val_h

#include "case.h"
#include "valtype.h"

union val_t {
  double num;
  char str[CASE_STR];
};
typedef union val_t val_t;

void val_init(val_t *val, valtype_t valtype);
long val_compare(val_t *val1, val_t *val2, valtype_t valtype);
void val_copy(val_t *source, val_t *dest, valtype_t valtype);
void val_print(val_t *val, valtype_t valtype);

#endif
