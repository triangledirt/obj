#ifndef obj_valtype_h
#define obj_valtype_h

enum obj_valtype_t {
  obj_valtype_num,
  obj_valtype_str
};

char *obj_valtype_name(enum obj_valtype_t valtype);

#endif
