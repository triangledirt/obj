#include <stdlib.h>
#include "game2.h"

static obj_bit_t value[16][2][2] = {
  { { 0, 0 }, { 0, 0 } },
  { { 0, 0 }, { 0, 1 } },
  { { 0, 0 }, { 1, 0 } },
  { { 0, 0 }, { 1, 1 } },
  { { 0, 1 }, { 0, 0 } },
  { { 0, 1 }, { 0, 1 } },
  { { 0, 1 }, { 1, 0 } },
  { { 0, 1 }, { 1, 1 } },
  { { 1, 0 }, { 0, 0 } },
  { { 1, 0 }, { 0, 1 } },
  { { 1, 0 }, { 1, 0 } },
  { { 1, 0 }, { 1, 1 } },
  { { 1, 1 }, { 0, 0 } },
  { { 1, 1 }, { 0, 1 } },
  { { 1, 1 }, { 1, 0 } },
  { { 1, 1 }, { 1, 1 } }
};

obj_bit_t obj_game2_play(obj_game2_t game2, obj_bit_t bit1, obj_bit_t bit2)
{
  return value[game2][bit1][bit2];
}

void obj_game2_randomize(obj_game2_t *game2)
{
  *game2 = random() % 16;
}
