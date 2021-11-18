#ifndef gene_h
#define gene_h

#include "bit.h"
#include "object.h"

void gene_learn(case_object_t objs[], long objs_size, long type);
case_bit_t gene_classify(case_object_t obj, long type);

#endif
