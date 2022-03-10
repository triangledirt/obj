#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "case.h"

#define DIABETES 0
#define DRUG 1
#define FOREST 2
#define MUSHROOM 3
#define RANDOM 4

static long total;
static long correct;

static void reset();
static void summarize();
static void testfile(char *filename, long classindx, long type);
static void testline(char *line, long classindx, long type);
static void testrand(long type);

void reset()
{
  total = 0;
  correct = 0;
}

void summarize()
{
#if CASE_VERBOSE
  printf("                                                =%0.3f%%\n", (double) correct / total);
#endif
}

void testfile(char *filename, long classindx, long type)
{
  FILE *file;
  char line[CASE_CSVOBJ];
  reset();
  file = fopen(filename, "r");
  while (fgets(line, CASE_CSVOBJ, file))
    testline(line, classindx, type);
  fclose(file);
  summarize();
}

void testline(char *line, long classindx, long type)
{
  case_obj_t obj;
  char c;
  case_bit_t guessclass;
  case_bit_t actualclass;
  obj = case_packfirst(line, classindx, type);
  case_observe(obj, type);
  guessclass = case_classify(obj, type);
  actualclass = case_obj_getclass(obj);
  total++;
  correct += (guessclass == actualclass) ? 1 : 0;
#if CASE_VERBOSE
  c = (guessclass == actualclass) ? '+' : '-';
  printf("type%ld obsrv ", type);
  printf("(%c) ", c);
  case_obj_print(obj);
  printf("\n");
#endif
}

void testrand(long type)
{
  case_obj_t obj;
  char c;
  case_bit_t guessclass;
  case_bit_t actualclass;
  long i;
  long j;
  case_bit_t bit;
  reset();
  for (j = 0; j < 16; j++) {
    for (i = 0; i < 32; i++) {
      case_bit_randomize(&bit);
      case_obj_setattr(&obj, i, bit);
    }
    case_observe(obj, type);
    guessclass = case_classify(obj, type);
    actualclass = case_obj_getclass(obj);
    total++;
    correct += (guessclass == actualclass) ? 1 : 0;
#if CASE_VERBOSE
    c = (guessclass == actualclass) ? '+' : '-';
    printf("type%ld obsrv ", type);
    printf("(%c) ", c);
    case_obj_print(obj);
    printf("\n");
#endif
  }
  summarize();
}

int main(int argc, char **argv)
{
  if (0) testrand(RANDOM);
  if (0) testfile("data/diabetes.csv", 0, DIABETES);
  if (0) testfile("data/drug.csv", 0, DRUG);
  if (0) testfile("data/forest.csv", 0, FOREST);
  if (1) testfile("data/mushroom.csv", 0, MUSHROOM);
}
