#ifndef obj_phrase_h
#define obj_phrase_h

#include "bool.h"
#include "mesh.h"

#define MESH_MAX 16

struct obj_phrase_t {
  struct obj_mesh_t mesh[MESH_MAX];
  long meshsz;
};

void obj_phrase_init(struct obj_phrase_t *phrase);
enum obj_bool_t obj_phrase_addmesh(struct obj_phrase_t *phrase, struct obj_mesh_t *mesh);
void obj_phrase_apply(struct obj_phrase_t *phrase, struct obj_mesh_t *data);
void obj_phrase_print(struct obj_phrase_t *phrase);

#endif
