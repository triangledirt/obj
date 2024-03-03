#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mesh.h"

#define RING 8

struct obj_mesh_t ring[RING];
long datap[RING];

static void meshprint();

void meshprint()
{
  long i;
  for (i = 0; i < RING; i++)
    obj_mesh_print(&ring[i]);
}

void obj_ring_play()
{
  long i;
  struct obj_mesh_t *gamemesh;
  struct obj_mesh_t *datamesh;
  srandom(time(NULL));
  for (i = 0; i < RING; i++) {
    obj_mesh_init(&ring[i]);
    datap[i] = random() % RING;
  }
  meshprint();
  while (1) {
    obj_mesh_mutate(&ring[random() % RING]);
    for (i = 0; i < RING; i++) {
      gamemesh = &ring[i];
      datamesh = &ring[datap[i]];
      obj_mesh_play(gamemesh, datamesh);
    }
    printf("\033c");
    meshprint();
    usleep(1000000 / 24);
  }
}
