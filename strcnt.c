#include "strcnt.h"

void strcnt_init(strcnt_t *strcnt, strcnt_t *prev, strcnt_t *next)
{
  strcnt->str[0] = '\0';
  strcnt->cnt = 0;
  strcnt->prev = prev;
  strcnt->next = next;
}
