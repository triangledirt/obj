#ifndef alib_h
#define alib_h

#include "abit.h"
#include "aobj.h"

#define ALIB_TYPE_COUNT 8
#define ALIB_VERBOSE 1

void alib_observe(aobj_t obj, long type);
abit_t alib_classify(aobj_t obj, long type);

double alib_indifrequency(aobj_t indicator, aobj_t target, long type);
double alib_targfrequency(aobj_t indicator, aobj_t target, long type);
double alib_indioverlap(aobj_t indicator, aobj_t target, long type);
double alib_targoverlap(aobj_t indicator, aobj_t target, long type);
double alib_overlap(aobj_t indicator, aobj_t target, long type);
double alib_indimismatch(aobj_t indicator, aobj_t target, long type);
double alib_targmismatch(aobj_t indicator, aobj_t target, long type);
double alib_indiimpertinence(aobj_t indicator, aobj_t target, long type);
double alib_targimpertinence(aobj_t indicator, aobj_t target, long type);
double alib_indiopacity(aobj_t indicator, aobj_t target, long type);
double alib_targopacity(aobj_t indicator, aobj_t target, long type);
double alib_inditransparency(aobj_t indicator, aobj_t target, long type);
double alib_targtransparency(aobj_t indicator, aobj_t target, long type);
double alib_transparency(aobj_t indicator, aobj_t target, long type);

#endif
