#ifndef alib_h
#define alib_h

#include "abit.h"
#include "aobj.h"

#define ALIB_TYPE_COUNT 8
#define ALIB_VERBOSE 1

void alib_observe(aobj_t obj, long type);
abit_t alib_classify(aobj_t obj, long type);

double alib_indifreq(aobj_t indicator, aobj_t target, long type);
double alib_targfreq(aobj_t indicator, aobj_t target, long type);
double alib_indiover(aobj_t indicator, aobj_t target, long type);
double alib_targover(aobj_t indicator, aobj_t target, long type);
double alib_overlap(aobj_t indicator, aobj_t target, long type);
double alib_indimism(aobj_t indicator, aobj_t target, long type);
double alib_targmism(aobj_t indicator, aobj_t target, long type);
double alib_indiimp(aobj_t indicator, aobj_t target, long type);
double alib_targimp(aobj_t indicator, aobj_t target, long type);
double alib_indiopac(aobj_t indicator, aobj_t target, long type);
double alib_targopac(aobj_t indicator, aobj_t target, long type);
double alib_inditrans(aobj_t indicator, aobj_t target, long type);
double alib_targtrans(aobj_t indicator, aobj_t target, long type);
double alib_transparency(aobj_t indicator, aobj_t target, long type);

#endif
