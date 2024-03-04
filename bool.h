#ifndef obj_bool_h
#define obj_bool_h

enum obj_bool_t {
  obj_bool_false,
  obj_bool_true
};

void obj_bool_invert(enum obj_bool_t *bool);
enum obj_bool_t obj_bool_random();

#endif
