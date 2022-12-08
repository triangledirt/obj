#ifndef obj_gene_h
#define obj_gene_h

#include "obj.h"

void obj_gene_learn(obj_t obj[], long objsz, long type);
double obj_gene_score(obj_t obj, long type);

double obj_gene_fitness(long type);

#endif
