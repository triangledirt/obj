#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "jack.h"
#include "node.h"
#include "obj.h"
#include "op.h"

static case_bit_t once = 0;
static double fitness[32];
static node_t node[32][31][31];

static void initonce();
static double score(case_obj_t obj, long type);

void initonce()
{
  long type;
  long i;
  long j;
  if (!once) {
    for (type = 0; type < 32; type++)
      for (i = 0; i < 31; i++)
        for (j = 0; j < (31 - i); j++)
          node_init(&node[type][i][j]);
    once = 1;
  }
}

case_bit_t jack_classify(case_obj_t obj, long type)
{
  /* return score(obj, type) > (0.9 * fitness[type]); */
}

void jack_learn(case_obj_t obj[], long objsz, long type)
{
  case_bit_t values[31][31];
  long i;
  long j;
  case_bit_t val;
  case_bit_t bit1;
  case_bit_t bit2;
  case_bit_t op;
  initonce();
  for (i = 0; i < 31; i++)
    for (j = 0; j < (31 - i); j++) {
      op = ..;
      bit1 = ..;
      bit2 = ..;
      val = op_calc(op, bit1, bit2);
      set val in values;
    }
  val = the last value in the net;
#if CASE_VERBOSE
  printf("type%ld net   ft1 ", type);
  /* case_obj_print(one[type]); */
  printf(" %0.3f%%\n", fitness[type]);
#endif
}

double score(case_obj_t obj, long type)
{
}
