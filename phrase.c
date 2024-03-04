#include <stdlib.h>
#include "phrase.h"

enum obj_bool_t obj_phrase_addmesh(struct obj_phrase_t *phrase, struct obj_mesh_t *mesh)
{
  enum obj_bool_t success;
  if (phrase->meshsz < OBJ_PHRASE_MESH) {
    phrase->mesh[phrase->meshsz] = *mesh;
    phrase->meshsz++;
    success = obj_bool_true;
  } else {
    success = obj_bool_false;
  }
  return success;
}

void obj_phrase_apply(struct obj_phrase_t *phrase, struct obj_mesh_t *data)
{
  long i;
  for (i = 0; i < phrase->meshsz; i++)
    obj_mesh_play(&phrase->mesh[i], data);
}

void obj_phrase_init(struct obj_phrase_t *phrase)
{
  phrase->meshsz = 0;
}

void obj_phrase_mutate(struct obj_phrase_t *phrase)
{
  long i;
  i = random() % phrase->meshsz;
  obj_mesh_mutate(&phrase->mesh[i]);
}

void obj_phrase_print(struct obj_phrase_t *phrase)
{
  long i;
  for (i = 0; i < phrase->meshsz; i++)
    obj_mesh_print(&phrase->mesh[i]);
}

void obj_phrase_randomize(struct obj_phrase_t *phrase)
{
  long i;
  phrase->meshsz = random() % OBJ_PHRASE_MESH;
  for (i = 0; i < phrase->meshsz; i++)
    obj_mesh_init(&phrase->mesh[i]);
}
