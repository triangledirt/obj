#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "abit.h"
#include "alib.h"

#define DIABETES 0
#define MUSHROOM 1
#define BUFF 1024
#define MAX_FIELDS 32

static long fields;
static char *firstline[MAX_FIELDS];
static long correct = 0;
static long total = 0;

static void freefirst();
static void initfirst();
static void savefirst(char line[MAX_FIELDS]);
static void testfile(char *filename, long type);
static void testline(char *line, long type);

void freefirst()
{
  long idx;
  for (idx = 0; idx < fields; idx++) {
    if (firstline[idx]) {
      free(firstline[idx]);
    }
  }
}

void initfirst()
{
  long idx;
  for (idx = 0; idx < fields; idx++) {
    firstline[idx] = 0;
  }
}

void savefirst(char line[BUFF])
{
  char *tok;
  long idx = 0;
  tok = strtok(line, ",");
  firstline[idx] = strdup(tok);
  while ((tok = strtok(NULL, ",\n"))) {
    idx++;
    firstline[idx] = strdup(tok);
  }
  fields = idx + 1;
}

void testfile(char *filename, long type)
{
  FILE *file;
  char line[BUFF];
  char first = 1;
  initfirst();
  file = fopen(filename, "r");
  while (fgets(line, BUFF, file)) {
    if (first) {
      savefirst(line);
      first = 0;
    } else {
      testline(line, type);
    }
  }
  fclose(file);
  freefirst();
}

void testline(char *line, long type)
{
  aobj_t obj;
  long idx = 0;
  abit_t val;
  char *tok;
#if ALIB_VERBOSE
  abit_t class;
#endif
  aobj_clear(&obj);
  tok = strtok(line, ",");
  if (0 == strcmp(tok, firstline[idx])) {
    val = 1;
  } else {
    val = 0;
  }
  aobj_setclass(&obj, val);
  while ((tok = strtok(NULL, ",\n"))) {
    idx++;
    if (0 == strcmp(tok, firstline[idx])) {
      val = 1;
    } else {
      val = 0;
    }
    aobj_setattr(&obj, idx, val);
  }
  alib_observe(obj, type);
#if ALIB_VERBOSE
  class = alib_classify(obj, type);
  total++;
  printf("type%ld obsrv ", type);
  printf("(%d) ", class);
  aobj_print(obj);
  printf("\n");
#endif
}

int main(int argc, char **argv)
{
  if (0) { testfile("data/diabetes.data", DIABETES); }
  if (1) { testfile("data/mushroom.data", MUSHROOM); }
}
