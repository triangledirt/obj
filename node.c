#include "node.h"

void node_init(node_t *node)
{
  op_randomize(&node->op);
  case_bit_randomize(&node->val);
  node->active1 = 0;
  node->active2 = 0;
}
