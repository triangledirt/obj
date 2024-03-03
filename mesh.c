#include <stdio.h>
#include <stdlib.h>
#include "game2.h"
#include "mesh.h"

#define multiply(bit0, bit1, bit2, bit3) ((bit0) + (2 * bit1) + (4 * bit2) + (8 * bit3))

char obj_mesh_face(struct obj_mesh_t *mesh)
{
  long place;
  long value = 1;
  long face = 97;
  for (place = 0; place < 4; place++) {
    face += mesh->bit[place] * value;
    value *= 2;
  }
  return face;
}

void obj_mesh_init(struct obj_mesh_t *mesh)
{
  long i;
  for (i = 0; i < 16; i++)
    mesh->bit[i] = obj_bit_random();
}

void obj_mesh_initlong(struct obj_meshlong_t *meshlong, struct obj_mesh_t *mesh)
{
  meshlong->game = multiply(mesh->bit[0], mesh->bit[1], mesh->bit[2], mesh->bit[3]);
  meshlong->inaddr1 = multiply(mesh->bit[4], mesh->bit[5], mesh->bit[6], mesh->bit[7]);
  meshlong->inaddr2 = multiply(mesh->bit[8], mesh->bit[9], mesh->bit[10], mesh->bit[11]);
  meshlong->outaddr = multiply(mesh->bit[12], mesh->bit[13], mesh->bit[14], mesh->bit[15]);
}

void obj_mesh_mutate(struct obj_mesh_t *mesh)
{
  obj_bit_randomize(&mesh->bit[random() % 16]);
}

void obj_mesh_play(struct obj_mesh_t *gamemesh, struct obj_mesh_t *datamesh)
{
  struct obj_meshlong_t gamelong;
  obj_bit_t in1;
  obj_bit_t in2;
  obj_bit_t out;
  obj_mesh_initlong(&gamelong, gamemesh);
  in1 = datamesh->bit[gamelong.inaddr1];
  in2 = datamesh->bit[gamelong.inaddr2];
  out = obj_game2_play(gamelong.game, in1, in2);
  datamesh->bit[gamelong.outaddr] = out;
}

void obj_mesh_print(struct obj_mesh_t *mesh)
{
  long i;
  for (i = 0; i < 16; i++)
    printf("%c ", obj_bit_char(mesh->bit[i]));
  printf("\n");
}

char obj_meshlong_face(struct obj_meshlong_t *meshlong)
{
  char c = 97;
  c += 0;
  ;;
  return c;
}

void obj_meshlong_print(struct obj_meshlong_t *meshlong)
{
  printf("game %d\n", meshlong->game);
  printf("inaddr1 %d\n", meshlong->inaddr1);
  printf("inaddr2 %d\n", meshlong->inaddr2);
  printf("outaddr %d\n", meshlong->outaddr);
}
