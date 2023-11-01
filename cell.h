#ifndef obj_cell_h
#define obj_cell_h

#include "binop.h"
#include "obj.h"

#define OBJ_CELL_TICKS OBJ

obj_t obj_cell_evolve(obj_t obj, obj_binop_t binop, long ticks);

#endif
