#ifndef case_h
#define case_h

#include "bit.h"
#include "object.h"

#define CASE_VERBOSE 1

void case_observe(case_object_t obj, long type);
case_bit_t case_classify(case_object_t obj, long type);

void case_lens(char *obj, long type);

double case_frequencyi(case_object_t indicator, case_object_t target,
  long type);
double case_frequencyt(case_object_t indicator, case_object_t target,
  long type);
double case_overlapi(case_object_t indicator, case_object_t target, long type);
double case_overlapt(case_object_t indicator, case_object_t target, long type);
double case_overlap(case_object_t indicator, case_object_t target, long type);
double case_mismatchi(case_object_t indicator, case_object_t target,
  long type);
double case_mismatcht(case_object_t indicator, case_object_t target,
  long type);
double case_impertinencei(case_object_t indicator, case_object_t target,
  long type);
double case_impertinencet(case_object_t indicator, case_object_t target,
  long type);
double case_opacityi(case_object_t indicator, case_object_t target, long type);
double case_opacityt(case_object_t indicator, case_object_t target, long type);
double case_transparencyi(case_object_t indicator, case_object_t target,
  long type);
double case_transparencyt(case_object_t indicator, case_object_t target,
  long type);
double case_transparency(case_object_t indicator, case_object_t target,
  long type);

#endif
