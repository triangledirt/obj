#ifndef gene_h
#define gene_h

#include "bit.h"
#include "obj.h"

void gene_learn(case_obj_t objs[], long objssze, long type);
case_bit_t gene_classify(case_obj_t obj, long type);

#endif
