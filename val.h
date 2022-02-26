#ifndef val_h
#define val_h

#define VAL_STRSZ 8

union val_t {
  double num;
  char str[VAL_STRSZ];
};
typedef union val_t val_t;

void val_init(val_t *val);
void val_copy(val_t *source, val_t *dest);

#endif
