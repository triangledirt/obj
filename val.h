#ifndef val_h
#define val_h

#define VAL_STR_SZ 8

union val_t {
  double num;
  char str[VAL_STR_SZ];
};
typedef union val_t val_t;

#endif
