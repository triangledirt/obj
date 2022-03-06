#ifndef type_h
#define type_h

enum type_t {
  type_num,
  type_str
};
typedef enum type_t type_t;

char *type_name(type_t type);

#endif
