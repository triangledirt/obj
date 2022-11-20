#include "person.h"

static obj_t person[OBJ_PERSON_TYPE][OBJ_PERSON_CACHE];
static obj_bool_t once = obj_bool_false;
static obj_classstat_t stat[OBJ_CLASS_TYPE];

static void init();

void init()
{
  long i;
  long type;
  if (!once) {
    srandom(time(NULL));
    for (type = 0; type < OBJ_PERSON_TYPE; type++) {
      for (i = 0; i < OBJ_PERSON_CACHE; i++)
        obj_randomize(&person[type][i]);
      obj_personstat_reset(&stat[type]);
    }
    once = obj_bool_true;
  }
}

long obj_person_classify(obj_t obj, long type)
{
  init();
}

void obj_person_observe(obj_t obj, long type)
{
  init();
}

obj_personstat_t *obj_person_stat(long type)
{
}

void obj_person_resetstat(long type)
{
}
