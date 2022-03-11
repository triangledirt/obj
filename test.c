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
static void testfile(char *filename, long classindx, long type, pack_f packfunc);
static void testline(char *line, long classindx, long type, pack_f packfunc);
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

void testfile(char *filename, long classindx, long type, pack_f packfunc)
{
  FILE *file;
  char line[CASE_CSVOBJ];
  reset();
  file = fopen(filename, "r");
  while (fgets(line, CASE_CSVOBJ, file))
    testline(line, classindx, type, packfunc);
  fclose(file);
  summarize();
}

void testline(char *line, long classindx, long type, pack_f packfunc)
{
  case_obj_t obj;
  char c;
  case_bit_t guessclass;
  case_bit_t actualclass;
  obj = packfunc(line, classindx, type);
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
  if (0) testfile("data/diabetes.csv", 0, DIABETES, case_packfirst);
  if (0) testfile("data/drug.csv", 3, DRUG, case_packfirst);
  if (0) testfile("data/forest.csv", 0, FOREST, case_packavg);
  if (1) testfile("data/mushroom.csv", 0, MUSHROOM, case_packavg);
}
