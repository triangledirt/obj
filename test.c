#include <stdio.h>
#include "class.h"

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
static void testpack(char *filename, long classindx, long type, obj_class_pack_f packfunc);
static void testcsvobj(char csvobj[OBJ_CLASS_CSV], long classindx, long type, obj_class_pack_f packfunc);
static void testrand(long type);

void testcsvobj(char csvobj[OBJ_CLASS_CSV], long classindx, long type, obj_class_pack_f packfunc)
{
  obj_t obj;
  char c;
  obj_bit_t guessclass;
  obj_bit_t actualclass;
  obj = packfunc(csvobj, classindx, type);
  obj_class_observe(obj, type);
  guessclass = obj_class_classifyknown(obj, type);
  actualclass = obj_getclass(obj);
#if OBJ_VERBOSE
  c = (guessclass == actualclass) ? '+' : '-';
  printf("type%02ld obsrv ", type);
  printf("(%c) ", c);
  obj_print(obj);
  printf("\n");
#endif
}

void testpack(char *filename, long classindx, long type, obj_class_pack_f packfunc)
{
  FILE *file;
  struct obj_classstat_t *stat;
  char csvobj[OBJ_CLASS_CSV];
  file = fopen(filename, "r");
  while (fgets(csvobj, OBJ_CLASS_CSV, file))
    testcsvobj(csvobj, classindx, type, packfunc);
  fclose(file);
#if OBJ_VERBOSE
  stat = obj_class_stat(type);
  obj_classstat_print(stat, type);
#endif
}

int main(int argc, char *argv[])
{
  if (0) {
    if (1) testpack("data/census.csv", 14, CENSUS, obj_class_packavg);
    if (1) testpack("data/connect4.csv", 42, CONNECT4, obj_class_packavg);
    if (1) testpack("data/letter.csv", 0, LETTER, obj_class_packavg);
    if (1) testpack("data/mushroom.csv", 0, MUSHROOM, obj_class_packavg);
    if (1) testpack("data/mushroom2.csv", 0, MUSHROOM2, obj_class_packavg);
    if (1) testpack("data/namegender.csv", 1, NAMEGENDER, obj_class_packavg);
    if (1) testpack("data/nursery.csv", 0, NURSERY, obj_class_packavg);
    if (1) testpack("data/occupancy.csv", 18, OCCUPANCY, obj_class_packavg);
    if (1) testpack("data/poker.csv", 10, POKER, obj_class_packavg);
    if (1) testpack("data/sepsis.csv", 3, SEPSIS, obj_class_packavg);
    if (1) testpack("data/shuttle.csv", 9, SHUTTLE, obj_class_packavg);
  } else {
    if (0) testpack("data/occupancy.csv", 18, OCCUPANCY, obj_class_packavg);
    if (1) testpack("data/mushroom.csv", 0, MUSHROOM, obj_class_packavg);
    if (0) testpack("data/mushroom2.csv", 0, MUSHROOM2, obj_class_packavg);
  }
}
