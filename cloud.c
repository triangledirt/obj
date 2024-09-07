#include <stdlib.h>
#include "bool.h"
#include "cloud.h"

static void init();

void obj_cloud_init(struct obj_cloud_t *cloud)
{
  long i;
  for (i = 0; i < CLOUD; i++)
    obj_randomize(&cloud->object[i]);
}

obj_t obj_cloud_pull(struct obj_cloud_t *cloud)
{
  long i;
  i = random() % CLOUD;
  return cloud->object[i];
}

void obj_cloud_push(struct obj_cloud_t *cloud, obj_t obj)
{
  long i;
  i = random() % CLOUD;
  cloud->object[i] = obj;
}

obj_t obj_cloud_swap(struct obj_cloud_t *cloud, obj_t obj)
{
  long i;
  obj_t o;
  i = random() % CLOUD;
  o = cloud->object[i];
  cloud->object[i] = obj;
  return o;
}
