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
#include "obj.h"
#include "sum.h"
#include "toss.h"

#define OBJECT_CACHE 64
#define SHOW_DETAILS 1

static case_obj_t objs[32][OBJECT_CACHE];
static case_bit_t once = 0;
static case_obj_t types;

static long count(long type, case_obj_t typ);
static long countboth(long type, case_obj_t typ1, case_obj_t typ2);
static long counteither(long type, case_obj_t typ1, case_obj_t typ2);
static long countsub(long type, case_obj_t typ1, case_obj_t typ2);
static long countxor(long type, case_obj_t typ1, case_obj_t typ2);
static void initonce();
static void learn();
static void uptypes(long seentype);

case_bit_t case_classify(case_obj_t obj, long type)
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
  printf("type%ld class     core=%d fold=%d gene=%d jung=%d sum=%d\n", type, coreclass, foldclass, geneclass, jungclass, sumclass);
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

void case_lens(char *obj, long type)
{
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
  long idx;
  initonce();
  uptypes(type);
  idx = random() % OBJECT_CACHE;
  objs[type][idx] = obj;
  if (toss_die(OBJECT_CACHE / 16))
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
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += case_obj_hastype(obj, typ);
  }
  return count;
}

long countboth(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (case_obj_hastype(obj, typ1) && case_obj_hastype(obj, typ2));
  }
  return count;
}

long counteither(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (case_obj_hastype(obj, typ1) || case_obj_hastype(obj, typ2));
  }
  return count;
}

long countsub(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (case_obj_hastype(obj, typ1) && !case_obj_hastype(obj, typ2));
  }
  return count;
}

long countxor(long type, case_obj_t typ1, case_obj_t typ2)
{
  long count = 0;
  case_obj_t obj;
  long idx;
  case_bit_t has1;
  case_bit_t has2;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    has1 = case_obj_hastype(obj, typ1);
    has2 = case_obj_hastype(obj, typ2);
    if (has1 ^ has2)
      count++;
  }
  return count;
}

void initonce()
{
  long idx;
  long type;
  case_obj_clear(&types);
  if (!once) {
    for (type = 0; type < 32; type++) {
      for (idx = 0; idx < OBJECT_CACHE; idx++)
        case_obj_randomize(&objs[type][idx]);
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
    if (case_obj_getattr(types, type)) {
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
#if CASE_VERBOSE && SHOW_DETAILS
      printf("type%ld times     core=%lld fold=%lld gene=%lld jung=%lld sum=%lld\n", type, coretime, foldtime, genetime, jungtime, sumtime);
#endif
    }
  }
}

void uptypes(long seentype)
{
  case_obj_setattr(&types, seentype, 1);
}
