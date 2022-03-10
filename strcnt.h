#ifndef strcnt_h
#define strcnt_h

#include "case.h"

struct strcnt_t {
  char str[CASE_STR];
  long cnt;
  struct strcnt_t *prev;
  struct strcnt_t *next;
};
typedef struct strcnt_t strcnt_t;

void strcnt_init(strcnt_t *strcnt, strcnt_t *prev, strcnt_t *next);

#endif
