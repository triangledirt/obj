#ifndef obj_stat_h
#define obj_stat_h

struct obj_stat_t {
  long zeroes;
  long ones;
};

void obj_stat_reset(struct obj_stat_t *stat);

#endif
