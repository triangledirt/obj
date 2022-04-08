#ifndef past_h
#define past_h

#define NUM 8

struct past_t {
  double num[NUM];
  long notecnt;
  double avg;
};
typedef struct past_t past_t;

void past_init(past_t *past);
void past_note(past_t *past, double num);
double past_avg(past_t *past);

#endif
