#ifndef bool_h
#define bool_h

enum case_bool_t {
  case_bool_false,
  case_bool_true
};
typedef enum case_bool_t case_bool_t;

void case_bool_invert(case_bool_t *bool);

#endif
