#ifndef valtype_h
#define valtype_h

enum valtype_t {
  valtype_num,
  valtype_str
};
typedef enum valtype_t valtype_t;

char *valtype_name(valtype_t valtype);

#endif
