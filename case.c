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

#define OBJCACHE 64
#define PACKCACHE OBJCACHE

static case_obj_t object[32][OBJCACHE];
static case_bool_t once = case_bool_false;
static case_obj_t types;

static val_t value[32][PACKCACHE][32];
static val_t firstval[32][32];
static type_t valtype[32][32];
static case_bool_t firstpack[32];

/*
  TODO: or whether a number is greater than (which is one) or less than the ideal object or characters come before or after the ideal object
*/

static void learn();
static void initonce();
static void notetype(long type);
static long count(long type, case_obj_t typ);
static long countboth(long type, case_obj_t typ1, case_obj_t typ2);
static long counteither(long type, case_obj_t typ1, case_obj_t typ2);
static long countsub(long type, case_obj_t typ1, case_obj_t typ2);
static long countxor(long type, case_obj_t typ1, case_obj_t typ2);

typedef case_bit_t (*pack_f)(val_t *, long);
static case_obj_t packgeneral(char *csvobj, long classidx, long type, pack_f packfunc);
static case_bit_t packavg(val_t *val, long idx);
static case_bit_t packfirst(val_t *val, long idx);
static case_bit_t packrand(val_t *val, long idx);
static void setvaltypes(char *csvobj, long classidx, long type);
static void insertfirstval(val_t valobj[32], long type);
static void insertval(val_t valobj[32], long type);
static void csv2valobj(char *csvobj, long classidx, val_t valobj[32], long type);
static void text2val(char *text, val_t *val, long validx, long type);
static case_bool_t isnum(char *str);
static long reorderidx(long attridx, long classidx);

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
  initonce();
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
  initonce();
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
  initonce();
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
  initonce();
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
  initonce();
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
  initonce();
  bothcnt = countboth(indicator, target, type);
  indisubcnt = countsub(indicator, target, type);
  if (0 == indisubcnt)
    indisubcnt = 1;
  return (long) bothcnt / indisubcnt;
}

void case_observe(case_obj_t obj, long type)
{
  long i;
  initonce();
  notetype(type);
  i = random() % OBJCACHE;
  object[type][i] = obj;
  if (die_toss(OBJCACHE / 16))
    learn();
}

double case_over(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long eithercnt;
  initonce();
  bothcnt = countboth(indicator, target, type);
  eithercnt = counteither(indicator, target, type);
  if (0 == eithercnt)
    eithercnt = 1;
  return (long) bothcnt / eithercnt;
}

case_obj_t case_packavg(char *csvobj, long classidx, long type)
{
  initonce();
  return packgeneral(csvobj, type, classidx, packavg);
}

case_obj_t case_packfirst(char *csvobj, long classidx, long type)
{
  initonce();
  packgeneral(csvobj, type, classidx, packfirst);
}

case_obj_t case_packrand(char *csvobj, long classidx, long type)
{
  initonce();
  packgeneral(csvobj, type, classidx, packrand);
}

double case_targfreq(case_obj_t indicator, case_obj_t target, long type)
{
  long targcnt;
  long indicnt;
  initonce();
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
  initonce();
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
  initonce();
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
  initonce();
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
  initonce();
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
  initonce();
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
  initonce();
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
  for (i = 0; i < OBJCACHE; i++) {
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
  for (i = 0; i < OBJCACHE; i++) {
    obj = object[type][i];
    count += case_obj_hastype(obj, typ1) && case_obj_hastype(obj, typ2);
  }
  return count;
}

long counteither(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < OBJCACHE; i++) {
    obj = object[type][i];
    count += case_obj_hastype(obj, typ1) || case_obj_hastype(obj, typ2);
  }
  return count;
}

long countsub(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < OBJCACHE; i++) {
    obj = object[type][i];
    count += case_obj_hastype(obj, typ1) && !case_obj_hastype(obj, typ2);
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
  for (i = 0; i < OBJCACHE; i++) {
    obj = object[type][i];
    has1 = case_obj_hastype(obj, typ1);
    has2 = case_obj_hastype(obj, typ2);
    if (has1 ^ has2)
      count++;
  }
  return count;
}

void csv2valobj(char *csvobj, long classidx, val_t valobj[32], long type)
{
  char *tok;
  long csvidx = 0;
  long validx;
  tok = strtok(csvobj, ",\n");
  validx = reorderidx(csvidx, classidx);
  text2val(tok, &valobj[validx], validx, type);
  while ((tok = strtok(NULL, ",\n")) && (csvidx < 31)) {
    csvidx++;
    validx = reorderidx(csvidx, classidx);
    text2val(tok, &valobj[validx], validx, type);
  }
}

void initonce()
{
  long i;
  long type;
  long attr;
  if (!once) {
    case_obj_clear(&types);
    for (type = 0; type < 32; type++) {
      for (i = 0; i < OBJCACHE; i++) {
        case_obj_randomize(&object[type][i]);
        for (attr = 0; attr < 32; attr++)
          val_init(&value[type][i][attr]);
      }
      for (attr = 0; attr < 32; attr++) {
        val_init(&firstval[type][attr]);
        valtype[type][attr] = type_str;
      }
      firstpack[type] = case_bool_true;
    }
    once = case_bool_true;
  }
}

void insertfirstval(val_t valobj[32], long type)
{
  long obj;
  long attr;
  obj = random() % PACKCACHE;
  for (attr = 0; attr < 32; attr++)
    val_copy(&valobj[attr], &firstval[type][attr]);
}

void insertval(val_t valobj[32], long type)
{
  long obj;
  long attr;
  obj = random() % PACKCACHE;
  for (attr = 0; attr < 32; attr++)
    val_copy(&valobj[attr], &value[type][obj][attr]);
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
      core_learn(object[type], OBJCACHE, type);
      gettimeofday(&tv2, NULL);
      coretime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
      gettimeofday(&tv1, NULL);
      filt_learn(object[type], OBJCACHE, type);
      gettimeofday(&tv2, NULL);
      filttime = tv2.tv_usec - tv1.tv_usec;
      ;
      gettimeofday(&tv1, NULL);
      fold_learn(object[type], OBJCACHE, type);
      gettimeofday(&tv2, NULL);
      foldtime = tv2.tv_usec - tv1.tv_usec;
      ;
/*
      gettimeofday(&tv1, NULL);
      gene_learn(object[type], OBJCACHE, type);
      gettimeofday(&tv2, NULL);
      genetime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
/*
      gettimeofday(&tv1, NULL);
      jack_learn(object[type], OBJCACHE, type);
      gettimeofday(&tv2, NULL);
      jacktime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
/*
      gettimeofday(&tv1, NULL);
      jung_learn(object[type], OBJCACHE, type);
      gettimeofday(&tv2, NULL);
      jungtime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
      gettimeofday(&tv1, NULL);
      sum_learn(object[type], OBJCACHE, type);
      gettimeofday(&tv2, NULL);
      sumtime = tv2.tv_usec - tv1.tv_usec;
#if CASE_VERBOSE && CASE_XVERBOSE
      printf("type%ld times     core=%ld filt=%ld fold=%ld gene=%ld jack=%ld jung=%ld sum=%ld\n", type, coretime, filttime, foldtime, genetime, jacktime, jungtime, sumtime);
#endif
    }
}

void notetype(long type)
{
  case_obj_setattr(&types, type, 1);
}

case_bit_t packavg(val_t *val, long idx)
{
  /* TODO: implement */
}

case_bit_t packfirst(val_t *val, long idx)
{
  /* TODO: implement */
}

case_obj_t packgeneral(char *csvobj, long classidx, long type, pack_f packfunc)
{
  case_obj_t obj;
  val_t valobj[32];
  long attr;
  case_bit_t bit;
  if (firstpack[type]) {
    setvaltypes(csvobj, classidx, type);
  }
  csv2valobj(csvobj, classidx, valobj, type);
  if (firstpack[type]) {
    insertfirstval(valobj, type);
    firstpack[type] = case_bool_false;
  }
  for (attr = 0; attr < 32; attr++) {
    bit = packfunc(&valobj[attr], attr);
    case_obj_setattr(&obj, attr, bit);
  }
  insertval(valobj, type);
  return obj;
}

case_bit_t packrand(val_t *val, long idx)
{
  /* TODO: implement */
}

long reorderidx(long attridx, long classidx)
{
  long reidx;
  if (0 == classidx) {
    reidx = attridx;
  } else {
    reidx = (attridx == classidx) ? 0 : attridx + 1;
  }
  return reidx;
}

void setvaltypes(char *csvobj, long classidx, long type)
{
  char *tok;
  long csvidx = 0;
  long validx;
  tok = strtok(csvobj, ",\n");
  validx = reorderidx(csvidx, classidx);
  valtype[type][validx] = (isnum(tok)) ? type_num : type_str;
  while ((tok = strtok(NULL, ",\n")) && (csvidx < 31)) {
    csvidx++;
    validx = reorderidx(csvidx, classidx);
    valtype[type][validx] = (isnum(tok)) ? type_num : type_str;
  }
}

void text2val(char *text, val_t *val, long validx, long type)
{
  if (type_num == valtype[type][validx]) {
    val->num = strtod(text, NULL);
  } else {
    memcpy(val->str, text, VAL_STRSZ);
  }
}
