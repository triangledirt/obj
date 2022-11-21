#ifndef obj_bool_h
#define obj_bool_h

enum obj_bool_t {
  obj_bool_false,
  obj_bool_true
};
typedef enum obj_bool_t obj_bool_t;

void obj_bool_invert(obj_bool_t *bool);

#endif

