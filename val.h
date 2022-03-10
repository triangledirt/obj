#ifndef val_h
#define val_h

#include "case.h"
#include "type.h"

union val_t {
  double num;
  char str[CASE_STR];
};
typedef union val_t val_t;

void val_init(val_t *val, type_t type);
long val_compare(val_t *val1, val_t *val2, type_t type);
void val_copy(val_t *source, val_t *dest, type_t type);
void val_print(val_t *val, type_t type);

#endif
