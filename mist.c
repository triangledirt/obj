#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mesh.h"

#define MIST 24

struct obj_mesh_t mist[MIST];
long focus[MIST];

static void print();

void print()
{
  long i;
  for (i = 0; i < MIST; i++)
    obj_mesh_print(&mist[i]);
}

void obj_mist_play()
{
  long i;
  struct obj_mesh_t *gamemesh;
  struct obj_mesh_t *datamesh;
  srandom(time(NULL));
  for (i = 0; i < MIST; i++) {
    obj_mesh_init(&mist[i]);
    focus[i] = random() % MIST;
  }
  while (1) {
    for (i = 0; i < MIST; i++) {
      gamemesh = &mist[i];
      datamesh = &mist[focus[i]];
      obj_mesh_play(gamemesh, datamesh);
      focus[i] = random() % MIST;
    }
    printf("\033c");
    print();
    usleep(1000000 / 24);
  }
}
