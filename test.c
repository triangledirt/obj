#include <stdio.h>
#include "class.h"
#include "fit.h"
#include "model.h"
#include "sense.h"
#include "sync.h"

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

double fit(obj_t obj, long type, void *context);
static void testclass();
void testcsvobj(char csvobj[OBJ_CSV], long classindx, long type, obj_class_pack_f packfunc);
static void testmodel(char *filename, long classindx, long type, obj_class_pack_f packfunc);
static void testobj();
static void testpack(char *filename, long classindx, long type, obj_class_pack_f packfunc);
static void testsense(char *filename, long classindx, long type, obj_class_pack_f packfunc);
static void testsync();

double fit(obj_t obj, long type, void *context)
{
  obj_t objzero;
  static char str[OBJ] = "0000000000000000000000000000000000000000000000000000000000000000";
  obj_setfromstr(&objzero, str);
  printf("%f\n", obj_compareoblivion(obj, objzero));
  return obj_compareoblivion(obj, objzero);
}

void testclass()
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

void testcsvobj(char csvobj[OBJ_CSV], long classindx, long type, obj_class_pack_f packfunc)
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

void testmodel(char *filename, long classindx, long type, obj_class_pack_f packfunc)
{
  FILE *file;
  char csvobj[OBJ_CSV];
  obj_t obj;
  struct obj_fit_t fittest;
  long e;
  struct obj_modelstat_t *stat;
  obj_model_setfitfunc(fit, NULL, type);
  file = fopen(filename, "r");
  while (fgets(csvobj, OBJ_CSV, file)) {
    obj = packfunc(csvobj, classindx, type);
    obj_model_insert(obj, type);
  }
  fclose(file);
  for (e = 0; e < 64; e++)
    obj_model_evolve(type);
#if OBJ_VERBOSE
  stat = obj_model_stat(type);
  obj_modelstat_print(stat, type);
  fittest = obj_model_fittest(type);
  printf("fittest ");
  obj_print(fittest.obj);
  printf("\n");
  printf("fittest fit %f\n", fittest.fit);
#endif
}

void testobj()
{
  obj_t obj1;
  obj_t obj2;
  obj_t obj3;
  obj_t obj4;
  char str1[OBJ] = "0000000000000000000000000000000000000000000000000000000000000000";
  char str2[OBJ] = "1111111111111111111111111111111111111111111111111111111111111111";
  char str3[OBJ] = "0000000000000000000000000000000011111111111111111111111111111111";
  char str4[OBJ] = "0101010101010101010101010101010111111111111111111111111111111111";
  double oblivion;
  long num;
  obj_setfromstr(&obj1, str1);
  obj_setfromstr(&obj2, str2);
  obj_setfromstr(&obj3, str3);
  obj_setfromstr(&obj4, str4);
  oblivion = obj_compareoblivion(obj1, obj2);
  printf("oblivion = %f\n", oblivion);
  num = obj_getnum(obj4, 0, 4);
  printf("num = %ld\n", num);
}

void testpack(char *filename, long classindx, long type, obj_class_pack_f packfunc)
{
  FILE *file;
  struct obj_classstat_t *stat;
  char csvobj[OBJ_CSV];
  file = fopen(filename, "r");
  while (fgets(csvobj, OBJ_CSV, file))
    testcsvobj(csvobj, classindx, type, packfunc);
  fclose(file);
#if OBJ_VERBOSE
  stat = obj_class_stat(type);
  obj_classstat_print(stat, type);
#endif
}

void testsense(char *filename, long classindx, long type, obj_class_pack_f packfunc)
{
  FILE *file;
  char csvobj[OBJ_CSV];
  obj_t obj;
  obj_t alive;
  file = fopen(filename, "r");
  while (fgets(csvobj, OBJ_CSV, file)) {
    obj = packfunc(csvobj, classindx, type);
    obj_sense_observe(obj, type);
    alive = obj_sense_alive(type);
    printf("obj   ");
    obj_print(obj);
    printf("\n");
    printf("alive ");
    obj_print(alive);
    printf("\n");
  }
  fclose(file);
}

void testsync()
{
}

int main(int argc, char *argv[])
{
  if (0) testclass();
  if (1) testmodel("data/mushroom.csv", 0, MUSHROOM, obj_class_packavg);
  if (0) testobj();
  if (0) testsense("data/mushroom.csv", 0, MUSHROOM, obj_class_packavg);
  if (0) testsync();
  return 0;
}
