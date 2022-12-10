#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bit.h"
#include "bool.h"
#include "indx.h"
#include "obj.h"

void obj_clear(obj_t *obj)
{
  *obj = 0;
}

double obj_comparebox(obj_t obj1, obj_t obj2)
{
  long edge;
  long i;
  long j;
  obj_bit_t o1bit;
  obj_bit_t o2bit;
  long and = 0;
  edge = obj_edge(obj1, obj2);
  for (i = 1; i <= edge; i++)
    for (j = 1; j <= edge; j++) {
      o1bit = obj_getattr(obj1, i);
      o2bit = obj_getattr(obj2, j);
      if (o1bit && o2bit)
        and++;
    }
  return and / pow(edge, 2);
}

double obj_compareequal(obj_t obj1, obj_t obj2)
{
  long bit;
  long edge;
  long correct = 0;
  obj_bit_t bit1;
  obj_bit_t bit2;
  edge = obj_edge(obj1, obj2);
  for (bit = 1; bit <= edge; bit++) {
    bit1 = obj_getattr(obj1, bit);
    bit2 = obj_getattr(obj2, bit);
    if (bit1 == bit2)
      correct++;
  }
  return (double) correct / (1 + edge);
}

double obj_comparefocus(obj_t obj1, obj_t obj2)
{
  return 1 - obj_compareoblivion(obj1, obj2);
}

double obj_compareoblivion(obj_t obj1, obj_t obj2)
{
  long match;
  long opposite = 0;
  long edge;
  long i;
  obj_bit_t o1bit;
  obj_bit_t o2bit;
  double oblivion;
  edge = obj_edge(obj1, obj2);
  for (i = 1; i <= edge; i++) {
    o1bit = obj_getattr(obj1, i);
    o2bit = obj_getattr(obj2, i);
    if (o1bit != o2bit)
      opposite++;
  }
  match = edge - opposite;
#ifdef OBJ_XVERBOSE
  printf("edge=%lu match=%lu opposite=%lu\n", edge, match, opposite);
#endif
  if (0 == edge) {
    oblivion = 1;
  } else {
    oblivion = (double) (edge - labs(match - opposite)) / edge;
  }
  return oblivion;
}

double obj_comparetypes(obj_t obj1, obj_t obj2)
{
  long bit;
  long correct = 0;
  long total = 0;
  obj_bit_t bit1;
  obj_bit_t bit2;
  for (bit = 1; bit < OBJ; bit++) {
    bit1 = obj_getattr(obj1, bit);
    bit2 = obj_getattr(obj2, bit);
    if ((1 == bit1) || (1 == bit2)) {
      total++;
      if ((1 == bit1) && (1 == bit2))
        correct++;
    }
  }
  return (double) correct / (1 + total);
}

double obj_comparexor(obj_t obj1, obj_t obj2)
{
  long bit;
  long edge;
  long correct = 0;
  obj_bit_t bit1;
  obj_bit_t bit2;
  edge = obj_edge(obj1, obj2);
  for (bit = 1; bit <= edge; bit++) {
    bit1 = obj_getattr(obj1, bit);
    bit2 = obj_getattr(obj2, bit);
    if (bit1 ^ bit2)
      correct++;
  }
  return (double) correct / (1 + edge);
}

double obj_defaultfit(obj_t obj, long type, void *context)
{
  return 0.0;
}

long obj_edge(obj_t obj1, obj_t obj2)
{
  long edge = OBJ - 1;
  obj_bit_t bit1;
  obj_bit_t bit2;
  bit1 = obj_getattr(obj1, edge);
  bit2 = obj_getattr(obj2, edge);
  while ((edge > 0) && !bit1 && !bit2) {
    edge--;
    bit1 = obj_getattr(obj1, edge);
    bit2 = obj_getattr(obj2, edge);
  }
  return edge;
}

void obj_fill(obj_t *obj)
{
  long bit;
  for (bit = 0; bit < OBJ; bit++)
    obj_setattr(obj, bit, 1);
}

long long obj_getnum(obj_t obj, long startbit, long length)
{
  long place = 1;
  long bit;
  long long num = 0;
  for (bit = startbit; bit < length; bit++) {
    num += place * obj_getattr(obj, bit);
    place *= 2;
  }
  return num;
}

obj_bool_t obj_hastype(obj_t obj, obj_t type)
{
  obj_bool_t has = obj_bool_true;
  long i;
  for (i = 0; i < OBJ; i++)
    if (obj_getattr(type, i))
      if (!obj_getattr(obj, i)) {
        has = obj_bool_false;
        break;
      }
  return has;
}

void obj_mutate(obj_t *obj)
{
  long i;
  obj_bit_t val;
  i = random() % OBJ;
  obj_bit_randomize(&val);
  obj_setattr(obj, i, val);
}

void obj_obscureclass(obj_t *obj)
{
  obj_setclass(obj, obj_bit_random());
}

void obj_print(obj_t obj)
{
  long i;
  for (i = 0; i < OBJ; i++)
    printf("%lld", obj_getattr(obj, i));
}

void obj_randomize(obj_t *obj)
{
  *obj = random();
}

void obj_rotate(obj_t *obj, long inc)
{
  obj_t obj2;
  long i;
  long newi;
  obj_bit_t bit;
  for (i = 0; i < OBJ; i++) {
    bit = obj_getattr(*obj, i);
    newi = obj_indx_wrap(i + inc, OBJ);
    obj_setattr(&obj2, newi, bit);
  }
  *obj = obj2;
}

void obj_setfromstr(obj_t *obj, char str[OBJ])
{
  long i;
  obj_bit_t val;
  for (i = 0; i < OBJ; i++) {
    val = ('0' == str[i]) ? 0 : 1;
    obj_setattr(obj, i, val);
  }
}

void obj_setnum(obj_t *obj, long startbit, long length, long long num)
{
  long place = 2;
  long bit = startbit;
  obj_bit_t val;
  long long rem = num;
  do {
    val = rem % place;
    obj_setattr(obj, bit, val);
    rem -= val;
    place *= 2;
    bit++;
  } while (bit < length);
}
