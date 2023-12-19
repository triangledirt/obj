#include <stdlib.h>
#include "bool.h"
#include "goo.h"

#define CLOUD 64

static enum obj_bool_t once = obj_bool_false;
static obj_t cloud[CLOUD];

static void init();

void init()
{
  long i;
  obj_t o;
  if (!once) {
    for (i = 0; i < CLOUD; i++) {
      obj_randomize(&o);
      cloud[i] = o;
    }
    once = obj_bool_true;
  }
}

obj_t obj_goo_cloud(obj_t obj)
{
  long i;
  init();
  i = random() % CLOUD;
  cloud[i] = obj;
  i = random() % CLOUD;
  return cloud[i];
}
