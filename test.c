#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bit.h"
#include "case.h"

#define DIABETES 0
#define MUSHROOM 1
#define BUFF 1024
#define MAX_FIELDS 32

static long fields;
static char *firstline[MAX_FIELDS];
static long total = 0;
static long correct = 0;

static void freefirst();
static void initfirst();
static void savefirst(char line[MAX_FIELDS]);
static void testfile(char *filename, long type);
static void testline(char *line, long type);

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

void savefirst(char line[BUFF])
{
  char *tok;
  long i = 0;
  tok = strtok(line, ",");
  firstline[i] = strdup(tok);
  while ((tok = strtok(NULL, ",\n"))) {
    i++;
    firstline[i] = strdup(tok);
  }
  fields = i + 1;
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
#if CASE_VERBOSE
  printf("                                                =%0.3f\%\n", (double) correct / total);
#endif
}

void testline(char *line, long type)
{
  case_obj_t obj;
  long i = 0;
  case_bit_t val;
  char *tok;
  case_bit_t class;
  case_obj_clear(&obj);
  tok = strtok(line, ",");
  val = (0 == strcmp(tok, firstline[i])) ? 1 : 0;
  case_obj_setclass(&obj, val);
  while ((tok = strtok(NULL, ",\n"))) {
    i++;
    val = (0 == strcmp(tok, firstline[i])) ? 1 : 0;
    case_obj_setattr(&obj, i, val);
  }
  case_observe(obj, type);
  class = case_classify(obj, type);
  total++;
  correct += (class == case_obj_getclass(obj)) ? 1 : 0;
#if CASE_VERBOSE
  printf("type%ld obsrv ", type);
  printf("(%d) ", class);
  case_obj_print(obj);
  printf("\n");
#endif
}

int main(int argc, char **argv)
{
  if (0) testfile("data/diabetes.data", DIABETES);
  if (1) testfile("data/mushroom.data", MUSHROOM);
}
