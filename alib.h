#ifndef alib_h
#define alib_h

#include "abit.h"
#include "aobj.h"

#define ALIB_VERBOSE 1

void alib_observe(aobj_t obj, long type);
abit_t alib_classify(aobj_t obj, long type);

double alib_frequencyi(aobj_t indicator, aobj_t target, long type);
double alib_frequencyt(aobj_t indicator, aobj_t target, long type);
double alib_overlapi(aobj_t indicator, aobj_t target, long type);
double alib_overlapt(aobj_t indicator, aobj_t target, long type);
double alib_overlap(aobj_t indicator, aobj_t target, long type);
double alib_mismatchi(aobj_t indicator, aobj_t target, long type);
double alib_mismatcht(aobj_t indicator, aobj_t target, long type);
double alib_impertinencei(aobj_t indicator, aobj_t target, long type);
double alib_impertinencet(aobj_t indicator, aobj_t target, long type);
double alib_opacityi(aobj_t indicator, aobj_t target, long type);
double alib_opacityt(aobj_t indicator, aobj_t target, long type);
double alib_transparencyi(aobj_t indicator, aobj_t target, long type);
double alib_transparencyt(aobj_t indicator, aobj_t target, long type);
double alib_transparency(aobj_t indicator, aobj_t target, long type);

#endif
