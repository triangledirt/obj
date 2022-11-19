#include "node.h"

void obj_node_init(obj_node_t *node)
{
  obj_op_randomize(&node->op);
  obj_bit_randomize(&node->val);
  node->active1 = 0;
  node->active2 = 0;
}
