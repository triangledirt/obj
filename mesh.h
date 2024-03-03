#ifndef obj_mesh_h
#define obj_mesh_h

#include "bit.h"
#include "meshlong.h"

struct obj_mesh_t {
  obj_bit_t bit[16];
};

char obj_mesh_face(struct obj_mesh_t *mesh);
void obj_mesh_init(struct obj_mesh_t *mesh);
void obj_mesh_initlong(struct obj_meshlong_t *meshlong, struct obj_mesh_t *mesh);
void obj_mesh_mutate(struct obj_mesh_t *mesh);
void obj_mesh_play(struct obj_mesh_t *gamemesh, struct obj_mesh_t *datamesh);
void obj_mesh_print(struct obj_mesh_t *mesh);

#endif
