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
    }
    testline(line, type);
  }
  fclose(file);
}

void testline(char *line, long type) {
  aobj_t obj;
  long idx = 0;
  abit_t val;
  char *tok;
  abit_t class;
  aobj_init(&obj);
  tok = strtok(line, ",");
  if ('e' == *tok) {
    val = 1;
  } else {
    val = 0;
  }
  aobj_setattr(&obj, 0, val);
  while ((tok = strtok(NULL, ","))) {
    idx++;
    if (*tok == firstline[idx * 2]) {
      val = 1;
    } else {
      val = 0;
    }
    /* printf("%ld %d\n", idx, val); */
    aobj_setattr(&obj, idx, val);
  }
  alib_observe(obj, type);
  aobj_print(obj);
  class = alib_classify(obj, type);
  printf("class::%d\n", class);
}

int main(int argc, char **argv) {
  testfile("mushroom.data", MUSHROOM);
  return 0;
}
