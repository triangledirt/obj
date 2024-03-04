#ifndef obj_phrase_h
#define obj_phrase_h

#include "bool.h"
#include "mesh.h"

#define OBJ_PHRASE_MESH 16

struct obj_phrase_t {
  struct obj_mesh_t mesh[OBJ_PHRASE_MESH];
  long meshsz;
};

void obj_phrase_init(struct obj_phrase_t *phrase);
enum obj_bool_t obj_phrase_addmesh(struct obj_phrase_t *phrase, struct obj_mesh_t *mesh);
void obj_phrase_apply(struct obj_phrase_t *phrase, struct obj_mesh_t *data);

void obj_phrase_randomize(struct obj_phrase_t *phrase);
void obj_phrase_mutate(struct obj_phrase_t *phrase);
struct obj_phrase_t obj_phrase_cross(struct obj_phrase_t *phrase1, struct obj_phrase_t *phrase2);

void obj_phrase_print(struct obj_phrase_t *phrase);

#endif
