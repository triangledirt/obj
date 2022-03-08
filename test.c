#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "case.h"

#define DIABETES 0
#define DRUG 1
#define FOREST 2
#define MUSHROOM 3

static long total = 0;
static long correct = 0;

static void testfile(char *filename, long classindx, long type);
static void testline(char *line, long classindx, long type);

void testfile(char *filename, long classindx, long type)
{
  FILE *file;
  char line[CASE_CSVOBJSZ];
  file = fopen(filename, "r");
  while (fgets(line, CASE_CSVOBJSZ, file))
    testline(line, classindx, type);
  fclose(file);
#if CASE_VERBOSE
  printf("                                                =%0.3f%%\n", (double) correct / total);
#endif
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

int main(int argc, char **argv)
{
  if (0) testfile("data/diabetes.csv", 0, DIABETES);
  if (0) testfile("data/drug.csv", 0, DRUG);
  if (0) testfile("data/forest.csv", 0, FOREST);
  if (1) testfile("data/mushroom.csv", 0, MUSHROOM);
}
