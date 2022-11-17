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
#include "moire.h"
#include "sum.h"
#include "timer.h"
#include "val.h"
#include "valtype.h"
#include "xdouble.h"

#define PACKCACHE (CASE_CACHE / 2)
#define SCORE 8
#define STRTOK ",;\n"

static case_obj_t object[CASE_TYPE][CASE_CACHE];
static case_bool_t once = case_bool_false;
static case_stat_t stat[CASE_TYPE];
static xdouble_t scorepast[CASE_TYPE][SCORE];

static val_t value[CASE_TYPE][PACKCACHE][CASE_OBJ];
static val_t firstval[CASE_TYPE][CASE_OBJ];
static valtype_t valtype[CASE_TYPE][CASE_OBJ];
static case_bool_t firstpack[CASE_TYPE];

typedef double (*score_f)(case_obj_t, long);
static score_f scorefunc[SCORE] = {core_score, filt_score, fold_score, gene_score, jack_score, jung_score, moire_score, sum_score};
static char *scorename[SCORE] = {"core", "filt", "fold", "gene", "jack", "jung", "moire", "sum"};
static long favscoreindx[CASE_TYPE];
static long scorefuncoverride = 6;

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
static void insertfirstval(val_t valobj[CASE_OBJ], long type);
static void insertval(val_t valobj[CASE_OBJ], long type);
static void csv2valobj(char csvobj[CASE_CSVOBJ], long classindx, val_t valobj[CASE_OBJ], long type);
static void text2val(char *text, val_t *val, long valindx, long type);
static case_bool_t isnum(char *str);
static long reorderindx(long attrindx, long classindx);

static long randomscoreindx(long exclude);

case_bit_t case_classify(case_obj_t obj, long type)
{
  case_bit_t class;
  double score;
  double rescore;
  double thresh;
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
  if ((scorefuncoverride < 0) && die_toss(CASE_CACHE / 2)) {
    rescorefindx = randomscoreindx(scorefindx);
    rescoref = scorefunc[rescorefindx];
    rescore = rescoref(obj, type);
    if ((rescore - score) > 0.2) {
#if CASE_VERBOSE && CASE_XVERBOSE
      scorefname = scorename[scorefindx];
      rescorefname = scorename[rescorefindx];
      printf("type%02ld class     switching algo from %s %0.3f >> %s %0.3f\n", type, scorefname, score, rescorefname, rescore);
#endif
      favscoreindx[type] = rescorefindx;
      score = rescore;
      scorefindx = rescorefindx;
      scoref = scorefunc[scorefindx];
    }
  }
  thresh = xdouble_avg(&scorepast[type][scorefindx]);
  xdouble_note(&scorepast[type][scorefindx], score);
  class = (score > thresh) ? 1 : 0;
#if CASE_VERBOSE && CASE_XVERBOSE
  c = case_bit_char(class);
  scorefname = scorename[scorefindx];
  printf("type%02ld class     class=%c scorefunc=%s score=%0.3f thresh=%0.3f\n", type, c, scorefname, score, thresh);
#endif
  return class;
}

case_bit_t case_classifyknown(case_obj_t obj, long type)
{
  case_bit_t guessclass;
  case_bit_t actualclass;
  guessclass = case_classify(obj, type);
  actualclass = case_obj_getclass(obj);
  case_stat_note(&stat[type], guessclass, actualclass);
  return guessclass;
}

double case_indifreq(case_obj_t indicator, case_obj_t target, long type)
{
  long indicnt;
  long targcnt;
  init();
  indicnt = count(indicator, type);
  targcnt = count(target, type);
  return (double) indicnt / 1 + targcnt;
}

double case_indiimp(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long targcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  targcnt = count(target, type);
  return (double) indisubcnt / 1 + targcnt;
}

double case_indimis(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long targsubcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  targsubcnt = countsub(target, indicator, type);
  return (double) indisubcnt / 1 + targsubcnt;
}

double case_indiopac(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long bothcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  bothcnt = countboth(indicator, target, type);
  return (double) indisubcnt / 1 + bothcnt;
}

double case_indiover(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long indicnt;
  init();
  bothcnt = countboth(indicator, target, type);
  indicnt = count(indicator, type);
  return (double) bothcnt / 1 + indicnt;
}

double case_inditrans(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long indisubcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  indisubcnt = countsub(indicator, target, type);
  return (double) bothcnt / 1 + indisubcnt;
}

void case_observe(case_obj_t obj, long type)
{
  long i;
  init();
  i = random() % CASE_CACHE;
  object[type][i] = obj;
  if (die_toss(CASE_CACHE / 8))
    learn(type);
}

double case_over(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long eithercnt;
  init();
  bothcnt = countboth(indicator, target, type);
  eithercnt = counteither(indicator, target, type);
  return (double) bothcnt / 1 + eithercnt;
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
  return (double) targcnt / 1 + indicnt;
}

double case_targimp(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long indicnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  indicnt = count(indicator, type);
  return (double) targsubcnt / 1 + indicnt;
}

double case_targmis(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long indisubcnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  indisubcnt = countsub(indicator, target, type);
  return (double) targsubcnt / 1 + indisubcnt;
}

double case_targopac(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long bothcnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  bothcnt = countboth(indicator, target, type);
  return (double) targsubcnt / 1 + bothcnt;
}

double case_targover(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long targcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  targcnt = count(target, type);
  return (double) bothcnt / 1 + targcnt;
}

double case_targtrans(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long targsubcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  targsubcnt = countsub(target, indicator, type);
  return (double) bothcnt / 1 + targsubcnt;
}

double case_trans(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long xorcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  xorcnt = countxor(indicator, target, type);
  return (double) bothcnt / 1 + xorcnt;
}

long count(case_obj_t objtype, long type)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < CASE_CACHE; i++) {
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
  for (i = 0; i < CASE_CACHE; i++) {
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
  for (i = 0; i < CASE_CACHE; i++) {
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
  for (i = 0; i < CASE_CACHE; i++) {
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
  for (i = 0; i < CASE_CACHE; i++) {
    obj = object[type][i];
    has1 = case_obj_hastype(obj, objtype1);
    has2 = case_obj_hastype(obj, objtype2);
    if (has1 ^ has2)
      count++;
  }
  return count;
}

void csv2valobj(char csvobj[CASE_CSVOBJ], long classindx, val_t valobj[CASE_OBJ], long type)
{
  char csvobjcopy[CASE_CSVOBJ];
  char *tok;
  long csvindx = 0;
  long valindx;
  strncpy(csvobjcopy, csvobj, CASE_CSVOBJ - 1);
  tok = strtok(csvobjcopy, STRTOK);
  valindx = reorderindx(csvindx, classindx);
  text2val(tok, &valobj[valindx], valindx, type);
  while ((tok = strtok(NULL, STRTOK)) && (csvindx < (CASE_OBJ - 1))) {
    csvindx++;
    valindx = reorderindx(csvindx, classindx);
    text2val(tok, &valobj[valindx], valindx, type);
  }
  for (valindx = csvindx + 1; valindx < CASE_OBJ; valindx++)
    val_init(&valobj[valindx], valtype[type][valindx]);
#if 0 && CASE_VERBOSE && CASE_XVERBOSE
  printf("type%02ld   csv     %s", type, csvobj);
  printf("type%02ld   val     ", type);
  for (valindx = 0; valindx < CASE_OBJ; valindx++) {
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
  long score;
  if (!once) {
    srandom(time(NULL));
    for (type = 0; type < CASE_TYPE; type++) {
      for (i = 0; i < CASE_CACHE; i++)
        case_obj_randomize(&object[type][i]);
      for (score = 0; score < SCORE; score++)
        xdouble_init(&scorepast[type][score]);
      case_stat_reset(&stat[type]);
      favscoreindx[type] = (scorefuncoverride < 0) ? random() % SCORE : scorefuncoverride;
      firstpack[type] = case_bool_true;
    }
    once = case_bool_true;
  }
}

void insertfirstval(val_t valobj[CASE_OBJ], long type)
{
  long attr;
  for (attr = 0; attr < CASE_OBJ; attr++)
    val_copy(&valobj[attr], &firstval[type][attr], valtype[type][attr]);
}

void insertval(val_t valobj[CASE_OBJ], long type)
{
  long obj;
  long attr;
  obj = random() % PACKCACHE;
  for (attr = 0; attr < CASE_OBJ; attr++)
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
  long coretime;
  long filttime;
  long foldtime;
  long genetime;
  long jacktime;
  long jungtime;
  long moiretime;
  long sumtime;
  coretime = learngeneral(object[type], CASE_CACHE, type, core_learn);
  filttime = learngeneral(object[type], CASE_CACHE, type, filt_learn);
  foldtime = learngeneral(object[type], CASE_CACHE, type, fold_learn);
  genetime = learngeneral(object[type], CASE_CACHE, type, gene_learn);
  jacktime = learngeneral(object[type], CASE_CACHE, type, jack_learn);
  jungtime = learngeneral(object[type], CASE_CACHE, type, jung_learn);
  sumtime = learngeneral(object[type], CASE_CACHE, type, sum_learn);
  moiretime = learngeneral(object[type], CASE_CACHE, type, moire_learn);
#if CASE_VERBOSE && CASE_XVERBOSE
  printf("type%02ld times     core=%ld filt=%ld fold=%ld gene=%ld jack=%ld jung=%ld moire=%ld sum=%ld\n", type, coretime, filttime, foldtime, genetime, jacktime, jungtime, moiretime, sumtime);
#endif
}

long learngeneral(case_obj_t obj[], long objsz, long type, learn_f learnfunc)
{
  timer_start(0);
  learnfunc(obj, objsz, type);
  return timer_stop();
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
  if (0.0 == val->num)
    return 0;
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
  if (0 == strlen(val->str))
    return 0;
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
  return 0 == val_compare(val, &firstval[type][attr], valtype[type][attr]);
}

case_obj_t packgeneral(char csvobj[CASE_CSVOBJ], long classindx, long type, pack_f packfunc)
{
  case_obj_t obj;
  val_t valobj[CASE_OBJ];
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
  for (attr = 0; attr < CASE_OBJ; attr++) {
    bit = packfunc(&valobj[attr], attr, type);
    case_obj_setattr(&obj, attr, bit);
  }
  return obj;
}

case_bit_t packrand(val_t *val, long attr, long type)
{
  long i;
  i = random() % PACKCACHE;
  return 0 == val_compare(val, &value[type][i][attr], valtype[type][attr]);
}

long randomscoreindx(long exclude)
{
  long r;
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
  while ((tok = strtok(NULL, STRTOK)) && (csvindx < (CASE_OBJ - 1))) {
    csvindx++;
    valindx = reorderindx(csvindx, classindx);
    valtype[type][valindx] = isnum(tok) ? valtype_num : valtype_str;
  }
  for (valindx = csvindx + 1; valindx < CASE_OBJ; valindx++)
    valtype[type][valindx] = valtype_str;
#if CASE_VERBOSE && CASE_XVERBOSE
  printf("type%02ld types     ", type);
  for (valindx = 0; valindx < CASE_OBJ; valindx++)
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
