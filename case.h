#ifndef case_h
#define case_h

#include "bit.h"
#include "obj.h"

#define CASE_VERBOSE 1
#define CASE_XVERBOSE 1

#define CASE_OBJCACHE 64
#define CASE_CSVOBJ 4096
#define CASE_STR 4

void case_observe(case_obj_t obj, long type);
case_bit_t case_classify(case_obj_t obj, long type);

case_obj_t case_packavg(char *csvobj, long classindx, long type);
case_obj_t case_packrand(char *csvobj, long classindx, long type);
case_obj_t case_packfirst(char *csvobj, long classindx, long type);

double case_indifreq(case_obj_t indicator, case_obj_t target, long type);
double case_targfreq(case_obj_t indicator, case_obj_t target, long type);
double case_indiover(case_obj_t indicator, case_obj_t target, long type);
double case_targover(case_obj_t indicator, case_obj_t target, long type);
double case_over(case_obj_t indicator, case_obj_t target, long type);
double case_indimis(case_obj_t indicator, case_obj_t target, long type);
double case_targmis(case_obj_t indicator, case_obj_t target, long type);
double case_indiimp(case_obj_t indicator, case_obj_t target, long type);
double case_targimp(case_obj_t indicator, case_obj_t target, long type);
double case_indiopac(case_obj_t indicator, case_obj_t target, long type);
double case_targopac(case_obj_t indicator, case_obj_t target, long type);
double case_inditrans(case_obj_t indicator, case_obj_t target, long type);
double case_targtrans(case_obj_t indicator, case_obj_t target, long type);
double case_trans(case_obj_t indicator, case_obj_t target, long type);

#endif
