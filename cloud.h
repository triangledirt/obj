#ifndef obj_cloud_h
#define obj_cloud_h

#include "obj.h"

#define CLOUD OBJ

struct obj_cloud_t {
 obj_t object[CLOUD];
};

void obj_cloud_init(struct obj_cloud_t *cloud);

void obj_cloud_push(struct obj_cloud_t *cloud, obj_t obj);
obj_t obj_cloud_pull(struct obj_cloud_t *cloud);

obj_t obj_cloud_swap(struct obj_cloud_t *cloud, obj_t obj);

#endif
