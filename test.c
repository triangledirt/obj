#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "case.h"

#define DIABETES 0
#define MUSHROOM 1

#define MAXFIELDS 32

static long fields;
static char *firstline[MAXFIELDS];
static long total = 0;
static long correct = 0;

static void testfile(char *filename, long classidx, long type);
static void testline(char *line, long classidx, long type);

void testfile(char *filename, long classidx, long type)
{
  FILE *file;
  char line[CASE_CSVOBJSZ];
  file = fopen(filename, "r");
  while (fgets(line, CASE_CSVOBJSZ, file))
    testline(line, classidx, type);
  fclose(file);
#if CASE_VERBOSE
  printf("                                                =%0.3f%%\n", (double) correct / total);
#endif
}

void testline(char *line, long classidx, long type)
{
  case_obj_t obj;
  char c;
  case_bit_t actualclass;
  case_bit_t guessclass;
  obj = case_packfirst(line, classidx, type);
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
  if (0) testfile("data/diabetes.data", 0, DIABETES);
  if (1) testfile("data/mushroom.data", 0, MUSHROOM);
}
