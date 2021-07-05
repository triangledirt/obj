#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "abit.h"
#include "alib.h"

#define MUSHROOM 0

char firstline[1024];
long correct = 0;
long total = 0;

void testfile(char *filename, long type);
void testline(char *line, long type);

void testfile(char *filename, long type)
{
  FILE *file;
  char line[1024];
  char first = 1;
  file = fopen(filename, "r");
  while (fgets(line, 1024, file)) {
    if (first) {
      strcpy(firstline, line);
      first = 0;
    } else {
      testline(line, type);
    }
  }
  fclose(file);
#if ALIB_VERBOSE
  printf("                                                "
    "=%0.3f%%!\n", (double) correct / total);
#endif
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
  if ('e' == *tok) {
    val = 1;
  } else {
    val = 0;
  }
  aobj_setclass(&obj, val);
  while ((tok = strtok(NULL, ","))) {
    idx++;
    if (*tok == firstline[idx * 2]) {
      val = 1;
    } else {
      val = 0;
    }
    aobj_setattr(&obj, idx, val);
  }
  alib_observe(obj, type);
#if ALIB_VERBOSE
  class = alib_classify(obj, type);
  if (class == aobj_getclass(obj)) {
    correct++;
  }
  total++;
  printf("type%ld obsrv (", type);
  printf("%d) ", class);
  aobj_print(obj);
  printf("\n");
#endif
}

int main(int argc, char **argv)
{
  testfile("mushroom.data", MUSHROOM);
/*
  aobj_t obj;
  long num;
  aobj_clear(&obj);
  aobj_print(obj); printf("\n");
  aobj_setnum(&obj, 0, 5, 16);
  aobj_print(obj); printf("\n");
  num = aobj_getnum(obj, 0, 5);
  printf("=%lu\n", num);
  return 0;
*/
}
