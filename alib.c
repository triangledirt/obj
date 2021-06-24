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
  if (tally >= 1) {
    class = 1;
  } else {
    class = 0;
  }
  return class;
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
