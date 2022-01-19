#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "net.h"
#include "obj.h"
#include "op.h"

static case_bit_t once = 0;
static double fitness[32];
static case_bit_t op[32][31][31];

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
        for (j = 0; j < (31 - i); j++) {
          op_randomize(&op[type][i][j]);
        }
    once = 1;
  }
}

case_bit_t net_classify(case_obj_t obj, long type)
{
  /* return score(obj, type) > (0.9 * fitness[type]); */
}

void net_learn(case_obj_t obj[], long objsz, long type)
{
  initonce();
  ;;
#if CASE_VERBOSE
  printf("type%ld net   ft1 ", type);
  /* case_obj_print(one[type]); */
  printf(" %0.3f%%\n", fitness[type]);
#endif
}

double score(case_obj_t obj, long type)
{
  long i;
  long j;
  case_bool_t objmatch;
  case_bool_t matrxmatch;
  double s = 0.0;
  if (!once)
    for (i = 0; i < 32; i++)
      for (j = 0; j < 32; j++) {
        /* objmatch = (..) ? x : y; */
        /* matrxmatch = (..) ? x : y; */
      }
  /* ;; derive a second object (matrix axis) from the matrix contents plus the other object ?? */
}
