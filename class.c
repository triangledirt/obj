#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "class.h"
#include "coin.h"
#include "die.h"
#include "filt.h"
#include "fold.h"
#include "gene.h"
#include "sum.h"
#include "timer.h"
#include "val.h"
#include "valtype.h"
#include "xdouble.h"

#define PACKCACHE (OBJ_CLASS_CACHE / 2)
#define SCORE 4
#define STRTOK ",;\n"

static obj_t object[OBJ_TYPE][OBJ_CLASS_CACHE];
static enum obj_bool_t once = obj_bool_false;
static struct obj_classstat_t stat[OBJ_TYPE];
static struct obj_xdouble_t scorepast[OBJ_TYPE][SCORE];

static union obj_val_t value[OBJ_TYPE][PACKCACHE][OBJ];
static union obj_val_t firstval[OBJ_TYPE][OBJ];
static enum obj_valtype_t valtype[OBJ_TYPE][OBJ];
static enum obj_bool_t firstpack[OBJ_TYPE];

typedef double (*getfit_f)(long type);
static getfit_f fitfunc[SCORE] = {obj_filt_fit, obj_fold_fit, obj_gene_fit, obj_sum_fit};

typedef double (*score_f)(obj_t obj, long type);
static score_f scorefunc[SCORE] = {obj_filt_score, obj_fold_score, obj_gene_score, obj_sum_score};
static char *scorename[SCORE] = {"filt", "fold", "gene", "sum"};

static long favscoreindex[OBJ_TYPE];
static long scorefuncoverride = -1;

typedef void (*learn_f)(obj_t[], long, long);
static void learn(long type);
static long learngeneral(obj_t obj[], long objsz, long type, learn_f learnfunc);
static void init();
static long count(obj_t objtype, long type);
static long countboth(obj_t objtype1, obj_t objtype2, long type);
static long counteither(obj_t objtype1, obj_t objtype2, long type);
static long countsub(obj_t objtype1, obj_t objtype2, long type);
static long countxor(obj_t objtype1, obj_t objtype2, long type);

typedef obj_bit_t (*pack_f)(union obj_val_t *, long, long);
static obj_t packgeneral(char csvobj[OBJ_CSV], long classindex, long type, pack_f packfunc);
static obj_bit_t packavg(union obj_val_t *val, long attr, long type);
static obj_bit_t packavgnum(union obj_val_t *val, long attr, long type);
static obj_bit_t packavgstr(union obj_val_t *val, long attr, long type);
static obj_bit_t packfirst(union obj_val_t *val, long attr, long type);
static obj_bit_t packrand(union obj_val_t *val, long attr, long type);
static void setvaltypes(char csvobj[OBJ_CSV], long classindex, long type);
static void insertfirstval(union obj_val_t valobj[OBJ], long type);
static void insertval(union obj_val_t valobj[OBJ], long type);
static void csv2valobj(char csvobj[OBJ_CSV], long classindex, union obj_val_t valobj[OBJ], long type);
static void text2val(char *text, union obj_val_t *val, long valindex, long type);
static enum obj_bool_t isnum(char *str);
static long reorderindex(long attrindex, long classindex);

static long randomscoreindex(long exclude);

long count(obj_t objtype, long type)
{
  long cnt = 0;
  obj_t obj;
  long i;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    cnt += obj_hastype(obj, objtype);
  }
  return cnt;
}

long countboth(obj_t objtype1, obj_t objtype2, long type)
{
  long cnt = 0;
  obj_t obj;
  long i;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    cnt += obj_hastype(obj, objtype1) && obj_hastype(obj, objtype2);
  }
  return cnt;
}

long counteither(obj_t objtype1, obj_t objtype2, long type)
{
  long cnt = 0;
  obj_t obj;
  long i;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    cnt += obj_hastype(obj, objtype1) || obj_hastype(obj, objtype2);
  }
  return cnt;
}

long countsub(obj_t objtype1, obj_t objtype2, long type)
{
  long cnt = 0;
  obj_t obj;
  long i;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    cnt += obj_hastype(obj, objtype1) && !obj_hastype(obj, objtype2);
  }
  return cnt;
}

long countxor(obj_t objtype1, obj_t objtype2, long type)
{
  long cnt = 0;
  obj_t obj;
  long i;
  obj_bit_t has1;
  obj_bit_t has2;
  for (i = 0; i < OBJ_CLASS_CACHE; i++) {
    obj = object[type][i];
    has1 = obj_hastype(obj, objtype1);
    has2 = obj_hastype(obj, objtype2);
    if (has1 ^ has2)
      cnt++;
  }
  return cnt;
}

void csv2valobj(char csvobj[OBJ_CSV], long classindex, union obj_val_t valobj[OBJ], long type)
{
  char csvobjcopy[OBJ_CSV];
  char *tok;
  long csvindex = 0;
  long valindex;
  strncpy(csvobjcopy, csvobj, OBJ_CSV - 1);
  tok = strtok(csvobjcopy, STRTOK);
  valindex = reorderindex(csvindex, classindex);
  text2val(tok, &valobj[valindex], valindex, type);
  while ((tok = strtok(NULL, STRTOK)) && (csvindex < (OBJ - 1))) {
    csvindex++;
    valindex = reorderindex(csvindex, classindex);
    text2val(tok, &valobj[valindex], valindex, type);
  }
  for (valindex = csvindex + 1; valindex < OBJ; valindex++)
    obj_val_init(&valobj[valindex], valtype[type][valindex]);
#if 0 && OBJ_VERBOSE && OBJ_XVERBOSE
  printf("type%02ld   csv     %s", type, csvobj);
  printf("type%02ld   val     ", type);
  for (valindex = 0; valindex < OBJ; valindex++) {
    val_print(&valobj[valindex], valtype[type][valindex]);
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
    for (type = 0; type < OBJ_TYPE; type++) {
      for (i = 0; i < OBJ_CLASS_CACHE; i++)
        obj_randomize(&object[type][i]);
      for (score = 0; score < SCORE; score++)
        obj_xdouble_init(&scorepast[type][score]);
      obj_classstat_reset(&stat[type]);
      favscoreindex[type] = (scorefuncoverride < 0) ? random() % SCORE : scorefuncoverride;
      firstpack[type] = obj_bool_true;
    }
    once = obj_bool_true;
  }
}

void insertfirstval(union obj_val_t valobj[OBJ], long type)
{
  long attr;
  for (attr = 0; attr < OBJ; attr++)
    obj_val_copy(&valobj[attr], &firstval[type][attr], valtype[type][attr]);
}

void insertval(union obj_val_t valobj[OBJ], long type)
{
  long obj;
  long attr;
  obj = random() % PACKCACHE;
  for (attr = 0; attr < OBJ; attr++)
    obj_val_copy(&valobj[attr], &value[type][obj][attr], valtype[type][attr]);
}

enum obj_bool_t isnum(char *str)
{
  long len;
  long i;
  char c;
  enum obj_bool_t is = obj_bool_true;
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
  long filttime;
  long foldtime;
  long genetime;
  long sumtime;
  filttime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_filt_learn);
  foldtime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_fold_learn);
  genetime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_gene_learn);
  sumtime = learngeneral(object[type], OBJ_CLASS_CACHE, type, obj_sum_learn);
#if OBJ_VERBOSE && OBJ_XVERBOSE
  printf("type%02ld times     filt=%ld fold=%ld gene=%ld sum=%ld\n", type, filttime, foldtime, genetime, sumtime);
#endif
}

long learngeneral(obj_t obj[], long objsz, long type, learn_f learnfunc)
{
  obj_timer_start(0);
  learnfunc(obj, objsz, type);
  return obj_timer_stop();
}

obj_bit_t obj_class_classify(obj_t obj, long type)
{
  obj_bit_t class;
  double score;
  double thresh;
  double fit;
  double altfit;
  long scorefindex;
  long altfitfindex;
  score_f scoref;
  getfit_f getfitf;
  getfit_f altgetfitf;
  char *scorefname;
  char c;
  init();
  obj_obscureclass(&obj);
  scorefindex = favscoreindex[type];
  if ((scorefuncoverride < 0) && obj_die_toss(OBJ_CLASS_CACHE / 2)) {
    getfitf = fitfunc[scorefindex];
    fit = getfitf(type);
    altfitfindex = randomscoreindex(scorefindex);
    altgetfitf = fitfunc[altfitfindex];
    altfit = altgetfitf(type);
    if ((altfit - fit) > 0.2) {
#if OBJ_VERBOSE && OBJ_XVERBOSE
      printf("type%02ld class     switching algo from %s %0.3f >> %s %0.3f\n", type, scorename[scorefindex], fit, scorename[altfitfindex], altfit);
#endif
      favscoreindex[type] = altfitfindex;
      scorefindex = altfitfindex;
    }
  }
  scoref = scorefunc[scorefindex];
  score = scoref(obj, type);
  thresh = obj_xdouble_avg(&scorepast[type][scorefindex]);
  obj_xdouble_note(&scorepast[type][scorefindex], score);
  class = (score > thresh) ? 1 : 0;
#if OBJ_VERBOSE && OBJ_XVERBOSE
  c = obj_bit_char(class);
  printf("type%02ld class     class=%c scorefunc=%s score=%0.3f thresh=%0.3f\n", type, c, scorename[scorefindex], score, thresh);
#endif
  return class;
}

obj_bit_t obj_class_classifyknown(obj_t obj, long type)
{
  obj_bit_t guessclass;
  obj_bit_t actualclass;
  guessclass = obj_class_classify(obj, type);
  actualclass = obj_class(obj);
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

obj_t obj_class_packavg(char csvobj[OBJ_CSV], long classindex, long type)
{
  init();
  return packgeneral(csvobj, classindex, type, packavg);
}

obj_t obj_class_packfirst(char csvobj[OBJ_CSV], long classindex, long type)
{
  init();
  return packgeneral(csvobj, classindex, type, packfirst);
}

obj_t obj_class_packrand(char csvobj[OBJ_CSV], long classindex, long type)
{
  init();
  return packgeneral(csvobj, classindex, type, packrand);
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

obj_bit_t packavg(union obj_val_t *val, long attr, long type)
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

obj_bit_t packavgnum(union obj_val_t *val, long attr, long type)
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

obj_bit_t packavgstr(union obj_val_t *val, long attr, long type)
{
  long samplesz = PACKCACHE / 8;
  long sampleobj[samplesz];
  long samplecnt[samplesz];
  long i;
  long j;
  long k;
  long max = 0;
  long maxindex = 0;
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
      maxindex = sampleobj[j];
    }
  return 0 == strncmp(val->str, value[type][maxindex][attr].str, OBJ_CLASS_STR - 1);
}

obj_bit_t packfirst(union obj_val_t *val, long attr, long type)
{
  return 0 == obj_val_compare(val, &firstval[type][attr], valtype[type][attr]);
}

obj_t packgeneral(char csvobj[OBJ_CSV], long classindex, long type, pack_f packfunc)
{
  obj_t obj;
  union obj_val_t valobj[OBJ];
  long attr;
  obj_bit_t bit;
  if (firstpack[type])
    setvaltypes(csvobj, classindex, type);
  csv2valobj(csvobj, classindex, valobj, type);
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

obj_bit_t packrand(union obj_val_t *val, long attr, long type)
{
  long i;
  i = random() % PACKCACHE;
  return 0 == obj_val_compare(val, &value[type][i][attr], valtype[type][attr]);
}

long randomscoreindex(long exclude)
{
  long r;
  do {
    r = random() % SCORE;
  } while (r == exclude);
  return r;
}

long reorderindex(long attrindex, long classindex)
{
  long reindex;
  if (attrindex == classindex) {
    reindex = 0;
  } else {
    reindex = (0 == attrindex) ? classindex : attrindex;
  }
  return reindex;
}

void setvaltypes(char csvobj[OBJ_CSV], long classindex, long type)
{
  char csvobjcopy[OBJ_CSV];
  char *tok;
  long csvindex = 0;
  long valindex;
  strncpy(csvobjcopy, csvobj, OBJ_CSV - 1);
  tok = strtok(csvobjcopy, STRTOK);
  valindex = reorderindex(csvindex, classindex);
  valtype[type][valindex] = isnum(tok) ? obj_valtype_num : obj_valtype_str;
  while ((tok = strtok(NULL, STRTOK)) && (csvindex < (OBJ - 1))) {
    csvindex++;
    valindex = reorderindex(csvindex, classindex);
    valtype[type][valindex] = isnum(tok) ? obj_valtype_num : obj_valtype_str;
  }
  for (valindex = csvindex + 1; valindex < OBJ; valindex++)
    valtype[type][valindex] = obj_valtype_str;
#if OBJ_VERBOSE && OBJ_XVERBOSE
  printf("type%02ld types     ", type);
  for (valindex = 0; valindex < OBJ; valindex++)
    printf("%s,", obj_valtype_name(valtype[type][valindex]));
  printf("\n");
#endif
}

void text2val(char *text, union obj_val_t *val, long valindex, long type)
{
  if (obj_valtype_num == valtype[type][valindex]) {
    val->num = strtod(text, 0);
  } else {
    strncpy(val->str, text, OBJ_CLASS_STR - 1);
  }
}
