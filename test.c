#include <stdio.h>
#include "case.h"

#define CENSUS 0
#define CONNECT4 1
#define LETTER 2
#define MUSHROOM 3
#define MUSHROOM2 4
#define NAMEGENDER 5
#define NURSERY 6
#define OCCUPANCY 7
#define POKER 8
#define SEPSIS 9
#define SHUTTLE 10

static void summarize(long type);
static void testpack(char *filename, long classindx, long type, case_pack_f packfunc);
static void testcsvobj(char csvobj[CASE_CSVOBJ], long classindx, long type, case_pack_f packfunc);
static void testrand(long type);

void testcsvobj(char csvobj[CASE_CSVOBJ], long classindx, long type, case_pack_f packfunc)
{
  case_obj_t obj;
  char c;
  case_bit_t guessclass;
  case_bit_t actualclass;
  obj = packfunc(csvobj, classindx, type);
  case_observe(obj, type);
  guessclass = case_classifyknown(obj, type);
  actualclass = case_obj_getclass(obj);
#if CASE_VERBOSE
  c = (guessclass == actualclass) ? '+' : '-';
  printf("type%02ld obsrv ", type);
  printf("(%c) ", c);
  case_obj_print(obj);
  printf("\n");
#endif
}

void testpack(char *filename, long classindx, long type, case_pack_f packfunc)
{
  FILE *file;
  /*  case_stat_t *stat;  */
  char csvobj[CASE_CSVOBJ];
  file = fopen(filename, "r");
  while (fgets(csvobj, CASE_CSVOBJ, file))
    testcsvobj(csvobj, classindx, type, packfunc);
  fclose(file);
#if CASE_VERBOSE
  case_printstat(type);
  /*  stat = case_stat(type);  */
  /*  case_stat_print(stat);  */
#endif
}

int main(int argc, char *argv[])
{
  if (0) testpack("data/census.csv", 14, CENSUS, case_packavg);
  if (0) testpack("data/connect4.csv", 42, CONNECT4, case_packavg);
  if (0) testpack("data/letter.csv", 0, LETTER, case_packavg);
  if (1) testpack("data/mushroom.csv", 0, MUSHROOM, case_packavg);
  if (0) testpack("data/mushroom2.csv", 0, MUSHROOM2, case_packavg);
  if (0) testpack("data/namegender.csv", 1, NAMEGENDER, case_packavg);
  if (0) testpack("data/nursery.csv", 0, NURSERY, case_packavg);
  if (0) testpack("data/occupancy.csv", 18, OCCUPANCY, case_packavg);
  if (0) testpack("data/poker.csv", 10, POKER, case_packavg);
  if (0) testpack("data/sepsis.csv", 3, SEPSIS, case_packavg);
  if (0) testpack("data/shuttle.csv", 9, SHUTTLE, case_packavg);
}
