#include <stdio.h>
#include <stdlib.h>
#include "abit.h"
#include "acore.h"
#include "agene.h"
#include "ajung.h"
#include "alib.h"
#include "aobj.h"
#include "asum.h"
#include "atoss.h"

#define OBJECT_CACHE 64

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
  long tally = 0;
  initonce();
  uptypes(type);
  tally += acore_classify(obj, type);
  tally += agene_classify(obj, type);
  tally += ajung_classify(obj, type);
  tally += asum_classify(obj, type);
  if (4 == tally) {
    class = 1;
  } else {
    class = 0;
  }
  return class;
}

double alib_frequencyi(aobj_t indicator, aobj_t target, long type)
{
  long indicount = count(indicator, type);
  long targcount = count(target, type);
  return (long) indicount / targcount;
}

double alib_frequencyt(aobj_t indicator, aobj_t target, long type)
{
  long targcount = count(target, type);
  long indicount = count(indicator, type);
  return (long) targcount / indicount;
}

double alib_impertinencei(aobj_t indicator, aobj_t target, long type)
{
  long indisubcount = countsub(indicator, target, type);
  long targcount = count(target, type);
  return (long) indisubcount / targcount;
}

double alib_impertinencet(aobj_t indicator, aobj_t target, long type)
{
  long targsubcount = countsub(target, indicator, type);
  long indicount = count(indicator, type);
  return (long) targsubcount / indicount;
}

double alib_mismatchi(aobj_t indicator, aobj_t target, long type)
{
  long indisubcount = countsub(indicator, target, type);
  long targsubcount = countsub(target, indicator, type);
  return (long) indisubcount / targsubcount;
}

double alib_mismatcht(aobj_t indicator, aobj_t target, long type)
{
  long targsubcount = countsub(target, indicator, type);
  long indisubcount = countsub(indicator, target, type);
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
  long indisubcount = countsub(indicator, target, type);
  long bothcount = countboth(indicator, target, type);
  return (long) indisubcount / bothcount;
}

double alib_opacityt(aobj_t indicator, aobj_t target, long type)
{
  long targsubcount = countsub(target, indicator, type);
  long bothcount = countboth(indicator, target, type);
  return (long) targsubcount / bothcount;
}

double alib_overlap(aobj_t indicator, aobj_t target, long type)
{
  long bothcount = countboth(indicator, target, type);
  long eithercount = counteither(indicator, target, type);
  return (long) bothcount / eithercount;
}

double alib_overlapi(aobj_t indicator, aobj_t target, long type)
{
  long bothcount = countboth(indicator, target, type);
  long indicount = count(indicator, type);
  return (long) bothcount / indicount;
}

double alib_overlapt(aobj_t indicator, aobj_t target, long type)
{
  long bothcount = countboth(indicator, target, type);
  long targcount = count(target, type);
  return (long) bothcount / targcount;
}

double alib_transparency(aobj_t indicator, aobj_t target, long type)
{
  long bothcount = countboth(indicator, target, type);
  long xorcount = countxor(indicator, target, type);
  return (long) bothcount / xorcount;
}

double alib_transparencyi(aobj_t indicator, aobj_t target, long type)
{
  long bothcount = countboth(indicator, target, type);
  long indisubcount = countsub(indicator, target, type);
  return (long) bothcount / indisubcount;
}

double alib_transparencyt(aobj_t indicator, aobj_t target, long type)
{
  long bothcount = countboth(indicator, target, type);
  long targsubcount = countsub(target, indicator, type);
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
  for (type = 0; type < types; type++) {
    if (aobj_getattr(types, type)) {
      acore_learn(objs[type], OBJECT_CACHE, type);
      agene_learn(objs[type], OBJECT_CACHE, type);
      ajung_learn(objs[type], OBJECT_CACHE, type);
      asum_learn(objs[type], OBJECT_CACHE, type);
    }
  }
}

void uptypes(long seentype)
{
  aobj_setattr(&types, seentype, 1);
}
