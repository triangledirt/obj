#include <stdio.h>
#include "case.h"

#define DIABETES 0
#define DRUG 1
#define FOREST 2
#define MUSHROOM 3

typedef case_obj_t (*pack_f)(char *, long, long);

static void summarize(long type);
static void testpack(char *filename, long classindx, long type, pack_f packfunc);
static void testcsvobj(char csvobj[CASE_CSVOBJ], long classindx, long type, pack_f packfunc);
static void testrand(long type);

void testcsvobj(char csvobj[CASE_CSVOBJ], long classindx, long type, pack_f packfunc)
{
  case_obj_t obj;
  char c;
  case_bit_t guessclass;
  case_bit_t actualclass;
  obj = packfunc(csvobj, classindx, type);
  actualclass = case_obj_getclass(obj);
  case_observe(obj, type);
  guessclass = case_classifyknown(obj, actualclass, type);
#if CASE_VERBOSE
  c = (guessclass == actualclass) ? '+' : '-';
  printf("type%ld obsrv ", type);
  printf("(%c) ", c);
  case_obj_print(obj);
  printf("\n");
#endif
}

void testpack(char *filename, long classindx, long type, pack_f packfunc)
{
  FILE *file;
  char csvobj[CASE_CSVOBJ];
  file = fopen(filename, "r");
  while (fgets(csvobj, CASE_CSVOBJ, file))
    testcsvobj(csvobj, classindx, type, packfunc);
  fclose(file);
#if CASE_VERBOSE
  case_printstat(type);
#endif
}

int main(int argc, char *argv[])
{
  if (0) testpack("data/diabetes.csv", 0, DIABETES, case_packavg);
  if (0) testpack("data/drug.csv", 3, DRUG, case_packavg);
  if (0) testpack("data/forest.csv", 0, FOREST, case_packavg);
  if (1) testpack("data/mushroom.csv", 0, MUSHROOM, case_packavg);
}
