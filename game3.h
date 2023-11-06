#ifndef obj_game3_h
#define obj_game3_h

#include "bit.h"

typedef char obj_game3_t;

obj_bit_t obj_game3_play(obj_game3_t game3, obj_bit_t bit1, obj_bit_t bit2, obj_bit_t bit3);

void obj_game3_randomize(obj_game3_t *game3);

#endif
