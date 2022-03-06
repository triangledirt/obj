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

static void freefirst();
static void initfirst();
static void savefirst(char line[MAXFIELDS]);
static void testfile(char *filename, long classidx, long type);
static void testline(char *line, long type);
static void testline2(char *line, long classidx, long type);

void freefirst()
{
  long i;
  for (i = 0; i < fields; i++)
    if (firstline[i])
      free(firstline[i]);
}

void initfirst()
{
  long i;
  for (i = 0; i < fields; i++)
    firstline[i] = 0;
}

void savefirst(char line[CASE_CSVOBJSZ])
{
  char *tok;
  long i = 0;
  tok = strtok(line, ",");
  firstline[i] = strdup(tok);
  while (tok = strtok(NULL, ",\n")) {
    i++;
    firstline[i] = strdup(tok);
  }
  fields = i + 1;
}

void testfile(char *filename, long classidx, long type)
{
  FILE *file;
  char line[CASE_CSVOBJSZ];
  char first = 1;
  initfirst();
  file = fopen(filename, "r");
  while (fgets(line, CASE_CSVOBJSZ, file)) {
    if (first) {
      savefirst(line);
      first = 0;
    } else {
      /*  testline(line, type);  */
      testline2(line, classidx, type);
    }
  }
  fclose(file);
  freefirst();
#if CASE_VERBOSE
  printf("                                                =%0.3f%%\n", (double) correct / total);
#endif
}

void testline(char *line, long type)
{
  case_obj_t obj;
  long i = 0;
  case_bit_t val;
  char *tok;
  char c;
  case_bit_t class;
  case_bit_t objclass;
  case_obj_clear(&obj);
  tok = strtok(line, ",");
  val = (0 == strcmp(tok, firstline[i])) ? 1 : 0;
  case_obj_setclass(&obj, val);
  while (tok = strtok(NULL, ",\n")) {
    i++;
    val = (0 == strcmp(tok, firstline[i])) ? 1 : 0;
    case_obj_setattr(&obj, i, val);
  }
  case_observe(obj, type);
  class = case_classify(obj, type);
  objclass = case_obj_getclass(obj);
  total++;
  correct += (class == objclass) ? 1 : 0;
#if CASE_VERBOSE
  c = (class == objclass) ? '+' : '-';
  printf("type%ld obsrv ", type);
  printf("(%c) ", c);
  case_obj_print(obj);
  printf("\n");
#endif
}

void testline2(char *line, long classidx, long type)
{
  case_obj_t obj;
  char c;
  case_bit_t class;
  case_bit_t objclass;
  obj = case_packfirst(line, classidx, type);
  case_observe(obj, type);
  class = case_classify(obj, type);
  objclass = case_obj_getclass(obj);
  total++;
  correct += (class == objclass) ? 1 : 0;
#if CASE_VERBOSE
  c = (class == objclass) ? '+' : '-';
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
