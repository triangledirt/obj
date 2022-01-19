#ifndef net_h
#define net_h

#include "bit.h"
#include "obj.h"

void net_learn(case_obj_t obj[], long objsz, long type);
case_bit_t net_classify(case_obj_t obj, long type);

#endif
