#include <stdio.h>
#include "bit.h"
#include "class.h"
#include "coin.h"
#include "jack.h"
#include "node.h"
#include "obj.h"
#include "op.h"

static obj_bool_t once = obj_bool_false;
static double fitness[OBJ];
static obj_node_t node[OBJ][OBJ - 1][OBJ - 1];

static void init();
static double score(obj_t obj, long type);

void init()
{
  long type;
  long i;
  long j;
  if (!once) {
    for (type = 0; type < OBJ; type++)
      for (i = 0; i < (OBJ - 1); i++)
        for (j = 0; j < ((OBJ - 1) - i); j++)
          obj_node_init(&node[type][i][j]);
    once = obj_bool_true;
  }
}

void obj_jack_learn(obj_t obj[], long objsz, long type)
{
  obj_bit_t values[OBJ - 1][OBJ - 1];
  long i;
  long j;
  obj_bit_t val;
  obj_bit_t bit1;
  obj_bit_t bit2;
  obj_bit_t op;
  init();
  for (i = 0; i < (OBJ - 1); i++)
    for (j = 0; j < ((OBJ - 1) - i); j++) {
/*
      op = ..;
      bit1 = ..;
      bit2 = ..;
      val = op_calc(op, bit1, bit2);
      set val in values;
*/
    }
  /* val = the last value in the net; */
#if OBJ_VERBOSE
  printf("type%02ld ideal jck                                                                 ", type);
  /* obj_print(one[type]); */
  printf(" %0.3f\n", fitness[type]);
#endif
}

double obj_jack_score(obj_t obj, long type)
{
  init();
  return score(obj, type);
}

double score(obj_t obj, long type)
{
}
