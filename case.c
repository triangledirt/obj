#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bit.h"
#include "case.h"
#include "coin.h"
#include "core.h"
#include "die.h"
#include "filt.h"
#include "fold.h"
#include "gene.h"
#include "jack.h"
#include "jung.h"
#include "obj.h"
#include "step.h"
#include "sum.h"
#include "timer.h"
#include "val.h"
#include "valtype.h"

#define PACKCACHE (CASE_OBJCACHE / 2)

static case_obj_t object[32][CASE_OBJCACHE];
static case_bool_t once = case_bool_false;
static case_obj_t types;
static case_step_t step[32];

static val_t value[32][PACKCACHE][32];
static val_t firstval[32][32];
static valtype_t valtype[32][32];
static case_bool_t firstpack[32];

typedef void (*learn_f)(case_obj_t[], long, long);
static void learn();
static long learngeneral(case_obj_t obj[], long objsz, long type, learn_f learnfunc);
static void init();
static void notetype(long type);
static long count(case_obj_t objtype, long type);
static long countboth(case_obj_t objtype1, case_obj_t objtype2, long type);
static long counteither(case_obj_t objtype1, case_obj_t objtype2, long type);
static long countsub(case_obj_t objtype1, case_obj_t objtype2, long type);
static long countxor(case_obj_t objtype1, case_obj_t objtype2, long type);

typedef case_bit_t (*pack_f)(val_t *, long, long);
static case_obj_t packgeneral(char csvobj[CASE_CSVOBJ], long classindx, long type, pack_f packfunc);
static case_bit_t packavg(val_t *val, long attr, long type);
static case_bit_t packavgnum(val_t *val, long attr, long type);
static case_bit_t packavgstr(val_t *val, long attr, long type);
static case_bit_t packfirst(val_t *val, long attr, long type);
static case_bit_t packrand(val_t *val, long attr, long type);
static void setvaltypes(char csvobj[CASE_CSVOBJ], long classindx, long type);
static void insertfirstval(val_t valobj[32], long type);
static void insertval(val_t valobj[32], long type);
static void csv2valobj(char csvobj[CASE_CSVOBJ], long classindx, val_t valobj[32], long type);
static void text2val(char *text, val_t *val, long valindx, long type);
static case_bool_t isnum(char *str);
static long reorderindx(long attrindx, long classindx);

case_bit_t case_classify(case_obj_t obj, long type)
{
  case_bit_t class;
  double corescore = 0.0;
  double filtscore = 0.0;
  double foldscore = 0.0;
  double genescore = 0.0;
  double jackscore = 0.0;
  double jungscore = 0.0;
  double sumscore = 0.0;
  init();
  notetype(type);
  case_obj_obscureclass(&obj);
  /*  corescore = core_score(obj, type);  */
  filtscore = filt_score(obj, type);
  foldscore = fold_score(obj, type);
  /*  genescore = gene_score(obj, type);  */
  /*  jackscore = jack_score(obj, type);  */
  /*  jungscore = jung_score(obj, type);  */
  sumscore = sum_score(obj, type);
  class = ((filtscore + foldscore + sumscore) > (4 * 0.5)) ? 1 : 0;
  class = ( (sumscore > 1 * 0.75) && (foldscore > 0.5) ) ? 1 : 0;
  class = (sumscore > 0.5) ? 1 : 0;
#if CASE_VERBOSE && CASE_XVERBOSE
  printf("type%ld class     core=%0.3f filt=%0.3f fold=%0.3f gene=%0.3f jack=%0.3f jung=%0.3f sum=%0.3f\n", type, corescore, filtscore, foldscore, genescore, jackscore, jungscore, sumscore);
#endif
  return class;
}

case_bit_t case_classifyknown(case_obj_t obj, case_bit_t knownclass, long type)
{
  case_bit_t guessclass;
  guessclass = case_classify(obj, type);
  case_step_noteclasses(&step[type], guessclass, knownclass);
  return guessclass;
}

case_step_t *case_getstep(long type)
{
  return &step[type];
}

double case_indifreq(case_obj_t indicator, case_obj_t target, long type)
{
  long indicnt;
  long targcnt;
  init();
  indicnt = count(indicator, type);
  targcnt = count(target, type);
  if (0 == targcnt)
    targcnt = 1;
  return (long) indicnt / targcnt;
}

double case_indiimp(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long targcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  targcnt = count(target, type);
  if (0 == targcnt)
    targcnt = 1;
  return (long) indisubcnt / targcnt;
}

double case_indimis(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long targsubcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  targsubcnt = countsub(target, indicator, type);
  if (0 == targsubcnt)
    targsubcnt = 1;
  return (long) indisubcnt / targsubcnt;
}

double case_indiopac(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long bothcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  bothcnt = countboth(indicator, target, type);
  if (0 == bothcnt)
    bothcnt = 1;
  return (long) indisubcnt / bothcnt;
/*  TODO (long) ?? (double) ??  */
}

double case_indiover(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long indicnt;
  init();
  bothcnt = countboth(indicator, target, type);
  indicnt = count(indicator, type);
  if (0 == indicnt)
    indicnt = 1;
  return (long) bothcnt / indicnt;
}

double case_inditrans(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long indisubcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  indisubcnt = countsub(indicator, target, type);
  if (0 == indisubcnt)
    indisubcnt = 1;
  return (long) bothcnt / indisubcnt;
}

void case_observe(case_obj_t obj, long type)
{
  long i;
  init();
  notetype(type);
  i = random() % CASE_OBJCACHE;
  object[type][i] = obj;
  if (die_toss(CASE_OBJCACHE / 16))
    learn();
}

double case_over(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long eithercnt;
  init();
  bothcnt = countboth(indicator, target, type);
  eithercnt = counteither(indicator, target, type);
  if (0 == eithercnt)
    eithercnt = 1;
  return (long) bothcnt / eithercnt;
}

case_obj_t case_packavg(char csvobj[CASE_CSVOBJ], long classindx, long type)
{
  init();
  return packgeneral(csvobj, classindx, type, packavg);
}

case_obj_t case_packfirst(char csvobj[CASE_CSVOBJ], long classindx, long type)
{
  init();
  return packgeneral(csvobj, classindx, type, packfirst);
}

case_obj_t case_packrand(char csvobj[CASE_CSVOBJ], long classindx, long type)
{
  init();
  return packgeneral(csvobj, classindx, type, packrand);
}

double case_targfreq(case_obj_t indicator, case_obj_t target, long type)
{
  long targcnt;
  long indicnt;
  init();
  targcnt = count(target, type);
  indicnt = count(indicator, type);
  if (0 == indicnt)
    indicnt = 1;
  return (long) targcnt / indicnt;
}

double case_targimp(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long indicnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  indicnt = count(indicator, type);
  if (0 == indicnt)
    indicnt = 1;
  return (long) targsubcnt / indicnt;
}

double case_targmis(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long indisubcnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  indisubcnt = countsub(indicator, target, type);
  if (0 == indisubcnt)
    indisubcnt = 1;
  return (long) targsubcnt / indisubcnt;
}

double case_targopac(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long bothcnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  bothcnt = countboth(indicator, target, type);
  if (0 == bothcnt)
    bothcnt = 1;
  return (long) targsubcnt / bothcnt;
}

double case_targover(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long targcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  targcnt = count(target, type);
  if (0 == targcnt)
    targcnt = 1;
  return (long) bothcnt / targcnt;
}

double case_targtrans(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long targsubcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  targsubcnt = countsub(target, indicator, type);
  if (0 == targsubcnt)
    targsubcnt = 1;
  return (long) bothcnt / targsubcnt;
}

double case_trans(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long xorcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  xorcnt = countxor(indicator, target, type);
  if (0 == xorcnt)
    xorcnt = 1;
  return (long) bothcnt / xorcnt;
}

long count(case_obj_t objtype, long type)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < CASE_OBJCACHE; i++) {
    obj = object[type][i];
    count += case_obj_hastype(obj, objtype);
  }
  return count;
}

long countboth(case_obj_t objtype1, case_obj_t objtype2, long type)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < CASE_OBJCACHE; i++) {
    obj = object[type][i];
    count += case_obj_hastype(obj, objtype1) && case_obj_hastype(obj, objtype2);
  }
  return count;
}

long counteither(case_obj_t objtype1, case_obj_t objtype2, long type)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < CASE_OBJCACHE; i++) {
    obj = object[type][i];
    count += case_obj_hastype(obj, objtype1) || case_obj_hastype(obj, objtype2);
  }
  return count;
}

long countsub(case_obj_t objtype1, case_obj_t objtype2, long type)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < CASE_OBJCACHE; i++) {
    obj = object[type][i];
    count += case_obj_hastype(obj, objtype1) && !case_obj_hastype(obj, objtype2);
  }
  return count;
}

long countxor(case_obj_t objtype1, case_obj_t objtype2, long type)
{
  long count = 0;
  case_obj_t obj;
  long i;
  case_bit_t has1;
  case_bit_t has2;
  for (i = 0; i < CASE_OBJCACHE; i++) {
    obj = object[type][i];
    has1 = case_obj_hastype(obj, objtype1);
    has2 = case_obj_hastype(obj, objtype2);
    if (has1 ^ has2)
      count++;
  }
  return count;
}

void csv2valobj(char csvobj[CASE_CSVOBJ], long classindx, val_t valobj[32], long type)
{
  char csvobjcopy[CASE_CSVOBJ];
  char *tok;
  long csvindx = 0;
  long valindx;
  strncpy(csvobjcopy, csvobj, CASE_CSVOBJ - 1);
  tok = strtok(csvobjcopy, ",\n");
  valindx = reorderindx(csvindx, classindx);
  text2val(tok, &valobj[valindx], valindx, type);
  while ((tok = strtok(0, ",\n")) && (csvindx < 31)) {
    csvindx++;
    valindx = reorderindx(csvindx, classindx);
    text2val(tok, &valobj[valindx], valindx, type);
  }
  for (valindx = csvindx + 1; valindx < 32; valindx++)
    val_init(&valobj[valindx], valtype[type][valindx]);
#if 0 && CASE_VERBOSE && CASE_XVERBOSE
  printf("type%ld   csv     %s", type, csvobj);
  printf("type%ld   val     ", type);
  for (valindx = 0; valindx < 32; valindx++) {
    val_print(&valobj[valindx], valtype[type][valindx]);
    printf(",");
  }
  printf("\n");
#endif
}

void init()
{
  long i;
  long type;
  if (!once) {
    srandom(time(NULL));
    case_obj_clear(&types);
    for (type = 0; type < 32; type++) {
      for (i = 0; i < CASE_OBJCACHE; i++)
        case_obj_randomize(&object[type][i]);
      case_step_reset(&step[type]);
      firstpack[type] = case_bool_true;
    }
    once = case_bool_true;
  }
}

void insertfirstval(val_t valobj[32], long type)
{
  long attr;
  for (attr = 0; attr < 32; attr++)
    val_copy(&valobj[attr], &firstval[type][attr], valtype[type][attr]);
}

void insertval(val_t valobj[32], long type)
{
  long obj;
  long attr;
  obj = random() % PACKCACHE;
  for (attr = 0; attr < 32; attr++)
    val_copy(&valobj[attr], &value[type][obj][attr], valtype[type][attr]);
}

case_bool_t isnum(char *str)
{
  long len;
  long i;
  char c;
  case_bool_t is = case_bool_true;
  len = strlen(str);
  for (i = 0; i < len; i++) {
    c = *(str + i);
    if (!(('+' == c) || ('-' == c) || ('.' == c) || isdigit(c))) {
      is = case_bool_false;
      break;
    }
  }
  return is;
}

void learn()
{
  long type;
  long coretime = 0;
  long filttime = 0;
  long foldtime = 0;
  long genetime = 0;
  long jacktime = 0;
  long jungtime = 0;
  long sumtime = 0;
  for (type = 0; type < 32; type++)
    if (case_obj_getattr(types, type)) {
      /*  coretime = learngeneral(object[type], CASE_OBJCACHE, type, core_learn);  */
      filttime = learngeneral(object[type], CASE_OBJCACHE, type, filt_learn);
      foldtime = learngeneral(object[type], CASE_OBJCACHE, type, fold_learn);
      /*  genetime = learngeneral(object[type], CASE_OBJCACHE, type, gene_learn);  */
      /*  jacktime = learngeneral(object[type], CASE_OBJCACHE, type, jack_learn);  */
      /*  jungtime = learngeneral(object[type], CASE_OBJCACHE, type, jung_learn);  */
      sumtime = learngeneral(object[type], CASE_OBJCACHE, type, sum_learn);
#if CASE_VERBOSE && CASE_XVERBOSE
      printf("type%ld times     core=%ld filt=%ld fold=%ld gene=%ld jack=%ld jung=%ld sum=%ld\n", type, coretime, filttime, foldtime, genetime, jacktime, jungtime, sumtime);
#endif
    }
}

long learngeneral(case_obj_t obj[], long objsz, long type, learn_f learnfunc)
{
  long time;
  timer_start(0);
  learnfunc(obj, objsz, type);
  time = timer_stop();
  return time;
}

void notetype(long type)
{
  case_obj_setattr(&types, type, 1);
}

case_bit_t packavg(val_t *val, long attr, long type)
{
  case_bit_t bit;
  switch (valtype[type][attr]) {
    case valtype_num:
      bit = packavgnum(val, attr, type);
      break;
    case valtype_str:
      bit = packavgstr(val, attr, type);
      break;
  }
  return bit;
}

case_bit_t packavgnum(val_t *val, long attr, long type)
{
  double avg;
  double tot = 0.0;
  long i;
  for (i = 0; i < PACKCACHE; i++)
    tot += value[type][i][attr].num;
  avg = tot / PACKCACHE;
  return val->num > avg;
}

case_bit_t packavgstr(val_t *val, long attr, long type)
{
  long samplesz = PACKCACHE / 8;
  long sampleobj[samplesz];
  long samplecnt[samplesz];
  long i;
  long j;
  long k;
  long max = 0;
  long maxindx = 0;
  for (i = 0; i < samplesz; i++) {
    sampleobj[i] = random() % PACKCACHE;
    samplecnt[i] = 0;
  }
  for (i = 0; i < PACKCACHE; i++)
    for (j = 0; j < samplesz; j++) {
      k = sampleobj[j];
      if (0 == strncmp(value[type][i][attr].str, value[type][k][attr].str, CASE_STR - 1)) {
        samplecnt[j]++;
        break;
      }
    }
  for (j = 0; j < samplesz; j++)
    if (samplecnt[j] > max) {
      max = samplecnt[j];
      maxindx = sampleobj[j];
    }
  return 0 == strncmp(val->str, value[type][maxindx][attr].str, CASE_STR - 1);
}

case_bit_t packfirst(val_t *val, long attr, long type)
{
  long compare;
  compare = val_compare(val, &firstval[type][attr], valtype[type][attr]);
  return compare == 0;
}

case_obj_t packgeneral(char csvobj[CASE_CSVOBJ], long classindx, long type, pack_f packfunc)
{
  case_obj_t obj;
  val_t valobj[32];
  long attr;
  case_bit_t bit;
  if (firstpack[type])
    setvaltypes(csvobj, classindx, type);
  csv2valobj(csvobj, classindx, valobj, type);
  if (firstpack[type]) {
    insertfirstval(valobj, type);
    firstpack[type] = case_bool_false;
  }
  insertval(valobj, type);
  for (attr = 0; attr < 32; attr++) {
    bit = packfunc(&valobj[attr], attr, type);
    case_obj_setattr(&obj, attr, bit);
  }
  return obj;
}

case_bit_t packrand(val_t *val, long attr, long type)
{
  case_bit_t bit;
  long compare;
  long i;
  i = random() % PACKCACHE;
  compare = val_compare(val, &value[type][i][attr], valtype[type][attr]);
  bit = compare == 0;
  return bit;
}

void case_printstep(long type)
{
  double fmeasure;
  double precision;
  double recall;
  fmeasure = case_step_fmeasure(&step[type]);
  precision = case_step_precision(&step[type]);
  recall = case_step_recall(&step[type]);
  printf("type%ld steps     fmeasure=%0.3f precision=%0.3f recall=%0.3f\n", type, fmeasure, precision, recall);
}

long reorderindx(long attrindx, long classindx)
{
  long reindx;
  if (attrindx == classindx) {
    reindx = 0;
  } else {
    reindx = (0 == attrindx) ? classindx : attrindx;
  }
  return reindx;
}

void case_resetstep(long type)
{
  case_step_reset(&step[type]);
}

void setvaltypes(char csvobj[CASE_CSVOBJ], long classindx, long type)
{
  char csvobjcopy[CASE_CSVOBJ];
  char *tok;
  long csvindx = 0;
  long valindx;
  strncpy(csvobjcopy, csvobj, CASE_CSVOBJ - 1);
  tok = strtok(csvobjcopy, ",\n");
  valindx = reorderindx(csvindx, classindx);
  valtype[type][valindx] = isnum(tok) ? valtype_num : valtype_str;
  while ((tok = strtok(0, ",\n")) && (csvindx < 31)) {
    csvindx++;
    valindx = reorderindx(csvindx, classindx);
    valtype[type][valindx] = isnum(tok) ? valtype_num : valtype_str;
  }
  for (valindx = csvindx + 1; valindx < 32; valindx++)
    valtype[type][valindx] = valtype_str;
#if CASE_VERBOSE && CASE_XVERBOSE
  printf("type%ld types     ", type);
  for (valindx = 0; valindx < 32; valindx++)
    printf("%s,", valtype_name(valtype[type][valindx]));
  printf("\n");
#endif
}

void text2val(char *text, val_t *val, long valindx, long type)
{
  if (valtype_num == valtype[type][valindx]) {
    val->num = strtod(text, 0);
  } else {
    strncpy(val->str, text, CASE_STR - 1);
  }
}
