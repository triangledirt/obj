#include <stdlib.h>
#include "phrase.h"

#define min(a, b) ((a < b) ? a : b)

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

struct obj_phrase_t obj_phrase_cross(struct obj_phrase_t *phrase1, struct obj_phrase_t *phrase2)
{
  struct obj_phrase_t new;
  long x;
  long m;
  long b;
  enum obj_bool_t on1 = obj_bool_true;
  obj_bit_t bit;
  long newmeshsz;
  long bitmax;
  newmeshsz = min(phrase1->meshsz, phrase2->meshsz);
  new.meshsz = newmeshsz;
  bitmax = 16 * newmeshsz;
  for (x = 0; x < bitmax; x++) {
    m = x / 16;
    b = x % 16;
    if (0 == (random() % 128))
      on1 = obj_bool_random();
    if (on1) {
      bit = phrase1->mesh[m].bit[b];
    } else {
      bit = phrase2->mesh[m].bit[b];
    }
    new.mesh[m].bit[b] = bit;
  }
  return new;
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
