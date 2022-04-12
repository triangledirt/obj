#ifndef gene_h
#define gene_h

#include "obj.h"

void gene_learn(case_obj_t obj[], long objsz, long type);
double gene_score(case_obj_t obj, long type);

#endif
