#include "indx.h"
#include "mach.h"

obj_t obj_mach_go(obj_t obj, obj_op3_t op3, long ticks)
{
  long tick;
  obj_bit_t in1;
  obj_bit_t in2;
  obj_bit_t in3;
  obj_bit_t out;
  long i;
  obj_t current;
  obj_t next;
  current = obj;
  for (tick = 0; tick < ticks; tick++) {
    for (i = 0; i < OBJ; i++) {
      in1 = obj_getattr(current, obj_indx_wrap(i - 1, OBJ));
      in2 = obj_getattr(current, i);
      in3 = obj_getattr(current, obj_indx_wrap(i + 1, OBJ));
      out = obj_op3_calc(op3, in1, in2, in3);
      obj_setattr(&next, i, out);
    }
    current = next;
  }
  return current;
}
