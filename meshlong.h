#ifndef obj_meshlong_h
#define obj_meshlong_h

struct obj_meshlong_t {
  char game;
  char inaddr1;
  char inaddr2;
  char outaddr;
};

char obj_meshlong_face(struct obj_meshlong_t *meshlong);
void obj_meshlong_print(struct obj_meshlong_t *meshlong);

#endif
