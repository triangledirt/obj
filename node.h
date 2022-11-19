#ifndef obj_node_h
#define obj_node_h

#include "bit.h"
#include "op.h"

struct obj_node_t {
  obj_op_t op;
  obj_bit_t val;
  long active1;
  long active2;
};
typedef struct obj_node_t obj_node_t;

void obj_node_init(obj_node_t *node);

#endif
