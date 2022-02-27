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
#define LENSCACHE OBJCACHE

static case_obj_t object[32][OBJCACHE];
static case_bool_t once = case_bool_false;
static case_obj_t types;

static val_t csv[32][LENSCACHE][32];
static val_t firstcsvobj[32][32];
static type_t csvtypes[32][32];
static case_bool_t isfirstcsv = case_bool_true;

/*
  TODO: or whether a number is greater than (which is one) or less than the ideal object or characters come before or after the ideal object
*/

static long count(long type, case_obj_t typ);
static long countboth(long type, case_obj_t typ1, case_obj_t typ2);
static long counteither(long type, case_obj_t typ1, case_obj_t typ2);
static long countsub(long type, case_obj_t typ1, case_obj_t typ2);
static long countxor(long type, case_obj_t typ1, case_obj_t typ2);
static void initonce();
static case_bool_t isnum(char *text);
static void learn();
static void notetype(long type);

typedef case_bit_t (*compress_f)(val_t *, long);
static void observecsvgeneral(char *csvobj, long classidx, long type, compress_f compressfunc);
static case_bit_t classifycsvgeneral(char *csvobj, long classidx, long type, compress_f compressfunc);
static case_bit_t compressvalavg(val_t *val, long idx);
static case_bit_t compressvalfirst(val_t *val, long idx);
static case_bit_t compressvalrand(val_t *val, long idx);
static void insertcsv(val_t valobj[32], long type);
static void csv2valobj(char *csvobj, long classidx, val_t valobj[32]);
static void text2val(char *text, val_t *val);
static case_obj_t csv2obj(char *csvobj, long classidx, compress_f compressfunc);

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

case_bit_t case_classifycsvavg(char *csvobj, long classidx, long type)
{
  classifycsvgeneral(csvobj, type, classidx, compressvalavg);
}

case_bit_t case_classifycsvfirst(char *csvobj, long classidx, long type)
{
  classifycsvgeneral(csvobj, type, classidx, compressvalfirst);
}

case_bit_t case_classifycsvrand(char *csvobj, long classidx, long type)
{
  classifycsvgeneral(csvobj, type, classidx, compressvalrand);
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

void case_observecsvavg(char *csvobj, long classidx, long type)
{
  observecsvgeneral(csvobj, classidx, type, compressvalavg);
}

void case_observecsvfirst(char *csvobj, long classidx, long type)
{
  observecsvgeneral(csvobj, classidx, type, compressvalfirst);
}

void case_observecsvrand(char *csvobj, long classidx, long type)
{
  observecsvgeneral(csvobj, classidx, type, compressvalrand);
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

case_bit_t classifycsvgeneral(char *csvobj, long classidx, long type, compress_f compressfunc)
{
  case_obj_t obj;
  initonce();
  obj = csv2obj(csvobj, classidx, compressfunc);
  return case_classify(obj, type);
}

case_bit_t compressvalavg(val_t *val, long idx)
{
}

case_bit_t compressvalfirst(val_t *val, long idx)
{
}

case_bit_t compressvalrand(val_t *val, long idx)
{
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
    count += (case_obj_hastype(obj, typ1) && case_obj_hastype(obj, typ2));
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
    count += (case_obj_hastype(obj, typ1) || case_obj_hastype(obj, typ2));
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
  for (i = 0; i < OBJCACHE; i++) {
    obj = object[type][i];
    has1 = case_obj_hastype(obj, typ1);
    has2 = case_obj_hastype(obj, typ2);
    if (has1 ^ has2)
      count++;
  }
  return count;
}

case_obj_t csv2obj(char *csvobj, long classidx, compress_f compressfunc)
{
  case_obj_t obj;
  val_t valobj[32];
  long field;
  case_bit_t bit;
  csv2valobj(csvobj, classidx, valobj);
  for (field = 0; field < 32; field++) {
    bit = compressfunc(&valobj[field], field);
    case_obj_setattr(&obj, field, bit);
  }
  return obj;
}

void csv2valobj(char *csvobj, long classidx, val_t valobj[32])
{
  char *tok;
  long csvidx = 0;
  long validx = 0;
  tok = strtok(csvobj, ",\n");
  validx = (classidx == csvidx) ? 0 : 1;
  text2val(tok, &valobj[validx]);
  while (tok = strtok(NULL, ",\n")) {
    csvidx++;
    validx = (classidx == csvidx) ? 0 : ++validx;
    text2val(tok, &valobj[validx]);
  }
}

void initonce()
{
  long i;
  long type;
  long attr;
  case_obj_clear(&types);
  if (!once) {
    for (type = 0; type < 32; type++) {
      for (i = 0; i < OBJCACHE; i++) {
        case_obj_randomize(&object[type][i]);
        for (attr = 0; attr < 32; attr++)
          val_init(&csv[type][i][attr]);
      }
      for (attr = 0; attr < 32; attr++) {
        val_init(&firstcsvobj[type][attr]);
        csvtypes[type][attr] = type_str;
      }
    }
    once = case_bool_true;
  }
}

void insertcsv(val_t valobj[32], long type)
{
  long obj;
  long field;
  obj = random() % LENSCACHE;
  for (field = 0; field < 32; field++)
    val_copy(&valobj[field], &csv[type][obj][field]);
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

void observecsvgeneral(char *csvobj, long classidx, long type, compress_f compressfunc)
{
/*
  case_obj_t obj;
  val_t valobj[32];
  initonce();
  case_obj_randomize(&obj);
  csv2valobj(csvobj, classidx, valobj);
  insertcsv(valobj, type);
  for (field ..) {
    bit = csvfunc(valob[f]j);  do for field
    set bit in field;
  }
*/
}

void notetype(long type)
{
  case_obj_setattr(&types, type, 1);
}

void text2val(char *text, val_t *val)
{
  if (isnum(text)) {
    val->num = strtod(text, NULL);
  } else {
    memcpy(val->str, text, VAL_STRSZ);
  }
}
