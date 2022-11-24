#include <stdio.h>
#include <stdlib.h>
#include "xbit.h"

static void calc(struct obj_xbit_t *xbit);

void calc(struct obj_xbit_t *xbit)
{
  long onecnt = 0;
  long i;
  for (i = 0; i < OBJ_XBIT_CNT; i++)
    onecnt += xbit->bit[i];
  xbit->zeropart = (double) (OBJ_XBIT_CNT - onecnt) / OBJ_XBIT_CNT;
}

void obj_xbit_init(struct obj_xbit_t *xbit)
{
  long i;
  for (i = 0; i < OBJ_XBIT_CNT; i++)
    xbit->bit[i] = 0;
  xbit->notecnt = 0;
  xbit->zeropart = 0.0;
}

void obj_xbit_note(struct obj_xbit_t *xbit, obj_bit_t bit)
{
  xbit->bit[random() % OBJ_XBIT_CNT] = bit;
  xbit->notecnt++;
}

double obj_xbit_zeropart(struct obj_xbit_t *xbit)
{
  if (xbit->notecnt >= (OBJ_XBIT_CNT / 4)) {
    calc(xbit);
    xbit->notecnt = 0;
  }
  return xbit->zeropart;
}
