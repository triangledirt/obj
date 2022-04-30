#include <stdio.h>
#include <stdlib.h>
#include "xbit.h"

static void calc(xbit_t *xbit);

void calc(xbit_t *xbit)
{
  long onecnt = 0;
  long i;
  for (i = 0; i < XBIT_CNT; i++)
    onecnt += xbit->bit[i];
  xbit->zeropart = (double) (XBIT_CNT - onecnt) / XBIT_CNT;
}

void xbit_init(xbit_t *xbit)
{
  long i;
  for (i = 0; i < XBIT_CNT; i++)
    xbit->bit[i] = 0;
  xbit->notecnt = 0;
  xbit->zeropart = 0.0;
}

void xbit_note(xbit_t *xbit, case_bit_t bit)
{
  xbit->bit[random() % XBIT_CNT] = bit;
  xbit->notecnt++;
}

double xbit_zeropart(xbit_t *xbit)
{
  if (xbit->notecnt >= (XBIT_CNT / 2)) {
    calc(xbit);
    xbit->notecnt = 0;
  }
  return xbit->zeropart;
}
