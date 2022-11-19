#ifndef obj_valtype_h
#define obj_valtype_h

enum obj_valtype_t {
  obj_valtype_num,
  obj_valtype_str
};
typedef enum obj_valtype_t obj_valtype_t;

char *obj_valtype_name(obj_valtype_t valtype);

#endif
