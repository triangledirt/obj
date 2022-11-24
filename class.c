#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "class.h"
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

#define PACKCACHE (OBJ_CLASS_CACHE / 2)
#define SCORE 8
#define STRTOK ",;\n"

static obj_t object[OBJ_CLASS_TYPE][OBJ_CLASS_CACHE];
static obj_bool_t once = obj_bool_false;
static struct obj_classstat_t stat[OBJ_CLASS_TYPE];
static struct obj_xdouble_t scorepast[OBJ_CLASS_TYPE][SCORE];

static obj_val_t value[OBJ_CLASS_TYPE][PACKCACHE][OBJ];
static obj_val_t firstval[OBJ_CLASS_TYPE][OBJ];
static obj_valtype_t valtype[OBJ_CLASS_TYPE][OBJ];
static obj_bool_t firstpack[OBJ_CLASS_TYPE];

typedef double (*score_f)(obj_t, long);
static score_f scorefunc[SCORE] = {obj_core_score, obj_filt_score, obj_fold_score, obj_gene_score, obj_jack_score, obj_jung_score, obj_moire_score, obj_sum_score};
static char *scorename[SCORE] = {"core", "filt", "fold", "gene", "jack", "jung", "moire", "sum"};
static long favscoreindx[OBJ_CLASS_TYPE];
static long scorefuncoverride = 6;

typedef void (*learn_f)(obj_t[], long, long);
static void learn(long type);
static long learngeneral(obj_t obj[], long objsz, long type, learn_f learnfunc);
static void init();
static long count(obj_t objtype, long type);
static long countboth(obj_t objtype1, obj_t objtype2, long type);
static long counteither(obj_t objtype1, obj_t objtype2, long type);
static long countsub(obj_t objtype1, obj_t objtype2, long type);
static long countxor(obj_t objtype1, obj_t objtype2, long type);

typedef obj_bit_t (*pack_f)(obj_val_t *, long, long);
static obj_t packgeneral(char csvobj[OBJ_CLASS_CSV], long classindx, long type, pack_f packfunc);
static obj_bit_t packavg(obj_val_t *val, long attr, long type);
static obj_bit_t packavgnum(obj_val_t *val, long attr, long type);
static obj_bit_t packavgstr(obj_val_t *val, long attr, long type);
static obj_bit_t packfirst(obj_val_t *val, long attr, long type);
static obj_bit_t packrand(obj_val_t *val, long attr, long type);
static void setvaltypes(char csvobj[OBJ_CLASS_CSV], long classindx, long type);
static void insertfirstval(obj_val_t valobj[OBJ], long type);
static void insertval(obj_val_t valobj[OBJ], long type);
static void csv2valobj(char csvobj[OBJ_CLASS_CSV], long classindx, obj_val_t valobj[OBJ], long type);
static void text2val(char *text, obj_val_t *val, long valindx, long type);
static obj_bool_t isnum(char *str);
static long reorderindx(long attrindx, long classindx);

static long randomscoreindx(long exclude);

obj_bit_t obj_class_classify(obj_t obj, long type)
{
  obj_bit_t class;
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
  obj_obscureclass(&obj);
  scorefindx = favscoreindx[type];
  scoref = scorefunc[scorefindx];
  score = scoref(obj, type);
  if ((scorefuncoverride < 0) && obj_die_toss(OBJ_CLASS_CACHE / 2)) {
    rescorefindx = randomscoreindx(scorefindx);
    rescoref = scorefunc[rescorefindx];
    rescore = rescoref(obj, type);
    if ((rescore - score) > 0.2) {
#if OBJ_VERBOSE && OBJ_XVERBOSE
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
  thresh = obj_xdouble_avg(&scorepast[type][scorefindx]);
  obj_xdouble_note(&scorepast[type][scorefindx], score);
  class = (score > thresh) ? 1 : 0;
#if OBJ_VERBOSE && OBJ_XVERBOSE
  c = obj_bit_char(class);
  scorefname = scorename[scorefindx];
  printf("type%02ld class     class=%c scorefunc=%s score=%0.3f thresh=%0.3f\n", type, c, scorefname, score, thresh);
#endif
  return class;
}

obj_bit_t obj_class_classifyknown(obj_t obj, long type)
{
  obj_bit_t guessclass;
  obj_bit_t actualclass;
  guessclass = obj_class_classify(obj, type);
  actualclass = obj_getclass(obj);
  obj_classstat_note(&stat[type], guessclass, actualclass);
  return guessclass;
}

double obj_class_indifreq(obj_t indicator, obj_t target, long type)
{
  long indicnt;
  long targcnt;
  init();
  indicnt = count(indicator, type);
  targcnt = count(target, type);
  return (double) indicnt / 1 + targcnt;
}

double obj_class_indiimp(obj_t indicator, obj_t target, long type)
{
  long indisubcnt;
  long targcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  targcnt = count(target, type);
  return (double) indisubcnt / 1 + targcnt;
}

double obj_class_indimis(obj_t indicator, obj_t target, long type)
{
  long indisubcnt;
  long targsubcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  targsubcnt = countsub(target, indicator, type);
  return (double) indisubcnt / 1 + targsubcnt;
}

double obj_class_indiopac(obj_t indicator, obj_t target, long type)
{
  long indisubcnt;
  long bothcnt;
  init();
  indisubcnt = countsub(indicator, target, type);
  bothcnt = countboth(indicator, target, type);
  return (double) indisubcnt / 1 + bothcnt;
}

double obj_class_indiover(obj_t indicator, obj_t target, long type)
{
  long bothcnt;
  long indicnt;
  init();
  bothcnt = countboth(indicator, target, type);
  indicnt = count(indicator, type);
  return (double) bothcnt / 1 + indicnt;
}

double obj_class_inditrans(obj_t indicator, obj_t target, long type)
{
  long bothcnt;
  long indisubcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  indisubcnt = countsub(indicator, target, type);
  return (double) bothcnt / 1 + indisubcnt;
}

void obj_class_observe(obj_t obj, long type)
{
  long i;
  init();
  i = random() % OBJ_CLASS_CACHE;
  object[type][i] = obj;
  if (obj_die_toss(OBJ_CLASS_CACHE / 8))
    learn(type);
}

double obj_class_over(obj_t indicator, obj_t target, long type)
{
  long bothcnt;
  long eithercnt;
  init();
  bothcnt = countboth(indicator, target, type);
  eithercnt = counteither(indicator, target, type);
  return (double) bothcnt / 1 + eithercnt;
}

obj_t obj_class_packavg(char csvobj[OBJ_CLASS_CSV], long classindx, long type)
{
  init();
  return packgeneral(csvobj, classindx, type, packavg);
}

obj_t obj_class_packfirst(char csvobj[OBJ_CLASS_CSV], long classindx, long type)
{
  init();
  return packgeneral(csvobj, classindx, type, packfirst);
}

obj_t obj_class_packrand(char csvobj[OBJ_CLASS_CSV], long classindx, long type)
{
  init();
  return packgeneral(csvobj, classindx, type, packrand);
}

void obj_class_resetstat(long type)
{
  obj_classstat_reset(&stat[type]);
}

struct obj_classstat_t *obj_class_stat(long type)
{
  return &stat[type];
}

double obj_class_targfreq(obj_t indicator, obj_t target, long type)
{
  long targcnt;
  long indicnt;
  init();
  targcnt = count(target, type);
  indicnt = count(indicator, type);
  return (double) targcnt / 1 + indicnt;
}

double obj_class_targimp(obj_t indicator, obj_t target, long type)
{
  long targsubcnt;
  long indicnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  indicnt = count(indicator, type);
  return (double) targsubcnt / 1 + indicnt;
}

double obj_class_targmis(obj_t indicator, obj_t target, long type)
{
  long targsubcnt;
  long indisubcnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  indisubcnt = countsub(indicator, target, type);
  return (double) targsubcnt / 1 + indisubcnt;
}

double obj_class_targopac(obj_t indicator, obj_t target, long type)
{
  long targsubcnt;
  long bothcnt;
  init();
  targsubcnt = countsub(target, indicator, type);
  bothcnt = countboth(indicator, target, type);
  return (double) targsubcnt / 1 + bothcnt;
}

double obj_class_targover(obj_t indicator, obj_t target, long type)
{
  long bothcnt;
  long targcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  targcnt = count(target, type);
  return (double) bothcnt / 1 + targcnt;
}

double obj_class_targtrans(obj_t indicator, obj_t target, long type)
{
  long bothcnt;
  long targsubcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  targsubcnt = countsub(target, indicator, type);
  return (double) bothcnt / 1 + targsubcnt;
}

double obj_class_trans(obj_t indicator, obj_t target, long type)
{
  long bothcnt;
  long xorcnt;
  init();
  bothcnt = countboth(indicator, target, type);
  xorcnt = countxor(indicator, target, type);
  return (double) bothcnt / 1 + xorcnt;
}

long count(obj_t objtype, long type)
{
  long count = 0;
  obj_t obj;
  long i;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    count += obj_hastype(obj, objtype);
  }
  return count;
}

long countboth(obj_t objtype1, obj_t objtype2, long type)
{
  long count = 0;
  obj_t obj;
  long i;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    count += obj_hastype(obj, objtype1) && obj_hastype(obj, objtype2);
  }
  return count;
}

long counteither(obj_t objtype1, obj_t objtype2, long type)
{
  long count = 0;
  obj_t obj;
  long i;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    count += obj_hastype(obj, objtype1) || obj_hastype(obj, objtype2);
  }
  return count;
}

long countsub(obj_t objtype1, obj_t objtype2, long type)
{
  long count = 0;
  obj_t obj;
  long i;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    count += obj_hastype(obj, objtype1) && !obj_hastype(obj, objtype2);
  }
  return count;
}

long countxor(obj_t objtype1, obj_t objtype2, long type)
{
  long count = 0;
  obj_t obj;
  long i;
  obj_bit_t has1;
  obj_bit_t has2;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    has1 = obj_hastype(obj, objtype1);
    has2 = obj_hastype(obj, objtype2);
    if (has1 ^ has2)
      count++;
  }
  return count;
}

void csv2valobj(char csvobj[OBJ_CLASS_CSV], long classindx, obj_val_t valobj[OBJ], long type)
{
  char csvobjcopy[OBJ_CLASS_CSV];
  char *tok;
  long csvindx = 0;
  long valindx;
  strncpy(csvobjcopy, csvobj, OBJ_CLASS_CSV - 1);
  tok = strtok(csvobjcopy, STRTOK);
  valindx = reorderindx(csvindx, classindx);
  text2val(tok, &valobj[valindx], valindx, type);
  while ((tok = strtok(NULL, STRTOK)) && (csvindx < (OBJ - 1))) {
    csvindx++;
    valindx = reorderindx(csvindx, classindx);
    text2val(tok, &valobj[valindx], valindx, type);
  }
  for (valindx = csvindx + 1; valindx < OBJ; valindx++)
    obj_val_init(&valobj[valindx], valtype[type][valindx]);
#if 0 && OBJ_VERBOSE && OBJ_XVERBOSE
  printf("type%02ld   csv     %s", type, csvobj);
  printf("type%02ld   val     ", type);
  for (valindx = 0; valindx < OBJ; valindx++) {
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
    for (type = 0; type < OBJ_CLASS_TYPE; type++) {
      for (i = 0; i < OBJ_CLASS_CACHE; i++)
        obj_randomize(&object[type][i]);
      for (score = 0; score < SCORE; score++)
      obj_xdouble_init(&scorepast[type][score]);
      obj_classstat_reset(&stat[type]);
      favscoreindx[type] = (scorefuncoverride < 0) ? random() % SCORE : scorefuncoverride;
      firstpack[type] = obj_bool_true;
    }
    once = obj_bool_true;
  }
}

void insertfirstval(obj_val_t valobj[OBJ], long type)
{
  long attr;
  for (attr = 0; attr < OBJ; attr++)
    obj_val_copy(&valobj[attr], &firstval[type][attr], valtype[type][attr]);
}

void insertval(obj_val_t valobj[OBJ], long type)
{
  long obj;
  long attr;
  obj = random() % PACKCACHE;
  for (attr = 0; attr < OBJ; attr++)
    obj_val_copy(&valobj[attr], &value[type][obj][attr], valtype[type][attr]);
}

obj_bool_t isnum(char *str)
{
  long len;
  long i;
  char c;
  obj_bool_t is = obj_bool_true;
  len = strlen(str);
  for (i = 0; i < len; i++) {
    c = *(str + i);
    if (!(('+' == c) || ('-' == c) || ('.' == c) || isdigit(c))) {
      is = obj_bool_false;
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
  coretime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_core_learn);
  filttime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_filt_learn);
  foldtime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_fold_learn);
  genetime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_gene_learn);
  jacktime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_jack_learn);
  jungtime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_jung_learn);
  sumtime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_sum_learn);
  moiretime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_moire_learn);
#if OBJ_VERBOSE && OBJ_XVERBOSE
  printf("type%02ld times     core=%ld filt=%ld fold=%ld gene=%ld jack=%ld jung=%ld moire=%ld sum=%ld\n", type, coretime, filttime, foldtime, genetime, jacktime, jungtime, moiretime, sumtime);
#endif
}

long learngeneral(obj_t obj[], long objsz, long type, learn_f learnfunc)
{
  obj_timer_start(0);
  learnfunc(obj, objsz, type);
  return obj_timer_stop();
}

obj_bit_t packavg(obj_val_t *val, long attr, long type)
{
  obj_bit_t bit;
  switch (valtype[type][attr]) {
  case obj_valtype_num:
    bit = packavgnum(val, attr, type);
    break;
  case obj_valtype_str:
    bit = packavgstr(val, attr, type);
    break;
  }
  return bit;
}

obj_bit_t packavgnum(obj_val_t *val, long attr, long type)
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

obj_bit_t packavgstr(obj_val_t *val, long attr, long type)
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
      if (0 == strncmp(value[type][i][attr].str, value[type][k][attr].str, OBJ_CLASS_STR - 1)) {
        samplecnt[j]++;
        break;
      }
    }
  for (j = 0; j < samplesz; j++)
    if (samplecnt[j] > max) {
      max = samplecnt[j];
      maxindx = sampleobj[j];
    }
  return 0 == strncmp(val->str, value[type][maxindx][attr].str, OBJ_CLASS_STR - 1);
}

obj_bit_t packfirst(obj_val_t *val, long attr, long type)
{
  return 0 == obj_val_compare(val, &firstval[type][attr], valtype[type][attr]);
}

obj_t packgeneral(char csvobj[OBJ_CLASS_CSV], long classindx, long type, pack_f packfunc)
{
  obj_t obj;
  obj_val_t valobj[OBJ];
  long attr;
  obj_bit_t bit;
  if (firstpack[type])
    setvaltypes(csvobj, classindx, type);
  csv2valobj(csvobj, classindx, valobj, type);
  if (firstpack[type]) {
    insertfirstval(valobj, type);
    firstpack[type] = obj_bool_false;
  }
  insertval(valobj, type);
  for (attr = 0; attr < OBJ; attr++) {
    bit = packfunc(&valobj[attr], attr, type);
    obj_setattr(&obj, attr, bit);
  }
  return obj;
}

obj_bit_t packrand(obj_val_t *val, long attr, long type)
{
  long i;
  i = random() % PACKCACHE;
  return 0 == obj_val_compare(val, &value[type][i][attr], valtype[type][attr]);
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

void setvaltypes(char csvobj[OBJ_CLASS_CSV], long classindx, long type)
{
  char csvobjcopy[OBJ_CLASS_CSV];
  char *tok;
  long csvindx = 0;
  long valindx;
  strncpy(csvobjcopy, csvobj, OBJ_CLASS_CSV - 1);
  tok = strtok(csvobjcopy, STRTOK);
  valindx = reorderindx(csvindx, classindx);
  valtype[type][valindx] = isnum(tok) ? obj_valtype_num : obj_valtype_str;
  while ((tok = strtok(NULL, STRTOK)) && (csvindx < (OBJ - 1))) {
    csvindx++;
    valindx = reorderindx(csvindx, classindx);
    valtype[type][valindx] = isnum(tok) ? obj_valtype_num : obj_valtype_str;
  }
  for (valindx = csvindx + 1; valindx < OBJ; valindx++)
    valtype[type][valindx] = obj_valtype_str;
#if OBJ_VERBOSE && OBJ_XVERBOSE
  printf("type%02ld types     ", type);
  for (valindx = 0; valindx < OBJ; valindx++)
    printf("%s,", obj_valtype_name(valtype[type][valindx]));
  printf("\n");
#endif
}

void text2val(char *text, obj_val_t *val, long valindx, long type)
{
  if (obj_valtype_num == valtype[type][valindx]) {
    val->num = strtod(text, 0);
  } else {
    strncpy(val->str, text, OBJ_CLASS_STR - 1);
  }
}
