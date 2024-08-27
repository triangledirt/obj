#include <stdlib.h>
#include "bool.h"
#include "cloud.h"

#define CLOUD OBJ

static enum obj_bool_t once = obj_bool_false;
static obj_t cloud[OBJ_CLOUD_TYPE][CLOUD];

static void init();

void init()
{
  long type;
  long i;
  if (!once) {
    for (type = 0; type < OBJ_CLOUD_TYPE; type++)
      for (i = 0; i < CLOUD; i++)
        obj_randomize(&cloud[type][i]);
    once = obj_bool_true;
  }
}

obj_t obj_cloud_swap(obj_t obj, long type)
{
  long i;
  obj_t o;
  init();
  i = random() % CLOUD;
  o = cloud[type][i];
  cloud[type][i] = obj;
  return o;
}
