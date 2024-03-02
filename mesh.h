#ifndef obj_mesh_h
#define obj_mesh_h

#include "bit.h"

struct obj_mesh_t {
  obj_bit_t bit[16];
};

struct obj_meshlong_t {
  char game;
  char inaddr1;
  char inaddr2;
  char outaddr;
};

void obj_mesh_init(struct obj_mesh_t *mesh);
void obj_mesh_initlong(struct obj_meshlong_t *meshlong, struct obj_mesh_t *mesh);
void obj_mesh_play(struct obj_mesh_t *mesh1, struct obj_mesh_t *mesh2);
void obj_mesh_print(struct obj_mesh_t *mesh);
void obj_meshlong_print(struct obj_meshlong_t *mesh);

#endif
