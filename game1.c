#include <stdlib.h>
#include "game1.h"

static obj_bit_t value[4][2] = {
  { 0, 0 },
  { 0, 1 },
  { 1, 0 },
  { 1, 1 }
};

obj_bit_t obj_game1_play(obj_game1_t game1, obj_bit_t bit)
{
  return value[game1][bit];
}

void obj_game1_randomize(obj_game1_t *game1)
{
  *game1 = random() % 4;
}
