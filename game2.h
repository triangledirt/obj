#ifndef obj_game2_h
#define obj_game2_h

#include "bit.h"

typedef char obj_game2_t;

obj_bit_t obj_game2_play(obj_game2_t game2, obj_bit_t bit1, obj_bit_t bit2);

void obj_game2_randomize(obj_game2_t *game2);

#endif
