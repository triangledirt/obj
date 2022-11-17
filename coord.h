#ifndef obj_coord_h
#define obj_coord_h

struct obj_coord_t {
  long x;
  long y;
  long z;
};
typedef struct obj_coord_t obj_coord_t;

void obj_coord_print(obj_coord_t *coord);

#endif
