#ifndef obj_xbit_h
#define obj_xbit_h

#include "bit.h"

#define OBJ_XBIT_CNT 8

struct obj_xbit_t {
  obj_bit_t bit[OBJ_XBIT_CNT];
  long notecnt;
  double zeropart;
};
typedef struct obj_xbit_t obj_xbit_t;

void obj_xbit_init(obj_xbit_t *xbit);
void obj_xbit_note(obj_xbit_t *xbit, obj_bit_t bit);
double obj_xbit_zeropart(obj_xbit_t *xbit);

#endif
