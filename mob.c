#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "index.h"
#include "mesh.h"

#define MOB 16

struct obj_mesh_t mob[MOB][MOB];
char focus[MOB][MOB];

static struct obj_mesh_t *finddatamesh(long i, long j);
static void *i2xy(long i, long *x, long *y);
static void print();

struct obj_mesh_t *finddatamesh(long i, long j)
{
  long x;
  long y;
  long c;
  long d;
  i2xy(focus[i][j], &x, &y);
  c = obj_index_wrap(i + x, MOB);
  d = obj_index_wrap(j + y, MOB);
  return &mob[c][d];
}

void *i2xy(long i, long *x, long *y)
{
  switch (i) {
  case 0:
    *x = -1;
    *y = -1;
    break;
  case 1:
    *x = -1;
    *y = 0;
    break;
  case 2:
    *x = 1;
    *y = -1;
    break;
  case 3:
    *x = -1;
    *y = 0;
    break;
  case 4:
    *x = 0;
    *y = 0;
    break;
  case 5:
    *x = 1;
    *y = 0;
    break;
  case 6:
    *x = 1;
    *y = -1;
    break;
  case 7:
    *x = 1;
    *y = 0;
    break;
  case 8:
    *x = 1;
    *y = 1;
    break;
  }
}

void print()
{
  long i;
  long j;
  struct obj_mesh_t *mesh;
  for (i = 0; i < MOB; i++) {
    for (j = 0; j < MOB; j++) {
      mesh = &mob[i][j];
      printf("%c ", obj_mesh_face(mesh));
    }
    printf("\n");
  }
}

void obj_mob_play()
{
  long i;
  long j;
  long tick;
  struct obj_mesh_t *gamemesh;
  struct obj_mesh_t *datamesh;
  srandom(time(NULL));
  for (i = 0; i < MOB; i++) {
    for (j = 0; j < MOB; j++) {
      obj_mesh_init(&mob[i][j]);
      focus[i][j] = random() % 9;
    }
  }
  while (1) {
    for (tick = 0; tick < MOB; tick++) {
      i = random() % MOB;
      j = random() % MOB;
      gamemesh = &mob[i][j];
      datamesh = finddatamesh(i, j);
      obj_mesh_play(gamemesh, datamesh);
      focus[i][j] = random() % 9;
    }
    printf("\033c");
    print();
    usleep(1000000 / 24);
  }
}
