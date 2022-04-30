#ifndef xbit_h
#define xbit_h

#include "bit.h"

#define XBIT_CNT 16

struct xbit_t {
  case_bit_t bit[XBIT_CNT];
  long notecnt;
  double zeropart;
};
typedef struct xbit_t xbit_t;

void xbit_init(xbit_t *xbit);
void xbit_note(xbit_t *xbit, case_bit_t bit);
double xbit_zeropart(xbit_t *xbit);

#endif
