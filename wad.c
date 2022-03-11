#include "strcnt.h"
#include "wad.h"

#define WADCACHE CASE_OBJCACHE

struct wad_t {
  strcnt_t strcnt[WADCACHE];
};

static case_bool_t once = case_bool_false;
static void init(wad_t *wad);

void init(wad_t *wad)
{
  long i;
  strcnt_t *prev;
  strcnt_t *next;
  if (!once) {
    prev = 0;
    next = &wad->strcnt[1];
    strcnt_init(&wad->strcnt[0], prev, next);
    for (i = 1; i < (WADCACHE - 1); i++) {
      prev = &wad->strcnt[i - 1];
      next = &wad->strcnt[i + 1];
      strcnt_init(&wad->strcnt[i], prev, next);
    }
    prev = &wad->strcnt[WADCACHE - 2];
    next = 0;
    strcnt_init(&wad->strcnt[WADCACHE - 1], prev, next);
    once = case_bool_true;
  }
}

void wad_update(wad_t *wad, char *str)
{
  init(wad);
}

char *wad_popular(wad_t *wad)
{
  init(wad);
}
