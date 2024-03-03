#ifndef obj_meshlong_h
#define obj_meshlong_h

struct obj_meshlong_t {
  char game;
  char inaddr1;
  char inaddr2;
  char outaddr;
};

void obj_meshlong_print(struct obj_meshlong_t *meshlong);

#endif
