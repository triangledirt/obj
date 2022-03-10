#ifndef gene_h
#define gene_h

#include "bit.h"
#include "obj.h"

void gene_learn(case_obj_t obj[], long objsz, long type);
double gene_classify(case_obj_t obj, long type);

#endif
