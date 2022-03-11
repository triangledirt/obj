#include <stdio.h>
#include "case.h"

#define DIABETES 0
#define DRUG 1
#define FOREST 2
#define MUSHROOM 3
#define RANDOM 4

static long total;
static long correct;

typedef case_obj_t (*pack_f)(char *, long, long);

static void reset();
static void summarize();
static void testpack(char *filename, long classindx, long type, pack_f packfunc);
static void testcsvobj(char csvobj[CASE_CSVOBJ], long classindx, long type, pack_f packfunc);
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

void testcsvobj(char csvobj[CASE_CSVOBJ], long classindx, long type, pack_f packfunc)
{
  case_obj_t obj;
  char c;
  case_bit_t guessclass;
  case_bit_t actualclass;
  obj = packfunc(csvobj, classindx, type);
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

void testpack(char *filename, long classindx, long type, pack_f packfunc)
{
  FILE *file;
  char csvobj[CASE_CSVOBJ];
  reset();
  file = fopen(filename, "r");
  while (fgets(csvobj, CASE_CSVOBJ, file))
    testcsvobj(csvobj, classindx, type, packfunc);
  fclose(file);
  summarize();
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

int main(int argc, char *argv[])
{
  if (0) testrand(RANDOM);
  if (0) testpack("data/diabetes.csv", 0, DIABETES, case_packfirst);
  if (0) testpack("data/drug.csv", 3, DRUG, case_packfirst);
  if (0) testpack("data/forest.csv", 0, FOREST, case_packavg);
  if (1) testpack("data/mushroom.csv", 0, MUSHROOM, case_packavg);
}
