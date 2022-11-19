#ifndef obj_class_h
#define obj_class_h

#include "bit.h"
#include "classstat.h"
#include "obj.h"

#define OBJ_CLASS_CACHE 64
#define OBJ_CLASS_CSVOBJ 4096
#define OBJ_CLASS_STR (4 + 1)
#define OBJ_CLASS_TYPE OBJ

void obj_class_observe(obj_t obj, long type);
obj_bit_t obj_class_classify(obj_t obj, long type);
obj_bit_t obj_class_classifyknown(obj_t obj, long type);

typedef obj_t (*obj_class_pack_f)(char *, long, long);
obj_t obj_class_packauto(char csvobj[OBJ_CLASS_CSVOBJ], long classindx, long type);
obj_t obj_class_packavg(char csvobj[OBJ_CLASS_CSVOBJ], long classindx, long type);
obj_t obj_class_packrand(char csvobj[OBJ_CLASS_CSVOBJ], long classindx, long type);
obj_t obj_class_packfirst(char csvobj[OBJ_CLASS_CSVOBJ], long classindx, long type);

void obj_class_resetstat(long type);
obj_classstat_t *obj_class_stat(long type);

double obj_class_indifreq(obj_t indicator, obj_t target, long type);
double obj_class_targfreq(obj_t indicator, obj_t target, long type);
double obj_class_indiover(obj_t indicator, obj_t target, long type);
double obj_class_targover(obj_t indicator, obj_t target, long type);
double obj_class_over(obj_t indicator, obj_t target, long type);
double obj_class_indimis(obj_t indicator, obj_t target, long type);
double obj_class_targmis(obj_t indicator, obj_t target, long type);
double obj_class_indiimp(obj_t indicator, obj_t target, long type);
double obj_class_targimp(obj_t indicator, obj_t target, long type);
double obj_class_indiopac(obj_t indicator, obj_t target, long type);
double obj_class_targopac(obj_t indicator, obj_t target, long type);
double obj_class_inditrans(obj_t indicator, obj_t target, long type);
double obj_class_targtrans(obj_t indicator, obj_t target, long type);
double obj_class_trans(obj_t indicator, obj_t target, long type);

#endif
