#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "case.h"
#include "coin.h"
#include "core.h"
#include "die.h"
#include "filt.h"
#include "fold.h"
#include "gene.h"
#include "jack.h"
#include "jung.h"
#include "sum.h"
#include "timer.h"
#include "val.h"
#include "valtype.h"

#define PACKCACHE (CASE_OBJCACHE / 2)
#define STRTOK ",;\n"

static case_obj_t object[32][CASE_OBJCACHE];
static case_bool_t once = case_bool_false;
static case_stat_t stat[32];

static val_t value[32][PACKCACHE][32];
static val_t firstval[32][32];
static valtype_t valtype[32][32];
static case_bool_t firstpack[32];

typedef double (*score_f)(case_obj_t, long);
#define SCORE 7
static score_f scorefunc[SCORE] = {core_score, filt_score, fold_score, gene_score, jack_score, jung_score, sum_score};
static char *scorename[SCORE] = {"core", "filt", "fold", "gene", "jack", "jung", "sum"};
static long favscoreindx[32];

typedef void (*learn_f)(case_obj_t[], long, long);
static void learn(long type);
static long learngeneral(case_obj_t obj[], long objsz, long type, learn_f learnfunc);
static void init();
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

static long randomscoreindx(long exclude);

case_bit_t case_classify(case_obj_t obj, long type)
{
  case_bit_t class;
  double score;
  double rescore;
  long scorefindx;
  long rescorefindx;
  score_f scoref;
  score_f rescoref;
  char *scorefname;
  char *rescorefname;
  char c;
  init();
  case_obj_obscureclass(&obj);
  scorefindx = favscoreindx[type];
  scoref = scorefunc[scorefindx];
  score = scoref(obj, type);
  if (die_toss(CASE_OBJCACHE / 1)) {
    rescorefindx = randomscoreindx(scorefindx);
    rescoref = scorefunc[rescorefindx];
    rescore = rescoref(obj, type);
    if ((rescore - score) > 0.1) {
#if CASE_VERBOSE && CASE_XVERBOSE
      scorefname = scorename[scorefindx];
      rescorefname = scorename[rescorefindx];
      printf("type%ld class     switching algo from %s >> %s\n", type, scorefname, rescorefname);
#endif
      favscoreindx[type] = rescorefindx;
      score = rescore;
      scorefindx = rescorefindx;
    }
  }
  class = (score > 0.75) ? 1 : 0;
  c = case_bit_char(class);
#if CASE_VERBOSE && CASE_XVERBOSE
  scorefname = scorename[scorefindx];
  printf("type%ld class     class=%c scorefunc=%s score=%0.3f\n", type, c, scorefname, score);
#endif
  return class;
}

case_bit_t case_classifyknown(case_obj_t obj, case_bit_t knownclass, long type)
{
  case_bit_t guessclass;
  guessclass = case_classify(obj, type);
  case_stat_noteclasses(&stat[type], guessclass, knownclass);
  return guessclass;
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
  return (double) indicnt / targcnt;
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
  return (double) indisubcnt / targcnt;
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
  return (double) indisubcnt / targsubcnt;
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
  return (double) indisubcnt / bothcnt;
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
  return (double) bothcnt / indicnt;
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
  return (double) bothcnt / indisubcnt;
}

void case_observe(case_obj_t obj, long type)
{
  long i;
  init();
  i = random() % CASE_OBJCACHE;
  object[type][i] = obj;
  if (die_toss(CASE_OBJCACHE / 8))
    learn(type);
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
  return (double) bothcnt / eithercnt;
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

void case_resetstat(long type)
{
  case_stat_reset(&stat[type]);
}

case_stat_t *case_stat(long type)
{
  return &stat[type];
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
  return (double) targcnt / indicnt;
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
  return (double) targsubcnt / indicnt;
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
  return (double) targsubcnt / indisubcnt;
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
  return (double) targsubcnt / bothcnt;
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
  return (double) bothcnt / targcnt;
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
  return (double) bothcnt / targsubcnt;
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
  return (double) bothcnt / xorcnt;
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
  tok = strtok(csvobjcopy, STRTOK);
  valindx = reorderindx(csvindx, classindx);
  text2val(tok, &valobj[valindx], valindx, type);
  while ((tok = strtok(0, STRTOK)) && (csvindx < 31)) {
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
    for (type = 0; type < 32; type++) {
      for (i = 0; i < CASE_OBJCACHE; i++)
        case_obj_randomize(&object[type][i]);
      case_stat_reset(&stat[type]);
      favscoreindx[type] = random() % SCORE;
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

void learn(long type)
{
  long coretime = 0;
  long filttime = 0;
  long foldtime = 0;
  long genetime = 0;
  long jacktime = 0;
  long jungtime = 0;
  long sumtime = 0;
  coretime = learngeneral(object[type], CASE_OBJCACHE, type, core_learn);
  filttime = learngeneral(object[type], CASE_OBJCACHE, type, filt_learn);
  foldtime = learngeneral(object[type], CASE_OBJCACHE, type, fold_learn);
  genetime = learngeneral(object[type], CASE_OBJCACHE, type, gene_learn);
  jacktime = learngeneral(object[type], CASE_OBJCACHE, type, jack_learn);
  jungtime = learngeneral(object[type], CASE_OBJCACHE, type, jung_learn);
  sumtime = learngeneral(object[type], CASE_OBJCACHE, type, sum_learn);
#if CASE_VERBOSE && CASE_XVERBOSE
      printf("type%ld times     core=%ld filt=%ld fold=%ld gene=%ld jack=%ld jung=%ld sum=%ld\n", type, coretime, filttime, foldtime, genetime, jacktime, jungtime, sumtime);
#endif
}

long learngeneral(case_obj_t obj[], long objsz, long type, learn_f learnfunc)
{
  long time;
  timer_start(0);
  learnfunc(obj, objsz, type);
  time = timer_stop();
  return time;
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

void case_printstat(long type)
{
  double fmeasure;
  double precision;
  double recall;
  fmeasure = case_stat_fmeasure(&stat[type]);
  precision = case_stat_precision(&stat[type]);
  recall = case_stat_recall(&stat[type]);
  printf("type%ld stats     fmeasure=%0.3f precision=%0.3f recall=%0.3f\n", type, fmeasure, precision, recall);
}

long randomscoreindx(long exclude)
{
  double r;
  do {
    r = random() % SCORE;
  } while (r == exclude);
  return r;
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

void setvaltypes(char csvobj[CASE_CSVOBJ], long classindx, long type)
{
  char csvobjcopy[CASE_CSVOBJ];
  char *tok;
  long csvindx = 0;
  long valindx;
  strncpy(csvobjcopy, csvobj, CASE_CSVOBJ - 1);
  tok = strtok(csvobjcopy, STRTOK);
  valindx = reorderindx(csvindx, classindx);
  valtype[type][valindx] = isnum(tok) ? valtype_num : valtype_str;
  while ((tok = strtok(0, STRTOK)) && (csvindx < 31)) {
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
