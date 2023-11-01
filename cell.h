#ifndef obj_cell_h
#define obj_cell_h

#include "obj.h"
#include "op2.h"

#define OBJ_CELL_TICKS OBJ

obj_t obj_cell_evolve(obj_t obj, obj_op2_t op2, long ticks);

#endif
