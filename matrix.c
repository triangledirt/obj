#include <stdio.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "matrix.h"
#include "obj.h"

static case_bit_t once = 0;
static double fitness[32];
static case_bit_t matrix[32][32];

static void initonce();
static double score(case_obj_t obj, long type);

void initonce()
{
  long i;
  long j;
  if (!once) {
    for (i = 0; i < 32; i++)
      for (j = 0; j < 32; j++)
        case_bit_randomize(&matrix[i][j]);
    once = 1;
  }
}

case_bit_t matrix_classify(case_obj_t obj, long type)
{
  /* return score(obj, type) > (0.9 * fitness[type]); */
}

void matrix_learn(case_obj_t obj[], long objsz, long type)
{
  initonce();
#if CASE_VERBOSE
  printf("type%ld matrx ft1 ", type);
  /* case_obj_print(one[type]); */
  printf(" %0.3f%%\n", fitness[type]);
#endif
}

double score(case_obj_t obj, long type)
{
  long i;
  long j;
  case_bool_t objmatch;
  case_bool_t matrixmatch;
  double s = 0.0;
  if (!once) {
    for (i = 0; i < 32; i++)
      for (j = 0; j < 32; j++) {
        objmatch = (..) ? x : y;
        matrixmatch = (..) ? x : y;
      }
}
