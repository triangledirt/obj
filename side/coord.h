#ifndef obj_coord_h
#define obj_coord_h

struct obj_coord_t {
  long x;
  long y;
  long z;
};

void obj_coord_print(struct obj_coord_t *coord);

#endif