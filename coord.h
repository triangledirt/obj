#ifndef coord_h
#define coord_h

struct coord_t {
  long x;
  long y;
  long z;
};
typedef struct coord_t coord_t;

void coord_print(coord_t *coord);

#endif
