#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "bit.h"
#include "case.h"
#include "core.h"
#include "fold.h"
#include "gene.h"
#include "jung.h"
#include "object.h"
#include "sum.h"
#include "toss.h"

#define OBJECT_CACHE 64
#define SHOW_DETAILS 1

static case_object_t objs[32][OBJECT_CACHE];
static case_bit_t once = 0;
static case_object_t types;

static long count(long type, case_object_t typ);
static long countboth(long type, case_object_t typ1, case_object_t typ2);
static long counteither(long type, case_object_t typ1, case_object_t typ2);
static long countsub(long type, case_object_t typ1, case_object_t typ2);
static long countxor(long type, case_object_t typ1, case_object_t typ2);
static void initonce();
static void learn();
static void uptypes(long seentype);

case_bit_t case_classify(case_object_t obj, long type)
{
  case_bit_t class;
  long tally;
  case_bit_t coreclass;
  case_bit_t foldclass;
  case_bit_t geneclass;
  case_bit_t jungclass;
  case_bit_t sumclass;
  initonce();
  uptypes(type);
  coreclass = core_classify(obj, type);
  foldclass = fold_classify(obj, type);
  geneclass = gene_classify(obj, type);
  jungclass = jung_classify(obj, type);
  sumclass = sum_classify(obj, type);
  tally = coreclass + foldclass + geneclass + jungclass + sumclass;
  if (tally >= 3) {
    class = 1;
  } else {
    class = 0;
  }
#if CASE_VERBOSE && SHOW_DETAILS
  printf("type%ld class     core=%d fold=%d gene=%d jung=%d sum=%d\n", type,
    coreclass, foldclass, geneclass, jungclass, sumclass);
#endif
  return class;
}

double case_frequencyi(case_object_t indicator, case_object_t target,
  long type)
{
  long indicount;
  long targcount;
  indicount = count(indicator, type);
  targcount = count(target, type);
  if (0 == targcount) {
    targcount = 1;
  }
  return (long) indicount / targcount;
}

double case_frequencyt(case_object_t indicator, case_object_t target,
  long type)
{
  long targcount;
  long indicount;
  targcount = count(target, type);
  indicount = count(indicator, type);
  if (0 == indicount) {
    indicount = 1;
  }
  return (long) targcount / indicount;
}

double case_impertinencei(case_object_t indicator, case_object_t target,
  long type)
{
  long indisubcount;
  long targcount;
  indisubcount = countsub(indicator, target, type);
  targcount = count(target, type);
  if (0 == targcount) {
    targcount = 1;
  }
  return (long) indisubcount / targcount;
}

double case_impertinencet(case_object_t indicator, case_object_t target,
  long type)
{
  long targsubcount;
  long indicount;
  targsubcount = countsub(target, indicator, type);
  indicount = count(indicator, type);
  if (0 == indicount) {
    indicount = 1;
  }
  return (long) targsubcount / indicount;
}

void case_lens(char *obj, long type)
{
}

double case_mismatchi(case_object_t indicator, case_object_t target, long type)
{
  long indisubcount;
  long targsubcount;
  indisubcount = countsub(indicator, target, type);
  targsubcount = countsub(target, indicator, type);
  if (0 == targsubcount) {
    targsubcount = 1;
  }
  return (long) indisubcount / targsubcount;
}

double case_mismatcht(case_object_t indicator, case_object_t target, long type)
{
  long targsubcount;
  long indisubcount;
  targsubcount = countsub(target, indicator, type);
  indisubcount = countsub(indicator, target, type);
  if (0 == indisubcount) {
    indisubcount = 1;
  }
  return (long) targsubcount / indisubcount;
}

void case_observe(case_object_t obj, long type)
{
  long idx;
  initonce();
  uptypes(type);
  idx = random() % OBJECT_CACHE;
  objs[type][idx] = obj;
  if (toss_die(OBJECT_CACHE / 16)) {
    learn();
  }
}

double case_opacityi(case_object_t indicator, case_object_t target, long type)
{
  long indisubcount;
  long bothcount;
  indisubcount = countsub(indicator, target, type);
  bothcount = countboth(indicator, target, type);
  if (0 == bothcount) {
    bothcount = 1;
  }
  return (long) indisubcount / bothcount;
}

double case_opacityt(case_object_t indicator, case_object_t target, long type)
{
  long targsubcount;
  long bothcount;
  targsubcount = countsub(target, indicator, type);
  bothcount = countboth(indicator, target, type);
  if (0 == bothcount) {
    bothcount = 1;
  }
  return (long) targsubcount / bothcount;
}

double case_overlap(case_object_t indicator, case_object_t target, long type)
{
  long bothcount;
  long eithercount;
  bothcount = countboth(indicator, target, type);
  eithercount = counteither(indicator, target, type);
  if (0 == eithercount) {
    eithercount = 1;
  }
  return (long) bothcount / eithercount;
}

double case_overlapi(case_object_t indicator, case_object_t target, long type)
{
  long bothcount;
  long indicount;
  bothcount = countboth(indicator, target, type);
  indicount = count(indicator, type);
  if (0 == indicount) {
    indicount = 1;
  }
  return (long) bothcount / indicount;
}

double case_overlapt(case_object_t indicator, case_object_t target, long type)
{
  long bothcount;
  long targcount;
  bothcount = countboth(indicator, target, type);
  targcount = count(target, type);
  if (0 == targcount) {
    targcount = 1;
  }
  return (long) bothcount / targcount;
}

double case_transparency(case_object_t indicator, case_object_t target,
  long type)
{
  long bothcount;
  long xorcount;
  bothcount = countboth(indicator, target, type);
  xorcount = countxor(indicator, target, type);
  if (0 == xorcount) {
    xorcount = 1;
  }
  return (long) bothcount / xorcount;
}

double case_transparencyi(case_object_t indicator, case_object_t target,
  long type)
{
  long bothcount;
  long indisubcount;
  bothcount = countboth(indicator, target, type);
  indisubcount = countsub(indicator, target, type);
  if (0 == indisubcount) {
    indisubcount = 1;
  }
  return (long) bothcount / indisubcount;
}

double case_transparencyt(case_object_t indicator, case_object_t target,
  long type)
{
  long bothcount;
  long targsubcount;
  bothcount = countboth(indicator, target, type);
  targsubcount = countsub(target, indicator, type);
  if (0 == targsubcount) {
    targsubcount = 1;
  }
  return (long) bothcount / targsubcount;
}

long count(long type, case_object_t typ)
{
  long count = 0;
  case_object_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += case_object_hastype(obj, typ);
  }
  return count;
}

long countboth(long type, case_object_t typ1, case_object_t typ2)
{
  long count = 0;
  case_object_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (case_object_hastype(obj, typ1)
      && case_object_hastype(obj, typ2));
  }
  return count;
}

long counteither(long type, case_object_t typ1, case_object_t typ2)
{
  long count = 0;
  case_object_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (case_object_hastype(obj, typ1)
      || case_object_hastype(obj, typ2));
  }
  return count;
}

long countsub(long type, case_object_t typ1, case_object_t typ2)
{
  long count = 0;
  case_object_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (case_object_hastype(obj, typ1)
      && !case_object_hastype(obj, typ2));
  }
  return count;
}

long countxor(long type, case_object_t typ1, case_object_t typ2)
{
  long count = 0;
  case_object_t obj;
  long idx;
  case_bit_t has1;
  case_bit_t has2;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    has1 = case_object_hastype(obj, typ1);
    has2 = case_object_hastype(obj, typ2);
    if (has1 ^ has2) {
      count++;
    }
  }
  return count;
}

void initonce()
{
  long idx;
  long type;
  case_object_clear(&types);
  if (!once) {
    for (type = 0; type < 32; type++) {
      for (idx = 0; idx < OBJECT_CACHE; idx++) {
        case_object_randomize(&objs[type][idx]);
      }
    }
    once = 1;
  }
}

void learn()
{
  long type;
  struct timeval tv1;
  struct timeval tv2;
  long long coretime;
  long long foldtime;
  long long genetime;
  long long jungtime;
  long long sumtime;
  for (type = 0; type < 32; type++) {
    if (case_object_getattr(types, type)) {
      gettimeofday(&tv1, NULL);
      core_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      coretime = tv2.tv_usec - tv1.tv_usec;

      gettimeofday(&tv1, NULL);
      fold_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      foldtime = tv2.tv_usec - tv1.tv_usec;

      gettimeofday(&tv1, NULL);
      gene_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      genetime = tv2.tv_usec - tv1.tv_usec;

      gettimeofday(&tv1, NULL);
      jung_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      jungtime = tv2.tv_usec - tv1.tv_usec;

      gettimeofday(&tv1, NULL);
      sum_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      sumtime = tv2.tv_usec - tv1.tv_usec;
#if ALIB_VERBOSE && SHOW_DETAILS
      printf("type%ld times     core=%lld fold=%lld gene=%lld jung=%lld "
        "sum=%lld\n", type, coretime, foldtime, genetime, jungtime, sumtime);
#endif
    }
  }
}

void uptypes(long seentype)
{
  case_object_setattr(&types, seentype, 1);
}
