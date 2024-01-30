#include <stdlib.h>
#include "bool.h"
#include "goo.h"

#define CLOUD OBJ

static enum obj_bool_t once = obj_bool_false;
static obj_t cloud[CLOUD];

static void init();

void init()
{
  long i;
  if (!once) {
    for (i = 0; i < CLOUD; i++)
      obj_randomize(&cloud[i]);
    once = obj_bool_true;
  }
}

obj_t obj_goo_cloud(obj_t obj)
{
  long i;
  obj_t o;
  init();
  i = random() % CLOUD;
  o = cloud[i];
  cloud[i] = obj;
  return o;
}
