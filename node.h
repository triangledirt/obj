#ifndef node_h
#define node_h

#include "bit.h"
#include "op.h"

struct node_t {
  op_t op;
  case_bit_t val;
  long active1;
  long active2;
};
typedef struct node_t node_t;

void node_init(node_t *node);

#endif
