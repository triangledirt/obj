#include <stdio.h>
#include <stdlib.h>
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

#define OBJECT_CACHE 64

static case_obj_t object[32][OBJECT_CACHE];
static case_bit_t once = 0;
static case_obj_t types;
static val_t lens[32][OBJECT_CACHE][32];
static long lensidx = 0;

/*
 a function that increments the lenspos
 insert into the lens at random
 have different lens methods--ideal object, choose an ideal object every once in a while, first line and then every once in a while in a stochastic manner and you would compare text fields (whether they're equal to the (ideal) object) or not (so that's a 1 or a 0) or whether a number is greater than (which is one) or less than the ideal object (which is 0)
 or instead of picking an ideal object you could store the last n character or numeric fields and see what the most common character value is or average numerical field value is
 for the historical collection of n characters you'd store the first say 8 characters to calculate with
 for the average of numerical values, could use a pseudo-average so I can store only one value
 provide a classification index parameter so you don't have to rearrange the csv--that's a parameter to the lens() functions
 for the lens history, have a struct containing a character value and a double value and then a field that says which one it is (since there are no unions in C)--There are unions in C, but it might be simpler to do it this way anyway
 use OBJECT_CACHE as the size of the history to save?
*/

static long count(long type, case_obj_t typ);
static long countboth(long type, case_obj_t typ1, case_obj_t typ2);
static long counteither(long type, case_obj_t typ1, case_obj_t typ2);
static long countsub(long type, case_obj_t typ1, case_obj_t typ2);
static long countxor(long type, case_obj_t typ1, case_obj_t typ2);
static void initonce();
static void learn();
static void lensread(char *csvobj, val_t valobj[32]);
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
#if CASE_VERBOSE && CASE_EXTRA_VERBOSE
  printf("type%ld class     core=%d filt=%d fold=%d gene=%d jack=%d jung=%d sum=%d\n", type, coreclass, filtclass, foldclass, geneclass, jackclass, jungclass, sumclass);
#endif
  return class;
}

double case_frequencyi(case_obj_t indicator, case_obj_t target, long type)
{
  long indicnt;
  long targcnt;
  indicnt = count(indicator, type);
  targcnt = count(target, type);
  if (0 == targcnt)
    targcnt = 1;
  return (long) indicnt / targcnt;
}

double case_frequencyt(case_obj_t indicator, case_obj_t target, long type)
{
  long targcnt;
  long indicnt;
  targcnt = count(target, type);
  indicnt = count(indicator, type);
  if (0 == indicnt)
    indicnt = 1;
  return (long) targcnt / indicnt;
}

double case_impertinencei(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long targcnt;
  indisubcnt = countsub(indicator, target, type);
  targcnt = count(target, type);
  if (0 == targcnt)
    targcnt = 1;
  return (long) indisubcnt / targcnt;
}

double case_impertinencet(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long indicnt;
  targsubcnt = countsub(target, indicator, type);
  indicnt = count(indicator, type);
  if (0 == indicnt)
    indicnt = 1;
  return (long) targsubcnt / indicnt;
}

void case_lensfirst(char *csvobj, long classidx, long type)
{
  case_obj_t obj;
  val_t valobj[32];
  case_obj_randomize(&obj);
  lensread(csvobj, valobj);
  ;
  case_observe(obj, type);
}

double case_mismatchi(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long targsubcnt;
  indisubcnt = countsub(indicator, target, type);
  targsubcnt = countsub(target, indicator, type);
  if (0 == targsubcnt)
    targsubcnt = 1;
  return (long) indisubcnt / targsubcnt;
}

double case_mismatcht(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long indisubcnt;
  targsubcnt = countsub(target, indicator, type);
  indisubcnt = countsub(indicator, target, type);
  if (0 == indisubcnt)
    indisubcnt = 1;
  return (long) targsubcnt / indisubcnt;
}

void case_observe(case_obj_t obj, long type)
{
  long i;
  initonce();
  notetype(type);
  i = random() % OBJECT_CACHE;
  object[type][i] = obj;
  if (die_toss(OBJECT_CACHE / 16))
    learn();
}

double case_opacityi(case_obj_t indicator, case_obj_t target, long type)
{
  long indisubcnt;
  long bothcnt;
  indisubcnt = countsub(indicator, target, type);
  bothcnt = countboth(indicator, target, type);
  if (0 == bothcnt)
    bothcnt = 1;
  return (long) indisubcnt / bothcnt;
}

double case_opacityt(case_obj_t indicator, case_obj_t target, long type)
{
  long targsubcnt;
  long bothcnt;
  targsubcnt = countsub(target, indicator, type);
  bothcnt = countboth(indicator, target, type);
  if (0 == bothcnt)
    bothcnt = 1;
  return (long) targsubcnt / bothcnt;
}

double case_overlap(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long eithercnt;
  bothcnt = countboth(indicator, target, type);
  eithercnt = counteither(indicator, target, type);
  if (0 == eithercnt)
    eithercnt = 1;
  return (long) bothcnt / eithercnt;
}

double case_overlapi(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long indicnt;
  bothcnt = countboth(indicator, target, type);
  indicnt = count(indicator, type);
  if (0 == indicnt)
    indicnt = 1;
  return (long) bothcnt / indicnt;
}

double case_overlapt(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long targcnt;
  bothcnt = countboth(indicator, target, type);
  targcnt = count(target, type);
  if (0 == targcnt)
    targcnt = 1;
  return (long) bothcnt / targcnt;
}

double case_transparency(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long xorcnt;
  bothcnt = countboth(indicator, target, type);
  xorcnt = countxor(indicator, target, type);
  if (0 == xorcnt)
    xorcnt = 1;
  return (long) bothcnt / xorcnt;
}

double case_transparencyi(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long indisubcnt;
  bothcnt = countboth(indicator, target, type);
  indisubcnt = countsub(indicator, target, type);
  if (0 == indisubcnt)
    indisubcnt = 1;
  return (long) bothcnt / indisubcnt;
}

double case_transparencyt(case_obj_t indicator, case_obj_t target, long type)
{
  long bothcnt;
  long targsubcnt;
  bothcnt = countboth(indicator, target, type);
  targsubcnt = countsub(target, indicator, type);
  if (0 == targsubcnt)
    targsubcnt = 1;
  return (long) bothcnt / targsubcnt;
}

long count(long type, case_obj_t typ)
{
  long count = 0;
  case_obj_t obj;
  long i;
  for (i = 0; i < OBJECT_CACHE; i++) {
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
  for (i = 0; i < OBJECT_CACHE; i++) {
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
  for (i = 0; i < OBJECT_CACHE; i++) {
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
  for (i = 0; i < OBJECT_CACHE; i++) {
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
  for (i = 0; i < OBJECT_CACHE; i++) {
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
  case_obj_clear(&types);
  if (!once) {
    for (type = 0; type < 32; type++)
      for (i = 0; i < OBJECT_CACHE; i++)
        case_obj_randomize(&object[type][i]);
    once = 1;
  }
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
      core_learn(object[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      coretime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
      gettimeofday(&tv1, NULL);
      filt_learn(object[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      filttime = tv2.tv_usec - tv1.tv_usec;
      ;
      gettimeofday(&tv1, NULL);
      fold_learn(object[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      foldtime = tv2.tv_usec - tv1.tv_usec;
      ;
/*
      gettimeofday(&tv1, NULL);
      gene_learn(object[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      genetime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
/*
      gettimeofday(&tv1, NULL);
      jack_learn(object[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      jacktime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
/*
      gettimeofday(&tv1, NULL);
      jung_learn(object[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      jungtime = tv2.tv_usec - tv1.tv_usec;
*/
      ;
      gettimeofday(&tv1, NULL);
      sum_learn(object[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      sumtime = tv2.tv_usec - tv1.tv_usec;
#if CASE_VERBOSE && CASE_EXTRA_VERBOSE
      printf("type%ld times     core=%ld filt=%ld fold=%ld gene=%ld jack=%ld jung=%ld sum=%ld\n", type, coretime, filttime, foldtime, genetime, jacktime, jungtime, sumtime);
#endif
    }
}

void lensread(char *csvobj, val_t valobj[32])
{
}

void notetype(long type)
{
  case_obj_setattr(&types, type, 1);
}
