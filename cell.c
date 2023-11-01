#include "cell.h"
#include "indx.h"

obj_t obj_cell_evolve(obj_t obj, obj_op_t op, long ticks)
{
  long tick;
  obj_bit_t in1;
  obj_bit_t in2;
  obj_bit_t out;
  long i;
  obj_t current;
  obj_t next;
  current = obj;
  for (tick = 0; tick < ticks; tick++) {
    for (i = 0; i < OBJ; i++) {
      in1 = obj_getattr(current, i);
      in2 = obj_getattr(current, obj_indx_wrap(i + 1, OBJ));
      out = obj_op_calc(op, in1, in2);
      obj_setattr(&next, i, out);
    }
    current = next;
  }
  return current;
}
