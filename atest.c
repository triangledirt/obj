#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../alib/abit.h"
#include "../alib/alib.h"

#define MUSHROOM 0

char firstline[1024];

void testfile(char *filename, long type);
void testline(char *line, long type);

void testfile(char *filename, long type) {
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
}

void testline(char *line, long type) {
  aobj_t obj;
  long idx = 0;
  abit_t val;
  char *tok;
#if ALIB_VERBOSE
  abit_t class;
#endif
  aobj_init(&obj);
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
  printf("type%ld obsrv (", type);
  printf("%d) ", class);
  aobj_print(obj);
  printf("\n");
#endif
}

int main(int argc, char **argv) {
  testfile("mushroom.d", MUSHROOM);
  return 0;
}
