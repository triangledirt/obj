#ifndef obj_sync_h
#define obj_sync_h

#include "obj.h"
#include "syncstyle.h"

#define OBJ_SYNC_CACHE 1024
#define OBJ_SYNC_SET 16
#define OBJ_SYNC_TYPE OBJ

void obj_sync_setstyle(enum obj_syncstyle_t style, long type);

void obj_sync_start(long set, long type);
obj_t obj_sync_next(long set, long type);

#endif
