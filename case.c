#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
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
#include "sum.h"
#include "type.h"
#include "val.h"

#define OBJECTCACHE 64
#define LENSCACHE OBJECTCACHE

static case_obj_t object[32][OBJECTCACHE];
static case_bool_t once = case_bool_false;
static case_obj_t types;

static val_t lens[32][LENSCACHE][32];
static val_t lensfirstobj[32][32];
static case_bool_t isfirstlens = case_bool_true;
static type_t lenstypes[32][32];

/*
 insert into the lens at random
 have different lens methods--ideal object, choose an ideal object every once in a while, first line and then every once in a while in a stochastic manner and you would compare text fields (whether they're equal to the (ideal) object) or not (so that's a 1 or a 0) or whether a number is greater than (which is one) or less than the ideal object (which is 0)
 or instead of picking an ideal object you could store the last n character or numeric fields and see what the most common character value is or average numerical field value is
*/

typedef void (*lens_f)(case_obj_t*);

static long count(long type, case_obj_t typ);
static long countboth(long type, case_obj_t typ1, case_obj_t typ2);
static long counteither(long type, case_obj_t typ1, case_obj_t typ2);
static long countsub(long type, case_obj_t typ1, case_obj_t typ2);
static long countxor(long type, case_obj_t typ1, case_obj_t typ2);
static void initonce();
static case_bool_t isnum(char *text);
static void learn();
static void lensavg(case_obj_t *obj);
static void lensfirst(case_obj_t *obj);
static void lensgeneral(char *csvobj, long classidx, long type, lens_f lensfunc);
static void lensinsert(val_t valobj[32], long type);
static void lensrand(case_obj_t *obj);
static void lensread(char *csvobj, long classidx, val_t valobj[32]);
static void lensreadfield(char *text, val_t *val);
static void notetype(long type);

case_bit_t case_classify(case_obj_t obj, long type)
{
  case_bit_t class;
  case_bit_t coreclass = 0;
  case_bit_t filtclass = 0;
  case_bit_t foldclass = 0;
  case_bit_t geneclass = 0;
  case_bit_t jackclass = 0;
  case_bit_t jungclass = 0;
  case_bit_t sumclass = 0;
  long onecount;
  long zerocount;
  initonce();
  notetype(type);
  /* coreclass = core_classify(obj, type); */
  filtclass = filt_classify(obj, type);
  foldclass = fold_classify(obj, type);
  /* geneclass = gene_classify(obj, type); */
  /* jackclass = jack_classify(obj, type); */
  /* jungclass = jung_classify(obj, type); */
  sumclass = sum_classify(obj, type);
  onecount = filtclass + foldclass + sumclass;
  zerocount = 3 - onecount;
  class = (zerocount >= onecount) ? 0 : 1;
#if CASE_VERBOSE && CASE_XVERBOSE
  printf("type%ld class     core=%d filt=%d fold=%d gene=%d jack=%d jung=%d sum=%d\n", type, coreclass, filtclass, foldclass, geneclass, jackclass, jungclass, sumclass);
#endif
  return class;
}

double case_indifreq(case_obj_t indicator, case_obj_t target, long type)
{
  long indicnt;
  long targcnt;
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
  indisubcnt = countsub(indicator, target, type);
  bothcnt = countboth(indicator, target, type);
  if (0 == bothcnt)
    bothcnt = 1;
  return (long) indisubcnt / bothcnt;
}

double case_indiover(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long indicnt;
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
  bothcnt = countboth(indicator, target, type);
  indisubcnt = countsub(indicator, target, type);
  if (0 == indisubcnt)
    indisubcnt = 1;
  return (long) bothcnt / indisubcnt;
}

void case_lensavg(char *csvobj, long classidx, long type)
{
  lensgeneral(csvobj, classidx, type, lensavg);
}

void case_lensfirst(char *csvobj, long classidx, long type)
{
  lensgeneral(csvobj, classidx, type, lensfirst);
}

void case_lensrand(char *csvobj, long classidx, long type)
{
  lensgeneral(csvobj, classidx, type, lensrand);
}

void case_observe(case_obj_t obj, long type)
{
  long i;
  initonce();
  notetype(type);
  i = random() % OBJECTCACHE;
  object[type][i] = obj;
  if (die_toss(OBJECTCACHE / 16))
    learn();
}

double case_over(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long eithercnt;
  bothcnt = countboth(indicator, target, type);
  eithercnt = counteither(indicator, target, type);
  if (0 == eithercnt)
    eithercnt = 1;
  return (long) bothcnt / eithercnt;
}

double case_targfreq(case_obj_t indicator, case_obj_t target, long type)
{
  long targcnt;
  long indicnt;
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
  bothcnt = countboth(indicator, target, type);
  xorcnt = countxor(indicator, target, type);
  if (0 == xorcnt)
    xorcnt = 1;
  return (long) bothcnt / xorcnt;
}

long count(long type, case_obj_t typ)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < OBJECTCACHE; i++) {
    obj = object[type][i];
    count += case_obj_hastype(obj, typ);
  }
  return count;
}

long countboth(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < OBJECTCACHE; i++) {
    obj = object[type][i];
    count += (case_obj_hastype(obj, typ1) && case_obj_hastype(obj, typ2));
  }
  return count;
}

long counteither(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < OBJECTCACHE; i++) {
    obj = object[type][i];
    count += (case_obj_hastype(obj, typ1) || case_obj_hastype(obj, typ2));
  }
  return count;
}

long countsub(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < OBJECTCACHE; i++) {
    obj = object[type][i];
    count += (case_obj_hastype(obj, typ1) && !case_obj_hastype(obj, typ2));
  }
  return count;
}

long countxor(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long i;
  case_bit_t has1;
  case_bit_t has2;
  for (i = 0; i < OBJECTCACHE; i++) {
    obj = object[type][i];
    has1 = case_obj_hastype(obj, typ1);
    has2 = case_obj_hastype(obj, typ2);
    if (has1 ^ has2)
      count++;
  }
  return count;
}

void initonce()
{
  long i;
  long type;
  long attr;
  case_obj_clear(&types);
  if (!once) {
    for (type = 0; type < 32; type++) {
      for (i = 0; i < OBJECTCACHE; i++) {
        case_obj_randomize(&object[type][i]);
        for (attr = 0; attr < 32; attr++)
          val_init(&lens[type][i][attr]);
      }
      for (attr = 0; attr < 32; attr++) {
        val_init(&lensfirstobj[type][attr]);
        lenstypes[type][attr] = type_str;
      }
    }
    once = case_bool_true;
  }
}

case_bool_t isnum(char *text)
{
  long textlen;
  long i;
  char c;
  case_bool_t is = case_bool_true;
  textlen = strlen(text);
  for (i = 0; i < textlen; i++) {
    c = *(text + i);
    if (!(('-' == c) || ('.' == c) || isdigit(c))) {
      is = case_bool_false;
      break;
    }
  }
  return is;
}

void learn()
{
  long type;
  struct timeval tv1;
  struct timeval tv2;
  long coretime;
  long filttime;
  long foldtime;
  long genetime;
  long jacktime;
  long jungtime;
  long sumtime;
  for (type = 0; type < 32; type++)
    if (case_obj_getattr(types, type)) {
/*
      gettimeofday(&tv1, NULL);
      core_learn(object[type], OBJECTCACHE, type);
      gettimeofday(&tv2, NULL);
      coretime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
      gettimeofday(&tv1, NULL);
      filt_learn(object[type], OBJECTCACHE, type);
      gettimeofday(&tv2, NULL);
      filttime = tv2.tv_usec - tv1.tv_usec;
      ;
      gettimeofday(&tv1, NULL);
      fold_learn(object[type], OBJECTCACHE, type);
      gettimeofday(&tv2, NULL);
      foldtime = tv2.tv_usec - tv1.tv_usec;
      ;
/*
      gettimeofday(&tv1, NULL);
      gene_learn(object[type], OBJECTCACHE, type);
      gettimeofday(&tv2, NULL);
      genetime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
/*
      gettimeofday(&tv1, NULL);
      jack_learn(object[type], OBJECTCACHE, type);
      gettimeofday(&tv2, NULL);
      jacktime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
/*
      gettimeofday(&tv1, NULL);
      jung_learn(object[type], OBJECTCACHE, type);
      gettimeofday(&tv2, NULL);
      jungtime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
      gettimeofday(&tv1, NULL);
      sum_learn(object[type], OBJECTCACHE, type);
      gettimeofday(&tv2, NULL);
      sumtime = tv2.tv_usec - tv1.tv_usec;
#if CASE_VERBOSE && CASE_XVERBOSE
      printf("type%ld times     core=%ld filt=%ld fold=%ld gene=%ld jack=%ld jung=%ld sum=%ld\n", type, coretime, filttime, foldtime, genetime, jacktime, jungtime, sumtime);
#endif
    }
}

void lensavg(case_obj_t *obj)
{
}

void lensfirst(case_obj_t *obj)
{
}

void lensgeneral(char *csvobj, long classidx, long type, lens_f lensfunc)
{
  case_obj_t obj;
  val_t valobj[32];
  initonce();
  case_obj_randomize(&obj);
  lensread(csvobj, classidx, valobj);
  lensinsert(valobj, type);
  lensfunc(&obj);
  case_observe(obj, type);
}

void lensinsert(val_t valobj[32], long type)
{
  long obj;
  long field;
  obj = random() % LENSCACHE;
  for (field = 0; field < 32; field++)
    val_copy(&valobj[field], &lens[type][obj][field]);
}

void lensrand(case_obj_t *obj)
{
}

void lensread(char *csvobj, long classidx, val_t valobj[32])
{
  char *tok;
  long csvidx = 0;
  long validx = 0;
  tok = strtok(csvobj, ",\n");
  validx = (classidx == csvidx) ? 0 : 1;
  lensreadfield(tok, &valobj[validx]);
  while (tok = strtok(NULL, ",\n")) {
    csvidx++;
    validx = (classidx == csvidx) ? 0 : ++validx;
    lensreadfield(tok, &valobj[validx]);
  }
}

void lensreadfield(char *text, val_t *val)
{
  if (isnum(text)) {
    val->num = strtod(text, NULL);
  } else {
    memcpy(val->str, text, VAL_STRSZ);
  }
}

void notetype(long type)
{
  case_obj_setattr(&types, type, 1);
}
