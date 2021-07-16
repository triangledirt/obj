#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "abit.h"
#include "acore.h"
#include "agene.h"
#include "ajung.h"
#include "alib.h"
#include "aobj.h"
#include "asum.h"
#include "atoss.h"

#define OBJECT_CACHE 64
#define SHOW_DETAILS 1

static aobj_t objs[32][OBJECT_CACHE];
static abit_t once = 0;
static aobj_t types;

static long count(long type, aobj_t typ);
static long countboth(long type, aobj_t typ1, aobj_t typ2);
static long counteither(long type, aobj_t typ1, aobj_t typ2);
static long countsub(long type, aobj_t typ1, aobj_t typ2);
static long countxor(long type, aobj_t typ1, aobj_t typ2);
static void initonce();
static void learn();
static void uptypes(long seentype);

abit_t alib_classify(aobj_t obj, long type)
{
  abit_t class;
  long tally;
  abit_t coreclass;
  abit_t geneclass;
  abit_t jungclass;
  abit_t sumclass;
  initonce();
  uptypes(type);
  coreclass = acore_classify(obj, type);
  geneclass = agene_classify(obj, type);
  jungclass = ajung_classify(obj, type);
  sumclass = asum_classify(obj, type);
  tally = coreclass + geneclass + jungclass + sumclass;
  if (4 == tally) {
    class = 1;
  } else {
    class = 0;
  }
#if SHOW_DETAILS
  printf("type%ld class     core=%d gene=%d jung=%d sum=%d\n", type, coreclass,
    geneclass, jungclass, sumclass);
#endif
  return class;
}

double alib_frequencyi(aobj_t indicator, aobj_t target, long type)
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

double alib_frequencyt(aobj_t indicator, aobj_t target, long type)
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

double alib_impertinencei(aobj_t indicator, aobj_t target, long type)
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

double alib_impertinencet(aobj_t indicator, aobj_t target, long type)
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

double alib_mismatchi(aobj_t indicator, aobj_t target, long type)
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

double alib_mismatcht(aobj_t indicator, aobj_t target, long type)
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

void alib_observe(aobj_t obj, long type)
{
  long idx;
  initonce();
  uptypes(type);
  idx = random() % OBJECT_CACHE;
  objs[type][idx] = obj;
  if (atoss_die(OBJECT_CACHE / 16)) {
    learn();
  }
}

double alib_opacityi(aobj_t indicator, aobj_t target, long type)
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

double alib_opacityt(aobj_t indicator, aobj_t target, long type)
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

double alib_overlap(aobj_t indicator, aobj_t target, long type)
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

double alib_overlapi(aobj_t indicator, aobj_t target, long type)
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

double alib_overlapt(aobj_t indicator, aobj_t target, long type)
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

double alib_transparency(aobj_t indicator, aobj_t target, long type)
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

double alib_transparencyi(aobj_t indicator, aobj_t target, long type)
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

double alib_transparencyt(aobj_t indicator, aobj_t target, long type)
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

long count(long type, aobj_t typ)
{
  long count = 0;
  aobj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[typ][idx];
    count += aobj_hastype(obj, typ);
  }
  return count;
}

long countboth(long type, aobj_t typ1, aobj_t typ2)
{
  long count = 0;
  aobj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (aobj_hastype(obj, typ1) && aobj_hastype(obj, typ2));
  }
  return count;
}

long counteither(long type, aobj_t typ1, aobj_t typ2)
{
  long count = 0;
  aobj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (aobj_hastype(obj, typ1) || aobj_hastype(obj, typ2));
  }
  return count;
}

long countsub(long type, aobj_t typ1, aobj_t typ2)
{
  long count = 0;
  aobj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    count += (aobj_hastype(obj, typ1) && !aobj_hastype(obj, typ2));
  }
  return count;
}

long countxor(long type, aobj_t typ1, aobj_t typ2)
{
  long count = 0;
  aobj_t obj;
  long idx;
  abit_t has1;
  abit_t has2;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    has1 = aobj_hastype(obj, typ1);
    has2 = aobj_hastype(obj, typ2);
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
  aobj_clear(&types);
  if (!once) {
    for (type = 0; type < 32; type++) {
      for (idx = 0; idx < OBJECT_CACHE; idx++) {
        aobj_randomize(&objs[type][idx]);
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
  long long genetime;
  long long jungtime;
  long long sumtime;
  for (type = 0; type < 32; type++) {
    if (aobj_getattr(types, type)) {
      gettimeofday(&tv1, NULL);
      acore_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      coretime = tv2.tv_usec - tv1.tv_usec;

      gettimeofday(&tv1, NULL);
      agene_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      genetime = tv2.tv_usec - tv1.tv_usec;

      gettimeofday(&tv1, NULL);
      ajung_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      jungtime = tv2.tv_usec - tv1.tv_usec;

      gettimeofday(&tv1, NULL);
      asum_learn(objs[type], OBJECT_CACHE, type);
      gettimeofday(&tv2, NULL);
      sumtime = tv2.tv_usec - tv1.tv_usec;
#if SHOW_DETAILS
      printf("type%d times     core=%lld gene=%lld jung=%lld sum=%lld\n", type,
        coretime, genetime, jungtime, sumtime);
#endif
    }
  }
}

void uptypes(long seentype)
{
  aobj_setattr(&types, seentype, 1);
}
