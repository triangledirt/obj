#ifndef case_h
#define case_h

#include "bit.h"
#include "obj.h"

#define CASE_VERBOSE 1
#define CASE_EXTRA_VERBOSE 1

void case_observe(case_obj_t obj, long type);
case_bit_t case_classify(case_obj_t obj, long type);

void case_lens(char *obj, long type);

double case_frequencyi(case_obj_t indicator, case_obj_t target, long type);
double case_frequencyt(case_obj_t indicator, case_obj_t target, long type);
double case_overlapi(case_obj_t indicator, case_obj_t target, long type);
double case_overlapt(case_obj_t indicator, case_obj_t target, long type);
double case_overlap(case_obj_t indicator, case_obj_t target, long type);
double case_mismatchi(case_obj_t indicator, case_obj_t target, long type);
double case_mismatcht(case_obj_t indicator, case_obj_t target, long type);
double case_impertinencei(case_obj_t indicator, case_obj_t target, long type);
double case_impertinencet(case_obj_t indicator, case_obj_t target, long type);
double case_opacityi(case_obj_t indicator, case_obj_t target, long type);
double case_opacityt(case_obj_t indicator, case_obj_t target, long type);
double case_transparencyi(case_obj_t indicator, case_obj_t target, long type);
double case_transparencyt(case_obj_t indicator, case_obj_t target, long type);
double case_transparency(case_obj_t indicator, case_obj_t target, long type);

#endif
