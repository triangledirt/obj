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

static aobj_t objs[ALIB_TYPE_COUNT][OBJECT_CACHE];
static abit_t once = 0;
static long types = 1;

static long cntwith(long type, aobj_t typ);
static long cntwithboth(long type, aobj_t typ1, aobj_t typ2);
static long cntwitheither(long type, aobj_t typ1, aobj_t typ2);
static long cntwithnotboth(long type, aobj_t typ1, aobj_t typ2);
static long cntwithsub(long type, aobj_t typ1, aobj_t typ2);
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
  if (tally >= 2) {
    class = 1;
  } else {
    class = 0;
  }
  return class;
}

long cntwith(long type, aobj_t typ)
{
  long cnt = 0;
  aobj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[typ][idx];
    cnt += aobj_hastype(obj, typ);
  }
  return cnt;
}

long cntwithboth(long type, aobj_t typ1, aobj_t typ2)
{
  long cnt = 0;
  aobj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    cnt += (aobj_hastype(obj, typ1) && aobj_hastype(obj, typ2));
  }
  return cnt;
}

long cntwitheither(long type, aobj_t typ1, aobj_t typ2)
{
  long cnt = 0;
  aobj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    cnt += (aobj_hastype(obj, typ1) || aobj_hastype(obj, typ2));
  }
  return cnt;
}

long cntwithnotboth(long type, aobj_t typ1, aobj_t typ2)
{
  long cnt = 0;
  aobj_t obj;
  long idx;
  abit_t has1;
  abit_t has2;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    has1 = aobj_hastype(obj, typ1);
    has2 = aobj_hastype(obj, typ2);
    if (1 == (has1 + has2)) {
      cnt++;
    }
  }
  return cnt;
}

long cntwithsub(long type, aobj_t typ1, aobj_t typ2)
{
  long cnt = 0;
  aobj_t obj;
  long idx;
  for (idx = 0; idx < OBJECT_CACHE; idx++) {
    obj = objs[type][idx];
    cnt += (aobj_hastype(obj, typ1) && !aobj_hastype(obj, typ2));
  }
  return cnt;
}

double alib_indifreq(aobj_t indicator, aobj_t target, long type)
{
  long targcnt = cntwith(target, type);
  long indicnt = cntwith(indicator, type);
  return (long) targcnt / indicnt;
}

double alib_indiover(aobj_t indicator, aobj_t target, long type)
{
  long bothcnt = cntwithboth(indicator, target, type);
  long indicnt = cntwith(indicator, type);
  return (long) bothcnt / indicnt;
}

double alib_indimism(aobj_t indicator, aobj_t target, long type)
{
  long indisubcnt = cntwithsub(indicator, target, type);
  long targsubcnt = cntwithsub(target, indicator, type);
  return (long) indisubcnt / targsubcnt;
}

double alib_indiimp(aobj_t indicator, aobj_t target, long type)
{
  long indisubcnt = cntwithsub(indicator, target, type);
  long targcnt = cntwith(target, type);
  return (long) indisubcnt / targcnt;
}

double alib_indiopac(aobj_t indicator, aobj_t target, long type)
{
  long indisubcnt = cntwithsub(indicator, target, type);
  long bothcnt = cntwithboth(indicator, target, type);
  return (long) indisubcnt / bothcnt;
}

double alib_inditrans(aobj_t indicator, aobj_t target, long type)
{
  long bothcnt = cntwithboth(indicator, target, type);
  long indisubcnt = cntwithsub(indicator, target, type);
  return (long) bothcnt / indisubcnt;
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

double alib_overlap(aobj_t indicator, aobj_t target, long type)
{
  long bothcnt = cntwithboth(indicator, target, type);
  long eithercnt = cntwitheither(indicator, target, type);
  return (long) bothcnt / eithercnt;
}

double alib_targfreq(aobj_t indicator, aobj_t target, long type)
{
  long targcnt = cntwith(target, type);
  long indicnt = cntwith(indicator, type);
  return (long) targcnt / indicnt;
}

double alib_targimp(aobj_t indicator, aobj_t target, long type)
{
  long targsubcnt = cntwithsub(target, indicator, type);
  long indicnt = cntwith(indicator, type);
  return (long) targsubcnt / indicnt;
}

double alib_targmism(aobj_t indicator, aobj_t target, long type)
{
  long targsubcnt = cntwithsub(target, indicator, type);
  long indisubcnt = cntwithsub(indicator, target, type);
  return (long) targsubcnt / indisubcnt;
}

double alib_targover(aobj_t indicator, aobj_t target, long type)
{
  long bothcnt = cntwithboth(indicator, target, type);
  long targcnt = cntwithsub(indicator, target, type);
  return (long) bothcnt / targcnt;
}

double alib_targopac(aobj_t indicator, aobj_t target, long type)
{
  long targsubcnt = cntwithsub(target, indicator, type);
  long bothcnt = cntwithboth(indicator, target, type);
  return (long) targsubcnt / bothcnt;
}

double alib_targtrans(aobj_t indicator, aobj_t target, long type)
{
  long bothcnt = cntwithboth(indicator, target, type);
  long targsubcnt = cntwithsub(target, indicator, type);
  return (long) bothcnt / targsubcnt;
}

double alib_transparency(aobj_t indicator, aobj_t target, long type)
{
  long bothcnt = cntwithboth(indicator, target, type);
  long notbothcnt = cntwithnotboth(indicator, target, type);
  return (long) bothcnt / notbothcnt;
}

void learn()
{
  long type;
  for (type = 0; type < types; type++) {
    acore_learn(objs[type], OBJECT_CACHE, type);
    agene_learn(objs[type], OBJECT_CACHE, type);
    ajung_learn(objs[type], OBJECT_CACHE, type);
    asum_learn(objs[type], OBJECT_CACHE, type);
  }
}

void initonce()
{
  long idx;
  long type;
  if (!once) {
    for (type = 0; type < ALIB_TYPE_COUNT; type++) {
      for (idx = 0; idx < OBJECT_CACHE; idx++) {
        aobj_randomize(&objs[type][idx]);
      }
    }
    once = 1;
  }
}

void uptypes(long seentype)
{
  if ((seentype + 1) > types) {
    types = seentype;
  }
}
