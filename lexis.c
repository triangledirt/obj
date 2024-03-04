#include "lexis.h"

void obj_lexis_init(struct obj_lexis_t *lexis, long meshsz)
{
  long p;
  long m;
  struct obj_mesh_t mesh;
  for (p = 0; p < OBJ_LEXIS_PHRASE; p++)
    for (m = 0; m < meshsz; m++) {
      obj_mesh_init(&mesh);
      obj_phrase_addmesh(&lexis->phrase[p], &mesh);
    }
}
