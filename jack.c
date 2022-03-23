#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "jack.h"
#include "node.h"
#include "obj.h"
#include "op.h"

static case_bool_t once = case_bool_false;
static double fitness[CASE_OBJ];
static node_t node[CASE_OBJ][CASE_OBJ - 1][CASE_OBJ - 1];

static void init();
static double score(case_obj_t obj, long type);

void init()
{
  long type;
  long i;
  long j;
  if (!once) {
    for (type = 0; type < CASE_OBJ; type++)
      for (i = 0; i < (CASE_OBJ - 1); i++)
        for (j = 0; j < ((CASE_OBJ - 1) - i); j++)
          node_init(&node[type][i][j]);
    once = case_bool_true;
  }
}

void jack_learn(case_obj_t obj[], long objsz, long type)
{
  case_bit_t values[CASE_OBJ - 1][CASE_OBJ - 1];
  long i;
  long j;
  case_bit_t val;
  case_bit_t bit1;
  case_bit_t bit2;
  case_bit_t op;
  init();
  for (i = 0; i < (CASE_OBJ - 1); i++)
    for (j = 0; j < ((CASE_OBJ - 1) - i); j++) {
/*
      op = ..;
      bit1 = ..;
      bit2 = ..;
      val = op_calc(op, bit1, bit2);
      set val in values;
*/
    }
  /* val = the last value in the net; */
#if CASE_VERBOSE
  printf("type%02ld ideal jck                                                                 ", type);
  /* case_obj_print(one[type]); */
  printf(" %0.3f\n", fitness[type]);
#endif
}

double jack_score(case_obj_t obj, long type)
{
  init();
  return score(obj, type);
}

double score(case_obj_t obj, long type)
{
}
